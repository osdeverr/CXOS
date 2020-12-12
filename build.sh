#!/bin/bash

./generate.sh
./pack-initrd.sh
make -C ./build
