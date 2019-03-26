/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 10:03:10 by lbopp             #+#    #+#             */
/*   Updated: 2019/03/26 13:45:32 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

uint32_t	swap_little_endian(uint32_t nb)
{
	uint32_t	swapped;

	swapped = nb >> 24 | (nb >> 8 & 0xff00) | (nb << 8 & 0xff0000) | nb << 24;
	return (swapped);
}

char	*get_section(struct load_command *lc, int n_sect, int ncmds)
{
	int							i;
	struct segment_command_64	*seg_cmd;
	struct section_64			section;
	int							total_nsects;

	i = 0;
	total_nsects = 0;
	while (i < ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
			seg_cmd = (struct segment_command_64*)lc;
			printf("fileoff = %llu\n", seg_cmd->fileoff);
			printf("vmsize = %llu\n", seg_cmd->fileoff);
			total_nsects += seg_cmd->nsects;
			if (total_nsects != 0)
			{
				printf("nombre de section de ce segment: %d\n", seg_cmd->nsects);
				printf("n_sects = %d\n", n_sect);
				if (n_sect && seg_cmd->nsects / n_sect)
				{
				    section = ((struct section_64*)(seg_cmd + 1))[n_sect - 1];
				    return (ft_strdup(section.sectname));
				}
			}
		}
		lc = (void*)lc + lc->cmdsize;
		i++;
	}
	return (NULL);
}

void	get_section_64(struct load_command *lc, int n_sect, int ncmds, t_data *data)
{
	int							i;
	struct segment_command_64	*seg_cmd;
	struct section_64			section;

	i = 0;
	while (i < ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
			seg_cmd = (struct segment_command_64*)lc;
				if (n_sect && seg_cmd->nsects / n_sect)
				{
				    section = ((struct section_64*)(seg_cmd + 1))[n_sect - 1];
					data->section = ft_strdup(section.sectname);
					data->segment = ft_strdup(seg_cmd->segname);
					return ;
				}
		}
		lc = (void*)lc + lc->cmdsize;
		i++;
	}
}

void	get_section(struct load_command *lc, int n_sect, int ncmds, t_data *data)
{
	int							i;
	struct segment_command	*seg_cmd;
	struct section			section;

	i = 0;
	while (i < ncmds)
	{
		if (lc->cmd == LC_SEGMENT)
		{
			seg_cmd = (struct segment_command*)lc;
				if (n_sect && seg_cmd->nsects / n_sect)
				{
				    section = ((struct section_64*)(seg_cmd + 1))[n_sect - 1];
					data->section = ft_strdup(section.sectname);
					data->segment = ft_strdup(seg_cmd->segname);
					return ;
				}
		}
		lc = (void*)lc + lc->cmdsize;
		i++;
	}
}

void	print_data(t_data *array, int size)
{
	int	i;
	for (i = 0; i < size; i++)
	{
		printf("%016llx ", array[i].n_value.n_value64);
		if (!array[i].section)
			printf("U ");
		else if (!ft_strcmp(array[i].section, "__text"))
			printf("T ");
		printf("[%s]\n", array[i].name);
		free(array[i].segment);
		free(array[i].section);
	}
}

void	fill_data_64(t_data *string_array, struct nlist_64 nlist, void *ptr)
{
	struct mach_header_64		*header;

	header = (struct mach_header_64*)ptr;
	if (nlist.n_type & N_PEXT)
	{
		string_array->is_pext = 1;
		printf("PEXTERNAL\n"); // TODO Quand c'est active, mettre lettre en minuscule (non-external)
	}
	if (nlist.n_type & N_TYPE)
	{
		printf("============================\n");
		if (nlist.n_type & N_UNDF && nlist.n_sect == NO_SECT)
		{
			printf("UNDEF\n");
			string_array->type.is_undef = 1;
		}
		else if (nlist.n_type & N_ABS && nlist.n_sect == NO_SECT)
		{
			string_array->type.is_absolute = 1;
			printf("ABSOLUTE\n"); // TODO semble etre le 'A'
		}
		else if (nlist.n_type & N_SECT)
			get_section_64(ptr + sizeof(struct mach_header_64), nlist.n_sect, header->ncmds, string_array);
		//if (nlist.n_type & N_PBUD && nlist.n_sect == NO_SECT)
		//	printf("PBUD\n");
		//if (nlist.n_type & N_INDR)
		//{
		//	//TODO A comprendre
		//	printf("INDR\n");
		//}
	}
	if (nlist.n_type & N_EXT)
	{
		string_array->is_external = 1;
		printf("EXTERNAL\n"); // SEMBLE ETRE 'U'
	}
}

void	browse_symtab_64(int symoff, int nsyms, int stroff, void *ptr)
{
	int		i;
	struct nlist_64	*array;
	t_data		*string_array;

	array = ptr + symoff;
	i = 0;
	printf("On creer un tableau de %d\n", nsyms);
	string_array = (t_data*)malloc(nsyms * sizeof(t_data));
	while (i < nsyms)
	{
		string_array[i].n_value.n_value64 = array[i].n_value;
		string_array[i].name = ft_strdup(ptr + stroff + array[i].n_un.n_strx);
		fill_data_64(&(string_array[i]), array[i], ptr);
		i++;
	}
	print_data(string_array, nsyms);
}

void	print_name(int symoff, int nsyms, int stroff, void *ptr)
{
	char		*stringtable;
	int		i;
	struct nlist	*array;
	char		*section;
	struct mach_header		*header;

	header = (struct mach_header*)ptr;
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
			if (array[i].n_type & N_UNDF && array[i].n_sect == NO_SECT)
				printf("UNDEF\n");
			if (array[i].n_type & N_ABS && array[i].n_sect == NO_SECT)
				printf("ABSOLUTE\n");
			if (array[i].n_type & N_SECT)
				section = get_section(ptr + sizeof(struct mach_header), array[i].n_sect, header->ncmds);
			printf("SECTION = %s\n", section);
			free(section);
			if (array[i].n_type & N_PBUD && array[i].n_sect == NO_SECT)
				printf("PBUD\n");
			if (array[i].n_type & N_INDR)
			{
				//TODO A comprendre
				printf("INDR\n");
				printf("n_value = %u\n", array[i].n_value);
			}
		}
		if (array[i].n_type & N_EXT)
		{
			printf("EXTERNAL\n"); // SEMBLE ETRE 'U'
		}
		i++;
	}

}

void	handle_64_big(const void *ptr)
{
	uint32_t				ncmds;
	struct mach_header_64		*header;
	struct load_command		*lc;
	uint32_t				i;
	struct symtab_command	*symtab;

	header = (struct mach_header_64*)ptr;
	lc = (void*)ptr + sizeof(struct mach_header_64);
	ncmds = swap_little_endian(header->ncmds);
	printf("ncmds: %d\n",ncmds);
	i = 0;
	while (i < ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			printf("ON ENTRE\n");
			symtab = (struct symtab_command*)lc;
			browse_symtab_64(symtab->symoff, symtab->nsyms, symtab->stroff, (void*)ptr);
			break;
		}
		lc = (void*)lc + lc->cmdsize;
		i++;
	}
}

void	browse_load_commands(void* ptr, struct load_command *lc, int ncmds, int mode)
{
	struct symtab_command	*symtab;
	int						i;

	i = 0;
	printf("ncmds: %d\n",ncmds);
	while (i < ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			symtab = (struct symtab_command*)lc;
			if (mode == 1)
				print_name(symtab->symoff, symtab->nsyms, symtab->stroff, (void*)ptr);
			else
				browse_symtab_64(symtab->symoff, symtab->nsyms, symtab->stroff, (void*)ptr);
			break;
		}
		lc = (void*)lc + lc->cmdsize;
		i++;
	}
}

void	handle(void *ptr)
{
	struct load_command		*lc;
	struct mach_header		*header;

	header = (struct mach_header*)ptr;
	lc = (void*)ptr + sizeof(struct mach_header);
	browse_load_commands(ptr, lc, header->ncmds, 1);
}

void	handle_64(void *ptr)
{
	struct load_command		*lc;
	struct mach_header_64		*header;

	printf("On passe ici\n");
	header = (struct mach_header_64*)ptr;
	lc = (void*)ptr + sizeof(struct mach_header_64);
	browse_load_commands(ptr, lc, header->ncmds, 2);
}

void	ft_nm(char *ptr)
{
	unsigned int			magic_number;

	magic_number = *(int *)ptr;
	if (magic_number == MH_MAGIC_64)
	{
		handle_64(ptr);
	}
	else if (magic_number == MH_MAGIC_64)
		handle(ptr);
	//else if (is_fat_file(ptr))
	//{
	//	handle_fat_file(ptr);
	//}
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
