/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/10 17:03:35 by lbopp             #+#    #+#             */
/*   Updated: 2019/05/10 17:06:25 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void	print_addr_hex(uint64_t n_value, int arch)
{
	char	*addr;
	size_t	len;
	char	*s;

	(void)arch;
	addr = ft_itoa_hex(n_value);
	len = ft_strlen(addr);
	if (arch == ARCH_32 && (s = malloc(sizeof(char) * (8 - len))))
	{
		ft_memset(s, '0', 8 - len);
		write(1, s, 8 - len);
	}
	else if (arch == ARCH_64 && (s = malloc(sizeof(char) * (16 - len))))
	{
		ft_memset(s, '0', 16 - len);
		write(1, s, 16 - len);
	}
	else
	{
		free(addr);
		return ;
	}
	free(s);
	ft_putstr(addr);
	ft_putchar(' ');
}

static char	get_char_sym(t_data elem)
{
	char	c;

	if (elem.section && !ft_strcmp(elem.section, "__text"))
		c = 't';
	else if (elem.section && !ft_strcmp(elem.section, "__bss"))
		c = 'b';
	else if (elem.section && !ft_strcmp(elem.section, "__data"))
		c = 'd';
	else if (elem.section)
		c = 's';
	else if (elem.is_abs)
		c = 'a';
	else if (elem.is_ind)
		c = 'i';
	else if (elem.is_undef && elem.n_value.n_value32 != 0)
		c = 'c';
	else
		c = 'u';
	if (elem.is_external)
		c = ft_toupper(c);
	return (c);
}

static void	print_addr(unsigned int *ptr, t_data elem)
{
	if ((*ptr == MH_MAGIC_64 || *ptr == MH_CIGAM_64)
			&& elem.is_undef && !elem.n_value.n_value64)
		ft_putstr("                 ");
	else if ((*ptr == MH_MAGIC || *ptr == MH_CIGAM) && elem.is_undef
			&& !elem.n_value.n_value32)
		ft_putstr("         ");
	else if (*ptr == MH_MAGIC_64 || *ptr == MH_CIGAM_64)
		print_addr_hex(elem.n_value.n_value64, ARCH_64);
	else
		print_addr_hex(elem.n_value.n_value32, ARCH_32);
}

void		print_data(void *ptr, t_data *array, int size)
{
	int		i;

	i = -1;
	while (++i < size)
	{
		if (array[i].is_stab)
		{
			if (array[i].segment)
				free(array[i].segment);
			continue;
		}
		print_addr((unsigned int*)ptr, array[i]);
		ft_putchar(get_char_sym(array[i]));
		ft_putchar(' ');
		ft_putendl(array[i].name);
		if (array[i].segment)
			free(array[i].segment);
		if (array[i].section)
			free(array[i].section);
	}
}
