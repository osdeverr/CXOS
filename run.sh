#!/bin/bash

source configs.shlib;

path=$1
if [ -z $path ]; then path="build/CXOS"; fi

"$(config_get CXOS_QEMU_PATH)" -m 256M -kernel $path
