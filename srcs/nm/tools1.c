/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/10 12:58:02 by lbopp             #+#    #+#             */
/*   Updated: 2019/05/10 17:16:18 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

uint32_t	swap_little_endian(uint32_t nb)
{
	uint32_t	swapped;

	swapped = nb >> 24 | (nb >> 8 & 0xff00) | (nb << 8 & 0xff0000) | nb << 24;
	return (swapped);
}

int			get_ncmds(void *ptr, t_info info)
{
	unsigned int	magic_number;

	if (info.f_ptr > ptr || info.f_ptr + info.size_file < ptr)
		return (0);
	magic_number = *(int*)ptr;
	if (magic_number == MH_MAGIC_64)
		return (((struct mach_header_64*)ptr)->ncmds);
	else if (magic_number == MH_MAGIC)
		return (((struct mach_header*)ptr)->ncmds);
	else if (magic_number == MH_CIGAM_64)
		return (swap_little_endian(((struct mach_header_64*)ptr)->ncmds));
	else if (magic_number == MH_CIGAM)
		return (swap_little_endian(((struct mach_header*)ptr)->ncmds));
	return (0);
}

uint32_t	get_32(void *ptr, uint32_t v)
{
	if (*(unsigned int*)ptr == MH_MAGIC || *(unsigned int*)ptr == MH_MAGIC_64)
		return (v);
	else
		return (swap_little_endian(v));
}

uint64_t	get_64(void *ptr, uint64_t v)
{
	if (*(unsigned int*)ptr == MH_MAGIC_64)
		return (v);
	else
		return (swap_little_endian(v));
}

int			is_macho64(void *ptr)
{
	if (*(unsigned int*)ptr == MH_CIGAM_64
			|| *(unsigned int*)ptr == MH_MAGIC_64)
		return (1);
	else
		return (0);
}
