CC = gcc

all: dir compile

dir:
	mkdir -p build

compile: build/read.o build/parse.o build/compile.o build/write.o build/symbol.o build/string.o build/scope.o build/main.o
#compile: build/read.o build/parse.o build/compile.o build/write.o build/main.o
	$(CC) $^ -o compile

build/main.o: src/main.c
	$(CC) -std=c99 -ggdb -c $< -o $@

build/read.o: src/read.c
	$(CC) -std=c99 -ggdb -c $< -o $@

build/parse.o: src/parse.c
	$(CC) -std=c99 -ggdb -c $< -o $@

build/compile.o: src/compile.c
	$(CC) -std=c99 -ggdb -c $< -o $@

build/write.o: src/write.c
	$(CC) -std=c99 -ggdb -c $< -o $@

build/symbol.o: src/symbol.c
	$(CC) -std=c99 -ggdb -c $< -o $@
	
build/string.o: src/string.c
	$(CC) -std=c99 -ggdb -c $< -o $@

build/scope.o: src/scope.c
	$(CC) -std=c99 -ggdb -c $< -o $@


.PHONY: clean

clean:
	rm build/*o
	rmdir build
	rm compile
