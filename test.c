#include <stdio.h>

void	funct(int *i)
{
	*i += 1;
	printf("i = %p\n", i);
}

int main(void)
{
	int	i;

	i = 0;
	printf("i = %p\n", &i);
	funct(&i);
	printf("i = %d\n", i);
	return (0);
}
