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

#include <stdlib.h>

#include "symbol.h"


/***************************************************************************//**
 *
 * @param argc The number of command line arguments.
 *
 * @param argv An array containing the command line arguments.
 *
 * @return The error level. 0 means no error.
 ******************************************************************************/
struct Symbol* newSymbol(enum SymbolType type)
{
	struct Symbol* symbol = malloc(sizeof(struct Symbol));
	symbol->type = type;
	symbol->id = 0;
	symbol->lhs = NULL;
	symbol->rhs = NULL;
	symbol->third = NULL;
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
		deleteSymbolList(base->lhs);
		deleteSymbolList(base->rhs);
		deleteSymbolList(base->next);
	deleteSymbol(base);
	return;
}

void addSymbol(struct Symbol* dest, struct Symbol* symbol)
{
	while (dest->next != NULL)
	{
		dest = dest->next;
	}
	dest->next = symbol;
	return;
}

// operatorID is type of symbol, not index of operators[]
char* operatorString(enum SymbolType operatorID)
{
	switch (operatorID)
	{
		case TERNARY_CONDITIONAL: return "idk";
		
		case ALSHIFT: return "<<=";
		case ARSHIFT: return ">>=";

		case EQUAL: return "==";
		case NEQUAL: return "!=";
		case INCREMENT: return "++";
		case DECREMENT: return "--";
		case GREATER_OR_EQUAL: return ">=";
		case LESS_OR_EQUAL: return "<=";
		case PTR_MEMBER: return "->";
		case LSHIFT: return "<<";
		case RSHIFT: return ">>";
		case AND: return "&&";
		case OR: return "||";
		case AADD: return "+=";
		case ASUBTRACT: return "-=";
		case AMULTIPLY: return "*=";
		case ADIVIDE: return "/=";
		case AMODULUS: return "%=";
		case ABAND: return "&=";
		case ABXOR: return "^=";
		case ABOR: return "|=";

		case ASSIGN: return "=";
		case BPAREN: return "(";
		case EPAREN: return ")";
		case BBRACK: return "{";
		case EBRACK: return "}";
		case BBRACE: return "[";
		case EBRACE: return "]";
		case SEMICOLON: return ";";
		case COMMA: return ",";
		case PLUS: return "+";
		case MINUS: return "-";
		case ASTERISK: return "*";
		case DIVIDE: return "/";
		case MEMBER: return ".";
		case GREATER: return ">";
		case LESS: return "<";
		case NOT: return "!";
		case BNOT: return "~";
		case AMPERSAND: return "&";
		case MODULUS: return "%";
		case BXOR: return "^";
		case BOR: return "|";
		case QUESTION: return "?";
		case COLON: return ":";
		
		case BLANK: return "blank";
		default: return "nonexistent";
	}
}

char* keywordString(enum SymbolType keywordID)
{
	switch (keywordID)
	{
		case AUTO: return "auto";
		case BREAK: return "break";
		case CASE: return "case";
		case CHAR: return "char";
		case CONST: return "const";
		case CONTINUE: return "continue";
		case DEFAULT: return "default";
		case DO: return "do";
		case DOUBLE: return "double";
		case ELSE: return "else";
		case ENUM: return "enum";
		case EXTERN: return "extern";
		case FLOAT: return "float";
		case FOR: return "for";
		case GOTO: return "goto";
		case IF: return "if";
//		case INLINE: return "inline";
		case INT: return "int";
		case LONG: return "long";
		case REGISTER: return "register";
//		case RESTRICT: return "restrict";
		case RETURN: return "return";
		case SHORT: return "short";
		case SIGNED: return "signed";
		case SIZEOF: return "sizeof";
		case STATIC: return "static";
		case STRUCT: return "struct";
		case SWITCH: return "switch";
		case TYPEDEF: return "typedef";
		case UNION: return "union";
		case UNSIGNED: return "unsigned";
		case VOID: return "void";
		case VOLATILE: return "volatile";
		case WHILE: return "while";
//		case _ALIGNAS: return "_Alignas";
//		case _ALIGNOF: return "_Alignof";
//		case _ATOMIC: return "_Atomic";
//		case _BOOL: return "_Bool";
//		case _COMPLEX: return "_Complex";
//		case _GENERIC: return "_Generic";
//		case _IMAGINARY: return "_Imaginary";
//		case _NORETURN: return "_Noreturn";
//		case _STATIC_ASSERT: return "_Static_assert";
//		case _THREAD_LOCAL: return "_Thread_local";
		default: return "garbage";
	}
}

struct Symbol* reverseSymbol(struct Symbol* root)
{
	struct Symbol* newRoot = NULL;
	while (root)
	{
		// flip operands
		struct Symbol* lhs = root->lhs;
		root->lhs = root->rhs;
		root->rhs = lhs;
		// reverse operand contents
		root->lhs = reverseSymbol(root->lhs);
		root->rhs = reverseSymbol(root->rhs);

		// flip ordering
		struct Symbol* next = root->next;
		root->next = newRoot;
		newRoot = root;
		root = next;
	}
	return newRoot;
}
