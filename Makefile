# =====================================================
# TARGET CONFIG (AVR)
# =====================================================
MCU=atmega328p
F_CPU=16000000UL
CC=avr-gcc

# =====================================================
# HOST CONFIG (PC SIMULATION)
# =====================================================
HOST_CC=gcc
HOST_DIR=host

# =====================================================
# DIRECTORIES
# =====================================================
SRC_DIR=src

# =====================================================
# SOURCE FILES (AVR - AUTO)
# =====================================================
SRC := $(shell find $(SRC_DIR) -name '*.c')

# =====================================================
# SOURCE FILES (HOST)
# =====================================================
HOST_SRC := \
	$(HOST_DIR)/main_host.c \
	$(HOST_DIR)/uart_mock.c \
	$(HOST_DIR)/stubs.c \
	src/app/cmd/cmd.c \
	src/app/cmd/cmd_led.c \
	src/app/cmd/cmd_state.c \
	src/app/cmd/cmd_debug.c \
	src/utils/log.c


# =====================================================
# FLAGS (AVR)
# =====================================================
CFLAGS=-Wall -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU)
CFLAGS+=-I$(SRC_DIR)/config
CFLAGS+=-I$(SRC_DIR)/utils
CFLAGS+=-I$(SRC_DIR)/app
CFLAGS+=-I$(SRC_DIR)/app/event
CFLAGS+=-I$(SRC_DIR)/app/scheduler
CFLAGS+=-I$(SRC_DIR)/app/cmd
CFLAGS+=-I$(SRC_DIR)/drivers/gpio
CFLAGS+=-I$(SRC_DIR)/drivers/led
CFLAGS+=-I$(SRC_DIR)/drivers/button
CFLAGS+=-I$(SRC_DIR)/drivers/timer
CFLAGS+=-I$(SRC_DIR)/drivers/led_pattern
CFLAGS+=-I$(SRC_DIR)/drivers/uart

# =====================================================
# FLAGS (DEBUG AVR)
# =====================================================
CFLAGS_DEBUG=-Wall -Og -g -mmcu=$(MCU) -DF_CPU=$(F_CPU)
CFLAGS_DEBUG+=-I$(SRC_DIR)/config
CFLAGS_DEBUG+=-I$(SRC_DIR)/utils
CFLAGS_DEBUG+=-I$(SRC_DIR)/app
CFLAGS_DEBUG+=-I$(SRC_DIR)/app/event
CFLAGS_DEBUG+=-I$(SRC_DIR)/app/scheduler
CFLAGS_DEBUG+=-I$(SRC_DIR)/app/cmd
CFLAGS_DEBUG+=-I$(SRC_DIR)/drivers/gpio
CFLAGS_DEBUG+=-I$(SRC_DIR)/drivers/led
CFLAGS_DEBUG+=-I$(SRC_DIR)/drivers/button
CFLAGS_DEBUG+=-I$(SRC_DIR)/drivers/timer
CFLAGS_DEBUG+=-I$(SRC_DIR)/drivers/led_pattern
CFLAGS_DEBUG+=-I$(SRC_DIR)/drivers/uart

# =====================================================
# FLAGS (HOST)
# =====================================================
HOST_CFLAGS=-Wall -Og -g
HOST_CFLAGS+=-I$(SRC_DIR)/config
HOST_CFLAGS+=-I$(SRC_DIR)/utils
HOST_CFLAGS+=-I$(SRC_DIR)/app
HOST_CFLAGS+=-I$(SRC_DIR)/app/event
HOST_CFLAGS+=-I$(SRC_DIR)/app/scheduler
HOST_CFLAGS+=-I$(SRC_DIR)/app/cmd
HOST_CFLAGS+=-I$(SRC_DIR)/drivers/uart
HOST_CFLAGS+=-Isrc/drivers/led
HOST_CFLAGS+=-Isrc/drivers/led_pattern
HOST_CFLAGS+=-Isrc/drivers/gpio


# =====================================================
# OUTPUT
# =====================================================
TARGET=main
HOST_TARGET=host_app

PORT=/dev/ttyUSB0
PROGRAMMER=arduino
BAUD=57600

# =====================================================
# AVR BUILD
# =====================================================
all:
	$(CC) $(CFLAGS) -o $(TARGET).elf $(SRC)
	avr-objcopy -O ihex $(TARGET).elf $(TARGET).hex

# =====================================================
# AVR DEBUG BUILD
# =====================================================
debug:
	$(CC) $(CFLAGS_DEBUG) -o $(TARGET).elf $(SRC)
	avr-objcopy -O ihex $(TARGET).elf $(TARGET).hex

# =====================================================
# FLASH TO MCU
# =====================================================
flash: all
	avrdude -c $(PROGRAMMER) -p m328p -P $(PORT) -b $(BAUD) \
	-U flash:w:$(TARGET).hex

# =====================================================
# HOST BUILD (NO MCU)
# =====================================================
host:
	$(HOST_CC) $(HOST_CFLAGS) -o $(HOST_TARGET) $(HOST_SRC)

# =====================================================
# CLEAN
# =====================================================
clean:
	rm -f *.elf *.hex $(HOST_TARGET)

.PHONY: all debug flash clean host
