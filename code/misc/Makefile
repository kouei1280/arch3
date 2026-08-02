cc = gcc
CFLAGS = -Wall -g

all: sample

sample: sample.c calculate.c inst_decode.c ../MemoryModule/MemoryModule.c
	$(CC) $(CFLAGS) -o sample sample.c calculate.c inst_decode.c ../MemoryModule/MemoryModule.c

module: 
	make -C ../MemoryModule/

module_clean:
	make -C ../MemoryModule/ clean

clean: 
	rm -f sample sample.o calculate.o inst_decode.o	