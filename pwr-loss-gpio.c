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
static unsigned int irqNumber;

static struct class *dev_class;
static struct cdev pwrloss_cdev;
static dev_t dev = 0;

static bool power = true;

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

    if(!gpio_is_valid(gpioNo)) {
        printk(KERN_WARNING "pwr-loss-detect-driver: Invalid GPIO number!\n");
        return -ENODEV;
    }
    else {
        printk(KERN_INFO "pwr-loss-detect-driver: Initialized..\n");
    }
    
    gpio_request(gpioNo, "sysfs");
    gpio_direction_input(gpioNo);
    gpio_export(gpioNo, false); //boolean value prevents direction change from 
                                //user space

    if (!gpio_get_value(gpioNo)) {
        printk(KERN_INFO "pwr-loss-detect-driver: Power source OK!");
    }
    else {
        printk(KERN_WARNING "pwr-loss-detect-driver: No power source!");
        
    }

    if((alloc_chrdev_region(&dev, 0, 1, "pwrloss_Dev")) <0) {
        printk(KERN_WARNING "Cannot allocate major number!\n");
        return -1;
    }
    printk(KERN_INFO "Major = %d Minor = %d \n", MAJOR(dev), MINOR(dev));
    

    cdev_init(&pwrloss_cdev, &pwrloss_fops);
    pwrloss_cdev.owner = THIS_MODULE;
    pwrloss_cdev.ops = &pwrloss_fops;
    
    if((cdev_add(&pwrloss_cdev, dev, 1)) < 0) {
        printk(KERN_WARNING "Cannot add the device to the system!\n");
        goto r_class;
    }

    if((dev_class = class_create(THIS_MODULE, "pwrloss_class")) == NULL) {
        printk(KERN_WARNING "Cannot create the struct class!\n");
        goto r_class;
    }
    
    if((device_create(dev_class, NULL, dev, NULL, "pwrloss_device")) == NULL) {
        printk(KERN_WARNING "Cannot create the device! \n");
        goto r_device;
    }
    printk(KERN_INFO "Device driver insert.. Done!\n");
    return 0;


    irqNumber = gpio_to_irq(gpioNo);
    
    status = request_irq(irqNumber,
                        (irq_handler_t) pwrloss_irq_handler,
                        (IRQF_TRIGGER_RISING && IRQF_TRIGGER_HIGH),
                        "power-loss-gpio-handler",
                        NULL);
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
    
    printk(KERN_WARNING "pwr-loss-detect-driver: Power loss detected!\n");
    power = false;
    return (irq_handler_t) IRQ_HANDLED;
}

static long pwrloss_ioctl(struct file * fd, unsigned int cmd, unsigned long arg) {
        
        switch(cmd){
            case PWRLOSS_GET_PID:
                break;

            case PWRLOSS_NOTIFY:
                copy_to_user((int32_t*) arg, &power, sizeof(power));
                break;
        }

        return 0;


}

module_init(pwrloss_gpio_init);
module_exit(pwrloss_gpio_exit);
