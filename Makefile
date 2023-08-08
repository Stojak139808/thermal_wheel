# compiler / linker
CC       := avr-gcc
OBJ_COPY := avr-objcopy

# directories
BUILD_DIR := build
SRC_DIR := src
OBJ_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)

FIRMWARE_NAME := pump

HEX := $(BIN_DIR)/$(FIRMWARE_NAME).hex
ELF := $(BIN_DIR)/$(FIRMWARE_NAME).elf
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# compiler flags
CFLAGS := -mmcu=atmega328 -c -O
LFLAGS := -mmcu=atmega328 -O
OBJCOPYFLAGS := -O ihex

# programmer
PROG := avrdude
PROG_PORT := /dev/ttyACM0
PROGFLAGS := -c arduino -p m328p -P $(PROG_PORT)

.PHONY: all clean flash

all: $(HEX)

$(HEX): $(ELF)
	$(OBJ_COPY) $(OBJCOPYFLAGS) $^ $@

$(ELF): $(OBJ) | $(BIN_DIR)
	$(CC) $(LFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -o $@ $(CFLAGS) -c $<

$(BIN_DIR) $(OBJ_DIR) $(BUILD_DIR):
	mkdir -p $@

debug: CFLAGS+= -DDEBUG_BUILD
debug: $(HEX)

flash:
	sudo $(PROG) $(PROGFLAGS) -U flash:w:$(HEX):i

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

help:
	@echo "help   -   display this help"
	@echo "all    -   (default) build the target and place it in $(BIN_DIR)"
	@echo "flash  -   program the target"
	@echo "debug  -   build the debug version"

-include $(OBJ:.o=.d)
