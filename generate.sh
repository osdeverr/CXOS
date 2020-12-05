#!/bin/bash

if [ -z "$CXOS_ARCH" ]; then echo "Fatal error: please specify CXOS_ARCH"; exit; fi
if [ -z "$CXOS_ASM_COMPILER" ]; then echo "Fatal error: please specify CXOS_ASM_COMPILER"; exit; fi
if [ -z "$CXOS_C_COMPILER" ]; then echo "Fatal error: please specify CXOS_C_COMPILER"; exit; fi
if [ -z "$CXOS_CXX_COMPILER" ]; then echo "Fatal error: please specify CXOS_CXX_COMPILER"; exit; fi
if [ -z "$CXOS_LINKER" ]; then echo "Fatal error: please specify CXOS_LINKER"; exit; fi

cmake \
	 -DCXOS_ARCH="$CXOS_ARCH" \
	 -DCXOS_ASM_COMPILER="$CXOS_ASM_COMPILER" \
	 -DCXOS_C_COMPILER="$CXOS_C_COMPILER" \
	 -DCXOS_CXX_COMPILER="$CXOS_CXX_COMPILER" \
	 -DCXOS_LINKER="$CXOS_LINKER" \
	 -S . -B build
