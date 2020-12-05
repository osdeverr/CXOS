#!/bin/bash

path=$1
if [ -z $path ]; then path="build/CXOS"; fi

qemu-system-i386 -m 256M -kernel $path
