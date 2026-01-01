CC = arm-none-eabi-gcc
CFLAGS = -mcpu=cortex-m4 -mthumb -O0 -Wall
LDFLAGS = -T linker.ld -nostartfiles

all: main.elf

main.elf:
	$(CC) $(CFLAGS) Core/main.c Startup/startup_stm32f401cc.s $(LDFLAGS) -o main.elf

clean:
	rm -f *.elf
