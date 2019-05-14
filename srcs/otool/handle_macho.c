/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_macho.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/10 17:47:37 by lbopp             #+#    #+#             */
/*   Updated: 2019/05/14 13:53:51 by lbopp            ###   ########.fr       */
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
	if ((arch == ARCH_32 || arch == PPC) && (s = malloc(sizeof(char) * (8 - len))))
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
	ft_putchar('\t');
	while (i < size)
	{
		addr_sect += 1;
		c = *(s + i) >> 4 & 0xf;
		c += c > 9 ? 'a' - 10: '0';
		ft_putchar(c);
		c = *(s + i) & 0xf;
		c += c > 9 ? 'a' - 10: '0';
		ft_putchar(c);
		if (arch != PPC || (arch == PPC && !((i  + 1) % 4)))
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

	(void)ptr;
	c = 0;
	sect = (struct section_64*)(seg + 1);
	while (c < get_64(ptr, seg->nsects))
	{
		if (!ft_strcmp(sect->sectname, "__text"))
			print_memory(ptr + get_64(ptr, sect->offset), get_64(ptr, sect->size), get_64(ptr, sect->addr), ARCH_64);
		sect += 1;
		c++;
	}
}

void		print_section_text(void *ptr, struct segment_command *seg, t_info i)
{
	uint32_t			c;
	struct section		*sect;

	(void)ptr;
	c = 0;
	sect = (struct section*)(seg + 1);
	while (c < get_32(ptr, seg->nsects))
	{
		if (!ft_strcmp(sect->sectname, "__text") && i.is_ppc)
			print_memory(ptr + get_32(ptr, sect->offset), get_32(ptr, sect->size), get_32(ptr, sect->addr), PPC);
		else if (!ft_strcmp(sect->sectname, "__text"))
			print_memory(ptr + get_32(ptr, sect->offset), get_32(ptr, sect->size), get_32(ptr, sect->addr), ARCH_32);
		sect += 1;
		c++;
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
		if (get_64(ptr, lc->cmd) == LC_SEGMENT_64)
		{
			seg = (struct segment_command_64*)lc;
			print_section_text_64(ptr, seg);
		}
		lc = (void*)lc + get_64(ptr, lc->cmdsize);
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
			print_section_text(ptr, seg, *i);
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
	if (ptr == (*i).f_ptr)
	{
		ft_putstr(filename);
		ft_putendl(":");
	}
	ft_putendl("Contents of (__TEXT,__text) section");
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
	if (ptr == (*i).f_ptr)
	{
		ft_putstr(filename);
		ft_putendl(":");
	}
	ft_putendl("Contents of (__TEXT,__text) section");
	if ((*i).f_ptr <= (void*)lc && (*i).f_ptr + (*i).size_file >= (void*)lc)
		browse_load_commands_64(ptr, lc, i);
}
