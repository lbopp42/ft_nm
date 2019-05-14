/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_section.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/10 13:05:41 by lbopp             #+#    #+#             */
/*   Updated: 2019/05/14 10:03:07 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static struct section		*fill_sect(void *ptr, struct segment_command *seg,
		int n_sect, int t_nsects)
{
	struct section			*sect;

	sect = (struct section*)(seg + 1) +
		(n_sect - (t_nsects - get_32(ptr, seg->nsects)) - 1);
	return (sect);
}

static struct section_64	*fill_sect_64(void *ptr,
		struct segment_command_64 *seg, int n_sect, int t_nsects)
{
	struct section_64			*sect;

	sect = (struct section_64*)(seg + 1) +
		(n_sect - (t_nsects - get_64(ptr, seg->nsects)) - 1);
	return (sect);
}

void						get_section_64(void *ptr, struct load_command *lc,
		int n_sect, t_info *i)
{
	int							c;
	struct segment_command_64	*seg;
	int							t_nsects;

	if ((*i).f_ptr > (void*)lc || (*i).f_ptr + (*i).size_file < (void*)lc)
		return ;
	c = -1;
	t_nsects = 0;
	while (++c < get_ncmds(ptr, *i))
	{
		if (get_64(ptr, lc->cmd) == LC_SEGMENT_64)
		{
			seg = (struct segment_command_64*)lc;
			if (n_sect && (t_nsects += get_64(ptr, seg->nsects)) / n_sect)
			{
				(*i).data[(*i).c].section = ft_strdup_secu(fill_sect_64(ptr, seg
					, n_sect, t_nsects)->sectname, (*i).f_ptr + (*i).size_file);
				return ;
			}
		}
		lc = (void*)lc + get_64(ptr, lc->cmdsize);
		if ((*i).f_ptr > (void*)lc || (*i).f_ptr + (*i).size_file < (void*)lc)
			break ;
	}
}

void						get_section(void *ptr, struct load_command *lc,
		int n_sect, t_info *i)
{
	int						c;
	struct segment_command	*seg;
	int						t_nsects;

	if ((*i).f_ptr > (void*)lc || (*i).f_ptr + (*i).size_file < (void*)lc)
		return ;
	c = -1;
	t_nsects = 0;
	while (++c < get_ncmds(ptr, *i))
	{
		if (get_32(ptr, lc->cmd) == LC_SEGMENT)
		{
			seg = (struct segment_command*)lc;
			if (n_sect && (t_nsects += get_32(ptr, seg->nsects)) / n_sect)
			{
				(*i).data[(*i).c].section = ft_strdup_secu(fill_sect(ptr, seg,
					n_sect, t_nsects)->sectname, (*i).f_ptr + (*i).size_file);
				return ;
			}
		}
		lc = (void*)lc + get_32(ptr, lc->cmdsize);
		if ((*i).f_ptr > (void*)lc || (*i).f_ptr + (*i).size_file < (void*)lc)
			break ;
	}
}
