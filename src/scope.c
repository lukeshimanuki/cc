/* =============================================================================
 * @file scope.c
 * @author Luke Shimanuki
 * @date 4 Nov 2014
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

#include <stdlib.h>
#include <string.h>

#include "scope.h"


/***************************************************************************//**
 *
 * @param argc The number of command line arguments.
 *
 * @param argv An array containing the command line arguments.
 *
 * @return The error level. 0 means no error.
 ******************************************************************************/
struct Scope* newScope()
{
	struct Scope* scope = malloc(sizeof(struct Scope));
	scope->bottom = 0;
	scope->offset = 0;
	scope->next = NULL;
	return scope;
}

void deleteScope(struct Scope* scope)
{
	if (scope != NULL)
		free(scope);
	return;
}

void addScope(struct Scope* base, size_t offset)
{
	struct Scope* scope = newScope();
	scope->offset = base->bottom + offset;
	base->next = scope;
}

int getOffset(struct Scope* scope, int varID)
{
	// for all others, ebp < scopeOffset (more negative)
	int ebp = scope->offset;
	while (scope)
	{
		// if it exists
		if (scope->variables[varID] > 0)
		{
			return scope->offset - ebp - scope->variables[varID];
		}
		scope = scope->next;
	}

	// variable does not exist
	return -15;
}

void addVariable(struct Scope* scope, int id, size_t size)
{
	scope->bottom += size;
	scope->variables[id] = scope->bottom;
}
