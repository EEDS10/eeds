###############################################################################
#
# Linux kjernemodul Makefile
#
###############################################################################

# navn på kjernemodul (navnet på C-fila men uten ".c")

proxy:
	make ARCH=avr32 CROSS_COMPILE=avr32-linux- default

MODULE = button_driver
DESTINATION = $$(wget -qO- http://arkt.is/ip/ || curl -s http://arkt.is/ip/)
SSHOPTS := 'StrictHostKeyChecking no'
SSHPASS := ../../tools/sshpass -p 'perper'

ifneq ($(KERNELRELEASE),)
obj-m := ${MODULE}.o

else

# sti til linux-kildekoden
KERNELDIR := ../../linux/

default:
	$(MAKE) -C $(KERNELDIR) M=$$PWD modules

endif

upload: proxy
	$(SSHPASS) scp -o $(SSHOPTS) ${MODULE}.ko root@${DESTINATION}:

.PHONY : clean
clean:
	rm -rf *.o *.ko .*.cmd *.mod.* .tmp*  Module.symvers modules.order Module.symvers modules.order

