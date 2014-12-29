/* =============================================================================
 * @file main.c
 * @author Luke Shimanuki
 * @date 28 Dec 2014
 * @brief Implementation for the main function.
 *
 * -----------------------------------------------------------------------------
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 Luke Shimanuki
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * ========================================================================== */

#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "symbol.h"

/***************************************************************************//**
 * This is executed on startup. It processes C code from the standard input and
 * writes assembly code (i386, GAS syntax) to the standard output.
 ******************************************************************************/
int main(int argc, char** argv)
{
	FILE* input = stdin;
	FILE* output = stdout;

	// check args for -i/-o flags

	// read the contents of the input
	char* data = read(input);
	if (data == NULL) // no input
	{
		return 1;
	}
	fprintf(stderr, "%s\n", data);
	// parse the file into organized structures
	struct Symbol* symbols = parse(data);
	printSymbol(stderr, symbols, 0);
	// generate assembly
	struct String* assembly = compile(symbols);
	// write to output
	size_t numBytes = write(output, assembly);
	if (numBytes != 0)
	{
		fprintf(stderr, "%i bytes written\n", numBytes);
		return 0;
	}
	else // nothing written
	{
		fprintf(stderr, "Error: no bytes written\n");
		return 1;
	}
}

