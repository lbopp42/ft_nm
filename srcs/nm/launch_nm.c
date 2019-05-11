/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_nm.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/11 11:27:18 by lbopp             #+#    #+#             */
/*   Updated: 2019/05/11 11:39:00 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	ft_nm(char *ptr, int size, int nb_file, t_info *info)
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
	i++;
}

int		nm_no_arg(void)
{
	int			fd;
	struct stat	buf;
	t_info		info;
	char		*ptr;

	if (!(fd = try_open("a.out")))
		return (1);
	if (get_stat_file("a.out", fd, &buf) == 1)
		return (close_fd_error(fd, 1));
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) ==
			MAP_FAILED)
		return (1);
	info.f_ptr = ptr;
	info.size_file = buf.st_size;
	info.filename = "a.out";
	ft_nm(ptr, buf.st_size, 1, &info);
	if ((munmap(ptr, buf.st_size)) == -1)
		return (close_fd_error(fd, 1));
	return (0);
}
