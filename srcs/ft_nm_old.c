/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 10:03:10 by lbopp             #+#    #+#             */
/*   Updated: 2019/03/15 10:07:37 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	print_name(int symoff, int nsyms, int stroff, void *ptr)
{
	char		*stringtable;
	int		i;
	struct nlist_64	*array;

	stringtable = ptr + stroff;
	array = ptr + symoff;
	i = 0;
	while (i < nsyms)
	{
		printf("%s\n", stringtable + array[i].n_un.n_strx);
		if (array[i].n_type & N_PEXT)
		{
			printf("PEXTERNAL\n");
		}
		if (array[i].n_type & N_TYPE)
		{
			printf("============================\n");
			if (array[i].n_type & N_UNDF)
				printf("UNDEF\n");
			if (array[i].n_type & N_ABS)
				printf("ABSOLUTE\n");
			if (array[i].n_type & N_SECT)
				printf("SECTION = %d\n", array[i].n_sect);
			if (array[i].n_type & N_PBUD)
				printf("PBUD\n");
			if (array[i].n_type & N_INDR)
				printf("INDR\n");
		}
		if (array[i].n_type & N_EXT)
		{
			printf("EXTERNAL\n"); // SEMBLE ETRE 'U'
		}
		i++;
	}

}

void	ft_nm(char *ptr)
{
	int				ncmds;
	unsigned int			magic_number;
	struct mach_header_64		*header;
	struct load_command		*lc;
	int				i;
	struct symtab_command	*symtab;
	struct segment_command_64	*seg_cmd;
	struct section_64		section;

	magic_number = *(int *)ptr;
	if (magic_number == MH_MAGIC_64)
	{
		header = (struct mach_header_64*)ptr;
		lc = (void*)ptr + sizeof(struct mach_header_64);
		ncmds = header->ncmds;
		printf("ncmds: %d\n",ncmds);
		i = 0;
		while (i < ncmds)
		{
			if (lc->cmd == LC_SYMTAB)
			{
				symtab = (struct symtab_command*)lc;
				print_name(symtab->symoff, symtab->nsyms, symtab->stroff, ptr);
				break;
			}
			if (lc->cmd == LC_SEGMENT_64)
			{
				seg_cmd = (struct segment_command_64*)lc;
				if (!ft_strcmp(seg_cmd->segname, "__TEXT"))
				{
					printf("nsects: %d\n", seg_cmd->nsects);
						printf("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n");
						printf("Segname = [%s]\n", seg_cmd->segname);
						printf("Size of segment = %d\n", seg_cmd->cmdsize);
						printf("Load_command size = [%d]\n", lc->cmdsize);
						for (uint32_t j = 0; j < seg_cmd->nsects; j++)
						{
							section = ((struct section_64*)(seg_cmd + 1))[j]; // TODO j = (n_sec de nlist) - 1
							printf("section name = [%s]\n", section.segname);
							printf("section name = [%s]\n", section.sectname);
						}
						printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
				}
			}
			lc = (void*)lc + lc->cmdsize;
			i++;
		}
	}
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
	}
	return (fd);
}

int	get_stat_file(char *filename, int fd, struct stat *buf)
{
	if (fstat(fd, buf) < 0)
	{
		printf("Erreur stat"); // TODO
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

int	main(int ac, char **av)
{
	char		*filename;
	int		fd;
	struct stat	buf;
	char		*ptr;

	(void)av;
	if (ac < 2)
	{
		filename = ft_strdup("a.out");
		printf("file = %s\n", filename); // TODO
	}
	else if (ac == 2)
	{
		filename = ft_strdup(av[1]);
		printf("file = %s\n", filename); //TODO
	}
	else
		return (0);
	if ((fd = try_open(filename)) < 0)
		return (1);
	if (get_stat_file(filename, fd, &buf) == 1)
		return (1);
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		printf("MAP FAILED"); //TODO ft_nm <(cat /bin/ls) FAIL !
		return(1);
	}
	ft_nm(ptr);
	if ((munmap(ptr, buf.st_size)) == -1)
	{
		printf("ERREUR MUNMAP"); //TODO
		return(1);
	}
}
