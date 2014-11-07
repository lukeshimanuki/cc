#include <stdlib.h>
#include <string.h>

#include "parse.h"
#include "symbol.h"

struct Symbol* parse(char* data)
{
	struct Symbol* symbols = translate(data);
	return symbols;
//	struct Symbol* tree = combine(symbols);
//	return tree;
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


// looks for patterns and combines
struct Symbol* combine(struct Symbol* symbols)
{
	// functions: type, variable, parentheses, bracket
	while (symbols)
	{
		// check if 4 symbols exist
		if (symbols->next && symbols->next->next && symbols->next->next->next)
		{
			// check pattern
			if (symbols->type == TYPE && symbols->next->type == VARIABLE && symbols->next->next->type == PARENTHESES && symbols->next->next->next->type == BRACKET)
			{
				// turn it into a function
				symbols->type = FUNCTION;
				// ignore type...
				// set id to same as variable id
				symbols->id = symbols->next->id;
				// set lhs to inside of parentheses
				symbols->lhs = symbols->next->next->rhs;
				// set rhs to inside of brackets
				symbols->rhs = symbols->next->next->next->rhs;
				// remove the 3 extra symbols
				struct Symbol* extra = symbols->next;
				symbols->next = symbols->next->next->next->next;
				deleteSymbol(extra->next->next);
				deleteSymbol(extra->next);
				deleteSymbol(extra);
			}
		}
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
