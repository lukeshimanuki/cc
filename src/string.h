/* =============================================================================
 * @file string.h
 * @author Luke Shimanuki
 * @date 2 Nov 2014
 * @brief Structure and functions for symbolic parsing.
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

#ifndef __STRING_H__
#define __STRING_H__

/***************************************************************************//**
 * @struct String
 *
 * @brief This structure...
 *
 * It does such and such... mention linked list, etc...
 ******************************************************************************/
struct String
{
	size_t size;
	char* contents;
	struct String* next;
};

/***************************************************************************//**
 * @brief Loads a file into a buffer.
 ******************************************************************************/
struct String* newString(size_t size);

struct String* getString(char* array);

void deleteString(struct String* string);

void deleteStringList(struct String* base);

void addString(struct String* dest, struct String* string);

#endif /* __STRING_H__ */
