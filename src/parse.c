/* =============================================================================
 * @file parse.c
 * @author Luke Shimanuki
 * @date 28 Dec 2014
 * @brief Implementation of functions relating to parsing.
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
#include <string.h>

#include "parse.h"
#include "symbol.h"

/***************************************************************************//**
 * Parses the code in the buffer to create a tree of symbols.
 ******************************************************************************/
struct Symbol* parse(char* data)
{
	struct Symbol* symbols = translate(data);
	symbols = interpret(symbols);
	printSymbol(stderr, symbols, 0);
	struct Symbol* tree = combine(symbols);
	return tree;
}

char* names[256];
size_t numNames = 0;
int numStrings = 0;

/***************************************************************************//**
 * Processes each word in the buffer and directly converts it to a symbol.
 ******************************************************************************/
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
				// same as with operators, but each word can only be one keyword
				size_t keywordID = 0;
				char found = 0;
				while (keywordID < sizeof(keywords) / sizeof(keywords[0]))
				{
					// if it is this keyword
					if (strlen(word) == strlen(keywordString(keywords[keywordID])) && strncmp(word, keywordString(keywords[keywordID]), strlen(word)) == 0)
					{
						// add the symbol
						addSymbol(symbols, newSymbol(keywords[keywordID]));
						found = 1;
						break;
					}
					// otherwise, try next keyword
					keywordID++;
				}
				// if it was a keyword, skip the rest
				if (found)
					break;
				
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

/***************************************************************************//**
 * Determines whether the specified symbol can be an operand on the left hand
 * side of the operator. Typically, operators that require an operand following
 * it cannot be a left hand side operand.
 ******************************************************************************/
int isOperand(enum SymbolType type)
{
	switch (type)
	{
		// the symbols that can be an operand because
		// they do not require an operand after
		// special
		case VARIABLE:
		case STRING:
		case VALUE:
		case TYPE:
		case SEMICOLON:
		case COMMA:
		// unary postfix
		case INCREMENT_POST:
		case DECREMENT_POST:
			return 1;
		default:
			return 0;
	}
}

/***************************************************************************//**
 * Some operators can be used in different ways. In this stage, the context on
 * both sides of the operator is used to determine which symbol it is.
 ******************************************************************************/
struct Symbol* interpret(struct Symbol* symbols)
{
	struct Symbol* current = symbols;
	// if at beginning, not operand before
	switch (current->type)
	{
		case PLUS: current->type = UPLUS; break;
		case MINUS: current->type = UMINUS; break;
		case ASTERISK: current->type = DEREFERENCE; break;
		case AMPERSAND: current->type = ADDRESS; break;
		case INCREMENT: current->type = INCREMENT_PRE; break;
		case DECREMENT: current->type = DECREMENT_PRE; break;
		default: break;
	}
	while (current && current->next)
	{
		switch (current->next->type)
		{
			// if there is an operand before +/-,
			// then it is add/sub; otherwise it
			// is unary plus/minus
			case PLUS:
				if (isOperand(current->type))
					current->next->type = ADD;
				else
					current->next->type = UPLUS;
				break;
			case MINUS:
				if (isOperand(current->type))
					current->next->type = SUBTRACT;
				else
					current->next->type = UMINUS;
				break;
			// multiply has operand before
			// dereference does not
			case ASTERISK:
				if (isOperand(current->type))
					current->next->type = MULTIPLY;
				else
					current->next->type = DEREFERENCE;
				break;
			// bitwise and has operand before
			// address does not
			case AMPERSAND:
				if (isOperand(current->type))
					current->next->type = BAND;
				else
					current->next->type = ADDRESS;
				break;
			// inc/dec post has operand before
			// inc/dec pre does not
			case INCREMENT:
				if (isOperand(current->type))
					current->next->type = INCREMENT_POST;
				else
					current->next->type = INCREMENT_PRE;
				break;
			case DECREMENT:
				if (isOperand(current->type))
					current->next->type = DECREMENT_POST;
				else
					current->next->type = DECREMENT_PRE;
				break;
			default:
				break;
		}
		current = current->next;
	}
	return symbols;
}

/***************************************************************************//**
 * Creates a tree structure by moving symbol operands inside the parent symbol.
 ******************************************************************************/
struct Symbol* combine(struct Symbol* symbols)
{
	struct Symbol* current;

	symbols = findPattern(symbols, CONTAINER, 0, PARENTHESES);
	symbols = findPattern(symbols, CONTAINER, 0, BRACKET);
	// only combines the inside of braces, does not connect to lhs
	symbols = findPattern(symbols, CONTAINER, 0, SUBSCRIPT);

