/* =============================================================================
 * @file defs.h
 * @author Luke Shimanuki
 * @date 28 Dec 2014
 * @brief Definitions for the main function and functions accessed by it.
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

#ifndef __DEFS_H__
#define __DEFS_H__

struct Symbol;
struct String;

/***************************************************************************//**
 * @brief main function
 *
 * @param argc The number of command line arguments.
 *
 * @param argv An array containing the command line arguments.
 *
 * @return The error level. 0 means no error.
 ******************************************************************************/
int main(int argc, char** argv);

/***************************************************************************//**
 * @brief reads data from stream
 *
 * @param file The input stream
 *
 * @return A buffer containing the contents of the stream.
 ******************************************************************************/
char* read(FILE* file);

/***************************************************************************//**
 * @brief parses data into symbols
 *
 * @param data The buffer of data to be translated into symbols.
 *
 * @return A tree of symbols representing the data.
 ******************************************************************************/
struct Symbol* parse(char* data);

/***************************************************************************//**
 * @brief creates assembly from symbols
 *
 * @param symbols Symbolic tree to be converted to assembly.
 *
 * @return A buffer containing the assembly code.
 ******************************************************************************/
struct String* compile(struct Symbol* symbols);

/***************************************************************************//**
 * @brief writes data to stream
 *
 * @param file The output stream.
 *
 * @param data The list of strings that is to be written.
 *
 * @return The number of bytes that were written.
 ******************************************************************************/
size_t write(FILE* file, struct String* data);

#endif /* __DEFS_H__ */

