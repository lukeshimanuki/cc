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
	struct Symbol* current;

	// parentheses: bparen, symbols, eparen
	current = symbols;
	struct Symbol* parentheses[32];
	int numParentheses = 0;
	while (current)
	{
		// check if it is (
		if (current->type == BPAREN)
		{
			parentheses[numParentheses] = current;
			numParentheses++;
		}
		// check if it is )
		if (current->next && current->next->type == EPAREN)
		{
			// connect it to last (: set rhs to inside, next to after
			struct Symbol* paren = parentheses[numParentheses - 1];
			paren->type = PARENTHESES;
			// check if empty
			if (current == paren)
			{
				struct Symbol* eparen = current->next;
				paren->next = current->next->next;
				deleteSymbol(eparen);
			}
			else
			{
				paren->rhs = paren->next;
				paren->next = current->next->next;
				current->next = NULL;
				current = paren;
			}
			// process contents
			combine(paren->rhs);
			numParentheses--;
		}
		current = current->next;
	}
	// brackets: bbrack, symbols, ebrack
	current = symbols;
	struct Symbol* brackets[32];
	int numBrackets = 0;
	while (current)
	{
		// check if it is {
		if (current->type == BBRACK)
		{
			brackets[numBrackets] = current;
			numBrackets++;
		}
		// check if it is }
		if (current->next && current->next->type == EBRACK)
		{
			// connect it to last {: set rhs to inside, next to after
			struct Symbol* bracket = brackets[numBrackets - 1];
			bracket->type = BRACKET;
			// check if empty
			if (current == bracket)
			{
				struct Symbol* ebrack = current->next;
				bracket->next = current->next->next;
				deleteSymbol(ebrack);
			}
			else
			{
				bracket->rhs = bracket->next;
				bracket->next = current->next->next;
				current->next = NULL;
				current = bracket;
			}
			// process contents
			combine(bracket->rhs);
			numBrackets--;
		}
		current = current->next;
	}
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
	// declare: type, rhs
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
	// mult/div/mod: prev, lhs, operator, rhs
	// doesn't work as first in line (cuz needs prev)
	current = symbols;
	while (current && current->next && current->next->next && current->next->next->next)
	{
		struct Symbol* operator = current->next->next;
		struct Symbol* lhs = current->next;
		struct Symbol* rhs = current->next->next->next;
		if (operator->type == MULTIPLY || operator->type == DIVIDE) // insert mod
		{
			operator->lhs = lhs;
			operator->rhs = rhs;
			operator->next = rhs->next;
			lhs->next = NULL;
			rhs->next = NULL;
			current->next = operator;
		}
		current = current->next;
	}
	// add/sub: prev, lhs, operator, rhs
	// doesn't work as first in line (cuz needs prev)
	current = symbols;
	while (current && current->next && current->next->next && current->next->next->next)
	{
		struct Symbol* operator = current->next->next;
		struct Symbol* lhs = current->next;
		struct Symbol* rhs = current->next->next->next;
		if (operator->type == ADD || operator->type == SUBTRACT)
		{
			operator->lhs = lhs;
			operator->rhs = rhs;
			operator->next = rhs->next;
			lhs->next = NULL;
			rhs->next = NULL;
			current->next = operator;
		}
		current = current->next;
	}
	// assign: prev, lhs, assign, rhs
	current = symbols;
	// if at beginning, combine by copy instead of modifying pointer
	if (current && current->next && current->next->type == ASSIGN && current->next->next)
	{
		// find lhs and rhs
		struct Symbol* lhs = newSymbol(BLANK);
		memcpy(lhs, current, sizeof(struct Symbol));
		struct Symbol* rhs = current->next->next;
		// copy assign to current
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
		struct Symbol* assign = current->next->next;
		if (assign->type == ASSIGN)
		{
			struct Symbol* lhs = current->next;
			struct Symbol* rhs = current->next->next->next;
			assign->lhs = lhs;
			assign->rhs = rhs;
			assign->next = rhs->next;
			lhs->next = NULL;
			rhs->next = NULL;
			current->next = assign;
		}
		current = current->next;
	}
	// return: ret, (optional rhs)
	current = symbols;
	while (current)
	{
		if (current->type == RETURN && current->next && current->next->type != SEMICOLON)
		{
			current->rhs = current->next;
			current->next = current->rhs->next;
			current->rhs->next = NULL;
		}
		current = current->next;
	}
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
