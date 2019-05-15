/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_text_section.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/15 14:53:43 by lbopp             #+#    #+#             */
/*   Updated: 2019/05/15 15:01:12 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static void	print_addr_hex(uint64_t n_value, int ar)
{
	char	*addr;
	size_t	len;
	char	*s;

	addr = ft_itoa_hex(n_value);
	len = ft_strlen(addr);
	if ((ar == ARCH_32 || ar == PPC) && (s = malloc(sizeof(char) * (8 - len))))
	{
		ft_memset(s, '0', 8 - len);
		write(1, s, 8 - len);
	}
	else if (ar == ARCH_64 && (s = malloc(sizeof(char) * (16 - len))))
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
	free(addr);
}

void		print_memory(const void *addr, size_t size, uint64_t addr_sect,
		int arch)
{
	int		c;
	size_t	i;

	i = 0;
	print_addr_hex(addr_sect, arch);
	ft_putchar('\t');
	while (i < size)
	{
		addr_sect += 1;
		c = *((char*)addr + i) >> 4 & 0xf;
		ft_putchar(c += c > 9 ? 'a' - 10 : '0');
		c = *((char*)addr + i) & 0xf;
		ft_putchar(c += c > 9 ? 'a' - 10 : '0');
		if (arch != PPC || (arch == PPC && !((i + 1) % 4)))
			ft_putchar(' ');
		if (i && !((i + 1) % 16) && (i + 1) < size)
		{
			ft_putchar('\n');
			print_addr_hex(addr_sect, arch);
			ft_putchar('\t');
		}
		i++;
	}
	ft_putchar('\n');
}

void		print_section_text_64(void *ptr, struct segment_command_64 *seg)
{
	uint32_t			c;
	struct section_64	*sect;

	c = 0;
	sect = (struct section_64*)(seg + 1);
	while (c < get_64(ptr, seg->nsects))
	{
		if (!ft_strcmp(sect->sectname, "__text") && get_64(ptr, sect->size))
			print_memory(ptr + get_64(ptr, sect->offset),
					get_64(ptr, sect->size), get_64(ptr, sect->addr), ARCH_64);
		sect += 1;
		c++;
	}
}

void		print_section_text(void *ptr, struct segment_command *seg, t_info i)
{
	uint32_t			c;
	struct section		*sect;

	c = 0;
	sect = (struct section*)(seg + 1);
	while (c < get_32(ptr, seg->nsects))
	{
		if (!ft_strcmp(sect->sectname, "__text") && i.is_ppc
				&& get_32(ptr, sect->size))
			print_memory(ptr + get_32(ptr, sect->offset),
					get_32(ptr, sect->size), get_32(ptr, sect->addr), PPC);
		else if (!ft_strcmp(sect->sectname, "__text")
				&& get_32(ptr, sect->size))
			print_memory(ptr + get_32(ptr, sect->offset),
					get_32(ptr, sect->size), get_32(ptr, sect->addr), ARCH_32);
		sect += 1;
		c++;
	}
}
