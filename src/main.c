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
#include <stdlib.h>
#include "defs.h"

#include "symbol.h"
// iterative over next, recursive over lhs/rhs
void printSymbol(FILE* out, struct Symbol* symbol, int depth)
{
	while (symbol)
	{
		int i = 0;
		while (i++ < depth)
			fprintf(out, "\t");
		switch (symbol->type)
		{
			case VARIABLE: fprintf(out, "var %i %s ", symbol->id, symbol->name); break;
			case STRING: fprintf(out, "str %i \"%s\" ", symbol->id, symbol->string); break;
			case VALUE: fprintf(out, "val %i ", symbol->value); break;
			case TYPE: fprintf(out, "type "); break;
			case DECLARE: fprintf(out, "dec "); break;
			case PARENTHESES: fprintf(out, "paren "); break;
			case BRACKET: fprintf(out, "brack "); break;
			case FUNCTION: fprintf(out, "func %s", symbol->name); break;
			
		   // 1
			case INCREMENT_POST: fprintf(out, "inc post "); break;
			case DECREMENT_POST: fprintf(out, "dec post "); break;
			case CALL: fprintf(out, "call %s ", symbol->name); break;
			case SUBSCRIPT: fprintf(out, "subscript "); break;
			case MEMBER: fprintf(out, "member "); break;
			case PTR_MEMBER: fprintf(out, "ptr member "); break;
			case DEREFERENCE: fprintf(out, "deref "); break;

			// 3
			case MULTIPLY: fprintf(out, "mult "); break;
			case DIVIDE: fprintf(out, "div "); break;

			// 4
			case ADD: fprintf(out, "add "); break;
			case SUBTRACT: fprintf(out, "sub "); break;
		
			case EQUAL: fprintf(out, "equal "); break;
			case TERNARY_CONDITIONAL: fprintf(out, "tern cond "); break;
			case ASSIGN: fprintf(out, "assi "); break;
			case RETURN: fprintf(out, "ret "); break;
			case BLANK: fprintf(out, "blank "); break;

			case PLUS: fprintf(out, "plus "); break;
			case MINUS: fprintf(out, "minus "); break;
			case ASTERISK: fprintf(out, "asterisk "); break;
			case QUESTION: fprintf(out, "question "); break;
			case COLON: fprintf(out, "colon "); break;
			case BPAREN: fprintf(out, "bparen "); break;
			case EPAREN: fprintf(out, "eparen "); break;
			case BBRACK: fprintf(out, "bbrack "); break;
			case EBRACK: fprintf(out, "ebrack "); break;
			case SEMICOLON: fprintf(out, "semicolon "); break;
			case COMMA: fprintf(out, "comma "); break;
			default: break;
		}
		fprintf(out, "\n");
		printSymbol(out, symbol->lhs, depth + 1);
		printSymbol(out, symbol->rhs, depth + 1);
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
