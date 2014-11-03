/* =============================================================================
 * @file symbol.c
 * @author Luke Shimanuki
 * @date 1 Nov 2014
 * @brief Implementation of Symbol related functions.
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


/***************************************************************************//**
 *
 * @param argc The number of command line arguments.
 *
 * @param argv An array containing the command line arguments.
 *
 * @return The error level. 0 means no error.
 ******************************************************************************/
struct Symbol* newSymbol(Type type)
{
	struct Symbol* symbol = malloc(sizeof(struct Symbol));
	symbol->type = type;
	symbol->id = 0;
	symbol->a = NULL;
	symbol->b = NULL;
	symbol->next = NULL;
	return symbol;
}

void deleteSymbol(struct Symbol* symbol)
{
	if (symbol != NULL)
		free(symbol);
	return;
}

void deleteSymbolList(struct Symbol* base)
{
	if (base->next != NULL)
		deleteList(base->next);
	deleteSymbol(base);
	return;
}

void addSymbol(struct Symbol* dest, struct Symbol* symbol)
{
	while (dest->next == NULL)
	{
		dest = dest->next;
	}
	dest->next = symbol;
	return;
}
