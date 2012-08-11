## fuses d7 d8 fc
# 4c normal lock
# 40 total lock

#curl http://seb.exse.net/ledtest.hex > ledtest.hex && avrflash.sh  m168 -F -v -v  -U flash:w:ledtest.hex

#MCU = atmega88p
#MCU = atmega328p

#MCU = atmega644
F_CPU = 8000000
#F_CPU = 20000000
QUARZ = 8MHZ

#internal RC/8
#FUSE_SETTINGS = -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xf9:m 

# default, except internal isn not RC/8 but instead RC = 8MHz
FUSE_SETTINGS = -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0x01:m 

#FUSE_SETTINGS = -U lfuse:w:0xd6:m -U hfuse:w:0xdf:m
#-U lfuse:w:0xD6:m -U hfuse:w:0xDF:m -U efuse:w:0x1:m
#FUSE_SETTINGS = -U lfuse:w:0xF7:m -U hfuse:w:0xDD:m -U efuse:w:0x1:m


# Output format. (can be srec, ihex, binary)
FORMAT = ihex

TARGET = pentabug


# Optimization level, can be [0, 1, 2, 3, s]. 0 turns off optimization.
# (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
OPT = s

##########################################################################################################
# List C source files here. (C dependencies are automatically generated.)
SRC	= main.c synth.c usart.c
##########################################################################################################


# List Assembler source files here.
# Make them always end in a capital .S.  Files ending in a lowercase .s
# will not be considered source files but generated files (assembler
# output from the compiler), and will be deleted upon "make clean"!
# Even though the DOS/Win* filesystem matches both .s and .S the same,
# it will preserve the spelling of the filenames, and gcc itself does
# care about how the name is spelled on its command-line.
ASRC = 



# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
EXTRAINCDIRS = 


# Optional compiler flags.
#  -g:        generate debugging information (for GDB, or for COFF conversion)
#  -O*:       optimization level
#  -f...:     tuning, see gcc manual and avr-libc documentation
#  -Wall...:  warning level
#  -Wa,...:   tell GCC to pass this to the assembler.
#    -ahlms:  create assembler listing
CFLAGS  = -O$(OPT) -g
CFLAGS += -funsigned-char
CFLAGS += -funsigned-bitfields
CFLAGS += -fpack-struct
CFLAGS += -fshort-enums
CFLAGS += -Wall
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wa,-adhlns=$(<:.c=.lst)
CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))


# Compiler flag to set the C Standard level.
#   Unremark just one line below to set the language standard to use.
#     c89   = "ANSI" C
#     gnu89 = c89 plus GCC extensions
#     c99   = ISO C99 standard (not yet fully implemented)
#     gnu99 = c99 plus GCC extensions
#CFLAGS += -std=c89
#CFLAGS += -std=gnu89
#CFLAGS += -std=c99
CFLAGS += -std=gnu99

CFLAGS += -DADDR=$(ADDR)
CFLAGS += -DF_CPU=$(F_CPU)
CFLAGS += -DQUARZ=$(QUARZ)
CFLAGS += -DADDR=$(ADDR)



# Optional assembler flags.
#  -Wa,...:   tell GCC to pass this to the assembler.
#  -ahlms:    create listing
#  -gstabs:   have the assembler create line number information; note that
#             for use in COFF files, additional information about filenames
#             and function names needs to be present in the assembler source
#             files -- see avr-libc docs [FIXME: not yet described there]
ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs 



# Optional linker flags.
#  -Wl,...:   tell GCC to pass this to linker.
#  -Map:      create map file
#  --cref:    add cross reference to  map file
LDFLAGS = -Wl,-Map=$(TARGET).map,--cref

# Additional libraries

# Minimalistic printf version
#LDFLAGS += -Wl,-u,vfprintf -lprintf_min

# Floating point printf version (requires -lm below)
#LDFLAGS += -Wl,-u,vfprintf -lprintf_flt

# -lm = math library
LDFLAGS += -lm 
#-Wl,--section-start=.text=0x1800


##LDFLAGS += -T./linkerfile/avr5.x



# Programming support using avrdude. Settings and variables.

# Programming hardware:
#
# Type: avrdude -c ?
# to get a full listing.
#

 # serial bootloader via USART
AVRDUDE_SER_PROGRAMMER = avr109 -b 38400


#johns programmer:
AVRDUDE_JS_PROGRAMMER = stk500v2
# USBTINY via ISP
AVRDUDE_PROGRAMMER = usbtiny 
#falls Ponyser ausgewählt wird, muss sich unsere avrdude-Configdatei im Bin-Verzeichnis des Compilers befinden

