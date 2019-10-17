# File:   Makefile
# Authors: Daniel Siemers (45944203) & Ann Ngo (39979230)
# Date:   18 October 2019
# Descr:  Makefile for HideAndSeek

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -I../../drivers -I../../fonts -I../../drivers/avr -I../../utils
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: HideAndSeek.out


# Compile: create object files from C source files.
HideAndSeek.o: HideAndSeek.c ../../drivers/avr/system.h ../../utils/pacer.h ../../drivers/navswitch.h ../../utils/tinygl.h ../../drivers/avr/ir_uart.h ../../drivers/led.h HideAndSeek.h nav_controller.h ir_controller.h score_calc.h board_display.h
	$(CC) -c $(CFLAGS) $< -o $@

nav_controller.o: nav_controller.c ../../utils/pacer.h ../../utils/tinygl.h ../../drivers/navswitch.h nav_controller.h ir_controller.h board_display.h
	$(CC) -c $(CFLAGS) $< -o $@

ir_controller.o: ir_controller.c ../../drivers/avr/ir_uart.h ir_controller.h
	$(CC) -c $(CFLAGS) $< -o $@

score_calc.o: score_calc.c score_calc.h
	$(CC) -c $(CFLAGS) $< -o $@

board_display.o: board_display.c ../../utils/tinygl.h ../../utils/pacer.h ../../drivers/navswitch.h ../../drivers/avr/ir_uart.h board_display.h
	$(CC) -c $(CFLAGS) $< -o $@

pio.o: ../../drivers/avr/pio.c ../../drivers/avr/pio.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: ../../drivers/avr/system.c ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

led.o: ../../drivers/led.c ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/led.h
	$(CC) -c $(CFLAGS) $< -o $@

timer.o: ../../drivers/avr/timer.c ../../drivers/avr/system.h ../../drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@

pacer.o: ../../utils/pacer.c ../../drivers/avr/system.h ../../drivers/avr/timer.h ../../utils/pacer.h
	$(CC) -c $(CFLAGS) $< -o $@

ir_uart.o: ../../drivers/avr/ir_uart.c ../../drivers/avr/ir_uart.h ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/avr/timer0.h ../../drivers/avr/usart1.h
	$(CC) -c $(CFLAGS) $< -o $@

prescale.o: ../../drivers/avr/prescale.c ../../drivers/avr/prescale.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

timer0.o: ../../drivers/avr/timer0.c ../../drivers/avr/bits.h ../../drivers/avr/prescale.h ../../drivers/avr/system.h ../../drivers/avr/timer0.h
	$(CC) -c $(CFLAGS) $< -o $@

usart1.o: ../../drivers/avr/usart1.c ../../drivers/avr/system.h ../../drivers/avr/usart1.h
	$(CC) -c $(CFLAGS) $< -o $@

display.o: ../../drivers/display.c ../../drivers/avr/system.h ../../drivers/display.h ../../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

ledmat.o: ../../drivers/ledmat.c ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

navswitch.o: ../../drivers/navswitch.c ../../drivers/avr/delay.h ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/navswitch.h
	$(CC) -c $(CFLAGS) $< -o $@

font.o: ../../utils/font.c ../../drivers/avr/system.h ../../utils/font.h
	$(CC) -c $(CFLAGS) $< -o $@

tinygl.o: ../../utils/tinygl.c ../../drivers/avr/system.h ../../drivers/display.h ../../utils/font.h ../../utils/tinygl.h
	$(CC) -c $(CFLAGS) $< -o $@


# Link: create ELF output file from object files.
HideAndSeek.out: HideAndSeek.o nav_controller.o ir_controller.o score_calc.o board_display.o pio.o system.o led.o timer.o pacer.o ir_uart.o prescale.o system.o timer0.o usart1.o display.o ledmat.o navswitch.o font.o tinygl.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@


# Target: clean project.
.PHONY: clean
clean:
	-$(DEL) *.o *.out *.hex


# Target: program project.
.PHONY: program
program: HideAndSeek.out
	$(OBJCOPY) -O ihex HideAndSeek.out HideAndSeek.hex
	dfu-programmer atmega32u2 erase; dfu-programmer atmega32u2 flash HideAndSeek.hex; dfu-programmer atmega32u2 start


