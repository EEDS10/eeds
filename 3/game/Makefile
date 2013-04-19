C_FILES   := $(wildcard src/*.c)
OBJ_FILES := $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))
LD_FLAGS := -lalleg
CC_FLAGS := -g -Iinclude/

CC := gcc

bin/game: $(OBJ_FILES)
	   $(CC) -o $@ $^ $(LD_FLAGS)

obj/%.o: src/%.c
	   $(CC) $(CC_FLAGS) -c -o $@ $<

run: bin/game
	bin/game

debug: bin/game
	cgdb bin/game
	

clean:
	rm -r bin; mkdir bin
	rm -r obj; mkdir obj
