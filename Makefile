
# Compilation flags for gdb
CFLAGS  = -O1 -g
ASFLAGS = -g 

# Object files
OBJS  = main.o
OBJS += startup_stm32f10x.o system_stm32f10x.o 
OBJS += stm32f10x_gpio.o stm32f10x_rcc.o
OBJS += stm32f10x_usart.o
OBJS += xprintf.o xuart.o
OBJS += encoder.o board_C8.o
#AO!:
#LDFLAGS_POST = -lm #Link math. Not needed for this project.

ELF=$(notdir $(CURDIR)).elf
MAP_FILE=$(notdir $(CURDIR)).map  #AO!
BIN_FILE=$(notdir $(CURDIR)).bin  #AO!


# Library path
LIBROOT=/home/onat/elektronik/ARM/Compiler/STM32F10x_StdPeriph_Lib_V3.5.0

# Tools
CC=arm-none-eabi-gcc
LD=arm-none-eabi-gcc
AR=arm-none-eabi-ar
AS=arm-none-eabi-as
OBJCOPY=arm-none-eabi-objcopy

# Tool path (Already on BASH path.)
#TOOLROOT=/home/onat/elektronik/ARM/Compiler/gcc-arm-none-eabi-4_9-2015q2/bin/

#CC=$(TOOLROOT)/arm-none-eabi-gcc
#LD=$(TOOLROOT)/arm-none-eabi-gcc
#AR=$(TOOLROOT)/arm-none-eabi-ar
#AS=$(TOOLROOT)/arm-none-eabi-as

# Code Paths

DEVICE=$(LIBROOT)/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x
CORE=$(LIBROOT)/Libraries/CMSIS/CM3/CoreSupport
PERIPH=$(LIBROOT)/Libraries/STM32F10x_StdPeriph_Driver

# Search path for peripheral library
vpath %.c $(CORE)
vpath %.c $(PERIPH)/src
vpath %.c $(DEVICE)


#  Processor  specific
#Note: Change the processor type. This defines the Proc. clock in the:
#STM32F10x_StdPeriph_Lib_V3.5.0/Libraries/CMSIS/CM3/Device
#Support/ST/STM32F10x/system_stm32f10x.c file which is eventually included.
PTYPE = STM32F10X_MD
#PTYPE = STM32F10X_MD_VL

#LDSCRIPT = stm32f100.ld
LDSCRIPT = stm32f103.ld

# Compilation Flags
FULLASSERT = -DUSE_FULL_ASSERT 


LDFLAGS+= -T$(LDSCRIPT) -mthumb -mcpu=cortex-m3 -Wl,-Map=$(MAP_FILE)
CFLAGS+= -mcpu=cortex-m3 -mthumb 
CFLAGS+= -I$(DEVICE) -I$(CORE) -I$(PERIPH)/inc -I.
CFLAGS+= -D$(PTYPE) -DUSE_STDPERIPH_DRIVER $(FULLASSERT)

OBJCOPYFLAGS = -O binary

# Build executable 

$(BIN_FILE) : $(ELF) #AO!
	$(OBJCOPY) $(OBJCOPYFLAGS) $< $@

$(ELF) : $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS) $(LDFLAGS_POST)


# compile and generate dependency info

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@
	$(CC) -MM $(CFLAGS) $< > $*.d

%.o: %.s
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJS) $(OBJS:.o=.d) $(ELF) $(MAP_FILE) $(CLEANOTHER) #AO!

debug: $(ELF)
	arm-none-eabi-gdb $(ELF)


# pull in dependencies

-include $(OBJS:.o=.d)
