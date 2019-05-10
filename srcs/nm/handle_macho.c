/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_macho.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/10 17:47:37 by lbopp             #+#    #+#             */
/*   Updated: 2019/05/10 17:49:46 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void	browse_load_commands(void *ptr, struct load_command *lc, t_info *i)
{
	struct symtab_command	*symtab;
	int						c;
	int						ncmds;
	void					*array;

	c = -1;
	ncmds = get_ncmds(ptr, *i);
	while (++c < ncmds)
	{
		if (get_32(ptr, lc->cmd) == LC_SYMTAB)
		{
			symtab = (struct symtab_command*)lc;
			array = ptr + get_32(ptr, symtab->symoff);
			if ((*i).f_ptr > (void*)array
					|| (*i).f_ptr + (*i).size_file < (void*)array)
				return ;
			browse_symtab(array, symtab, (void*)ptr, i);
			break ;
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
		browse_load_commands(ptr, lc, i);
}
