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

#include "string.h"

/***************************************************************************//**
 * The contents of data are written to the file specified by fileName. It
 * returns the number of bytes written.
 ******************************************************************************/
size_t write(char* fileName, struct String* data)
{
	FILE* file;

	// open file
	fopen(fileName, "w");
	if (file)
	{
		// write to file
		size_t numBytes = 0;
		while (data != NULL)
		{
			// only write if not empty
			if (data->contents)
				numBytes += fprintf(file, "%s", data->contents);
			data = data->next;
		}
		return numBytes;
	}

	return 0;
}
