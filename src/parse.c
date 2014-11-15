#include <stdlib.h>
#include <string.h>

#include "parse.h"
#include "symbol.h"

struct Symbol* parse(char* data)
{
	struct Symbol* symbols = translate(data);
	struct Symbol* tree = combine(symbols);
	return tree;
}

char* names[256];
size_t numNames = 0;
int numStrings = 0;

struct Symbol* translate(char* data)
{
	struct Symbol* symbols = newSymbol(BLANK);
	// repeat until \0
	while (data[0] != '\0')
	{
		// check for string literal
		if (data[0] == '"')
		{
			// find next ", ignoring the first one
			int length = strchr(data + 1, '"') - data - 1;
			// create new string symbol
			struct Symbol* symbol = newSymbol(STRING);
			// set string literal (null terminated)
			symbol->string = malloc(length + 1);
			memcpy(symbol->string, data + 1, length);
			symbol->string[length] = '\0';
			// and the id
			symbol->id = numStrings;
			numStrings++;
			// add it to the list
			addSymbol(symbols, symbol);
			// continue on to the next word
			data += length + 2;
			continue;
		}
		// check for character literal
		if (data[0] == '\'')
		{ // idk what to do here...
//			addSymbol(symbols, newSymbol(BLANK));
		}
		// one time redundancy, but whatever
		enum CharType type = getType(data[0]);
		char word[32];
		size_t indice = 0;
		// read characters of string
		while (getType(data[indice]) == type)
		{
			word[indice] = data[indice];
			indice++;
		}
		word[indice] = '\0'; // null terminate
		// remove word from data
		data += strlen(word);
	
		// process word
		switch (type)
		{
			case WHITESPACE:
				// do nothing
				break;
			case SPECIAL:
			{
				size_t id = 0; // index of operators[]
				size_t i = 0; // indice of word
				// sizeof arr / sizeof element -> number of elements
				while (id < sizeof(operators) / sizeof(operators[0]) && strlen(word + i) > 0)
				{
					// if it is this operator
					if (strncmp(word + i, operatorString(operators[id]), strlen(operatorString(operators[id]))) == 0)
					{
						// add the symbol
						addSymbol(symbols, newSymbol(operators[id]));
						// increase indice
						i += strlen(operatorString(operators[id]));
						// restart search
						id = 0;
					}
					// otherwise move on to the next operator
					else
						id++;
				}
				break;
			}
			case NORMAL:
			{
				// check return
				if (strcmp(word, "return") == 0)
				{
					addSymbol(symbols, newSymbol(RETURN));
					break;
				}
				// check types: currently only supports int
				if (strcmp(word, "int") == 0)
				{
					struct Symbol* type = newSymbol(TYPE);
					type->id = 0;
					addSymbol(symbols, type);
					break;
				}
				
				// check number
				int isNumber = 1;
				int digitIndex = 0;
				while (digitIndex < strlen(word))
				{
					// if not a digit, then it's not a number
					if (word[digitIndex] < '0' || word[digitIndex] > '9')
					{
						isNumber = 0;
						break;
					}
					digitIndex++;
				}
				if (isNumber && strlen(word) > 0)
				{
					struct Symbol* number = newSymbol(VALUE);
					// convert to number
					number->value = strtol(word, NULL, 10);
					addSymbol(symbols, number);
					break;
				}

				// check return
				if (strcmp(word, "return") == 0)
				{
					addSymbol(symbols, newSymbol(RETURN));
					break;
				}

				// create symbol with name
				struct Symbol* name = newSymbol(VARIABLE);
				name->name = malloc(strlen(word) + 1);
				strcpy(name->name, word);
				// check if name exists
				size_t id = 0;
				while (id < numNames)
				{
					// if names are same
					if (strcmp(word, names[id]) == 0)
					{
						// set id to same value
						name->id = id;
						break;
					}
					id++;
				}
				// if no names matched
				if (id == numNames)
				{
					// create new name
					names[id] = malloc(strlen(word) + 1);
					strcpy(names[id], word);
					name->id = numNames;
					numNames++;
				}
				addSymbol(symbols, name);
				break;
			}
		}
	}
	// remove blanks from beginning
	while (symbols && symbols->type == BLANK)
	{
		struct Symbol* blank = symbols;
		symbols = symbols->next;
		deleteSymbol(blank);
	}
	// remove blanks after beginning
	struct Symbol* current = symbols;
	while (current)
	{
		if (current->next && current->next->type == BLANK)
		{
			struct Symbol* blank = current->next;
			current->next = current->next->next;
			deleteSymbol(blank);
		}
		else current = current->next;
	}
	return symbols;
}

