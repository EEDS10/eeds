insmod led_driver.ko
mknod /dev/leds c $(grep leds /proc/devices | awk '{print $1}') 0
insmod button_driver.ko
mknod /dev/buttons c $(grep button /proc/devices | awk '{print $1}') 0
            