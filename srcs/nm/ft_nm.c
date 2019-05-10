/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 10:03:10 by lbopp             #+#    #+#             */
/*   Updated: 2019/05/10 18:00:49 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int		is_fat_file(const void *ptr, t_info info)
{
	unsigned int	magic_number;

	if (info.f_ptr > ptr || info.f_ptr + info.size_file < ptr)
	 	return (0);
	magic_number = *(int*)ptr;
	if (magic_number == FAT_MAGIC || magic_number == FAT_CIGAM)
		return (1);
	else if (magic_number == FAT_MAGIC_64 || magic_number == FAT_CIGAM_64)
		return (1);
	return (0);
}

int		search_host_arch(void *ptr, char *filename, int nb_file, t_info *i)
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
			ft_nm(new_ptr, swap_little_endian(fat_arch->size),
					filename, nb_file, i);
			return (1);
		}
		fat_arch = (void*)fat_arch + sizeof(struct fat_arch);
		if ((*i).f_ptr > (void*)fat_arch
				|| (*i).f_ptr + (*i).size_file < (void*)fat_arch)
			break ;
	}
	return (0);
}

int		search_host_arch_64(void *ptr, char *filename, int nb_file,
		t_info *info)
{
	struct fat_arch_64		*fat_arch;
	void					*new_ptr;
	uint32_t				i;

	i = -1;
	fat_arch = (struct fat_arch_64*)((void*)ptr + sizeof(struct fat_header));
	if ((*info).f_ptr > (void*)fat_arch
			|| (*info).f_ptr + (*info).size_file < (void*)fat_arch)
		return (1);
	while (++i < swap_little_endian(((struct fat_header*)ptr)->nfat_arch))
	{
		new_ptr = (void*)ptr + swap_little_endian(fat_arch->offset);
		if (swap_little_endian(fat_arch->cputype) == CPU_TYPE_X86_64)
		{
			ft_nm(new_ptr, swap_little_endian(fat_arch->size),
					filename, nb_file, info);
			return (1);
		}
		fat_arch = (void*)fat_arch + sizeof(struct fat_arch_64);
		if ((*info).f_ptr > (void*)fat_arch
				|| (*info).f_ptr + (*info).size_file < (void*)fat_arch)
			break ;
	}
	return (0);
}

void	print_arch_name(struct fat_header *fat_header, char *filename,
		char *cpu_name)
{
	if (swap_little_endian(fat_header->nfat_arch) > 1)
	{
		ft_putchar('\n');
		ft_putstr(filename);
		ft_putstr(" (for architecture ");
		ft_putstr(cpu_name);
		ft_putendl("):");
	}
	else
	{
		ft_putstr(filename);
		ft_putendl(":");
	}
}

void	process_fat_64(void *ptr, char *filename, int nb_file, t_info *info)
{
	void				*new_ptr;
	struct fat_arch_64	*fat_arch;

	fat_arch = (struct fat_arch_64*)((void*)ptr + sizeof(struct fat_header));
	if ((*info).f_ptr > (void*)fat_arch
			|| (*info).f_ptr + (*info).size_file < (void*)fat_arch)
		return ;
	new_ptr = (void*)ptr + swap_little_endian(fat_arch->offset);
	if ((*info).f_ptr > new_ptr || (*info).f_ptr + (*info).size_file < new_ptr)
		return ;
	ft_nm(new_ptr, swap_little_endian(fat_arch->size), filename, nb_file, info);
	fat_arch = (void*)fat_arch + sizeof(struct fat_arch_64);
	if ((*info).f_ptr > (void*)fat_arch
			|| (*info).f_ptr + (*info).size_file < (void*)fat_arch)
		return ;
}

void	process_fat_32(void *ptr, char *filename, int nb_file, t_info *info)
{
	void				*new_ptr;
	struct fat_arch		*fat_arch;

	fat_arch = (struct fat_arch*)((void*)ptr + sizeof(struct fat_header));
	if ((*info).f_ptr > (void*)fat_arch
			|| (*info).f_ptr + (*info).size_file < (void*)fat_arch)
		return ;
	new_ptr = (void*)ptr + swap_little_endian(fat_arch->offset);
	if ((*info).f_ptr > new_ptr || (*info).f_ptr + (*info).size_file < new_ptr)
		return ;
	ft_nm(new_ptr, swap_little_endian(fat_arch->size), filename, nb_file, info);
	fat_arch = (void*)fat_arch + sizeof(struct fat_arch);
	if ((*info).f_ptr > (void*)fat_arch
			|| (*info).f_ptr + (*info).size_file < (void*)fat_arch)
		return ;
}

void	process_fat_file(void *ptr, char *f_name, int nb_file, t_info *i)
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
		print_arch_name(fat_header, f_name, get_cputype(ptr, c, *i));
		new_ptr = (void*)ptr + swap_little_endian(fat_arch->offset);
		if ((*i).f_ptr > new_ptr || (*i).f_ptr + (*i).size_file < new_ptr)
			break ;
		if (swap_little_endian(fat_arch->size))
			break;
		ft_nm(new_ptr, swap_little_endian(fat_arch->size), f_name, nb_file, i);
		fat_arch = (void*)fat_arch + sizeof(struct fat_arch);
		if ((*i).f_ptr > (void*)fat_arch
				|| (*i).f_ptr + (*i).size_file < (void*)fat_arch)
			break ;
	}
}

void	handle_fat_file(const void *ptr, char *filename,
		int nb_file, t_info *info)
{
	struct fat_header		*fat_header;

	fat_header = (struct fat_header*)ptr;
	if (fat_header->magic == FAT_CIGAM_64 &&
			!search_host_arch_64((void*)ptr, filename, nb_file, info))
		process_fat_file((void*)ptr, filename, nb_file, info);
	else if (fat_header->magic == FAT_CIGAM &&
			!search_host_arch((void*)ptr, filename, nb_file, info))
		process_fat_file((void*)ptr, filename, nb_file, info);
}

void	print_ar_name(int size_name, struct ar_hdr *ar, t_info info)
{
	if (info.f_ptr > (void*)ar + sizeof(*ar)
		|| info.f_ptr + info.size_file < (void*)ar + sizeof(*ar))
		return ;
	if (size_name != 0)
	{
		ft_putstr("(");
		if (info.f_ptr > (void*)ar
				|| info.f_ptr + info.size_file < (void*)ar)
			return ;
		ft_putstr(ar->ar_name + sizeof(struct ar_hdr));
		ft_putendl("):");
	}
	else
	{
		ft_putstr("(");
		ft_putstr(ar->ar_name);
		ft_putendl("):");
	}
}

void	launch_nm(struct ar_hdr *ar, unsigned int size_name, char *filename,
		t_info *info)
{
	if ((*info).f_ptr > (void*)ar + sizeof(struct ar_hdr) + size_name
			|| (*info).f_ptr + (*info).size_file <
			(void*)ar + sizeof(struct ar_hdr) + size_name)
		return ;
	ft_nm((void*)ar + sizeof(struct ar_hdr) + size_name,
			ft_atoi(ar->ar_size), filename, -1, info);
}

void	handle_arch(void *ptr, int size, char *filename, t_info *i)
{
	struct ar_hdr	*ar;
	unsigned int	size_name;

	ar = ptr + 8;
	if ((*i).f_ptr > (void*)ar || (*i).f_ptr + (*i).size_file < (void*)ar)
		return ;
	size -= 8 + sizeof(struct ar_hdr) + ft_atoi(ar->ar_size);
	ar = (void*)ar + sizeof(struct ar_hdr) + ft_atoi(ar->ar_size);
	if ((*i).f_ptr > (void*)ar || (*i).f_ptr + (*i).size_file < (void*)ar)
		return ;
	while (size)
	{
		size_name = 0;
		ft_putchar('\n');
		ft_putstr(filename);
		size -= sizeof(struct ar_hdr) + ft_atoi(ar->ar_size);
		if (!ft_strncmp(ar->ar_name, "#1/", 3))
			size_name = ft_atoi(ar->ar_name + 3);
		print_ar_name(size_name, ar, *i);
		if (ft_strcmp(ar->ar_name, ""))
			launch_nm(ar, size_name, filename, i);
		ar = (void*)ar + sizeof(struct ar_hdr) + ft_atoi(ar->ar_size);
		if ((*i).f_ptr > (void*)ar || (*i).f_ptr + (*i).size_file < (void*)ar)
			return ;
	}
}

void	ft_nm(char *ptr, int size, char *filename, int nb_file, t_info *info)
{
	unsigned int	magic_number;
	static int		i = 0;

	if ((*info).f_ptr > (void*)ptr
			|| (*info).f_ptr + (*info).size_file < (void*)ptr)
		return ;
	magic_number = *(int *)ptr;
	if (magic_number == MH_MAGIC_64 || magic_number == MH_CIGAM_64)
		handle_64(ptr, info, filename, nb_file);
	else if (magic_number == MH_MAGIC || magic_number == MH_CIGAM)
		handle(ptr, info, filename, nb_file);
	else if (is_fat_file(ptr, (*info)))
		handle_fat_file(ptr, filename, nb_file, info);
	else if (!ft_memcmp(ptr, ARMAG, SARMAG))
		handle_arch(ptr, size, filename, info);
	i++;
}

int	try_open(char *filename)
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

int	get_stat_file(char *filename, int fd, struct stat *buf)
{
	if (fstat(fd, buf) < 0)
	{
		//printf("Erreur stat"); // TODO
		return(1);
	}
	if (S_IFDIR & buf->st_mode)
	{
		ft_putstr_fd("ft_nm: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putendl_fd(": Is a directory.", 2);
		return (1);
	}
	return (0);
}

int	close_fd_error(int fd, int return_nb)
{
	close(fd);
	return (return_nb);
}

int	nm_no_arg(void)
{
	int			fd;
	struct stat	buf;
	t_info		info;
	char		*ptr;

	if (!(fd = try_open("a.out")))
		return(1);
	if (get_stat_file("a.out", fd, &buf) == 1)
		return (close_fd_error(fd, 1));
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) ==
			MAP_FAILED)
		return (1);
	info.f_ptr = ptr;
	info.size_file = buf.st_size;
	ft_nm(ptr, buf.st_size, "a.out", 1, &info);
	if ((munmap(ptr, buf.st_size)) == -1)
		return (close_fd_error(fd, 1));
	return (0);
}

int	main(int ac, char **av)
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
		info.f_ptr = ptr;
		info.size_file = buf.st_size;
		ft_nm(ptr, buf.st_size, av[i], ac - 1, &info);
		if ((munmap(ptr, buf.st_size)) == -1)
			return (close_fd_error(fd, 1));
	}
	return (close_fd_error(fd, 0));
}
