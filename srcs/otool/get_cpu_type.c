/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cpu_type.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/10 17:59:37 by lbopp             #+#    #+#             */
/*   Updated: 2019/05/13 10:43:37 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static char	*get_cputype_64(void *ptr, int i, t_info info)
{
	struct fat_arch_64	*fat_arch;

	fat_arch = (struct fat_arch_64*)((void*)ptr +
			sizeof(struct fat_header)) + i;
	if (info.f_ptr > (void*)fat_arch
			|| info.f_ptr + info.size_file < (void*)fat_arch)
		return ("undefined");
	if (swap_little_endian(fat_arch->cputype) == CPU_TYPE_POWERPC)
		return ("ppc");
	else if (swap_little_endian(fat_arch->cputype) == CPU_TYPE_X86)
		return ("i386");
	else
		return ("undefined");
}

static char	*get_cputype_32(void *ptr, int i, t_info info)
{
	struct fat_arch	*fat_arch;

	fat_arch = (struct fat_arch*)((void*)ptr + sizeof(struct fat_header)) + i;
	if (info.f_ptr > (void*)fat_arch
			|| info.f_ptr + info.size_file < (void*)fat_arch)
		return ("undefined");
	if (swap_little_endian(fat_arch->cputype) == CPU_TYPE_POWERPC)
		return ("ppc");
	else if (swap_little_endian(fat_arch->cputype) == CPU_TYPE_X86)
		return ("i386");
	else
		return ("undefined");
}

char		*get_cputype(void *ptr, int i, t_info info)
{
	struct fat_header		*fat_header;

	fat_header = ptr;
	if (fat_header->magic == FAT_CIGAM_64)
		return (get_cputype_64(ptr, i, info));
	else
		return (get_cputype_32(ptr, i, info));
}