#AVRDUDE_PORT = com1    # programmer connected to serial device
#AVRDUDE_PORT = lpt1    # programmer connected to parallel port
AVRDUDE_PORT =      # programmer connected to USB

AVRDUDE_PORT_SER = /dev/ttyUSB0     # programmer connected to USB

AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex 
AVRDUDE_WRITE_FUSE = $(FUSE_SETTINGS)
AVRDUDE_WRITE_EEPROM = -U eeprom:w:$(TARGET).eep

#avrdude -c avrispv2 -P usb -p m32 -U flash:w:blink.hex
#AVRDUDE_FLAGS = -p m168 -F -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER)
AVRDUDE_FLAGS = -p $(MCU) -F -c $(AVRDUDE_PROGRAMMER) -v -v 
AVRDUDE_JOFLAGS = -p $(MCU) -F  -P /dev/serial/by-id/usb-FTDI_USB__-__Serial-if00-port0   -c $(AVRDUDE_JS_PROGRAMMER) -v -v 
AVRDUDE_FLAGS_SER = -p $(MCU) -F -P $(AVRDUDE_PORT_SER) $(AVRDUDE_PROGRAMMER_SER) -v -v 
#AVRDUDE_UFLAGS = -p atmega88 -F -P /dev/ttyUSB0 -c avr109 -b 38400  -v -v 
#AVRDUDE_UFLAGS = -p $(MCU) -P /dev/cu.usbserial-A2002QDi -c avr109 -b 230400  -v -v 
AVRDUDE_UFLAGS = -p $(MCU) -F -P /dev/cu.usbserial-A4016EMT -c avr109 -b 38400  -v -v 
#AVRDUDE_UFLAGS = -p $(MCU) -F -P /dev/ttyUSB0 -c avr109 -b 38400  -v -v 
#AVRDUDE_FLAGS = -p $(MCU) -c $(AVRDUDE_PROGRAMMER)

# Uncomment the following if you want avrdude's erase cycle counter.
# Note that this counter needs to be initialized first using -Yn,
# see avrdude manual.
#AVRDUDE_ERASE += -y

# Uncomment the following if you do /not/ wish a verification to be
# performed after programming the device.
#AVRDUDE_FLAGS += -V

# Increase verbosity level.  Please use this when submitting bug
# reports about avrdude. See <http://savannah.nongnu.org/projects/avrdude> 
# to submit bug reports.
#AVRDUDE_FLAGS += -v -v

# ---------------------------------------------------------------------------
# Define directories, if needed.
#DIRAVR = c:/winavr
#DIRAVRBIN = $(DIRAVR)/bin
#DIRAVRUTILS = $(DIRAVR)/utils/bin
#DIRINC = .
#DIRLIB = $(DIRAVR)/avr/lib


# Define programs and commands.
SHELL = sh

CC = avr-gcc

OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size

# Programming support using avrdude.
AVRDUDE = avrdude

REMOVE = rm -f
COPY = cp

HEXSIZE = $(SIZE) --target=$(FORMAT) $(TARGET).hex
##ELFSIZE = $(SIZE) -A $(TARGET).elf
ELFSIZE = $(SIZE) --mcu=$(MCU) --format=avr $(TARGET).elf

# Define Messages
# English
MSG_ERRORS_NONE = Errors: none
MSG_BEGIN = -------- begin --------
MSG_END = --------  end  --------
MSG_SIZE_BEFORE = Size before: 
MSG_SIZE_AFTER = Size after:
MSG_COFF = Converting to AVR COFF:
MSG_EXTENDED_COFF = Converting to AVR Extended COFF:
MSG_FLASH = Creating load file for Flash:
MSG_EEPROM = Creating load file for EEPROM:
MSG_EXTENDED_LISTING = Creating Extended Listing:
MSG_SYMBOL_TABLE = Creating Symbol Table:
MSG_LINKING = Linking:
MSG_COMPILING = Compiling:
MSG_ASSEMBLING = Assembling:
MSG_CLEANING = Cleaning project:


# Define all object files.
OBJ = $(SRC:.c=.o) $(ASRC:.S=.o) 

# Define all listing files.
LST = $(SRC:.c=.lst) $(ASRC:.S=.lst)

# Combine all necessary flags and optional flags.
# Add target processor to flags.
#ALL_CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -I. $(CFLAGS)
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS) 
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)


# Default target.
all: begin gccversion sizebefore $(TARGET).elf $(TARGET).hex $(TARGET).eep \
	$(TARGET).lss $(TARGET).sym sizeafter finished end


