/* =============================================================================
 * @file compile.h
 * @author Luke Shimanuki
 * @date 2 Nov 2014
 * @brief Functions for converting symbolic structure to assembly.
 *
 * This file is part of MCC.
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

#include <stdio.h>

#include "symbol.h"
#include "string.h"
#include "scope.h"

struct Scope* scope = NULL;

enum Pass
{
	VAL,
	REF
};

struct String* getAssembly(struct Symbol* symbols, enum Pass pass);

struct String* data = NULL;

struct String* compile(struct Symbol* symbols)
{
	char buffer[512];
	scope = newScope();
	data = newString(0);
	struct String* text = NULL;
	while (symbols)
	{
		if (text == NULL)
			text = getAssembly(symbols, VAL);
		else
			addString(text, getAssembly(symbols, VAL));
		symbols = symbols->next;
	}
	// insert data before text
	addString(data, text);
	return data;
}

// iterative over lists, recursive over children
// eg: recurse over function->parameters->declare->variable
// iterative over declare, declare, add, call
// but the string list is iterative
struct String* getAssembly(struct Symbol* symbols, enum Pass pass)
{
	struct String* assembly = NULL;
	if (symbols)
	{
		// create empty string
		struct String* current = newString(0);
		// create temporary buffer
		char buffer[32] = "";
		// process the current symbol
		switch (symbols->type)
		{
			// results stored in %eax
			case VARIABLE:
				sprintf(buffer, "\t# var %i %s\n", symbols->id, symbols->name);
				addString(current, getString(buffer));
				if (pass == VAL)
					sprintf(buffer, "\tmov %i(%%ebp),%%eax\n", getOffset(scope, symbols->id));
				else
					sprintf(buffer, "\tlea %i(%%ebp),%%eax\n", getOffset(scope, symbols->id));
				addString(current, getString(buffer));
				break;
			case STRING: // strings are stored as "STR#:" where # is the id
				addString(current, getString("\t# str\n"));
				// store string in read only data section
				addString(data, getString("\t.section .rodata\n"));
				sprintf(buffer, "STR%i:\n", symbols->id);
				addString(data, getString(buffer));
				sprintf(buffer, "\t.string \"%s\"\n", symbols->string);
				addString(data, getString(buffer));
				// reference string's location
				sprintf(buffer, "\tmov $STR%i,%%eax\n", symbols->id);
				addString(current, getString(buffer));
				break;
			case VALUE:
				addString(current, getString("\t# val\n"));
				sprintf(buffer, "\tmov $%i,%%eax\n", symbols->value);
				addString(current, getString(buffer));
				break;
			case TYPE:
				addString(current, getString("\t# type\n"));
				break;
			case DECLARE:
				sprintf(buffer, "\t# dec %i %s\n", symbols->rhs->id, symbols->rhs->name);
				addString(current, getString(buffer));
				addString(current, getString("\tsub $4,%esp\n"));
				if (pass == VAL)
					addString(current, getString("\tmov (%esp),%eax\n"));
				else
					addString(current, getString("\tmov %esp,%eax\n"));
				addVariable(scope, symbols->rhs->id, 4);
				break;
			case PARENTHESES: // rhs: symbol
				addString(current, getString("\t# paren\n"));
				if (symbols->rhs)
					addString(current, getAssembly(symbols->rhs, pass));
				break;
			case BRACKET: // rhs: instructions (arrays are given a separate symbol)
			{
	 			addString(current, getString("\t# brack\n"));
				// TODO: implement scoping
				struct Symbol* symbol = symbols->rhs;
				while (symbol)
				{
					addString(current, getAssembly(symbol, VAL));
					symbol = symbol->next;
				}
				break;
			}
			case FUNCTION: // lhs: parameters; rhs: instructions
			{
				addString(current, getString("\t# func\n"));
				// header
				addString(current, getString("\t.text\n"));
				sprintf(buffer, "\t.globl %s\n", symbols->name);
				addString(current, getString(buffer));
				sprintf(buffer, "\t.type %s, @function\n", symbols->name);
				addString(current, getString(buffer));
				// label
				sprintf(buffer, "%s:\n", symbols->name);
				addString(current, getString(buffer));
				// save stack pointer
				addString(current, getString("\tpush %ebp\n"));
				addString(current, getString("\tmov %esp,%ebp\n"));
				// create scope
				struct Scope* oldScope = scope;
				scope = newScope();
				// process parameters
				struct Symbol* parameter = symbols->lhs;
				unsigned int parameterIndex = 0;
				while (parameter)
				{
					// declare: reference to var is stored in %eax and pushed
					addString(current, getAssembly(parameter, REF));
					addString(current, getString("\tmov %eax,%ecx\n"));
					// evaluate argument: +8 because we push %ebp and instruction pointer
					sprintf(buffer, "\tmov %i(%%ebp),%%eax\n", 4 * parameterIndex + 8);
					addString(current, getString(buffer));
					// assign
					addString(current, getString("\tmov %eax,(%ecx)\n"));
					parameterIndex++;
					parameter = parameter->next;
				}
				// process instructions
				struct Symbol* instruction = symbols->rhs;
				while (instruction)
				{
					addString(current, getAssembly(instruction, VAL));
					instruction = instruction->next;
				}
				// restore scope
				deleteScope(scope);
				scope = oldScope;
				break;
			}
			
			// 1
			/*
			case INCREMENT_POST: // lhs: operand
				addString(current, getString("\t# inc post\n"));
				addString(current, getAssembly(symbols->lhs));
				
				break;*/
			case CALL: // rhs: arguments
			{
				addString(current, getString("\t# call\n"));
				addString(current, getString(buffer));
				// process arguments in reverse order
				int numArgs = 0;
				struct Symbol* argument = symbols->rhs;
				struct String* args = NULL;
				while (argument)
				{
					numArgs++;
					// evaluate
					struct String* arg = getAssembly(argument, VAL);
					// push
					addString(arg, getString("\tpush %eax\n"));
					// insert string at beginning
					addString(arg, args);
					args = arg;
					argument = argument->next;
				}
				addString(current, args);
				// call the function, the result is stored in %eax
				// it implicitly pushes the instruction pointer
				sprintf(buffer, "\tcall %s\n", symbols->name);
				addString(current, getString(buffer));
				// pop arguments
				sprintf(buffer, "\tadd $%i,%%esp\n", numArgs * 4);
				addString(current, getString(buffer));
				break;
			}

			// 2

			// 3
			case MULTIPLY:
				addString(current, getString("\t# mult\n"));
				// place lhs in %eax and rhs in %ecx
				addString(current, getAssembly(symbols->rhs, VAL));
				addString(current, getString("\tpush %eax\n"));
				addString(current, getAssembly(symbols->lhs, VAL));
				addString(current, getString("\tpop %ecx\n"));
				addString(current, getString("\timul %ecx,%eax\n"));
				break;
			case DIVIDE: // a / b: lhs = a, rhs = b
				addString(current, getString("\t# div\n"));
				// place lhs in %eax and rhs in %ecx, %edx = 0
				addString(current, getAssembly(symbols->rhs, VAL));
				addString(current, getString("\tpush %eax\n"));
				addString(current, getAssembly(symbols->lhs, VAL));
				addString(current, getString("\tpop %ecx\n"));
				addString(current, getString("\tmov $0,%edx\n"));
				addString(current, getString("\tidiv %ecx\n"));
				break;
			case ADD:
				addString(current, getString("\t# add\n"));
				// place lhs in %eax and rhs in %ecx
				addString(current, getAssembly(symbols->rhs, VAL));
				addString(current, getString("\tpush %eax\n"));
				addString(current, getAssembly(symbols->lhs, VAL));
				addString(current, getString("\tpop %ecx\n"));
				addString(current, getString("\tadd %ecx,%eax\n"));
				break;
			case SUBTRACT: // a - b: lhs = a, rhs = b
				addString(current, getString("\t# sub\n"));
				// place lhs in %eax and rhs in %ecx
				addString(current, getAssembly(symbols->rhs, VAL));
				addString(current, getString("\tpush %eax\n"));
				addString(current, getAssembly(symbols->lhs, VAL));
				addString(current, getString("\tpop %ecx\n"));
				addString(current, getString("\tsub %ecx,%eax\n"));
				break;
			case ASSIGN:
				addString(current, getString("\t# assi\n"));
				// evaluate location and push it (to be popped into %ecx)
				addString(current, getAssembly(symbols->lhs, REF));
				addString(current, getString("\tpush %eax\n"));
				// evaluate value and store it in %eax
				addString(current, getAssembly(symbols->rhs, VAL));
				addString(current, getString("\tpop %ecx\n"));
				// store value in location
				addString(current, getString("\tmov %eax,(%ecx)\n"));
				// return the value (which is in %eax)
				break;
			case RETURN:
				addString(current, getString("\t# ret\n"));
				// if there is an operand (to the right), store it in %eax
				if (symbols->rhs)
					addString(current, getAssembly(symbols->rhs, VAL));
				// reset stack pointer and return
				addString(current, getString("\tmov %ebp,%esp\n"));
				addString(current, getString("\tpop %ebp\n"));
				addString(current, getString("\tret\n"));
				break;
			default:
				break;
		}
		// clean up empty strings
		while (current && sizeof(current->contents) == 0)
		{ // at beginning
			struct String* blank = current;
			current = current->next;
			deleteString(blank);
		}
		struct String* strIndex = current;
		while (strIndex && strIndex->next)
		{ // in middle
			if (sizeof(strIndex->next->contents) == 0)
			{
				struct String* blank = strIndex->next;
				strIndex->next = strIndex->next->next;
				deleteString(blank);
			}
			strIndex = strIndex->next;
		}
		if (assembly == NULL)
			assembly = current;
		else
			addString(assembly, current);
		// process the next symbol in the next cycle
		symbols = symbols->next;
	}
	return assembly;
}
