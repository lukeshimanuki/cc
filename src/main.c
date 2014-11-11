/* =============================================================================
 * @file main.c
 * @author Luke Shimanuki
 * @date 1 Nov 2014
 * @brief Implements the main function.
 *
 * This file is part of MCC.
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

#include "defs.h"

#include "symbol.h"
// iterative over next, recursive over lhs/rhs
void printSymbol(struct Symbol* symbol, int depth)
{
	while (symbol)
	{
		int i = 0;
		while (i++ < depth)
			printf("\t");
		switch (symbol->type)
		{
			case VARIABLE: printf("var %i %s ", symbol->id, symbol->name); break;
			case STRING: printf("str %i \"%s\" ", symbol->id, symbol->string); break;
			case VALUE: printf("val %i ", symbol->value); break;
			case TYPE: printf("type "); break;
			case DECLARE: printf("dec "); break;
			case ASSIGN: printf("assi "); break;
			case ADD: printf("add "); break;
			case SUBTRACT: printf("sub "); break;
			case MULTIPLY: printf("mult "); break;
			case DIVIDE: printf("div "); break;
			case PARENTHESES: printf("paren "); break;
			case BRACKET: printf("brack "); break;
			case FUNCTION: printf("func "); break;
			case CALL: printf("call %s ", symbol->name); break;
			case RETURN: printf("ret "); break;
			case BLANK: printf("blank "); break;

			case BPAREN: printf("bparen "); break;
			case EPAREN: printf("eparen "); break;
			case BBRACK: printf("bbrack "); break;
			case EBRACK: printf("ebrack "); break;
			case SEMICOLON: printf("semicolon "); break;
			case COMMA: printf("comma "); break;

		}
		printf("\n");
		printSymbol(symbol->lhs, depth + 1);
		printSymbol(symbol->rhs, depth + 1);
		symbol = symbol->next;
	}
	return;
}

/***************************************************************************//**
 * This is executed on startup. It processes the command line arguments to
 * determine the file to compile, and runs through each step.
 ******************************************************************************/
int main(int argc, char** argv)
{
	// the first argument is the compiler
	// if there is no second argument, there is no file to compile
	if (argc < 2)
	{
		printf("Error: not enough arguments\n");
		return 0;
	}

	// read the contents of the file
	char* data = read(argv[1]);
	// parse the file into organized structures
	struct Symbol* symbols = parse(data);
	printSymbol(symbols, 0);
	printf("\n");
	// generate assembly
	struct String* assembly = compile(symbols);
	// write to file
	size_t numBytes = write("out.s", assembly);
	if (numBytes)
	{
		printf("%i bytes written\n", numBytes);
		return 0;
	}
	else
	{
		printf("Error: no bytes written\n");
		return 1;
	}
}
