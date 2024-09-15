CC = gcc
AS = as
CFLAGS = -Wall -Wextra -g -I/usr/local/opt/libelf/include
LDFLAGS = -lm -L/usr/local/opt/libelf/lib -lelf

all: woody_woodpacker

woody_woodpacker: woody_woodpacker.o assembly.o elf_utils.o
	$(CC) $(CFLAGS) -o woody_woodpacker woody_woodpacker.o assembly.o elf_utils.o $(LDFLAGS)

woody_woodpacker.o: woody_woodpacker.c
	$(CC) $(CFLAGS) -c woody_woodpacker.c

elf_utils.o: elf_utils.c
	$(CC) $(CFLAGS) -c elf_utils.c

assembly.o: assembly.S
	$(AS) -o assembly.o assembly.S

clean:
	rm -f *.o woody_woodpacker woody

.PHONY: all clean
