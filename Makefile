ARCH        := i386
BINFORMAT   := elf
CROSSBIN    := /opt/elf-cross/bin/
CC          := $(CROSSBIN)/$(ARCH)-$(BINFORMAT)-gcc
CXX         := $(CROSSBIN)/$(ARCH)-$(BINFORMAT)-g++
CFLAGS      := -ffreestanding -nostdlib -DCXK_ARCH=$(ARCH)
SRC_DIR     := src
OBJ_DIR     := obj
INCLUDE_DIR := include
LDSCRIPT    := link.ld
KRNLPATH    := build/Kernel.elf
ASM	        := yasm
MBOOTLDR    := $(SRC_DIR)/mbootmain/mbootmain.asm
MBOOTOBJ    := $(OBJ_DIR)/mbootmain.o
LD	        := $(CROSSBIN)$(ARCH)-$(BINFORMAT)-ld

SOURCES := $(wildcard $(SRC_DIR)/*.asm)
SOURCES += $(wildcard $(SRC_DIR)/*.c)
SOURCES += $(wildcard $(SRC_DIR)/*.cpp)
SOURCES += $(wildcard $(SRC_DIR)/$(ARCH)/*.asm)
SOURCES += $(wildcard $(SRC_DIR)/$(ARCH)/*.c)
SOURCES += $(wildcard $(SRC_DIR)/$(ARCH)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%.o, $(SOURCES))

all: $(OBJECTS)
	$(ASM) -f elf $(MBOOTLDR) -o $(MBOOTOBJ)
	$(LD) -T $(LDSCRIPT) $(MBOOTOBJ) $^ -o $(KRNLPATH)
	qemu-system-i386 -kernel $(KRNLPATH)

$(OBJ_DIR)/%.c.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(SRC_DIR) -I$(INCLUDE_DIR) -c $< -o $@
$(OBJ_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CFLAGS) -I$(SRC_DIR) -I$(INCLUDE_DIR) -c $< -o $@
$(OBJ_DIR)/%.asm.o: $(SRC_DIR)/%.asm
	$(ASM) -f elf $< -o $@
								 
clean:
	rm obj/*
