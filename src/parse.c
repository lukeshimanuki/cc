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

struct Symbol* translate(char* data)
{
	struct Symbol* symbols = newSymbol(BLANK);
	// repeat until \0
	while (data[0] != '\0')
	{
		// one time redundancy, but whatever
		enum CharType type = getType(data[0]);
		char word[32];
		size_t indice = 0;
		while (getType(data[indice]) == type)
		{
			word[indice] = data[indice];
			indice++;
		}
		word[indice] = '\0'; // null terminate
		// remove word from data
		data += strlen(word);
		
		switch (type)
		{
			case WHITESPACE:
				// do nothing
				break;
			case SPECIAL:
			{
				size_t id = 0;
				size_t i = 0;
				// sizeof arr / sizeof element -> number of elements
				while (id < sizeof(operators) / sizeof(operators[0]) && strlen(word + i) > 0)
				{
					// if it is this operator
					if (strncmp(word + i, operators[id], strlen(operators[id])) == 0)
					{
						// add the symbol
						addSymbol(symbols, newSymbol(getSymbolType(id)));
						// increase indice
						i += strlen(operators[id]);
						// restart search
						id = 0;
					}
				}
				break;
			}
			case NORMAL:
			{
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
	return symbols;
}

struct Symbol* combine(struct Symbol* symbols)
{
	return NULL;
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
			return SPECIAL;
		default:
			return NORMAL;
	}
}

enum Type getSymbolType(size_t operatorID)
{
	switch (operatorID)
	{
		case 0:
			return ADD;
		case 1:
			return SUBTRACT;
		case 2:
			return MULTIPLY;
		case 3:
			return DIVIDE;
	}
	return BLANK;
}
