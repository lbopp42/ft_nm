/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   browse_symtab.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/10 17:29:45 by lbopp             #+#    #+#             */
/*   Updated: 2019/05/15 14:55:45 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static int			check_symtab(void *ptr, void *array, t_info i)
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

struct section		*get_section_text(struct load_command *lc, t_info *i)
{
	struct section			*sect;
	struct segment_command	*seg;

	(void)i;
	seg = (struct segment_command*)lc;
	seg = seg + sizeof(*seg) * 2;
	sect = (struct section*)(seg + 1);
	return (sect);
}

struct section_64	*get_section_text_64(struct load_command *lc, t_info *i)
{
	struct section_64			*sect;
	struct segment_command_64	*seg;

	(void)i;
	seg = (struct segment_command_64*)lc;
	printf("%s\n", seg->segname);
	sect = (struct section_64*)(seg + 1);
	printf("%s %p\n", sect[0].sectname, sect[0].sectname);
	return (sect);
}

void				browse_symtab(void *array, struct symtab_command *symtab,
		void *ptr, t_info *i)
{
	(*i).c = -1;
	while (++(*i).c < (int)get_32(ptr, symtab->nsyms))
	{
		if (is_macho64(ptr) && check_symtab(ptr, array, (*i)))
		{
			get_section_text(ptr + sizeof(struct mach_header_64), i);
		}
		else if (check_symtab(ptr, array, (*i)))
		{
			get_section_text(ptr + sizeof(struct mach_header), i);
		}
	}
}
