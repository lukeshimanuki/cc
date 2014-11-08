#!/bin/sh
echo compiling
./compile $1
echo assembling
as -32 out.s -o out.o
echo linking
ld -o a.out -dynamic-linker /lib/ld-linux.so.2 /usr/lib/i386-linux-gnu/crt1.o /usr/lib/i386-linux-gnu/crti.o -lc out.o /usr/lib/i386-linux-gnu/crtn.o
echo done
