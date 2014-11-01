all: dir compile

dir:
	mkdir -p build

compile: build/read.o build/write.o build/main.o
#compile: build/read.o build/parse.o build/compile.o build/write.o build/main.o
	gcc $^ -o compile

build/main.o: src/main.c
	gcc -std=c99 -c $< -o $@

build/read.o: src/read.c
	gcc -std=c99 -c $< -o $@

build/parse.o: src/parse.c
	gcc -std=c99 -c $< -o $@

build/compile.o: src/compile.c
	gcc -std=c99 -c $< -o $@

build/write.o: src/write.c
	gcc -std=c99 -c $< -o $@

.PHONY: clean

clean:
	rm build/*o
	rmdir build
	rm compile
