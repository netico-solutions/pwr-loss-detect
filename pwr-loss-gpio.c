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

/* ----------------------------------------------------------------------------
 * FUNCTION PROTOTYPES
 * ----------------------------------------------------------------------------
 */
static irq_handler_t pwrloss_irq_handler(unsigned int irq, void *dev_id, \
                                         struct pt_regs *regs);

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


    irqNumber = gpio_to_irq(gpioNo);
    
    status = request_irq(irqNumber,
                        (irq_handler_t) pwrloss_irq_handler,
                        (IRQF_TRIGGER_RISING && IRQF_TRIGGER_HIGH),
                        "power-loss-gpio-handler",
                        NULL);
    return status;
}

static void __exit pwrloss_gpio_exit(void) {
    free_irq(irqNumber, NULL);
    gpio_unexport(gpioNo);
    gpio_free(gpioNo);
}

static irq_handler_t pwrloss_irq_handler(unsigned int irq, void *dev_id, \
                                         struct pt_regs *regs) {
    
    printk(KERN_WARNING "pwr-loss-detect-driver: Power loss detected!\n"); 
    return (irq_handler_t) IRQ_HANDLED;
}

module_init(pwrloss_gpio_init);
module_exit(pwrloss_gpio_exit);
