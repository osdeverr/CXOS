#!/bin/bash

path=$1
if [ -z $path ]; then path="build/CXOS"; fi

$CXOS_QEMU_PATH -m 256M -kernel $path
