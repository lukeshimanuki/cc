/* =============================================================================
 * @file scope.h
 * @author Luke Shimanuki
 * @date 4 Nov 2014
 * @brief Structure and functions for symbolic parsing.
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

#ifndef __SCOPE_H__
#define __SCOPE_H__

/***************************************************************************//**
 * @struct Scope
 *
 * @brief This structure...
 *
 * base scope is most local, each "next" goes more global
 ******************************************************************************/
struct Scope
{
	size_t offset; // distance from beginning (the very first %ebp)
	// it is equivalent to the first %ebp - the current %ebp
	
	size_t bottom; // the bottom of this stack, increases with each push

	size_t variables[256];

	struct Scope* next;
};

/***************************************************************************//**
 * @brief Loads a file into a buffer.
 ******************************************************************************/
struct Scope* newScope();

void deleteScope(struct Scope* scope);

void addScope(struct Scope* base, size_t offset);

// returns distance from current %ebp
int getOffset(struct Scope* scope, int varID);

// adds new variable to scope
void addVariable(struct Scope* scope, int id, size_t size);

#endif /* __SCOPE_H__ */
