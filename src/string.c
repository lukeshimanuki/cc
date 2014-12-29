/* =============================================================================
 * @file string.c
 * @author Luke Shimanuki
 * @date 28 Dec 2014
 * @brief Implementation of functions relating to the string structure.
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

#include "string.h"


/***************************************************************************//**
 * Allocates and initializes a new string of a given size.
 ******************************************************************************/
struct String* newString(size_t size)
{
	struct String* string = malloc(sizeof(struct String));
	string->size = size;
	if (size > 0)
		string->contents = malloc(size);
	else
		string->contents = NULL;
	string->next = NULL;
	return string;
}

/***************************************************************************//**
 * Allocates and initializes a new string with the given contents.
 ******************************************************************************/
struct String* getString(char* array)
{
	struct String* string = malloc(sizeof(struct String));
	string->size = strlen(array);
	string->contents = malloc(string->size + 1);
	strcpy(string->contents, array);
	string->contents[string->size] = '\0';
	string->next = NULL;
	return string;
}

/***************************************************************************//**
 * Destroys the given string.
 ******************************************************************************/
void deleteString(struct String* string)
{
	if (string != NULL)
		free(string);
	return;
}

/***************************************************************************//**
 * Destroys all strings in the given list.
 ******************************************************************************/
void deleteStringList(struct String* base)
{
	if (base->next != NULL)
		deleteStringList(base->next);
	deleteString(base);
	return;
}

/***************************************************************************//**
 * Adds string to the end of dest.
 ******************************************************************************/
void addString(struct String* dest, struct String* string)
{
	while (dest->next != NULL)
	{
		dest = dest->next;
	}
	dest->next = string;
	return;
}

