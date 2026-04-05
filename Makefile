MCU=atmega328p
F_CPU=16000000UL
CC=avr-gcc

# -----------------------------
# Directories
# -----------------------------
SRC_DIR=src

# -----------------------------
# Source files (AUTO)
# -----------------------------
SRC := $(shell find $(SRC_DIR) -name '*.c')

# -----------------------------
# Flags
# -----------------------------
CFLAGS=-Wall -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU)
CFLAGS+=-I$(SRC_DIR)/app
CFLAGS+=-I$(SRC_DIR)/drivers/gpio
CFLAGS+=-I$(SRC_DIR)/drivers/led

CFLAGS_DEBUG=-Wall -Og -g -mmcu=$(MCU) -DF_CPU=$(F_CPU)
CFLAGS_DEBUG+=-I$(SRC_DIR)/app
CFLAGS_DEBUG+=-I$(SRC_DIR)/drivers/gpio
CFLAGS_DEBUG+=-I$(SRC_DIR)/drivers/led

# -----------------------------
# Output
# -----------------------------
TARGET=main

PORT=/dev/ttyUSB0
PROGRAMMER=arduino
BAUD=57600

# -----------------------------
# Build
# -----------------------------
all:
	$(CC) $(CFLAGS) -o $(TARGET).elf $(SRC)
	avr-objcopy -O ihex $(TARGET).elf $(TARGET).hex

debug:
	$(CC) $(CFLAGS_DEBUG) -o $(TARGET).elf $(SRC)
	avr-objcopy -O ihex $(TARGET).elf $(TARGET).hex

flash: all
	avrdude -c $(PROGRAMMER) -p m328p -P $(PORT) -b $(BAUD) \
	-U flash:w:$(TARGET).hex

clean:
	rm -f *.elf *.hex
