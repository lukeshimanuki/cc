/* =============================================================================
 * @file string.h
 * @author Luke Shimanuki
 * @date 28 Dec 2014
 * @brief Definition of the string structure and related functions.
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
 * @brief a character string
 *
 * This is a linked list of null terminated strings. Each element stores its own
 * size and a pointer to the next string.
 ******************************************************************************/
struct String
{
	size_t size;         /**< The number of characters, excluding the terminating null character. */
	char* contents;      /**< The character array containing the string. */

	struct String* next; /**< A pointer to the next string. */
};

/***************************************************************************//**
 * @brief string object constructor
 *
 * @param size The size of the buffer to be allocated.
 *
 * @return A pointer to the created string.
 ******************************************************************************/
struct String* newString(size_t size);

/***************************************************************************//**
 * @brief string object constructor
 *
 * @param array Contains the string to be created.
 *
 * @return A pointer to the created string.
 ******************************************************************************/
struct String* getString(char* array);

/***************************************************************************//**
 * @brief string object destructor
 *
 * @param string The string to be destroyed.
 ******************************************************************************/
void deleteString(struct String* string);

/***************************************************************************//**
 * @brief string list destructor
 *
 * @param base The string list to be destroyed
 ******************************************************************************/
void deleteStringList(struct String* base);

/***************************************************************************//**
 * @brief concatenate strings
 *
 * @param dest The base string.
 *
 * @param string The string to be added to the base string.
 ******************************************************************************/
void addString(struct String* dest, struct String* string);

#endif /* __STRING_H__ */

