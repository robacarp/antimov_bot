#TODO build support for more than one file.

SHELL := /bin/bash

# the name of your main .cpp file
#TARGET = `/bin/ls . | /usr/bin/grep pde | /usr/bin/sed 's/^\(.*\).pde$$/\1/'`
TARGET = Antimov

.PHONY: genheader
FUNCTIONTYPES = (void|int|unsigned int|bool|boolean|float|double)
genheader: binfolder
	@if [ ! -f $(TARGET).h ]; then \
		echo "making header file"; \
		grep "$(FUNCTIONTYPES).*(.*)" bin/$(TARGET).cpp ;\
	else \
		echo "copying header file"; \
	fi


# the serial port that the arduino stays on....unless you have >1 you shouldn't have to mess with this

PORT = `/bin/ls /dev | grep "tty.usbserial" | sed 's/^.*\(tty.usbserial.*\)/\/dev\/\1/'`
MCU = atmega328p
UPLOAD_RATE = 57600
F_CPU = 16000000
FORMAT = ihex

#must set these for your system
ARDUINO_DIR = /Applications/Arduino.app/Contents/Resources/Java
AVR_TOOLS = /usr/bin
AVRDUDE_PROGRAMMER = stk500v1

#these *might* work on all *nix systems
ARDUINO = $(ARDUINO_DIR)/hardware/arduino/cores/arduino
AVRDUDE_DIR = $(ARDUINO_DIR)/hardware/tools/avr/bin
AVRDUDE_CONFIG = $(ARDUINO_DIR)/hardware/tools/avr/etc/avrdude.conf

#TODO source all the files in the $ARDUINO directory...
SRC = $(ARDUINO)/pins_arduino.c $(ARDUINO)/wiring.c $(ARDUINO)/wiring_analog.c $(ARDUINO)/wiring_digital.c \
$(ARDUINO)/wiring_pulse.c $(ARDUINO)/wiring_shift.c $(ARDUINO)/WInterrupts.c

CXXSRC = $(ARDUINO)/HardwareSerial.cpp $(ARDUINO)/WMath.cpp $(ARDUINO)/Print.cpp

#program flags
CFLAGS = -g -DF_CPU=$(F_CPU) -I$(ARDUINO) -I. -Os -Wall -Wstrict-prototypes -std=gnu99 -mmcu=$(MCU)
CXXFLAGS = -DF_CPU=$(F_CPU) -I$(ARDUINO) -I. -Os -mmcu=$(MCU)
AVRDUDE_FLAGS = -C$(AVRDUDE_CONFIG) -p$(MCU) -c$(AVRDUDE_PROGRAMMER) -P$(PORT) -D -b$(UPLOAD_RATE)
LDFLAGS = -lm

# Programs used by the make file
CC = $(AVRDUDE_DIR)/avr-gcc
CXX = $(AVRDUDE_DIR)/avr-g++
OBJCOPY = $(AVRDUDE_DIR)/avr-objcopy
OBJDUMP = $(AVRDUDE_DIR)/avr-objdump
AR  = $(AVRDUDE_DIR)/avr-ar
NM = $(AVRDUDE_DIR)/avr-nm
RM = rm -f

# Define all object files.
OBJ = $(SRC:.c=.o) $(CXXSRC:.cpp=.o) $(ASRC:.S=.o)

# Default target.
all: pdefix bin/$(TARGET).hex

# Program the device
upload: bin/$(TARGET).hex
	$(AVRDUDE_DIR)/avrdude $(AVRDUDE_FLAGS) -Uflash:w:bin/$(TARGET).hex:i

#create the bin folder
binfolder:
	@echo -n "Creating bin folder."
	@if [ ! -d "bin" ]; then \
	mkdir bin; \
	echo "...success."; \
	else \
	echo "...no need."; \
	fi

pdefix: binfolder
	@echo -n "Compiling PDE -> CPP"
	@if [ -f "$(TARGET).pde" ]; then \
	echo "//AUTO GENERATED HEADER."; > bin/$(TARGET).cpp \
	echo "#include \"WProgram.h\"" > bin/$(TARGET).cpp; \
	echo "int main(){ init(); setup(); for(;;) {loop();} }" >> bin/$(TARGET).cpp; \
	cat $(TARGET).pde >> bin/$(TARGET).cpp; \
	echo "...Done"; \
	else \
	cat $(TARGET).cpp > bin/$(TARGET).cpp; \
	echo "...PDE not found"; \
	fi
	@cp *.c bin/
	@cp *.h bin/
	@cp *.cpp bin/

#Generally these will happen in this order.  More or less...

# Compile: create object files from C/C++ source files.
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

#Compile: create a library file called arduino_core.a, with all the .o files from the library
arduino_core.a: $(OBJ)
	@$(AR) rcs bin/arduino_core.a $(OBJ)

bin/$(TARGET).o: pdefix bin/$(TARGET).cpp

#Link: create ELF output file from library.  The -L. pulls out all the .o files from the current directory.
bin/$(TARGET).elf: bin/$(TARGET).o arduino_core.a
	$(CC) $(CFLAGS) -o $@ bin/$(TARGET).cpp -Lbin bin/arduino_core.a $(LDFLAGS)

#Assemble: make the .hex file that actually gets uploaded to the device
bin/$(TARGET).hex: bin/$(TARGET).elf
	$(OBJCOPY) -O ihex  -R .eeprom $< $@

clean:
	$(RM) -r bin

.PHONY: all binfolder pdefix local buildmedammit
