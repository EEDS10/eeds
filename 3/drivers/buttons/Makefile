###############################################################################
#
# Linux kjernemodul Makefile
#
###############################################################################

# navn på kjernemodul (navnet på C-fila men uten ".c")

proxy:
	make ARCH=avr32 CROSS_COMPILE=avr32-linux- default

MODULE = button_driver
DESTINATION = 129.241.103.244

ifneq ($(KERNELRELEASE),)
obj-m := ${MODULE}.o

else

# sti til linux-kildekoden
KERNELDIR := ../../linux/

default:
	$(MAKE) -C $(KERNELDIR) M=$$PWD modules

endif

upload: proxy
	scp ${MODULE}.ko root@${DESTINATION}:

.PHONY : clean
clean:
	rm -rf *.o *.ko .*.cmd *.mod.* .tmp*  Module.symvers modules.order Module.symvers modules.order

