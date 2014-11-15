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
	
	BLANK
};

struct Symbol* parse(char* data);

struct Symbol* translate(char* data);

struct Symbol* combine(struct Symbol* symbols);

enum CharType getType(char c);
