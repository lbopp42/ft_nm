/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_otool.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/11 11:27:18 by lbopp             #+#    #+#             */
/*   Updated: 2019/05/14 15:39:07 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

void	ft_otool(char *ptr, int size, int nb_file, t_info *info)
{
	unsigned int	magic_number;
	static int		i = 0;

	if ((*info).f_ptr > (void*)ptr
			|| (*info).f_ptr + (*info).size_file < (void*)ptr)
		return ;
	magic_number = *(int *)ptr;
	if (magic_number == MH_MAGIC_64 || magic_number == MH_CIGAM_64)
		handle_64(ptr, info, (*info).filename, nb_file);
	else if (magic_number == MH_MAGIC || magic_number == MH_CIGAM)
		handle(ptr, info, (*info).filename, nb_file);
	else if (is_fat_file(ptr, (*info)))
		handle_fat_file(ptr, (*info).filename, nb_file, info);
	else if (!ft_memcmp(ptr, ARMAG, SARMAG))
		handle_arch(ptr, size, (*info).filename, info);
	else
		ft_putendstr(info->filename, ": is not an object file\n");
	i++;
}
