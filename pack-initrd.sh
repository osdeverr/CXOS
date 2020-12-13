#!/bin/bash

cd initrd
COPYFILE_DISABLE=1 tar -cf ../build/initrd.tar *
cd ..
xxd -i build/initrd.tar > src/cx/os/kernel/initrd_tar.hpp
