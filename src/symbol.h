/* =============================================================================
 * @file symbol.h
 * @author Luke Shimanuki
 * @date 1 Nov 2014
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

#ifndef __SYMBOL_H__
#define __SYMBOL_H__

/***************************************************************************//**
 * @struct Symbol
 *
 * @brief This structure...
 *
 * It does such and such... mention linked list, etc...
 ******************************************************************************/
struct Symbol
{
	//! The type of symbol.
	/*! details... */
	enum Type
	{
		VARIABLE, /*!< description */
		STRING,
		VALUE,
		TYPE,

		DECLARE,
		ASSIGN,

		ADD,
		SUBTRACT,
		MULTIPLY,
		DIVIDE,

		FUNCTION,
		CALL,
		RETURN,
	} type;

	int id;
	int value;
	char* string;
	char* name;

	struct Symbol* lhs;
	struct Symbol* rhs;

	struct Symbol* next;
};

/***************************************************************************//**
 * @brief Loads a file into a buffer.
 ******************************************************************************/
struct Symbol* newSymbol(enum Type type);

void deleteSymbol(struct Symbol* symbol);

void deleteSymbolList(struct Symbol* base);

void addSymbol(struct Symbol* dest, struct Symbol* symbol);

#endif /* __SYMBOL_H__ */
