#include "symbol.h"

enum CharType
{
	WHITESPACE,
	SPECIAL,
	NORMAL
};

// these are the ones that can be read directly
// not interpreted (like functions)
enum Type operators[] = 
{ // sorted by priority
	// two characters
	EQUAL,
	INCREMENT,
	DECREMENT,
	GREATER_OR_EQUAL,
	LESS_OR_EQUAL,
	// one character
	ASSIGN,
	BPAREN,
	EPAREN,
	BBRACK,
	EBRACK,
	SEMICOLON,
	COMMA,
	ADD,
	SUBTRACT,
	MULTIPLY,
	DIVIDE,
	GREATER,
	LESS,
	
	BLANK
};

struct Symbol* parse(char* data);

struct Symbol* translate(char* data);

struct Symbol* combine(struct Symbol* symbols);

enum CharType getType(char c);
