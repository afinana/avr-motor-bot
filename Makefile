# Generic AVR Makefile supports several c files. 
# Example:
#
# make clean
# make all
# make upload
#
# Settings
#---------------------------------------------------
# Program name
PROG =motor
# c files
OBJS= main.c dc_motor.o adc.o term_io.o uart.o 
AVRDUDE=avrdude -F -V
OBJCOPY=avr-objcopy
CC=avr-gcc
RM=rm -f

# AVR parameters
MCU=atmega328
F_CPU=16000000UL
BIN_FORMAT=ihex
PROTOCOL=arduino
PART=atmega328p
CFLAGS=-Wall -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU)
LDFLAGS=
LIBS=

# Arduino COM / Linux dev file
PORT=COM4
BAUD=115200

# avrdude configuration file path 
CONFIG='\Development\arduino-1.8.5\hardware\tools\avr\etc\avrdude.conf'
# sudo user for linux system
SUDO=

.SUFFIXES: .elf .hex


.PHONY: all
all: ${PROG}.hex


${PROG}.hex: ${PROG}.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@


${PROG}.elf: ${OBJS}
	${CC} $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean
clean:
	$(RM) ${OBJS} ${PROG}.elf ${PROG}.hex

.PHONY: upload
upload: ${PROG}.hex
	${SUDO} $(AVRDUDE) -CC:$(CONFIG) -c$(PROTOCOL) -p$(PART) -P$(PORT) -b$(BAUD) -D -U flash:w:${PROG}.hex:i
