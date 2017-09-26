OS := $(shell uname)

COMPILER_PREFIX=${CROSS_PREFIX}

ifeq ($(OS),Darwin)
CC =${COMPILER_PREFIX}clang 
else
CC = ${COMPILER_PREFIX}gcc
endif

STRIP =${COMPILER_PREFIX}strip

$(info "${TARGET_ARCH}")

multicastsender: sender.o
	$(CC) sender.o -o sender
	$(STRIP) sender
	
sender.o:	sender.c
	$(CC) -c -Wall sender.c 
		
clean:	
	rm -rf *.o;rm -rf sender
