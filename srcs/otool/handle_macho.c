/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_macho.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/10 17:47:37 by lbopp             #+#    #+#             */
/*   Updated: 2019/05/15 14:55:34 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static void	browse_load_commands_64(void *ptr, struct load_command *lc,
		t_info *i)
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
	int						c;
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

	if (nb_file > 1)
	{
		ft_putchar('\n');
		ft_putstr(filename);
		ft_putendl(":");
	}
	lc = (void*)ptr + sizeof(struct mach_header);
	if (ptr == (*i).f_ptr && !i->is_arch)
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
	if (ptr == (*i).f_ptr && !i->is_arch)
	{
		ft_putstr(filename);
		ft_putendl(":");
	}
	ft_putendl("Contents of (__TEXT,__text) section");
	if ((*i).f_ptr <= (void*)lc && (*i).f_ptr + (*i).size_file >= (void*)lc)
		browse_load_commands_64(ptr, lc, i);
}
