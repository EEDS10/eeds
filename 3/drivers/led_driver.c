/* taking some random stuff from http://tldp.org/LDP/lkmpg/2.6/html/x121.html */

#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>


int init_module(){
    printk(KERN_INFO, "Hello, world! :)\n");
	return 0;
}

void cleanup_module(){
    printk(KERN_INFO, "Goodbye, cruel world :(\n"); 
}

module_init(mymodule_init);
module_exit(mymodule_exit);

MODULE_LICENSE("All rights reserved, biatch.");
