/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 10:03:10 by lbopp             #+#    #+#             */
/*   Updated: 2019/05/11 11:55:57 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int		try_open(char *filename)
{
	int fd;

	if ((fd = open(filename, O_RDONLY)) < 0)
	{
		if (errno == 13)
			ft_putendl_fd("ft_nm: Permission Denied.", 2);
		else
			ft_putendl_fd("ft_nm: No such file or directory.", 2);
		return (0);
	}
	return (fd);
}

int		get_stat_file(char *filename, int fd, struct stat *buf)
{
	if (fstat(fd, buf) < 0)
		return (1);
	if (S_IFDIR & buf->st_mode)
	{
		ft_putstr_fd("ft_nm: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putendl_fd(": Is a directory.", 2);
		return (1);
	}
	return (0);
}

int		close_fd_error(int fd, int return_nb)
{
	close(fd);
	return (return_nb);
}

t_info	init_info(void *ptr, int size, char *filename)
{
	t_info		info;

	info.f_ptr = ptr;
	info.size_file = size;
	info.filename = filename;
	return (info);
}

int		main(int ac, char **av)
{
	int			fd;
	struct stat	buf;
	char		*ptr;
	int			i;
	t_info		info;

	if (!av[1])
		return (nm_no_arg());
	i = 0;
	while (++i < ac)
	{
		if (!(fd = try_open(av[i])))
			return (1);
		if (get_stat_file(av[i], fd, &buf) == 1)
			return (close_fd_error(fd, 1));
		if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) ==
				MAP_FAILED)
			return (close_fd_error(fd, 1));
		info = init_info(ptr, buf.st_size, av[i]);
		ft_nm(ptr, buf.st_size, ac - 1, &info);
		if ((munmap(ptr, buf.st_size)) == -1)
			return (close_fd_error(fd, 1));
	}
	return (close_fd_error(fd, 0));
}
