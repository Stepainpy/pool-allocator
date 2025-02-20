.PHONY: all clean
TRASH += $(OBJECTS)
TRASH += $(wildcard *.exe)

SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst src/%.c,\
	bin/%.o,$(SOURCES))

CFLAGS += -Wall -Wextra -pedantic
CFLAGS += -std=c11 -O2 -Iinc
CC = cc

all: $(OBJECTS)
	$(CC) -o tp $^ $(CFLAGS)

bin/%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
# windows stuffs
ifneq ($(strip $(TRASH)),)
	del /Q $(subst /,\,$(TRASH))
endif