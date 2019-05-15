/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/11 11:13:53 by lbopp             #+#    #+#             */
/*   Updated: 2019/05/15 14:26:23 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static void	print_arch_name(struct fat_header *fat_header, char *filename,
		char *cpu_name, t_info *i)
{
	struct fat_arch		*fat_arch;

	fat_arch = (struct fat_arch*)((void*)i->f_ptr + sizeof(struct fat_header));
	(void)i;
	if (swap_little_endian(fat_header->nfat_arch) > 1)
	{
		ft_putstr(filename);
		ft_putstr(" (architecture ");
		ft_putstr(cpu_name);
		ft_putendl("):");
	}
	else if (ft_memcmp(i->f_ptr + swap_little_endian(fat_arch->offset),
				ARMAG, SARMAG))
		ft_putendstr(filename, ":\n");
}

static void	process_fat_file(void *p, char *f_name, int nb_file, t_info *i)
{
	struct fat_header	*fat_header;
	uint32_t			c;
	void				*new_ptr;
	struct fat_arch		*ar;

	c = -1;
	fat_header = p;
	ar = (struct fat_arch*)((void*)p + sizeof(struct fat_header));
	if ((*i).f_ptr > (void*)ar || (*i).f_ptr + (*i).size_file < (void*)ar)
		return ;
	while (++c < swap_little_endian(fat_header->nfat_arch))
	{
		i->is_ppc = (get_cputype(p, c, *i) && !ft_strcmp(get_cputype(p, c, *i),
				"ppc")) ? 1 : 0;
		print_arch_name(fat_header, f_name, get_cputype(p, c, *i), i);
		new_ptr = (void*)p + swap_little_endian(ar->offset);
		if ((*i).f_ptr > new_ptr || (*i).f_ptr + (*i).size_file < new_ptr)
			break ;
		if (!swap_little_endian(ar->size))
			break ;
		ft_otool(new_ptr, swap_little_endian(ar->size), nb_file, i);
		ar = (void*)ar + sizeof(struct fat_arch);
		if ((*i).f_ptr > (void*)ar || (*i).f_ptr + (*i).size_file < (void*)ar)
			break ;
	}
}

void		handle_fat_file(const void *ptr, char *filename,
		int nb_file, t_info *info)
{
	struct fat_header		*fat_header;

	fat_header = (struct fat_header*)ptr;
	if (fat_header->magic == FAT_CIGAM_64 &&
			!search_host_arch_64((void*)ptr, nb_file, info))
		process_fat_file((void*)ptr, filename, nb_file, info);
	else if (fat_header->magic == FAT_CIGAM &&
			!search_host_arch((void*)ptr, nb_file, info))
		process_fat_file((void*)ptr, filename, nb_file, info);
}
