/* =============================================================================
 * @file read.c
 * @author Luke Shimanuki
 * @date 28 Dec 2014
 * @brief Implementation of the read function.
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
#include <stdlib.h>
#include <string.h>

/***************************************************************************//**
 * Reads all content from the given stream until an EOF is reached. The data is
 * stored in a dynamically allocated buffer that is null terminated.
 ******************************************************************************/
char* read(FILE* file)
{
	char* dest = NULL;
	size_t size = 0;
	const size_t buffer_size = 512;
	char buffer[buffer_size];

	if (file)
	{
		// grab each character and add it to the buffer
		int c = fgetc(file);
		int i = 0;
		while (c != EOF)
		{
			buffer[i] = (char) c;
			i++;
			// if buffer is full, copy it to dest
			if (i == buffer_size)
			{
				size += buffer_size;
				dest = realloc(dest, size);
				memcpy(dest + size - buffer_size, buffer, buffer_size);
				i = 0;
			}
			c = fgetc(file);
		}
		// copy buffer to dest and null terminate
		dest = realloc(dest, size + i + 1);
		memcpy(dest + size, buffer, i);
		dest[size + i] = '\0';
		
		if (strlen(dest) != 0)
			return dest;
		else
			return NULL;
	}

	return NULL;
}