enum Pattern
{
	CONTAINER,
	UNARY_POST,
	UNARY_PRE,
	BINARY,
	TERNARY
};

void findPattern(struct Symbol* symbols, enum Pattern pattern, enum SymbolType type);

// looks for patterns and combines
struct Symbol* combine(struct Symbol* symbols)
{
	struct Symbol* current;

	findPattern(symbols, CONTAINER, PARENTHESES);
	findPattern(symbols, CONTAINER, BRACKET);
	// only combines the inside of braces, does not connect to lhs
	findPattern(symbols, CONTAINER, SUBSCRIPT);

	// functions: type, variable, parentheses, bracket
	current = symbols;
	while (current)
	{
		// check if 4 symbols exist
		if (current->next && current->next->next && current->next->next->next)
		{
			// check pattern
			if (current->type == TYPE && current->next->type == VARIABLE && current->next->next->type == PARENTHESES && current->next->next->next->type == BRACKET)
			{
				// turn it into a function
				current->type = FUNCTION;
				// ignore type...
				// set id to same as variable id
				current->id = current->next->id;
				// set function name
				current->name = malloc(strlen(current->next->name) + 1);
				strcpy(current->name, current->next->name);
				// set lhs to inside of parentheses
				current->lhs = current->next->next->rhs;
				// set rhs to inside of brackets
				current->rhs = current->next->next->next->rhs;
				// remove the 3 extra symbols
				struct Symbol* extra = current->next;
				current->next = current->next->next->next->next;
				deleteSymbol(extra->next->next);
				deleteSymbol(extra->next);
				deleteSymbol(extra) ;
			}
		}
		current = current->next;
	}
	// call: variable, parentheses
	current = symbols;
	while (current && current->next)
	{
		if (current->type == VARIABLE && current->next->type == PARENTHESES)
		{
			struct Symbol* paren = current->next;
			current->type = CALL;
			current->rhs = paren->rhs;
			current->next = paren->next;
			deleteSymbol(paren);
		}
		current = current->next;
	}
	// declare: type, variable
	current = symbols;
	while (current && current->next)
	{
		if (current->type == TYPE && current->next->type == VARIABLE)
		{
			// replace them with declare
			struct Symbol* var = current->next;
			current->type = DECLARE;
			current->rhs = var;
			current->next = var->next;
			var->next = NULL;
		}
		current = current->next;
	}

	findPattern(symbols, UNARY_POST, INCREMENT_POST);
	findPattern(symbols, UNARY_POST, DECREMENT_POST);
	findPattern(symbols, BINARY, MEMBER);
	findPattern(symbols, BINARY, PTR_MEMBER);

	findPattern(symbols, UNARY_PRE, INCREMENT_PRE);
	findPattern(symbols, UNARY_PRE, DECREMENT_PRE);
//	findPattern(symbols, UNARY_PRE, UPLUS);
//	findPattern(symbols, UNARY_PRE, UMINUS);
	findPattern(symbols, UNARY_PRE, NOT);
	findPattern(symbols, UNARY_PRE, BNOT);
//	findPattern(symbols, UNARY_PRE, CAST);
//	findPattern(symbols, UNARY_PRE, DEREFERENCE);
	findPattern(symbols, UNARY_PRE, ADDRESS);
//	findPattern(symbols, UNARY_PRE, SIZEOF);

	findPattern(symbols, BINARY, MULTIPLY);
	findPattern(symbols, BINARY, DIVIDE);
	findPattern(symbols, BINARY, MODULUS);
	
	findPattern(symbols, BINARY, ADD);
	findPattern(symbols, BINARY, SUBTRACT);

