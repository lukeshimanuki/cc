/* =============================================================================
 * @file symbol.c
 * @author Luke Shimanuki
 * @date 28 Dec 2014
 * @brief Implementation of functions relating to the symbol structure.
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
#include <stdio.h>

#include "symbol.h"


/***************************************************************************//**
 * Allocates and initializes a new symbol of the given type.
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

/***************************************************************************//**
 * Destroys the given symbol.
 ******************************************************************************/
void deleteSymbol(struct Symbol* symbol)
{
	if (symbol != NULL)
		free(symbol);
	return;
}

/***************************************************************************//**
 * Destroys all symbols in the given tree.
 ******************************************************************************/
void deleteSymbolList(struct Symbol* base)
{
	if (base->next != NULL)
		deleteSymbolList(base->lhs);
		deleteSymbolList(base->rhs);
		deleteSymbolList(base->next);
	deleteSymbol(base);
	return;
}

/***************************************************************************//**
 * Adds symbol to the end of dest.
 ******************************************************************************/
void addSymbol(struct Symbol* dest, struct Symbol* symbol)
{
	while (dest->next != NULL)
	{
		dest = dest->next;
	}
	dest->next = symbol;
	return;
}

/***************************************************************************//**
 * Gets the string that represents the given operator.
 ******************************************************************************/
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

/***************************************************************************//**
 * Gets the string that represents the given keyword.
 ******************************************************************************/
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

/***************************************************************************//**
 * Reverses both the ordering and the arguments of all symbols in the tree.
 * The third argument is left unchanged.
 ******************************************************************************/
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

/***************************************************************************//**
 * Prints the given symbol tree. It is processes each symbol in the list
 * iteratively, and processes each branch recursively on a new indent level.
 ******************************************************************************/
void printSymbol(FILE* out, struct Symbol* symbol, int depth)
{
	while (symbol)
	{
		int i = 0;
		while (i++ < depth)
			fprintf(out, "\t");
		switch (symbol->type)
		{
			case VARIABLE: fprintf(out, "var %i %s ", symbol->id, symbol->name); break;
			case STRING: fprintf(out, "str %i \"%s\" ", symbol->id, symbol->string); break;
			case VALUE: fprintf(out, "val %i ", symbol->value); break;
			case TYPE: fprintf(out, "type "); break;
			case DECLARE: fprintf(out, "dec "); break;
			case PARENTHESES: fprintf(out, "paren "); break;
			case BRACKET: fprintf(out, "brack "); break;
			case FUNCTION: fprintf(out, "func %s", symbol->name); break;

			case IF: fprintf(out, "if "); break;
			case WHILE: fprintf(out, "while "); break;
			
			case INCREMENT_POST: fprintf(out, "inc post "); break;
			case DECREMENT_POST: fprintf(out, "dec post "); break;
			case CALL: fprintf(out, "call %s ", symbol->name); break;
			case SUBSCRIPT: fprintf(out, "subscript "); break;
			case MEMBER: fprintf(out, "member "); break;
			case PTR_MEMBER: fprintf(out, "ptr member "); break;
			case DEREFERENCE: fprintf(out, "deref "); break;
			case ADDRESS: fprintf(out, "addr "); break;

			case MULTIPLY: fprintf(out, "mult "); break;
			case DIVIDE: fprintf(out, "div "); break;

			case ADD: fprintf(out, "add "); break;
			case SUBTRACT: fprintf(out, "sub "); break;
		
			case EQUAL: fprintf(out, "equal "); break;
			case TERNARY_CONDITIONAL: fprintf(out, "tern cond "); break;
			case ASSIGN: fprintf(out, "assi "); break;
			case RETURN: fprintf(out, "ret "); break;
			case BLANK: fprintf(out, "blank "); break;

			case PLUS: fprintf(out, "plus "); break;
			case MINUS: fprintf(out, "minus "); break;
			case ASTERISK: fprintf(out, "asterisk "); break;
			case QUESTION: fprintf(out, "question "); break;
			case COLON: fprintf(out, "colon "); break;
			case BPAREN: fprintf(out, "bparen "); break;
			case EPAREN: fprintf(out, "eparen "); break;
			case BBRACK: fprintf(out, "bbrack "); break;
			case EBRACK: fprintf(out, "ebrack "); break;
			case SEMICOLON: fprintf(out, "semicolon "); break;
			case COMMA: fprintf(out, "comma "); break;
			default: break;
		}
		fprintf(out, "\n");
		printSymbol(out, symbol->lhs, depth + 1);
		printSymbol(out, symbol->rhs, depth + 1);
		symbol = symbol->next;
	}
	return;
}

