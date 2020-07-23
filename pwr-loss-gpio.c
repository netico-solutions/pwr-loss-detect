/*
 *  
 * Power loss detection gpio driver
 *
 * Author: Nemanja Stefanovic <nemanja.stef92@gmail.com>
 *
 * Licensed under GPL-v2 or later
 * ----------------------------------------------------------------------------
 *
 * Description:
 *
 * This driver manages state of gpio pin value which purpose is to detect power
 * loss of the uRTU device.
 *
 * Irq is being sent to user space on rising edge of the signal.
 *
 * ----------------------------------------------------------------------------
 */


/* ----------------------------------------------------------------------------
 * INCLUDES
 * ----------------------------------------------------------------------------
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/ioctl.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/fs.h>


#include "pwr-loss-gpio.h"

/* ----------------------------------------------------------------------------
 * MODULE INFO
 * ----------------------------------------------------------------------------
 */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nemanja Stefanovic");
MODULE_DESCRIPTION("Power loss detection gpio driver");
MODULE_VERSION("1.0");

/* ----------------------------------------------------------------------------
 * VARIABLES
 * ----------------------------------------------------------------------------
 */
static unsigned int gpioNo = 46; //hardcoded gpio pin number gpio1_14
                                 // 46 = 1 (from gpio1_14) * 32 (for 32 bit arch) \
                                 // + 14 (from gpio1_14)
static unsigned int irqNumber;

static struct class *dev_class;
static struct cdev pwrloss_cdev;
static dev_t dev = 0;

static int ret;
static int power = 1;

/* ----------------------------------------------------------------------------
 * FUNCTION PROTOTYPES
 * ----------------------------------------------------------------------------
 */
static irq_handler_t pwrloss_irq_handler(unsigned int irq, void *dev_id, \
                                         struct pt_regs *regs);

static long pwrloss_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

/* ----------------------------------------------------------------------------
 * MODULE PARAMETERS 
 * ----------------------------------------------------------------------------
 */
static const struct file_operations pwrloss_fops = 
{
        .owner              = THIS_MODULE,
        //.open               = pwrloss_open,
        //.release            = pwrloss_release,
        //.read               = pwrloss_read,
        .unlocked_ioctl     = pwrloss_ioctl
};

/* ----------------------------------------------------------------------------
 * FUNCTION DEFINITIONS
 * ----------------------------------------------------------------------------
 */
static int __init pwrloss_gpio_init(void) {
    int status = 0;
    
    printk(KERN_INFO "pwr-loss-detect-driver: Initializing..\n");

    // -------- Prvo nesto vezano za GPIO sto mislim da nema veze sa cdev --------
    
    // First check pin number validity
    if(!gpio_is_valid(gpioNo)) {
        printk(KERN_WARNING "pwr-loss-detect-driver: Invalid GPIO number!\n");
        return -ENODEV;
    }
    else {
        printk(KERN_INFO "pwr-loss-detect-driver: Initialized..\n");
    }
    
    // This is clear
    gpio_request(gpioNo, "sysfs"); // Claim pin with gpioNo and name it "sysfs"
    // TOD maybe check return value here for errors?
    gpio_direction_input(gpioNo);
    gpio_set_debounce(gpioNo, 1);
    gpio_export(gpioNo, false); //boolean value prevents direction change from 
                                //user space

    if (!gpio_get_value(gpioNo)) {
        printk(KERN_INFO "pwr-loss-detect-driver: Power source OK!");
    }
    else {
        printk(KERN_WARNING "pwr-loss-detect-driver: No power source!");
        
    }
    
    // -------- IRQ (interrupt request) --------
    
    // Map GPIO number to IRQ number
    irqNumber = gpio_to_irq(gpioNo);
    
    status = request_irq(irqNumber,
                        (irq_handler_t) pwrloss_irq_handler,  // Function called when the interrupt happens
                        (IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING),  // Interrupt flags
                        "power-loss-gpio-handler",  // Shown in /proc/interrupts
                        NULL);
    
    // TOD maybe check the status here for errors?
    if (status < 0) {
        printk(KERN_WARNING "pwr-loss-detect-driver: Invalid GPIO number!\n");
        free_irq(irqNumber, NULL);
        gpio_unexport(gpioNo);
        gpio_free(gpioNo);
        printk(KERN_INFO "Device driver initialization aborted!\n");
        return -ENODEV;
    }

    // -------- Odavde kod za cdev --------
    
    // Allocate mem block
    if((alloc_chrdev_region(&dev, 0, 1, "pwrloss_Dev")) <0) {
        printk(KERN_WARNING "Cannot allocate major number!\n");
        return -1;
    }
    printk(KERN_INFO "Major = %d Minor = %d \n", MAJOR(dev), MINOR(dev));
    

    // Allocate and initialize the cdev
    cdev_init(&pwrloss_cdev, &pwrloss_fops);
    pwrloss_cdev.owner = THIS_MODULE;
    pwrloss_cdev.ops = &pwrloss_fops;
    
    // Add cdev to system
    if((cdev_add(&pwrloss_cdev, dev, 1)) < 0) {
        printk(KERN_WARNING "Cannot add the device to the system!\n");
        goto r_class;
    }

    // Create a /dev/ entry from kernel space
    
    // Create a virtual device class that will appear in /sys/class/pwrloss_class
    if((dev_class = class_create(THIS_MODULE, "pwrloss_class")) == NULL) {
        printk(KERN_WARNING "Cannot create the struct class!\n");
        goto r_class;
    }
    
    // Create a device entry in /dev/pwrloss_device and register it with sysfs
    if((device_create(dev_class, NULL, dev, NULL, "pwrloss_device")) == NULL) {
        printk(KERN_WARNING "Cannot create the device! \n");
        goto r_device;
    }
    printk(KERN_INFO "Device driver insert.. Done!\n");

    return status;

r_device: 
    class_destroy(dev_class);
r_class:
    unregister_chrdev_region(dev, 1);
    return -1;
}

static void __exit pwrloss_gpio_exit(void) {
    free_irq(irqNumber, NULL);
    gpio_unexport(gpioNo);
    gpio_free(gpioNo);

    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&pwrloss_cdev);
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "Device driver remove.. Done!\n");
}

static irq_handler_t pwrloss_irq_handler(unsigned int irq, void *dev_id, \
                                         struct pt_regs *regs) {

    power = !gpio_get_value(gpioNo);
        
    if(!power) {
        printk(KERN_WARNING "pwr-loss-detect-driver: Power loss detected!\n");
    }
    else {
        printk(KERN_WARNING "pwr-loss-detect-driver: Power OK!\n");
    }
    printk(KERN_WARNING "power: %d\n", power);
    return (irq_handler_t) IRQ_HANDLED;
}

static long pwrloss_ioctl(struct file * fd, unsigned int cmd, unsigned long arg) {
        
        switch(cmd){

            case PWRLOSS_NOTIFY:
                ret = copy_to_user((int32_t*) arg, &power, sizeof(power));
                break;
            
            case PWRLOSS_READ:
                ret = copy_to_user((int32_t*) arg, &power, sizeof(power));
                break;
            
            //case PWRLOSS_WRITE:
            //    ret = copy_from_user((int32_t*) arg, &power, sizeof(power));
            //    break;
        }

        return 0;


}

module_init(pwrloss_gpio_init);
module_exit(pwrloss_gpio_exit);
