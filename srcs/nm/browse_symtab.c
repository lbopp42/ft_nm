/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   browse_symtab.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/10 17:29:45 by lbopp             #+#    #+#             */
/*   Updated: 2019/05/10 17:40:21 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void	fill_name(char *s, t_info *i, int c)
{
	(*i).data[c].segment = NULL;
	(*i).data[c].section = NULL;
	if ((*i).f_ptr > (void*)s || (*i).f_ptr + (*i).size_file < (void*)s)
	{
		(*i).data[c].name = NULL;
		return ;
	}
	(*i).data[c].name = ft_strdup_secu(s, (*i).f_ptr + (*i).size_file);
}

static void	sort_print_data(void *ptr, t_data *data, int nsyms)
{
	qs_data(data, 0, nsyms - 1);
	print_data(ptr, data, nsyms);
}

static int	check_symtab(void *ptr, void *array, t_info i)
{
	if (is_macho64(ptr)
		&& (i.f_ptr <= (void*)((struct nlist_64*)array + i.c)
			&& i.f_ptr + i.size_file >= (void*)((struct nlist_64*)array + i.c)))
		return (1);
	else if (!is_macho64(ptr)
		&& (i.f_ptr <= (void*)((struct nlist*)array + i.c)
			&& i.f_ptr + i.size_file >= (void*)((struct nlist*)array + i.c)))
		return (1);
	else
		return (0);
}

void		browse_symtab(void *array, struct symtab_command *symtab,
		void *ptr, t_info *i)
{
	char		*name;

	if (!((*i).data = (t_data*)malloc(get_32(ptr, symtab->nsyms) *
					sizeof(t_data))))
		return ;
	(*i).c = -1;
	while (++(*i).c < (int)get_32(ptr, symtab->nsyms))
	{
		name = ptr + get_32(ptr, symtab->stroff) +
			get_n_strx(ptr, array, (*i).c, *i);
		fill_name(name, i, (*i).c);
		if (is_macho64(ptr) && check_symtab(ptr, array, (*i)))
		{
			(*i).data[(*i).c].n_value.n_value64 =
				get_64(ptr, ((struct nlist_64*)array)[(*i).c].n_value);
			fill_data_64(((struct nlist_64*)array)[(*i).c], ptr, i, (*i).c);
		}
		else if (check_symtab(ptr, array, (*i)))
		{
			(*i).data[(*i).c].n_value.n_value32 =
				get_32(ptr, ((struct nlist*)array)[(*i).c].n_value);
			fill_data_32(((struct nlist*)array)[(*i).c], ptr, i, (*i).c);
		}
	}
	sort_print_data(ptr, (*i).data, get_32(ptr, symtab->nsyms));
}
