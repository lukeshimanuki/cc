/* =============================================================================
 * @file symbol.h
 * @author Luke Shimanuki
 * @date 28 Dec 2014
 * @brief Definition of the symbol structure and related functions.
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

/** The type of symbol. */
enum SymbolType
{
	// special stuff
	VARIABLE,
	STRING,
	VALUE,
	TYPE,
	DECLARE,
	
	PARENTHESES,
	BRACKET,

	FUNCTION,

	// keywords
	AUTO,
	BREAK,
	CASE,
	CHAR,
	CONST,
	CONTINUE,
	DEFAULT,
	DO,
	DOUBLE,
	ELSE,
	ENUM,
	EXTERN,
	FLOAT,
	FOR,
	GOTO,
	IF,
//	INLINE,
	INT,
	LONG,
	REGISTER,
//	RESTRICT,
	RETURN,
	SHORT,
	SIGNED,
	SIZEOF,
	STATIC,
	STRUCT,
	SWITCH,
	TYPEDEF,
	UNION,
	UNSIGNED,
	VOID,
	VOLATILE,
	WHILE,
//	_ALIGNAS,
//	_ALIGNOF,
//	_ATOMIC,
//	_BOOL,
//	_COMPLEX,
//	_GENERIC,
//	_IMAGINARY,
//	_NORETURN,
//	_STATIC_ASSERT,
//	_THREAD_LOCAL,

	// 1 lr
	INCREMENT_POST,
	DECREMENT_POST,
	CALL,
	SUBSCRIPT,
	MEMBER,
	PTR_MEMBER,
//	COMPOUND_LITERAL, // C99

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
//	SIZEOF, // also a keyword?
//	ALIGN, // C11
	
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
};


/***************************************************************************//**
 * @struct Symbol
 *
 * @brief a symbol representing a C construct
 *
 * This is a tree of symbols. Each symbol has a follower and up to three
 * arguments, as well as a type and various forms of symbol data.
 ******************************************************************************/
struct Symbol
{
	enum SymbolType type; /**< The type of symbol. */

	int id;               /**< The identification number of the symbol. */
	int value;            /**< The value of the symbol. */
	char* string;         /**< String data for the symbol. */
	char* name;           /**< The name of the symbol. */

	struct Symbol* lhs;   /**< A pointer to the left argument symbol. */
	struct Symbol* rhs;   /**< A pointer to the right argument symbol. */
	struct Symbol* third; /**< A pointer to the third argument symbol.
	                       **  Only used by the ternary conditional */

	struct Symbol* next;  /**< A pointer to the next symbol in the list. */
};

/***************************************************************************//**
 * @brief symbol object constructor
 *
 * @param type The type of symbol to be created.
 *
 * @return A pointer to the created symbol.
 ******************************************************************************/
struct Symbol* newSymbol(enum SymbolType type);

/***************************************************************************//**
 * @brief symbol object destructor
 *
 * @param symbol The symbol to be destroyed.
 ******************************************************************************/
void deleteSymbol(struct Symbol* symbol);

/***************************************************************************//**
 * @brief symbol tree destructor
 *
 * @param base The symbol tree to be destroyed.
 ******************************************************************************/
void deleteSymbolList(struct Symbol* base);

/***************************************************************************//**
 * @brief concatenate symbols
 *
 * @param dest The base symbol.
 *
 * @param symbol The symbol to be added to the base symbol.
 ******************************************************************************/
void addSymbol(struct Symbol* dest, struct Symbol* symbol);

/***************************************************************************//**
 * @brief string representing an operator
 *
 * @param operatorID The operator to look up.
 *
 * @return The string that represents the given operator.
 ******************************************************************************/
char* operatorString(enum SymbolType operatorID);

/***************************************************************************//**
 * @brief string representing a keyword
 *
 * @param keywordID The keyword to look up.
 *
 * @return The string that represents the given keyword.
 ******************************************************************************/
char* keywordString(enum SymbolType keywordID);

/***************************************************************************//**
 * @brief reverses the tree
 *
 * @param root The symbol tree to reverse.
 *
 * @return A pointer to the reversed tree.
 ******************************************************************************/
struct Symbol* reverseSymbol(struct Symbol* root);

/***************************************************************************//**
 * @brief prints symbol tree
 *
 * @param out The output stream.
 *
 * @param symbol The symbol tree to print.
 *
 * @param depth The indent level.
 ******************************************************************************/
void printSymbol(FILE* out, struct Symbol* symbol, int depth);

#endif /* __SYMBOL_H__ */

