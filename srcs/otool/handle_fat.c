/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/11 11:13:53 by lbopp             #+#    #+#             */
/*   Updated: 2019/05/15 14:02:38 by lbopp            ###   ########.fr       */
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

static void	process_fat_file(void *ptr, char *f_name, int nb_file, t_info *i)
{
	struct fat_header	*fat_header;
	uint32_t			c;
	void				*new_ptr;
	struct fat_arch		*fat_arch;

	c = -1;
	fat_header = ptr;
	fat_arch = (struct fat_arch*)((void*)ptr + sizeof(struct fat_header));
	if ((*i).f_ptr > (void*)fat_arch
			|| (*i).f_ptr + (*i).size_file < (void*)fat_arch)
		return ;
	while (++c < swap_little_endian(fat_header->nfat_arch))
	{
		if (get_cputype(ptr, c, *i) && !ft_strcmp(get_cputype(ptr, c, *i), "ppc"))
			(*i).is_ppc = 1;
		else
			(*i).is_ppc = 0;
		print_arch_name(fat_header, f_name, get_cputype(ptr, c, *i), i);
		new_ptr = (void*)ptr + swap_little_endian(fat_arch->offset);
		if ((*i).f_ptr > new_ptr || (*i).f_ptr + (*i).size_file < new_ptr)
			break ;
		if (!swap_little_endian(fat_arch->size))
			break ;
		ft_otool(new_ptr, swap_little_endian(fat_arch->size), nb_file, i);
		fat_arch = (void*)fat_arch + sizeof(struct fat_arch);
		if ((*i).f_ptr > (void*)fat_arch
				|| (*i).f_ptr + (*i).size_file < (void*)fat_arch)
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
