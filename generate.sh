#!/bin/bash

source configs.shlib;

cmake \
	 -DCXOS_ARCH="$(config_get CXOS_ARCH)" \
	 -DCXOS_ASM_COMPILER="$(config_get CXOS_ASM_COMPILER)" \
	 -DCXOS_C_COMPILER="$(config_get CXOS_C_COMPILER)" \
	 -DCXOS_CXX_COMPILER="$(config_get CXOS_CXX_COMPILER)" \
	 -DCXOS_LINKER="$(config_get CXOS_LINKER)" \
	 -S . -B build
