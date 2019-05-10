/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/10 17:12:08 by lbopp             #+#    #+#             */
/*   Updated: 2019/05/10 17:14:22 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void	init_string_array(t_data **string_array, int c)
{
	(*string_array)[c].is_ind = 0;
	(*string_array)[c].is_external = 0;
	(*string_array)[c].is_stab = 0;
	(*string_array)[c].is_pext = 0;
	(*string_array)[c].is_pbud = 0;
	(*string_array)[c].is_abs = 0;
	(*string_array)[c].is_undef = 0;
}

void		fill_data_64(struct nlist_64 nlist, void *ptr,
		t_info *info, int c)
{
	struct mach_header_64		*header;

	header = (struct mach_header_64*)ptr;
	init_string_array(&(*info).data, c);
	if (nlist.n_type & N_STAB)
		(*info).data[c].is_stab = 1;
	if (nlist.n_type & N_PEXT)
		(*info).data[c].is_pext = 1;
	if ((nlist.n_type & N_TYPE) == N_UNDF)
		(*info).data[c].is_undef = 1;
	else if ((nlist.n_type & N_TYPE) == N_ABS)
		(*info).data[c].is_abs = 1;
	else if ((nlist.n_type & N_TYPE) == N_SECT)
	{
		(*info).c = c;
		get_section_64(ptr, ptr + sizeof(struct mach_header_64), nlist.n_sect
				, info);
	}
	else if ((nlist.n_type & N_TYPE) == N_PBUD)
		(*info).data[c].is_pbud = 1;
	else if ((nlist.n_type & N_TYPE) == N_INDR)
		(*info).data[c].is_ind = 1;
	if (nlist.n_type & N_EXT)
		(*info).data[c].is_external = 1;
}

void		fill_data_32(struct nlist nlist, void *ptr,
		t_info *info, int c)
{
	struct mach_header		*header;

	header = (struct mach_header*)ptr;
	init_string_array(&(*info).data, c);
	if (nlist.n_type & N_STAB)
		(*info).data[c].is_stab = 1;
	if (nlist.n_type & N_PEXT)
		(*info).data[c].is_pext = 1;
	if ((nlist.n_type & N_TYPE) == N_UNDF)
		(*info).data[c].is_undef = 1;
	else if ((nlist.n_type & N_TYPE) == N_ABS)
		(*info).data[c].is_abs = 1;
	else if ((nlist.n_type & N_TYPE) == N_SECT)
	{
		(*info).c = c;
		get_section(ptr, ptr + sizeof(struct mach_header), nlist.n_sect, info);
	}
	else if ((nlist.n_type & N_TYPE) == N_PBUD)
		(*info).data[c].is_pbud = 1;
	else if ((nlist.n_type & N_TYPE) == N_INDR)
		(*info).data[c].is_ind = 1;
	if (nlist.n_type & N_EXT)
		(*info).data[c].is_external = 1;
}