# Eye candy.
# AVR Studio 3.x does not check make's exit code but relies on
# the following magic strings to be generated by the compile job.
begin:
	@echo
	@echo $(MSG_BEGIN)

finished:
	@echo $(MSG_ERRORS_NONE)

end:
	@echo $(MSG_END)
	@echo


# Display size of file.
sizebefore:
	@if [ -f $(TARGET).elf ]; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE); echo; fi

sizeafter:
	@if [ -f $(TARGET).elf ]; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); echo; fi



# Display compiler version information.
gccversion : 
	@$(CC) --version


# Convert ELF to COFF for use in debugging / simulating in
# AVR Studio or VMLAB.
COFFCONVERT=$(OBJCOPY) --debugging \
	--change-section-address .data-0x800000 \
	--change-section-address .bss-0x800000 \
	--change-section-address .noinit-0x800000 \
	--change-section-address .eeprom-0x810000 


coff: $(TARGET).elf
	@echo
	@echo $(MSG_COFF) $(TARGET).cof
	$(COFFCONVERT) -O coff-avr $< $(TARGET).cof


extcoff: $(TARGET).elf
	@echo
	@echo $(MSG_EXTENDED_COFF) $(TARGET).cof
	$(COFFCONVERT) -O coff-ext-avr $< $(TARGET).cof




# Program the device.

serflash: $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS_SER) $(AVRDUDE_WRITE_FLASH)


flash: $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)

uflash: $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_UFLAGS) $(AVRDUDE_WRITE_FLASH)

fljohn: $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_JOFLAGS) $(AVRDUDE_WRITE_FLASH)

eeprom: $(TARGET).eep
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_EEPROM)

fuse:
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FUSE)

reset:
	$(AVRDUDE) $(AVRDUDE_FLAGS)


# Create final output files (.hex, .eep) from ELF output file.
%.hex: %.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

%.eep: %.elf
	@echo
	@echo $(MSG_EEPROM) $@
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O $(FORMAT) $< $@

# Create extended listing file from ELF output file.
%.lss: %.elf
	@echo
	@echo $(MSG_EXTENDED_LISTING) $@
	$(OBJDUMP) -h -S $< > $@

# Create a symbol table from ELF output file.
%.sym: %.elf
	@echo
	@echo $(MSG_SYMBOL_TABLE) $@
	avr-nm -n $< > $@


# Link: create ELF output file from object files.
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(OBJ)
%.elf: $(OBJ)
	@echo
	@echo $(MSG_LINKING) $@
	$(CC) $(ALL_CFLAGS) $(OBJ) --output $@ $(LDFLAGS)


# Compile: create object files from C source files.
%.o : %.c
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(ALL_CFLAGS) $< -o $@


# Compile: create assembler files from C source files.
%.s : %.c
	$(CC) -S $(ALL_CFLAGS) $< -o $@


# Assemble: create object files from assembler source files.
%.o : %.S
	@echo
	@echo $(MSG_ASSEMBLING) $<
	$(CC) -c $(ALL_ASFLAGS) $< -o $@


# Target: clean project.
clean: begin clean_list finished end

clean_list :
	@echo
	@echo $(MSG_CLEANING)
	$(REMOVE) $(TARGET).hex
	$(REMOVE) $(TARGET).eep
	$(REMOVE) $(TARGET).obj
	$(REMOVE) $(TARGET).cof
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).map
	$(REMOVE) $(TARGET).obj
	$(REMOVE) $(TARGET).a90
	$(REMOVE) $(TARGET).sym
	$(REMOVE) $(TARGET).lnk
	$(REMOVE) $(TARGET).lss
	$(REMOVE) $(OBJ)
	$(REMOVE) $(LST)
	$(REMOVE) $(SRC:.c=.s)
	$(REMOVE) $(SRC:.c=.d)
	@echo


# Automatically generate C source code dependencies. 
# (Code originally taken from the GNU make user manual and modified 
# (See README.txt Credits).)
#
# Note that this will work with sh (bash) and sed that is shipped with WinAVR
# (see the SHELL variable defined above).
# This may not work with other shells or other seds.
#
%.d: %.c
	@set -e; $(CC) -MM $(ALL_CFLAGS) $< \
	| sed 's,\(.*\)\.o[ :]*,\1.o \1.d : ,g' > $@; \
	[ -s $@ ] || rm -f $@


# Remove the '-' if you want to see the dependency files generated.
-include $(SRC:.c=.d)


# Listing of phony targets.
.PHONY : all begin finish end sizebefore sizeafter gccversion coff extcoff clean clean_list flash eeprom fuse reset
