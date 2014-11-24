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
	enum SymbolType
	{
		// special stuff
		VARIABLE, /*!< description */
		STRING,
		VALUE,
		TYPE,
		DECLARE,
		
		PARENTHESES,
		BRACKET,

		FUNCTION,

		// 1 lr
		INCREMENT_POST,
		DECREMENT_POST,
		CALL,
		SUBSCRIPT,
		MEMBER,
		PTR_MEMBER,
//		COMPOUND_LITERAL, // C99

		// 2 rl
		INCREMENT_PRE,
		DECREMENT_PRE,
		UPLUS,
		UMINUS,
		NOT,
		BNOT,
		CAST,
		DEREFERENCE,
		ADDRESS,
		SIZEOF,
//		ALIGN, // C11
		
		// 3 lr
		MULTIPLY,
		DIVIDE,
		MODULUS,

		// 4
		ADD,
		SUBTRACT,

		// 5
		LSHIFT,
		RSHIFT,

		// 6
		LESS,
		LESS_OR_EQUAL,
		GREATER,
		GREATER_OR_EQUAL,

		// 7
		EQUAL,
		NEQUAL,

		// 8
		BAND,
		// 9
		BXOR,
		// 10
		BOR,

		// 11
		AND,
		// 12
		OR,

		// 13 rl
		TERNARY_CONDITIONAL,

		// 14
		ASSIGN,
		AADD,
		ASUBTRACT,
		AMULTIPLY,
		ADIVIDE,
		AMODULUS,
		ALSHIFT,
		ARSHIFT,
		ABAND,
		ABXOR,
		ABOR,

		// 15 lr
		COMMA,

		RETURN,
		
		// only used for parsing
		ASTERISK,
		PLUS,
		MINUS,
		AMPERSAND,
		INCREMENT,
		DECREMENT,
		QUESTION,
		COLON,
		BPAREN,
		EPAREN,
		BBRACK,
		EBRACK,
		BBRACE,
		EBRACE,
		SEMICOLON,

		// placeholders
		BLANK
	} type;

	int id;
	int value;
	char* string;
	char* name;

	struct Symbol* lhs;
	struct Symbol* rhs;
	struct Symbol* third; // only for ternary conditional

	struct Symbol* next;
};

/***************************************************************************//**
 * @brief Loads a file into a buffer.
 ******************************************************************************/
struct Symbol* newSymbol(enum SymbolType type);

void deleteSymbol(struct Symbol* symbol);

void deleteSymbolList(struct Symbol* base);

void addSymbol(struct Symbol* dest, struct Symbol* symbol);

char* operatorString(enum SymbolType operatorID);

struct Symbol* reverseSymbol(struct Symbol* root);

#endif /* __SYMBOL_H__ */