	findPattern(symbols, BINARY, LSHIFT);
	findPattern(symbols, BINARY, RSHIFT);

	findPattern(symbols, BINARY, LESS);
	findPattern(symbols, BINARY, LESS_OR_EQUAL);
	findPattern(symbols, BINARY, GREATER);
	findPattern(symbols, BINARY, GREATER_OR_EQUAL);
	
	findPattern(symbols, BINARY, EQUAL);
	findPattern(symbols, BINARY, NEQUAL);

	findPattern(symbols, BINARY, BAND);
	
	findPattern(symbols, BINARY, BXOR);

	findPattern(symbols, BINARY, BOR);

	findPattern(symbols, BINARY, AND);

	findPattern(symbols, BINARY, OR);

//	findPattern(symbols, BINARY, TERNARY_CONDITIONAL);

	findPattern(symbols, BINARY, ASSIGN);
	findPattern(symbols, BINARY, AADD);
	findPattern(symbols, BINARY, ASUBTRACT);
	findPattern(symbols, BINARY, AMULTIPLY);
	findPattern(symbols, BINARY, ADIVIDE);
	findPattern(symbols, BINARY, AMODULUS);
	findPattern(symbols, BINARY, ALSHIFT);
	findPattern(symbols, BINARY, ARSHIFT);
	findPattern(symbols, BINARY, ABAND);
	findPattern(symbols, BINARY, ABXOR);
	findPattern(symbols, BINARY, ABOR);

//	findPattern(symbols, BINARY, COMMA);

	findPattern(symbols, UNARY_PRE, RETURN);

	// clean blanks, commas, semicolons
	current = symbols;
	while (current && (current->type == BLANK || current->type == COMMA || current->type == SEMICOLON))
	{
		if (current->next)
		{
			struct Symbol* next = current->next;
			memcpy(current, current->next, sizeof(struct Symbol));
			deleteSymbol(next);
		}
		current = current->next;
	}
	while (current && current->next)
	{
		if (current->next->type == BLANK || current->next->type == COMMA || current->next->type == SEMICOLON)
		{
			struct Symbol* blank = current->next;
			current->next = current->next->next;
			deleteSymbol(blank);
		}
		current = current->next;
	}
	return symbols;
}

enum CharType getType(char c)
{
	switch (c)
	{
		case ' ':
		case '\t':
		case '\n':
			return WHITESPACE;
		case '+':
		case '-':
		case '*':
		case '/':
		case '=':
		case '>':
		case '<':
		case '(':
		case ')':
		case '{':
		case '}':
		case ';':
		case ',':
			return SPECIAL;
		default:
			return NORMAL;
	}
}

void patternContainer(struct Symbol* symbols, enum SymbolType type, enum SymbolType open, enum SymbolType close);
void patternUnaryPost(struct Symbol* symbols, enum SymbolType type, enum SymbolType alias);
void patternUnaryPre(struct Symbol* symbols, enum SymbolType type, enum SymbolType alias);
void patternBinary(struct Symbol* symbols, enum SymbolType type, enum SymbolType alias);
void patternTernary(struct Symbol* symbols, enum SymbolType type, enum SymbolType first, enum SymbolType second);

