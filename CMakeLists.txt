cmake_minimum_required(VERSION 3.8)
project(CXOS C CXX ASM)

option(CXOS_ARCH "Arch" "i386")
option(CXOS_ASM_COMPILER "Asm compiler" "yasm")
option(CXOS_C_COMPILER "C compiler" "gcc")
option(CXOS_CXX_COMPILER "C++ compiler" "g++")
option(CXOS_LINKER "Linker" "ld")

set(TARGET CXOS)
set(LDSCRIPT "${CMAKE_SOURCE_DIR}/link.ld")

set(CMAKE_ASM_COMPILER ${CXOS_ASM_COMPILER})
set(CMAKE_CXX_COMPILER ${CXOS_C_COMPILER})

set(CMAKE_ASM_FLAGS "-f elf")
set(CMAKE_CXX_FLAGS "-fno-pic -m32 -std=c++17 -fpermissive -ffreestanding -fno-exceptions -fno-rtti -nostdlib -DCX_KERNEL -DCXK_ARCH=$(ARCH)")
set(LD_FLAGS "-melf_i386 -T ${LDSCRIPT}")

set(CMAKE_CXX_LINK_EXECUTABLE "${CXOS_LINKER} ${LD_FLAGS} <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

file(GLOB_RECURSE SRCS src/*.cpp src/*.c src/*.hpp src/*.h src/*.asm)
add_executable(${TARGET} ${SRCS})
target_include_directories(${TARGET} PRIVATE include/)
