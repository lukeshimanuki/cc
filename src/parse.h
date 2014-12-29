#ifndef __PARSE_H__
#define __PARSE_H__

#include "symbol.h"

/***************************************************************************//**
 * The type of character.
 ******************************************************************************/
enum CharType
{
	WHITESPACE, /**< A whitespace character such as space, tab, or newline. */
	SPECIAL,    /**< A character that cannot be used in a variable name. */
	NORMAL      /**< A normal character that can be used for a variable name. */
};

/***************************************************************************//**
 * The type of pattern.
 ******************************************************************************/
enum Pattern
{
	CONTAINER,
	UNARY_POST,
	UNARY_PRE,
	BINARY,
	TERNARY
};

/***************************************************************************//**
 * The direction to traverse.
 ******************************************************************************/
enum Direction
{
	RIGHT,
	LEFT
};

// these are the ones that can be read directly
// not interpreted (like functions)
enum SymbolType operators[] = 
{ // sorted by priority
	// three characters
	ALSHIFT,
	ARSHIFT,

	// two characters
	EQUAL,
	NEQUAL,
	INCREMENT,
	DECREMENT,
	GREATER_OR_EQUAL,
	LESS_OR_EQUAL,
	PTR_MEMBER,
	LSHIFT,
	RSHIFT,
	AND,
	OR,
	AADD,
	ASUBTRACT,
	AMULTIPLY,
	ADIVIDE,
	AMODULUS,
	ABAND,
	ABXOR,
	ABOR,

	// one character
	ASSIGN,
	BPAREN,
	EPAREN,
	BBRACK,
	EBRACK,
	BBRACE,
	EBRACE,
	SEMICOLON,
	COMMA,
	PLUS,
	MINUS,
	ASTERISK,
	DIVIDE,
	MEMBER,
	GREATER,
	LESS,
	NOT,
	BNOT,
	AMPERSAND,
	MODULUS,
	BXOR,
	BOR,
	QUESTION,
	COLON,
	
	BLANK
};

// These are the symbols that are keywords.
enum SymbolType keywords[] = 
{
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
//	_THREAD_LOCAL
};

/***************************************************************************//**
 * @brief converts directly to symbols
 *
 * @param data The data to read from.
 *
 * @return A pointer to the list of symbols.
 ******************************************************************************/
struct Symbol* translate(char* data);

/***************************************************************************//**
 * @brief interprets multipurpose symbols
 *
 * @param symbols The symbol list to examine.
 *
 * @return The symbol list.
 ******************************************************************************/
struct Symbol* interpret(struct Symbol* symbols);

/***************************************************************************//**
 * @brief organizes symbol list into a tree
 *
 * @param symbols The symbol list.
 *
 * @return A pointer to the resulting tree.
 ******************************************************************************/
struct Symbol* combine(struct Symbol* symbols);

/***************************************************************************//**
 * @brief looks for a pattern
 *
 * @param symbols The symbol list to search.
 *
 * @param pattern The type of pattern to look for.
 *
 * @param direction The direction to traverse the list.
 *
 * @param type The symbol to look for.
 *
 * @return A pointer to the resulting tree.
 ******************************************************************************/
struct Symbol* findPattern(struct Symbol* symbols, enum Pattern pattern, enum Direction direction, enum SymbolType type);

/***************************************************************************//**
 * @brief looks for a container
 *
 * @param symbols The symbol list to search.
 *
 * @param type The symbol to look for.
 *
 * @param open The symbol to mark the beginning of a container.
 *
 * @param close The symbol to mark the end of a container.
 *
 * @return A pointer to the resulting tree.
 ******************************************************************************/
struct Symbol* patternContainer(struct Symbol* symbols, enum SymbolType type, enum SymbolType open, enum SymbolType close);

/***************************************************************************//**
 * @brief looks for a unary postfix operator
 *
 * @param symbols The symbol list to search.
 *
 * @param direction The direction to traverse the list.
 *
 * @param type The symbol to look for.
 *
 * @return A pointer to the resulting tree.
 ******************************************************************************/
struct Symbol* patternUnaryPost(struct Symbol* symbols, enum Direction direction, enum SymbolType type);
struct Symbol* patternUnaryPre(struct Symbol* symbols, enum Direction direction, enum SymbolType type);

/***************************************************************************//**
 * @brief looks for a unary prefix operator
 *
 * @param symbols The symbol list to search.
 *
 * @param direction The direction to traverse the list.
 *
 * @param type The symbol to look for.
 *
 * @return A pointer to the resulting tree.
 ******************************************************************************/
struct Symbol* patternBinary(struct Symbol* symbols, enum Direction direction, enum SymbolType type);

/***************************************************************************//**
 * @brief looks for a ternary operator
 *
 * @param symbols The symbol list to search.
 *
 * @param direction The direction to traverse the list.
 *
 * @param type The symbol to look for.
 *
 * @param first The separator between the first and second operands.
 *
 * @param second The separator between the second and third operands.
 *
 * @return A pointer to the resulting tree.
 ******************************************************************************/
struct Symbol* patternTernary(struct Symbol* symbols, enum Direction direction, enum SymbolType type, enum SymbolType first, enum SymbolType second);

/***************************************************************************//**
 * @brief get type of character
 *
 * @param c The character to examine.
 *
 * @return The type of character.
 ******************************************************************************/
enum CharType getType(char c);

/***************************************************************************//**
 * @brief determines if symbol can be operand
 *
 * @param type The symbol to examine.
 *
 * @return 1 if the symbol can be used as a left hand side operand. 0 otherwise.
 ******************************************************************************/
int isOperand(enum SymbolType type);

#endif /* __PARSE_H__ */

