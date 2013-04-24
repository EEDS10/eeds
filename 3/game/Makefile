C_FILES   := $(wildcard src/*.c)
OBJ_FILES := $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))
TESTS_C_FILES   := $(wildcard tests/*.c)
TESTS_OBJ_FILES := $(addprefix obj/tests/,$(notdir $(TESTS_C_FILES:.c=.o)))
LD_FLAGS := -lalleg -lm -O2
CC_FLAGS := -std=c99 -g -Iinclude/ -O2
SHELL=/bin/bash

SSHPASS := ../tools/sshpass -p 'perper' 

CC := gcc

bin/game: $(OBJ_FILES)
	   $(CC) -o $@ $^ $(LD_FLAGS)

obj/%.o: src/%.c
	   $(CC) $(CC_FLAGS) -c -o $@ $<

obj/tests/%.o: tests/%.c
	   $(CC) $(CC_FLAGS) $(patsubst obj/main.o,, $(OBJ_FILES)) $< $(LD_FLAGS) -lrt -o $(patsubst %.c, bin/%, $<)

run: bin/game
	bin/game

run-avr32: upload
	$(SSHPASS) ssh root@$$(wget -qO- http://arkt.is/ip/) -t 'cd game && bin/game'

debug: bin/game
	cgdb bin/game

avr32: 
	make CC=avr32-linux-gcc  CC_FLAGS=-"-std=c99 -g -D NO_ALLEGRO -D BIG_ENDIAN -Iinclude/" LD_FLAGS=" -lm "

upload: avr32
	$(SSHPASS) scp -p -r bin root@$$(wget -qO- http://arkt.is/ip/):game/

upload-all: avr32
	$(SSHPASS) scp -p -r bin res root@$$(wget -qO- http://arkt.is/ip/):game/
	
test: bin/game $(TESTS_OBJ_FILES)
	python tests/testrunner.py

clean:
	rm -r bin; mkdir bin; mkdir bin/tests
	rm -r obj; mkdir obj; mkdir obj/tests

connect:
	$(SSHPASS) ssh root@$$(wget -qO- http://arkt.is/ip/)
	
