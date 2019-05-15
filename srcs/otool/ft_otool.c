/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 10:03:10 by lbopp             #+#    #+#             */
/*   Updated: 2019/05/15 14:14:25 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static int	close_free_fd(int **fd, int size, int return_nb)
{
	int i;

	i = 0;
	while (i < size)
	{
		close((*fd)[i]);
		i++;
	}
	free(*fd);
	return (return_nb);
}

int			*try_open_otool(char *filename, int nb)
{
	int		*fd;
	int		i;

	i = 0;
	if (!(fd = malloc(sizeof(int) * nb)))
		return (NULL);
	while (i < nb)
	{
		if ((fd[i] = open(filename, O_RDONLY)) < 0)
		{
			ft_putstr_fd("ft_otool: '", 2);
			ft_putstr_fd(filename, 2);
			if (errno == 13)
				ft_putendl_fd("': Permission Denied.", 2);
			else
				ft_putendl_fd("': No such file or directory.", 2);
			close_free_fd(&fd, i, 0);
			return (NULL);
		}
		i++;
		filename += ft_strlen(filename) + 1;
	}
	return (fd);
}

t_info		init_info(void *ptr, int size, char *filename)
{
	t_info		info;

	info.f_ptr = ptr;
	info.size_file = size;
	info.filename = filename;
	info.is_ppc = 0;
	info.is_arch = 0;
	return (info);
}

int			otool_no_arg(int ac)
{
	if (ac < 2)
	{
		ft_putendl_fd("ft_otool: at least one file must be specified", 2);
		return (1);
	}
	return (0);
}

int			main(int ac, char **av)
{
	int			*fd;
	struct stat	buf;
	char		*ptr;
	int			i;
	t_info		info;

	if (otool_no_arg(ac))
		return (1);
	if (!(fd = try_open_otool(av[1], ac - 1)))
		return (1);
	i = -1;
	while (++i < ac - 1)
	{
		if (get_stat(av[i + 1], fd[i], &buf) == 1)
			return (close_free_fd(&fd, ac - 1, 1));
		if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd[i], 0)) ==
				MAP_FAILED)
			return (close_free_fd(&fd, ac - 1, 1));
		info = init_info(ptr, buf.st_size, av[i + 1]);
		ft_otool(ptr, buf.st_size, ac - 1, &info);
		if ((munmap(ptr, buf.st_size)) == -1)
			return (close_free_fd(&fd, ac - 1, 1));
	}
	return (close_free_fd(&fd, ac - 1, 0));
}
