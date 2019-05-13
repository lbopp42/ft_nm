/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_macho.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/10 17:47:37 by lbopp             #+#    #+#             */
/*   Updated: 2019/05/13 17:57:07 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

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
	free(addr);
}

void print_memory(const void *addr, size_t size, uint64_t addr_sect, int arch)
{
	char	*s;
	int		c;
	size_t	i;

	i = 0;
	s = (char*)addr;
	print_addr_hex(addr_sect, arch);
	ft_putstr("        ");
	while (i < size)
	{
		addr_sect += 1;
		c = *(s + i) >> 4 & 0xf;
		c += c > 9 ? 'a' - 10: '0';
		ft_putchar(c);
		c = *(s + i) & 0xf;
		c += c > 9 ? 'a' - 10: '0';
		ft_putchar(c);
		if (i && !((i + 1) % 16))
		{
			ft_putchar('\n');
			print_addr_hex(addr_sect, arch);
			ft_putstr("        ");
		}
		else
			ft_putchar(' ');
		i++;
	}
	ft_putchar('\n');
}

void		print_section_text_64(void *ptr, struct segment_command_64 *seg)
{
	uint32_t			i;
	struct section_64	*sect;

	(void)ptr;
	i = 0;
	sect = (struct section_64*)(seg + 1);
	while (i < seg->nsects)
	{
		if (!ft_strcmp(sect->sectname, "__text"))
		{
			print_memory(ptr + sect->offset, sect->size, sect->addr, ARCH_64);
		}
		sect += 1;
		i++;
	}
}

void		print_section_text(void *ptr, struct segment_command *seg)
{
	uint32_t			i;
	struct section		*sect;

	(void)ptr;
	i = 0;
	sect = (struct section*)(seg + 1);
	while (i < seg->nsects)
	{
		if (!ft_strcmp(sect->sectname, "__text"))
		{
			print_memory(ptr + sect->offset, sect->size, sect->addr, ARCH_32);
		}
		sect += 1;
		i++;
	}
}

static void	browse_load_commands_64(void *ptr, struct load_command *lc, t_info *i)
{
	int							c;
	struct segment_command_64	*seg;

	if ((*i).f_ptr > (void*)lc || (*i).f_ptr + (*i).size_file < (void*)lc)
		return ;
	c = -1;
	while (++c < get_ncmds(ptr, *i))
	{
		if (get_32(ptr, lc->cmd) == LC_SEGMENT_64)
		{
			seg = (struct segment_command_64*)lc;
			if (!ft_strcmp(seg->segname, "__TEXT"))
				print_section_text_64(ptr, seg);
		}
		lc = (void*)lc + get_32(ptr, lc->cmdsize);
		if ((*i).f_ptr > (void*)lc || (*i).f_ptr + (*i).size_file < (void*)lc)
			break ;
	}
}

static void	browse_load_commands(void *ptr, struct load_command *lc, t_info *i)
{
	int							c;
	struct segment_command	*seg;

	if ((*i).f_ptr > (void*)lc || (*i).f_ptr + (*i).size_file < (void*)lc)
		return ;
	c = -1;
	while (++c < get_ncmds(ptr, *i))
	{
		if (get_32(ptr, lc->cmd) == LC_SEGMENT)
		{
			seg = (struct segment_command*)lc;
			if (!ft_strcmp(seg->segname, "__TEXT"))
				print_section_text(ptr, seg);
		}
		lc = (void*)lc + get_32(ptr, lc->cmdsize);
		if ((*i).f_ptr > (void*)lc || (*i).f_ptr + (*i).size_file < (void*)lc)
			break ;
	}
}

void		handle(void *ptr, t_info *i, char *filename, int nb_file)
{
	struct load_command		*lc;


	(void)i;
	if (nb_file > 1)
	{
		ft_putchar('\n');
		ft_putstr(filename);
		ft_putendl(":");
	}
	lc = (void*)ptr + sizeof(struct mach_header);
	if ((*i).f_ptr <= (void*)lc && (*i).f_ptr + (*i).size_file >= (void*)lc)
		browse_load_commands(ptr, lc, i);
}

void		handle_64(void *ptr, t_info *i, char *filename, int nb_file)
{
	struct load_command		*lc;

	if (nb_file > 1)
	{
		ft_putchar('\n');
		ft_putstr(filename);
		ft_putendl(":");
	}
	lc = (void*)ptr + sizeof(struct mach_header_64);
	if ((*i).f_ptr <= (void*)lc && (*i).f_ptr + (*i).size_file >= (void*)lc)
		browse_load_commands_64(ptr, lc, i);
}