	// functions: type, variable, parentheses, bracket
	current = symbols;
	while (current)
	{
		// check if 4 symbols exist
		if (current->next && current->next->next && current->next->next->next)
		{
			// check pattern
			if ((current->type == TYPE || current->type == INT || current->type == SHORT || current->type == LONG || current->type == CHAR || current->type == FLOAT || current->type == DOUBLE) && current->next->type == VARIABLE && current->next->next->type == PARENTHESES && current->next->next->next->type == BRACKET)
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
		if ((current->type == TYPE || current->type == INT || current->type == SHORT || current->type == LONG || current->type == CHAR || current->type == FLOAT || current->type == DOUBLE) && current->next->type == VARIABLE)
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
	// if: if, paren, command/brack
	current = symbols;
	while (current && current->next)
	{
		if (current->type == IF && current->next->type == PARENTHESES)
		{
			// move inside of paren into lhs
			struct Symbol* paren = current->next;
			current->lhs = paren->rhs;
			struct Symbol* command = current->next->next;
			current->next = command->next;
			// take as single command, even if brack
			current->rhs = command;
			command->next = NULL;
			deleteSymbol(paren);
		}
		current = current->next;
	}
	// while: while, paren, command/brack
	current = symbols;
	while (current && current->next)
	{
		if (current->type == WHILE && current->next->type == PARENTHESES)
		{
			// move inside of paren into lhs
			struct Symbol* paren = current->next;
			current->lhs = paren->rhs;
			struct Symbol* command = current->next->next;
			current->next = command->next;
			// take as single command, even if brack
			current->rhs = command;
			command->next = NULL;
			deleteSymbol(paren);
		}
		current = current->next;
	}

	// TODO: make it so the operators in a group are all searched at
	// the same time
	symbols = findPattern(symbols, UNARY_POST, RIGHT, INCREMENT_POST);
	symbols = findPattern(symbols, UNARY_POST, RIGHT, DECREMENT_POST);
	symbols = findPattern(symbols, BINARY, RIGHT, MEMBER);
	symbols = findPattern(symbols, BINARY, RIGHT, PTR_MEMBER);

	symbols = findPattern(symbols, UNARY_PRE, LEFT, INCREMENT_PRE);
	symbols = findPattern(symbols, UNARY_PRE, LEFT, DECREMENT_PRE);
	symbols = findPattern(symbols, UNARY_PRE, LEFT, UPLUS);
	symbols = findPattern(symbols, UNARY_PRE, LEFT, UMINUS);
	symbols = findPattern(symbols, UNARY_PRE, LEFT, NOT);
	symbols = findPattern(symbols, UNARY_PRE, LEFT, BNOT);
	symbols = findPattern(symbols, UNARY_PRE, LEFT, CAST);
	symbols = findPattern(symbols, UNARY_PRE, LEFT, DEREFERENCE);
	symbols = findPattern(symbols, UNARY_PRE, LEFT, ADDRESS);
	symbols = findPattern(symbols, UNARY_PRE, LEFT, SIZEOF); 

	symbols = findPattern(symbols, BINARY, RIGHT, MULTIPLY);
	symbols = findPattern(symbols, BINARY, RIGHT, DIVIDE);
	symbols = findPattern(symbols, BINARY, RIGHT, MODULUS);
	
	symbols = findPattern(symbols, BINARY, RIGHT, ADD);
	symbols = findPattern(symbols, BINARY, RIGHT, SUBTRACT);

	symbols = findPattern(symbols, BINARY, RIGHT, LSHIFT);
	symbols = findPattern(symbols, BINARY, RIGHT, RSHIFT);

	symbols = findPattern(symbols, BINARY, RIGHT, LESS);
	symbols = findPattern(symbols, BINARY, RIGHT, LESS_OR_EQUAL);
	symbols = findPattern(symbols, BINARY, RIGHT, GREATER);
	symbols = findPattern(symbols, BINARY, RIGHT, GREATER_OR_EQUAL);
	
	symbols = findPattern(symbols, BINARY, RIGHT, EQUAL);
	symbols = findPattern(symbols, BINARY, RIGHT, NEQUAL);

	symbols = findPattern(symbols, BINARY, RIGHT, BAND);
	
	symbols = findPattern(symbols, BINARY, RIGHT, BXOR);

	symbols = findPattern(symbols, BINARY, RIGHT, BOR);

	symbols = findPattern(symbols, BINARY, RIGHT, AND);

	symbols = findPattern(symbols, BINARY, RIGHT, OR);

	symbols = findPattern(symbols, TERNARY, LEFT, TERNARY_CONDITIONAL);

	symbols = findPattern(symbols, BINARY, LEFT, ASSIGN);
	symbols = findPattern(symbols, BINARY, LEFT, AADD);
	symbols = findPattern(symbols, BINARY, LEFT, ASUBTRACT);
	symbols = findPattern(symbols, BINARY, LEFT, AMULTIPLY);
	symbols = findPattern(symbols, BINARY, LEFT, ADIVIDE);
	symbols = findPattern(symbols, BINARY, LEFT, AMODULUS);
	symbols = findPattern(symbols, BINARY, LEFT, ALSHIFT);
	symbols = findPattern(symbols, BINARY, LEFT, ARSHIFT);
	symbols = findPattern(symbols, BINARY, LEFT, ABAND);
	symbols = findPattern(symbols, BINARY, LEFT, ABXOR);
	symbols = findPattern(symbols, BINARY, LEFT, ABOR);

//	symbols = findPattern(symbols, BINARY, RIGHT, COMMA);

	symbols = findPattern(symbols, UNARY_PRE, LEFT, RETURN);

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

/***************************************************************************//**
 * Determines whether the given character is whitespace, special, or normal. A
 * special character is typically reserved for operators.
 ******************************************************************************/
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
		case '%':
		case '=':
		case '>':
		case '<':
		case '(':
		case ')':
		case '{':
		case '}':
		case '[':
		case ']':
		case '&':
		case '|':
		case '^':
		case '!':
		case '~':
		case '.':
		case '?':
		case ':':
		case ';':
		case ',':
			return SPECIAL;
		default:
			return NORMAL;
	}
}

/***************************************************************************//**
 * Searches the symbol list to find the given symbol in the given pattern. The
 * list is traversed in the specified direction.
 ******************************************************************************/
struct Symbol* findPattern(struct Symbol* symbols, enum Pattern pattern, enum Direction direction, enum SymbolType type)
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
			return patternContainer(symbols, type, open, close);
			break;
		}
		case UNARY_POST: return patternUnaryPost(symbols, direction, type);
		case UNARY_PRE: return patternUnaryPre(symbols, direction, type);
		case BINARY: return patternBinary(symbols, direction, type);
		case TERNARY:
		{
			enum SymbolType first, second;
			switch (type)
			{
				case TERNARY_CONDITIONAL:
					first = QUESTION;
					second = COLON;
					break;
				default:
					first = type;
					second = type;
					break;
			}
			return patternTernary(symbols, direction, type, first, second);
		}
	}
}

