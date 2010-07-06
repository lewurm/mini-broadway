#!/bin/sh
cd libbroadway
if [ "$1" = "clean" ]; then
	make clean
fi
make
make install
cd ../libdiskmii
if [ "$1" = "clean" ]; then
	make clean
fi
make
make install
cd ../libhextwelve
if [ "$1" = "clean" ]; then
	make clean
fi
make
make install
cd ../libcereal
if [ "$1" = "clean" ]; then
	make clean
fi
make
make install
cd ../mini
if [ "$1" = "clean" ]; then
	make clean
fi
make
cd ../ppcskel
if [ "$1" = "clean" ]; then
	make clean
fi
make 
cd ../
cp ppcskel/ppcboot.elf ./
cp mini/armboot.bin ./
