int foo(int arg)
{
	return arg + 5;
}

int recurse(int val)
{
	printf("%i\n", val);
	if (val)
	{
		return recurse(val - 1);
	}
	return 0;
}

int main(int argc, int argv)
{
	int indice = 6;
	while (indice)
	{
		printf("%i\n", indice);
		indice = indice - 1;
	}
	
	if (1)
	{
		printf("it works\n");
	}
	if (0)
	{
		printf("this shouldn't happen\n");
	}

	int z = 10;
	int address = &z;
	int deref = *address;
	printf("%i\n", deref);
	
	int g = 3 + 4 * (5 + 2);
	printf("%i\n", g);
	
	int str = malloc(10);
	strcpy(str, "hello");
	printf("%s\n", str);
	free(str);

	int c;
	int d;
	c = d = 3;
	printf("%i %i\n", c, d);

	printf("%i\n", foo(5));

	recurse(5);

	return 0;
}

