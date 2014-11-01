#include <stdio.h>

#include "defs.h"

int main(int argc, char** argv)
{
	// the first argument is the compiler
	// if there is no second argument, there is no file to compile
	if (argc < 2)
	{
		printf("Error: not enough arguments\n");
		return 0;
	}

	// compile the first file
	// read the contents of the file
	char* data = read(argv[1]);
	// parse the file into organized structures
//	parse(jkjkj, file);
	// generate assembly
	char* assembly;
//	compile(assembly, jkjkj);
	// write to file
	write("out.s", data);

	return 0;
}
