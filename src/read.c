/* =============================================================================
 * @file read.c
 * @author Luke Shimanuki
 * @date 1 Nov 2014
 * @brief Contains a function to load files into memory.
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

/***************************************************************************//**
 * The filename is passed as a string, and the buffer containing the
 * contents is returned.
 ******************************************************************************/
char* read(char* fileName)
{
	char* dest;
	size_t size;
	FILE* file;

	// open file
	file = fopen(fileName, "r");
	if (file)
	{
		// find the last character to determine the size
		fseek(file, 0, SEEK_END);
		size = ftell(file);
		fseek(file, 0, SEEK_SET);
		// allocate memory
		dest = malloc(size + 1);
		if (dest)
		{
			// read file into buffer
			if (fread(dest, 1, size, file) == size)
			{
				fclose(file);
				// null terminate the string
				dest[size] = 0;
				return dest;
			}
			free(dest);
		}
		fclose(file);
	}

	return NULL;
}
