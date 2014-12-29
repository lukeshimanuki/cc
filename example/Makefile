all:
	../cc < example.c > out.s
	as --gstabs -32 out.s -o out.o
	ld -o a.out -dynamic-linker /lib/ld-linux.so.2 /usr/lib/i386-linux-gnu/crt1.o /usr/lib/i386-linux-gnu/crti.o -lc out.o /usr/lib/i386-linux-gnu/crtn.o

clean:
	rm out.s out.o a.out

