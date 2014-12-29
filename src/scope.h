/* =============================================================================
 * @file scope.h
 * @author Luke Shimanuki
 * @date 28 Dec 2014
 * @brief Definition of the scope structure and related functions.
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
 * @brief keeps track of object declarations
 *
 * The base scope is the most local, and each recursion becomes more global.
 ******************************************************************************/
struct Scope
{
	size_t offset;         /**< Total memory offset from the beginning (the very first %ebp). */
	size_t bottom;         /**< Memory offset between the base of this scope and the bottom. */
	size_t variables[256]; /**< The memory offset between each object in the scope and the base. */

	struct Scope* next;    /**< A pointer to the broader scope. */
};

/***************************************************************************//**
 * @brief scope object constructor
 *
 * @return A pointer to the scope object.
 ******************************************************************************/
struct Scope* newScope();

/***************************************************************************//**
 * @brief scope object destructor
 *
 * @param scope A pointer to the scope object to be destroyed.
 ******************************************************************************/
void deleteScope(struct Scope* scope);

/***************************************************************************//**
 * @brief add new scope to end
 *
 * @param base The scope to add a new scope to.
 *
 * @param offset The memory offset between the scopes.
 ******************************************************************************/
void addScope(struct Scope* base, size_t offset);

/***************************************************************************//**
 * @brief finds the memory offset of an object within a scope
 *
 * @param scope The scope to look for the object in.
 *
 * @param varID The id of the object to look for.
 *
 * @return The memory offset of the object.
 ******************************************************************************/
// returns distance from current %ebp
int getOffset(struct Scope* scope, int varID);

/***************************************************************************//**
 * @brief adds new object to scope
 *
 * @param scope The scope to add the object to.
 *
 * @param id The id of the object to be added.
 *
 * @param size The size in bytes of the object to be added.
 ******************************************************************************/
void addVariable(struct Scope* scope, int id, size_t size);

#endif /* __SCOPE_H__ */

