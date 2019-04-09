#include <stdio.h>
#include <mach-o/fat.h>

typedef struct	s_test
{
	int	a;
	int b;
	int c;
}				t_test;

typedef struct	s_test_64
{
	int	a;
	int d;
	int b;
	int c;
}				t_test_64;

typedef union	u_fat_arch
{
	t_test_64	test_64;
	t_test	test;
}				t_fat_arch;

int main(void)
{
	t_fat_arch	fat_arch;

	fat_arch.test.b = 32;
	fat_arch.test_64.b = 64;
	if (!fat_arch.test.b)
		printf("Ok\n");
	printf("%d\n", fat_arch.test.b);
	printf("%d\n", fat_arch.test_64.b);
	return (0);
}
