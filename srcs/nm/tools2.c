/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/10 17:16:44 by lbopp             #+#    #+#             */
/*   Updated: 2019/05/10 17:17:52 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int		get_n_strx(void *ptr, void *nlist, int c, t_info i)
{
	if (*(unsigned int*)ptr == MH_MAGIC || *(unsigned int*)ptr == MH_CIGAM)
	{
		if (i.f_ptr > (void*)((struct nlist*)nlist + c)
				|| i.f_ptr + i.size_file < (void*)((struct nlist*)nlist + c))
			return (0);
	}
	else if (is_macho64(ptr))
		if (i.f_ptr > (void*)((struct nlist_64*)nlist + c)
				|| i.f_ptr + i.size_file < (void*)((struct nlist*)nlist + c))
			return (0);
	if (*(unsigned int*)ptr == MH_MAGIC)
		return (((struct nlist*)nlist)[c].n_un.n_strx);
	else if (*(unsigned int*)ptr == MH_MAGIC_64)
		return (((struct nlist_64*)nlist)[c].n_un.n_strx);
	else if (*(unsigned int*)ptr == MH_CIGAM)
		return (swap_little_endian(((struct nlist*)nlist)[c].n_un.n_strx));
	else
		return (swap_little_endian(((struct nlist_64*)nlist)[c].n_un.n_strx));
}

int		get_n_value(void *ptr, void *nlist, int i)
{
	if (*(unsigned int*)ptr == MH_MAGIC)
		return (((struct nlist*)nlist)[i].n_value);
	else if (*(unsigned int*)ptr == MH_MAGIC_64)
		return (((struct nlist_64*)nlist)[i].n_value);
	else if (*(unsigned int*)ptr == MH_CIGAM)
		return (swap_little_endian(((struct nlist*)nlist)[i].n_value));
	else
		return (swap_little_endian(((struct nlist_64*)nlist)[i].n_value));
}
