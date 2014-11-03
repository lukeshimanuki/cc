/* =============================================================================
 * @file compile.h
 * @author Luke Shimanuki
 * @date 2 Nov 2014
 * @brief Functions for converting symbolic structure to assembly.
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

#include "symbol.h"
#include "string.h"

// iterative over lists, recursive over children
// eg: recurse over function->parameters->declare->variable
// iterative over declare, declare, add, call
// but the string list is iterative
struct String* compile(struct Symbol* symbols)
{
	struct String* assembly = NULL;
	while (symbols != NULL)
	{
		struct String* current = NULL;
		// process the current symbol
		switch (symbols.type)
		{
			// results stored in %eax
			case VARIABLE:
				break;
			case STRING:
				break;
			case VALUE:
				break;
			case TYPE:
				break;
			case DECLARE:
				break;
			case ASSIGN:
				break;
			case ADD:
				current = compile(symbols->a);
				addString(current, toString("\tmov %eax,%edx\n"));
				addString(current, compile(symbols->b));
				addString(current, toString("\tadd %edx,%eax\n"));
				break;
			case SUBTRACT:
				break;
			case MULTIPLY:
				break;
			case DIVIDE:
				break;
			case FUNCTION:
				break;
			case CALL:
				break;
			case RETURN:
				break;
			case SEMICOLON:
				break;
		}
		if (assembly == NULL)
			assembly = current;
		else
			addString(assembly, current);
		// process the next symbol in the next cycle
		symbols = symbols->next;
	}
}