/***************************************************************************//**
 * Searches the symbol list for a container. These are a set of symbols that are
 * enclosed by an opening symbol and a closing symbol.
 ******************************************************************************/
struct Symbol* patternContainer(struct Symbol* symbols, enum SymbolType type, enum SymbolType open, enum SymbolType close)
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
			container->rhs = combine(container->rhs);
			depth--;
		}
		current = current->next;
	}
	return symbols;
}

/***************************************************************************//**
 * Searches the symbol list for a unary postfix pattern. These are single
 * operand symbols in which the operator appears after the operand.
 ******************************************************************************/
struct Symbol* patternUnaryPost(struct Symbol* symbols, enum Direction direction, enum SymbolType type)
{
	if (direction == RIGHT)
	{
		while (symbols && symbols->next && symbols->next->type == type)
		{
			struct Symbol* operator = symbols->next;
			struct Symbol* operand = symbols;
			operator->lhs = operand;
			operand->next = NULL;
			symbols = operator;
		}
		struct Symbol* current = symbols;
		while (current && current->next && current->next->next)
		{
			struct Symbol* operator = current->next->next;
			struct Symbol* operand = current->next;
			if (operator->type == type)
			{
				operator->lhs = operand;
				operand->next = NULL;
				current->next = operator;
			}
			else current = current->next;
		}
	}
	else
	{
		symbols = reverseSymbol(symbols);
		struct Symbol* last = symbols;
		struct Symbol* current = symbols;
		while (current && current->next)
		{
			if (current->type == type)
			{
				struct Symbol* var = current->next;
				current->lhs = var;
				current->next = var->next;
				var->next = NULL;
			}
			current = current->next;
		}
		symbols = reverseSymbol(last);
	}
	return symbols;
}

/***************************************************************************//**
 * Searches the symbol list for a unary prefix pattern. These are single operand
 * symbols in which the operator appears before the operand.
 ******************************************************************************/
