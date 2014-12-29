# =============================================================================
# The MIT License (MIT)
#
# Copyright (c) 2014 Luke Shimanuki
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ============================================================================

CC = gcc

all: dir compile

dir:
	mkdir -p build

compile: build/read.o build/parse.o build/compile.o build/write.o build/symbol.o build/string.o build/scope.o build/main.o
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
	rm -f compile
	rm -f build/*.o
	rmdir --ignore-fail-on-non-empty build

