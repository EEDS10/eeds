#!/bin/bash

avr32program program -vef0,8MB u-boot.bin
echo "Press RESET on the STK1000..."
read
minicom -o -D /dev/ttyS