struct Symbol* patternUnaryPre(struct Symbol* symbols, enum Direction direction, enum SymbolType type)
{
	if (direction == RIGHT)
	{
		struct Symbol* current = symbols;
		while (current && current->next)
		{
			if (current->type == type)
			{
				struct Symbol* var = current->next;
				current->rhs = var;
				current->next = var->next;
				var->next = NULL;
			}
			current = current->next;
		}
	}
	else
	{
		symbols = reverseSymbol(symbols);
		struct Symbol* last = symbols;
		while (symbols && symbols->next && symbols->next->type == type)
		{
			struct Symbol* operator = symbols->next;
			struct Symbol* operand = symbols;
			operator->rhs = operand;
			operand->next = NULL;
			symbols = operator;
		}
		struct Symbol* current = symbols;
		while (current && current->next && current->next->next)
		{
			struct Symbol* operator = current->next->next;
			struct Symbol* operand = current->next;
			if (operator->type == type)
			{
				operator->lhs = operand;
				operand->next = NULL;
				current->next = operator;
			}
			else current = current->next;
		}
		symbols = reverseSymbol(last);
	}
	return symbols;
}

/***************************************************************************//**
 * Searches the symbol list for a binary pattern. These are symbols in which
 * there is an operand on both sides.
 ******************************************************************************/
struct Symbol* patternBinary(struct Symbol* symbols, enum Direction direction, enum SymbolType type)
{
	struct Symbol* current = symbols;
	struct Symbol* last;
	
	if (direction == LEFT)
	{
		last = reverseSymbol(current);
		current = last;
	}

	// if at beginning
	while (current && current->next && current->next->type == type && current->next->next)
	{
		struct Symbol* operator = current->next;
		struct Symbol* lhs = current;
		struct Symbol* rhs = current->next->next;
		operator->lhs = lhs;
		operator->rhs = rhs;
		operator->next = rhs->next;
		lhs->next = NULL;
		rhs->next = NULL;
		current = operator;
		symbols = operator;
	}
	// if in middle
	while (current && current->next && current->next->next && current->next->next->next)
	{
		struct Symbol* operator = current->next->next;
		if (operator->type == type)
		{
			struct Symbol* lhs = current->next;
			struct Symbol* rhs = current->next->next->next;
			operator->lhs = lhs;
			operator->rhs = rhs;
			operator->next = rhs->next;
			lhs->next = NULL;
			rhs->next = NULL;
			current->next = operator;
		}
		else current = current->next;
	}

	if (direction == LEFT)
	{
		symbols = reverseSymbol(last);
	}
	return symbols;
}

// ignore dir for now
/***************************************************************************//**
 * Searches the symbol list for a ternary pattern. The ternary conditional
 * involves three operands separated by two operators.
 ******************************************************************************/
struct Symbol* patternTernary(struct Symbol* symbols, enum Direction direction, enum SymbolType type, enum SymbolType first, enum SymbolType second)
{/*
	while (symbols && symbols->next && symbols->next->type == first && symbols->next->next && symbols->next->next->next && symbols->next->next->next->type == second && symbols->next->next->next->next)
	{
		struct Symbol* operator = symbols->next;
		struct Symbol* extra = symbols->next->next->next;
		operator->type = type;
		operator->third = symbols; // third is conditional, first is if true, second is if false
		operator->lhs = symbols->next->next;
		operator->rhs = symbols->next->next->next->next;
		operator->next = operator->rhs->next;
		operator->third->next = NULL;
		operator->lhs->next = NULL;
		operator->rhs->next = NULL;
		deleteSymbol(extra);
		symbols = operator;
	}
*/
	// ISSUE: nodes repeat every other cycle
	struct Symbol* current = symbols;
	while (current && current->next && current->next->next && current->next->next->next && current->next->next->next->next && current->next->next->next->next->next)
	{
		struct Symbol* operator = current->next->next;
		if (operator->type == first && operator->next->next->type == second)
		{
			struct Symbol* extra = symbols->next->next->next->next;
			operator->type = type;
			operator->third = symbols->next; // third is conditional, first is if true, second is if false
			operator->lhs = symbols->next->next->next;
			operator->rhs = symbols->next->next->next->next->next;
			operator->next = operator->rhs->next;
			operator->third->next = NULL;
			operator->lhs->next = NULL;
			operator->rhs->next = NULL;
			deleteSymbol(extra);
			current->next = operator;
		}
		else current = current->next;
	}
	return symbols;
}

