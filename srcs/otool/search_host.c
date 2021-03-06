/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_host.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/11 11:10:05 by lbopp             #+#    #+#             */
/*   Updated: 2019/05/15 14:30:17 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

int	search_host_arch(void *ptr, int nb_file, t_info *i)
{
	struct fat_arch		*fat_arch;
	void				*new_ptr;
	uint32_t			c;

	c = -1;
	fat_arch = (struct fat_arch*)((void*)ptr + sizeof(struct fat_header));
	if ((*i).f_ptr > (void*)fat_arch
			|| (*i).f_ptr + (*i).size_file < (void*)fat_arch)
		return (1);
	while (++c < swap_little_endian(((struct fat_header*)ptr)->nfat_arch))
	{
		new_ptr = (void*)ptr + swap_little_endian(fat_arch->offset);
		if (swap_little_endian(fat_arch->cputype) == CPU_TYPE_X86_64)
		{
			if (ft_memcmp(new_ptr, ARMAG, SARMAG))
				ft_putendstr(i->filename, ":\n");
			ft_otool(new_ptr, swap_little_endian(fat_arch->size), nb_file, i);
			return (1);
		}
		fat_arch = (void*)fat_arch + sizeof(struct fat_arch);
		if ((*i).f_ptr > (void*)fat_arch
				|| (*i).f_ptr + (*i).size_file < (void*)fat_arch)
			break ;
	}
	return (0);
}

int	search_host_arch_64(void *ptr, int nb_file,
		t_info *i)
{
	struct fat_arch_64		*fat_arch;
	void					*new_ptr;
	uint32_t				c;

	c = -1;
	fat_arch = (struct fat_arch_64*)((void*)ptr + sizeof(struct fat_header));
	if ((*i).f_ptr > (void*)fat_arch
			|| (*i).f_ptr + (*i).size_file < (void*)fat_arch)
		return (1);
	while (++c < swap_little_endian(((struct fat_header*)ptr)->nfat_arch))
	{
		new_ptr = (void*)ptr + swap_little_endian(fat_arch->offset);
		if (swap_little_endian(fat_arch->cputype) == CPU_TYPE_X86_64)
		{
			if (ft_memcmp(new_ptr, ARMAG, SARMAG))
				ft_putendstr(i->filename, ":\n");
			ft_otool(new_ptr, swap_little_endian(fat_arch->size), nb_file, i);
			return (1);
		}
		fat_arch = (void*)fat_arch + sizeof(struct fat_arch_64);
		if ((*i).f_ptr > (void*)fat_arch
				|| (*i).f_ptr + (*i).size_file < (void*)fat_arch)
			break ;
	}
	return (0);
}
