#include "symbol.h"

enum CharType
{
	WHITESPACE,
	SPECIAL,
	NORMAL
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
	

struct Symbol* parse(char* data);

struct Symbol* translate(char* data);

struct Symbol* combine(struct Symbol* symbols);

enum CharType getType(char c);