void findPattern(struct Symbol* symbols, enum Pattern pattern, enum SymbolType type)
{
	switch (pattern)
	{
		case CONTAINER:
		{
			enum SymbolType open, close;
			switch (type)
			{
				case PARENTHESES:
					open = BPAREN;
					close = EPAREN;
					break;
				case BRACKET:
					open = BBRACK;
					close = EBRACK;
					break;
				case SUBSCRIPT:
					open = BBRACE;
					close = EBRACE;
					break;
				default:
					open = type;
					close = type;
					break;
			}
			patternContainer(symbols, type, open, close);
			break;
		}
		case UNARY_POST:
		{
			enum SymbolType alias;
			switch (type)
			{
				case INCREMENT_POST: alias = INCREMENT; break;
				case DECREMENT_POST: alias = DECREMENT; break;
				default: alias = type; break;
			}
			patternUnaryPost(symbols, type, alias);
			break;
		}
		case UNARY_PRE:
		{
			enum SymbolType alias;
			switch (type)
			{
				case INCREMENT_PRE: alias = INCREMENT; break;
				case DECREMENT_PRE: alias = DECREMENT; break;
				case UPLUS: alias = PLUS; break;
				case UMINUS: alias = MINUS; break;
				case DEREFERENCE: alias = ASTERISK; break;
				case ADDRESS: alias = AMPERSAND; break;
				default: alias = type; break;
			}
			patternUnaryPre(symbols, type, alias);
			break;
		}
		case BINARY:
		{
			enum SymbolType alias;
			switch (type)
			{
				case ADD: alias = PLUS; break;
				case SUBTRACT: alias = MINUS; break;
				case MULTIPLY: alias = ASTERISK; break;
				case BAND: alias = AMPERSAND; break;
				default: alias = type; break;
			}
			patternBinary(symbols, type, alias);
			break;
		}
		case TERNARY:
		{
/*			enum SymbolType first, second;
			switch (type)
			{
				case TERNARY_CONDITIONAL:
					first = COLON;
					second = QUESTION;
					break;
				default:
					first = type;
					second = type;
					break;
			}
			patternTernary(symbols, type, first, second);
*/
			break;
		}
	}
}

void patternContainer(struct Symbol* symbols, enum SymbolType type, enum SymbolType open, enum SymbolType close)
{
	struct Symbol* current = symbols;
	struct Symbol* openSymbols[32];
	int depth = 0;
	while (current)
	{
		// check if it is open symbol
		if (current->type == open)
		{
			openSymbols[depth] = current;
			depth++;
		}
		// check if it is close symbol
		if (current->next && current->next->type == close)
		{
			// connect it to last open: set rhs to inside, next to after
			struct Symbol* container = openSymbols[depth - 1];
			container->type = type;
			// check if empty
			if (current == container)
			{
				struct Symbol* ebrack = current->next;
				container->next = current->next->next;
				deleteSymbol(ebrack);
			}
			else
			{
				container->rhs = container->next;
				container->next = current->next->next;
				current->next = NULL;
				current = container;
			}
			// process contents
			combine(container->rhs);
			depth--;
		}
		current = current->next;
	}
}

void patternUnaryPost(struct Symbol* symbols, enum SymbolType type, enum SymbolType alias)
{
}

void patternUnaryPre(struct Symbol* symbols, enum SymbolType type, enum SymbolType alias)
{
	struct Symbol* current = symbols;
	while (current && current->next)
	{
		if (current->type == alias)
		{
			current->type = type;
			struct Symbol* var = current->next;
			current->rhs = var;
			current->next = var->next;
			var->next = NULL;
		}
		current = current->next;
	}
}

void patternBinary(struct Symbol* symbols, enum SymbolType type, enum SymbolType alias)
{
	struct Symbol* current = symbols;

	// if at beginning, combine by copy instead of modifying pointer
	if (current && current->next && current->next->type == alias && current->next->next)
	{
		current->next->type = type;
		// find lhs and rhs
		struct Symbol* lhs = newSymbol(BLANK);
		memcpy(lhs, current, sizeof(struct Symbol));
		struct Symbol* rhs = current->next->next;
		// copy operator to current
		memcpy(current, current->next, sizeof(struct Symbol));
		// set lhs and rhs
		current->lhs = lhs;
		current->rhs = rhs;
		current->next = rhs->next;
		lhs->next = NULL;
		rhs->next = NULL;
	}
	// if in middle, combine by moving pointers
	while (current && current->next && current->next->next && current->next->next->next)
	{
		struct Symbol* operator = current->next->next;
		if (operator->type == alias)
		{
			operator->type = type;
			struct Symbol* lhs = current->next;
			struct Symbol* rhs = current->next->next->next;
			operator->lhs = lhs;
			operator->rhs = rhs;
			operator->next = rhs->next;
			lhs->next = NULL;
			rhs->next = NULL;
			current->next = operator;
		}
		current = current->next;
	}
}

void patternTernary(struct Symbol* symbols, enum SymbolType type, enum SymbolType first, enum SymbolType second)
{
}
