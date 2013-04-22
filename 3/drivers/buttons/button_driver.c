/*****************************************************************************
 *
 * Øving 3 uCSysDes, driverkoden
 *
 *****************************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/ioport.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#include "ap7000.h"

/* prototyper */

static int __init driver_init(void);
static void __exit driver_exit(void);
static int driver_open (struct inode *inode, struct file *filp);
static int driver_release (struct inode *inode, struct file *filp);
static ssize_t driver_read (struct file *filp, char __user *buff,
        size_t count, loff_t *offp);
static ssize_t driver_write (struct file *filp, const char __user *buff,
        size_t count, loff_t *offp);

/* fops-struct */

static struct file_operations driver_fops = {
    .owner = THIS_MODULE,
    .read = driver_read,
    .write = driver_write,
    .open = driver_open,
    .release = driver_release
};

dev_t device_number;
avr32_pio_t* pio;
struct cdev driver_cdev;

static long remap_to_physical(int logical){
    printk( KERN_INFO "remap_to_physical( logical: 0x%X )\n", logical);
    long out = 0;
    out |= (logical & 0x07) << 8;
    out |= (logical & 0x78) << 10;
    out |= (logical & 0x80) << 23;
    printk( KERN_INFO "<-- 0x%X --\n", out);
    return out;
}

/*****************************************************************************/
/* init-funksjon (kalles når modul lastes) */

static int __init driver_init (void) {
    printk( KERN_INFO "Trying to init Button driver...\n");

    /* allokere device-nummer */
    alloc_chrdev_region(&device_number, 0, 1, "buttons");

    /* be om tilgang til I/O-porter */
    request_region(AVR32_PIOB_ADDRESS, AVR32_PIOC_ADDRESS - AVR32_PIOB_ADDRESS, "buttons");

    /* initialisere PIO-maskinvaren (som i øving 2) */

    pio = (avr32_pio_t*) AVR32_PIOB_ADDRESS;

    printk( KERN_INFO "pio: %p\n", pio);

    /* Enable IO pins */
    pio->per = remap_to_physical(0xFF);

    /* Set pins to act as output */
    pio->oer = remap_to_physical(0xFF);

    /* registrere device i systemet (må gjøres når alt annet er initialisert) */
    cdev_init(&driver_cdev, &driver_fops);
    driver_cdev.owner = THIS_MODULE;
    cdev_add(&driver_cdev, device_number, 1);
    printk( KERN_INFO "Button init complete!\n");
    return 0;
}

/*****************************************************************************/
/* exit-funksjon (kalles når modul fjernes fra systemet) */

static void __exit driver_exit (void) {

    /* turn off buttons */
    pio->codr = remap_to_physical(0xFF);

    /* disable IO-pins */
    pio->per = remap_to_physical(0x00);

    cdev_del(&driver_cdev);
    release_region(AVR32_PIOB_ADDRESS, AVR32_PIOC_ADDRESS - AVR32_PIOB_ADDRESS);
    unregister_chrdev_region(device_number, 1);
    printk( KERN_INFO "Button driver stopped.\n");
}

/*****************************************************************************/
/* fops-funksjoner */

static int driver_open (struct inode *inode, struct file *filp) {
    printk( KERN_INFO "Button driver opened.\n");
    return 0;
}

/*---------------------------------------------------------------------------*/

static int driver_release (struct inode *inode, struct file *filp) {
    printk( KERN_INFO "Button driver released.\n");
    return 0;
}

/*---------------------------------------------------------------------------*/

static ssize_t driver_read (struct file *filp, char __user *buff,
        size_t count, loff_t *offp) {

    /* need to read the appropriate bit from PIOB PDSR */
    int buttons = pio->pdsr;
    printk( KERN_INFO "buttons read.\n");
    /* Should we implement debouncing and such here? */ 
    return buttons;
}

/*---------------------------------------------------------------------------*/

static ssize_t driver_write (struct file *filp, const char __user *buff,
        size_t count, loff_t *offp) {

    printk( KERN_INFO "buttons written to. buff: %c, count: %i, offp: %s\n", buff[0], count, offp);
    printk( KERN_INFO "why would you write to the buttons?");
    return 0;
}



/*****************************************************************************/
/* modul-beskrivelsesmakroer */

module_init (driver_init);  /* angir hva som er init-funksjon */
module_exit (driver_exit);  /* angir hva som er exit-funksjon */

MODULE_LICENSE ("GPL");     /* programlisens for modulen */
MODULE_DESCRIPTION ("");    /* tekstlig beskrivelse */
MODULE_VERSION ("");        /* versjonsnummer */
MODULE_AUTHOR ("");         /* forfatter(e) */
