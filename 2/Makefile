CC=avr32-gcc
CFLAGS=-Wall -Iinclude -O3
LDFLAGS=-Wall -pedantic -Llib -lavr32b-interrupts -lmodam -lm -O3
TARGET=oeving2.elf


# Command to upload program to flash 
AVR32PROGRAM = avr32program

####### RULES ##########################################################
.PHONY : all
all: $(TARGET)

oeving2.elf: oeving2.o leds.o buttons.o audio.o synth.o
	$(CC) $^ -o $@ $(LDFLAGS)

oeving2.o: oeving2.c 
	$(CC) $(CFLAGS) -c $< -o $@

buttons.o: buttons.c 
	$(CC) $(CFLAGS) -c $< -o $@

audio.o: audio.c 
	$(CC) $(CFLAGS) -c $< -o $@

leds.o: leds.c 
	$(CC) $(CFLAGS) -c $< -o $@

synth.o: synth.c
	$(CC) $(CFLAGS) -c $< -o $@

# upload to STK1000 (program flash)
.PHONY : upload
upload : $(TARGET)
	$(AVR32PROGRAM) halt
	sleep 3
	$(AVR32PROGRAM) program -v -e -f0,8Mb $<

debug : 
	avr32gdbproxy &
	sleep 3
	(echo target remote:4711;cat) | avr32-gdb oeving2.elf
	killall avr32gdbproxy

.PHONY : clean
clean :
	rm -rf *.o *.elf *.*~
