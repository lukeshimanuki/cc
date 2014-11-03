/* =============================================================================
 * @file string.c
 * @author Luke Shimanuki
 * @date 2 Nov 2014
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

#include "string.h"


/***************************************************************************//**
 *
 * @param argc The number of command line arguments.
 *
 * @param argv An array containing the command line arguments.
 *
 * @return The error level. 0 means no error.
 ******************************************************************************/
struct String* newString(size_t size)
{
	struct String* string = malloc(sizeof(struct String));
	string->size = size;
	string->contents = malloc(size);
	string->next = NULL;
	return string;
}

// copies string to contents
struct String* getString(char* array)
{
	struct String* string = malloc(sizeof(struct String));
	string->size = strlen(array);
	string->contents = malloc(string->size);
	strcpy(contents, array);
	string->next = NULL;
	return string;
}

void deleteString(struct String* string)
{
	if (string != NULL)
		free(string);
	return;
}

void deleteStringList(struct String* base)
{
	if (base->next != NULL)
		deleteList(base->next);
	deleteString(base);
	return;
}

// adds to the "next" line, not the "child" line
void addString(struct String* dest, struct String* string)
{
	while (dest->next == NULL)
	{
		dest = dest->next;
	}
	dest->next = string;
	return;
}
