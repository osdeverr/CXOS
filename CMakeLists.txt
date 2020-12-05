cmake_minimum_required(VERSION 3.8)
project(CXOS C CXX ASM)

option(CXOS_ARCH "Arch" "i386")
option(CXOS_ASM_COMPILER "Asm compiler" "yasm")
option(CXOS_C_COMPILER "C compiler" "gcc")
option(CXOS_CXX_COMPILER "C++ compiler" "g++")
option(CXOS_LINKER "Linker" "ld")

function(group_files_by_directory)
	cmake_parse_arguments(GROUP_FILES "" "" "SRCS" ${ARGN})

	foreach(FILE ${GROUP_FILES_SRCS})
		# Get the directory of the source file
		get_filename_component(PARENT_DIR ${FILE} DIRECTORY)

		# Remove common directory prefix to make the group
		string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR} "" GROUP ${PARENT_DIR})

		# Make sure we are using windows slashes
		string(REPLACE "/" "\\" GROUP ${GROUP})

		# Group into "Source Files" and "Header Files"
		#    if ("${FILE}" MATCHES ".*\\.cpp")
		#       set(GROUP "Source Files${GROUP}")
		#    elseif("${FILE}" MATCHES ".*\\.h")
		#       set(GROUP "Header Files${GROUP}")
		#    endif()

		source_group(${GROUP} FILES ${FILE})
	endforeach()
endfunction()

function(dir_to_var_recurse PATH SOURCES)
	file(GLOB_RECURSE TMP_SRC ${PATH}/*.cpp ${PATH}/*.c ${PATH}/*.hpp ${PATH}/*.h ${PATH}/*.asm)
	group_files_by_directory(SRCS ${TMP_SRC})
	set(${SOURCES} ${TMP_SRC} PARENT_SCOPE)
endfunction()


set(TARGET CXOS)
set(LDSCRIPT "${CMAKE_SOURCE_DIR}/link.ld")

set(CMAKE_ASM_COMPILER ${CXOS_ASM_COMPILER})
set(CMAKE_CXX_COMPILER ${CXOS_C_COMPILER})

set(CMAKE_ASM_FLAGS "-f elf")
set(CMAKE_CXX_FLAGS "-fno-pic -m32 -std=c++17 -fpermissive -ffreestanding -fno-exceptions -fno-rtti -nostdlib -DCX_KERNEL -DCXK_ARCH=$(ARCH)")
set(LD_FLAGS "-melf_i386 -T ${LDSCRIPT}")

set(CMAKE_CXX_LINK_EXECUTABLE "${CXOS_LINKER} ${LD_FLAGS} <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

dir_to_var_recurse(src SRCS)
add_executable(${TARGET} ${SRCS})
target_include_directories(${TARGET} PRIVATE include/)
