/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 10:03:10 by lbopp             #+#    #+#             */
/*   Updated: 2019/04/09 14:34:08 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

t_data		*sort_data(t_data *array, int size)
{
	int		i;
	t_data	tmp;

	i = 0;
	while (i + 1 < size)
	{
		if (ft_strcmp(array[i].name, array[i + 1].name) > 0)
		{
			tmp = array[i];
			array[i] = array[i + 1];
			array[i + 1] = tmp;
			i = 0;
			continue ;
		}
		i++;
	}
	return (array);
}

uint32_t	swap_little_endian(uint32_t nb)
{
	uint32_t	swapped;

	swapped = nb >> 24 | (nb >> 8 & 0xff00) | (nb << 8 & 0xff0000) | nb << 24;
	return (swapped);
}

int		get_ncmds(void *ptr)
{
	void			*header;
	unsigned int	magic_number;

	magic_number = *(int*)ptr;
	if (magic_number == MH_MAGIC_64)
	{
		header = (struct mach_header_64*)ptr;
		return (((struct mach_header_64*)header)->ncmds);
	}
	else if (magic_number == MH_MAGIC)
	{
		header = (struct mach_header*)ptr;
		return (((struct mach_header*)header)->ncmds);
	}
	else if (magic_number == MH_CIGAM_64)
	{
		header = (struct mach_header_64*)ptr;
		return (swap_little_endian(((struct mach_header_64*)header)->ncmds));
	}
	else if (magic_number == MH_CIGAM)
	{
		header = (struct mach_header*)ptr;
		return (swap_little_endian(((struct mach_header*)header)->ncmds));
	}
	return (0);
}


void	get_section_64(void *ptr, struct load_command *lc, int n_sect, t_data *data)
{
	int							i;
	struct segment_command_64	*seg_cmd;
	struct section_64			section;
	int							ncmds;

	ncmds = get_ncmds(ptr);
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

void	get_section(void* ptr, struct load_command *lc, int n_sect, t_data *data)
{
	int							i;
	struct segment_command	*seg_cmd;
	struct section			section;
	int						ncmds;

	ncmds = get_ncmds(ptr);
	i = 0;
	while (i < ncmds)
	{
		if (lc->cmd == LC_SEGMENT)
		{
			seg_cmd = (struct segment_command*)lc;
			if (n_sect && seg_cmd->nsects / n_sect)
			{
			    section = ((struct section*)(seg_cmd + 1))[n_sect - 1];
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
		if (array[i].is_pext && !array[i].section)
		{
			if (array[i].segment)
				free(array[i].segment);
			continue;
			printf("- ");
		}
		if (!array[i].section)
			printf("                 ");
		else
			printf("%016llx ", array[i].n_value.n_value64);
		if (array[i].section && !ft_strcmp(array[i].section, "__text"))
		{
			if (array[i].is_external)
				printf("T ");
			else
				printf("t ");
		}
		else if (array[i].section && !ft_strcmp(array[i].section, "__bss"))
		{
			if (array[i].is_external)
				printf("B ");
			else
				printf("b ");
		}
		else if (array[i].section && !ft_strcmp(array[i].section, "__data"))
		{
			if (array[i].is_external)
				printf("D ");
			else
				printf("d ");
		}
		else if (array[i].section)
		{
			if (array[i].is_external)
				printf("S ");
			else
				printf("s ");
		}
		else
			printf("U ");
		printf("%s\n", array[i].name);
		if (array[i].segment)
			free(array[i].segment);
		if (array[i].section)
			free(array[i].section);
	}
}

void	fill_data_64(t_data *string_array, struct nlist_64 nlist, void *ptr)
{
	struct mach_header_64		*header;

	header = (struct mach_header_64*)ptr;
	string_array->is_external = 0;
	if (nlist.n_type & N_PEXT)
	{
		string_array->is_pext = 1;
		//printf("PEXTERNAL\n"); // TODO Quand c'est active, mettre lettre en minuscule (non-external)
	}
	if (nlist.n_type & N_TYPE)
	{
		if (nlist.n_type & N_UNDF && nlist.n_sect == NO_SECT)
		{
			//printf("UNDEF\n");
			string_array->type.is_undef = 1;
		}
		else if (nlist.n_type & N_ABS && nlist.n_sect == NO_SECT)
		{
			string_array->type.is_absolute = 1;
			//printf("ABSOLUTE\n"); // TODO semble etre le 'A'
		}
		else if (nlist.n_type & N_SECT)
			get_section_64(ptr, ptr + sizeof(struct mach_header_64), nlist.n_sect, string_array);
		else if (nlist.n_type & N_PBUD && nlist.n_sect == NO_SECT)
		{
			string_array->is_pbud = 1;
			//printf("PBUD\n");
		}
		//if (nlist.n_type & N_INDR)
		//{
		//	//TODO A comprendre
		//	//printf("INDR\n");
		//}
	}
	if (nlist.n_type & N_EXT)
	{
		string_array->is_external = 1;
		//printf("EXTERNAL\n"); // SEMBLE ETRE 'U'
	}
}

void	fill_data(t_data *string_array, struct nlist nlist, void *ptr)
{
	struct mach_header		*header;

	header = (struct mach_header*)ptr;
	if (nlist.n_type & N_PEXT)
	{
		string_array->is_pext = 1;
		//printf("PEXTERNAL\n"); // TODO Quand c'est active, mettre lettre en minuscule (non-external)
	}
	if (nlist.n_type & N_TYPE)
	{
		if (nlist.n_type & N_UNDF && nlist.n_sect == NO_SECT)
		{
			//printf("UNDEF\n");
			string_array->type.is_undef = 1;
		}
		else if (nlist.n_type & N_ABS && nlist.n_sect == NO_SECT)
		{
			string_array->type.is_absolute = 1;
			//printf("ABSOLUTE\n"); // TODO semble etre le 'A'
		}
		else if (nlist.n_type & N_SECT)
			get_section(ptr, ptr + sizeof(struct mach_header), nlist.n_sect, string_array);
		else if (nlist.n_type & N_PBUD && nlist.n_sect == NO_SECT)
		{
			string_array->is_pbud = 1;
			//printf("PBUD\n");
		}
		//if (nlist.n_type & N_INDR)
		//{
		//	//TODO A comprendre
		//	printf("INDR\n");
		//}
	}
	if (nlist.n_type & N_EXT)
		string_array->is_external = 1;
}

void	browse_symtab(int symoff, int nsyms, int stroff, void *ptr)
{
	int			i;
	void		*array;
	t_data		*string_array;

	array = ptr + symoff;
	i = 0;
	string_array = (t_data*)malloc(nsyms * sizeof(t_data));
	while (i < nsyms)
	{
		string_array[i].segment = NULL;
		string_array[i].section = NULL;
		if (*(unsigned int*)ptr == MH_CIGAM_64 || *(unsigned int*)ptr == MH_MAGIC_64)
		{
			string_array[i].n_value.n_value64 = ((struct nlist_64*)array)[i].n_value;
			string_array[i].name = ft_strdup(ptr + stroff + ((struct nlist_64*)array)[i].n_un.n_strx);
			fill_data_64(&(string_array[i]), ((struct nlist_64*)array)[i], ptr);
		}
		else
		{
			string_array[i].n_value.n_value32 = ((struct nlist*)array)[i].n_value;
			string_array[i].name = ft_strdup(ptr + stroff + ((struct nlist*)array)[i].n_un.n_strx);
			fill_data(&(string_array[i]), ((struct nlist*)array)[i], ptr);
		}
		i++;
	}
	sort_data(string_array, nsyms);
	print_data(string_array, nsyms);
}

void	browse_load_commands(void* ptr, struct load_command *lc)
{
	struct symtab_command	*symtab;
	int						i;
	int						ncmds;

	i = 0;
	ncmds = get_ncmds(ptr);
	while (i < ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			symtab = (struct symtab_command*)lc;
			browse_symtab(symtab->symoff, symtab->nsyms, symtab->stroff, (void*)ptr);
			break;
		}
		lc = (void*)lc + lc->cmdsize;
		i++;
	}
}

void	handle(void *ptr)
{
	struct load_command		*lc;

	lc = (void*)ptr + sizeof(struct mach_header);
	browse_load_commands(ptr, lc);
}

void	handle_64(void *ptr)
{
	struct load_command		*lc;

	lc = (void*)ptr + sizeof(struct mach_header_64);
	browse_load_commands(ptr, lc);
}

int		is_fat_file(const void *ptr)
{
	unsigned int	magic_number;

	magic_number = *(int*)ptr;
	if (magic_number == FAT_MAGIC || magic_number == FAT_CIGAM)
		return (1);
	else if (magic_number == FAT_MAGIC_64 || magic_number == FAT_CIGAM_64)
		return (1);
	return (0);
}

//int		search_host_arch(void *ptr, char *filename)
//{
//	struct fat_header		*fat_header;
//	t_fat_arch				fat_arch;
//	void					*new_ptr;
//	uint32_t				i;
//
//	i = -1;
//	fat_header = ptr;
//	if (fat_header->magic == FAT_CIGAM_64)
//		fat_arch.is_64 = (struct fat_arch_64*)((void*)ptr + sizeof(struct fat_header));
//	else
//		fat_arch.is_32 = (struct fat_arch*)((void*)ptr + sizeof(struct fat_header));
//	while (++i < swap_little_endian(fat_header->nfat_arch))
//	{
//		if (fat_header->magic == FAT_CIGAM_64 &&
//				swap_little_endian(fat_arch.is_64->cputype) == CPU_TYPE_X86_64)
//		{
//			new_ptr = (void*)ptr + swap_little_endian(fat_arch.is_64->offset);
//			ft_nm(new_ptr, swap_little_endian(fat_arch.is_64->size), filename);
//			fat_arch.is_64 = (void*)fat_arch.is_64 + sizeof(struct fat_arch_64);
//			return (1);
//		}
//		else if (swap_little_endian(fat_arch.is_32->cputype) == CPU_TYPE_X86_64)
//		{
//			new_ptr = (void*)ptr + swap_little_endian(fat_arch.is_32->offset);
//			ft_nm(new_ptr, swap_little_endian(fat_arch.is_32->size), filename);
//			fat_arch.is_32 = (void*)fat_arch.is_32 + sizeof(struct fat_arch);
//			return (1);
//		}
//		if (fat_header->magic == FAT_CIGAM_64)
//			fat_arch.is_64 = (struct fat_arch_64*)((void*)ptr + sizeof(struct fat_header));
//		else
//			fat_arch.is_32 = (struct fat_arch*)((void*)ptr + sizeof(struct fat_header));
//	}
//	return (0);
//}

int		search_host_arch(void *ptr, char *filename)
{
	struct fat_header		*fat_header;
	struct fat_arch			*fat_arch;
	void					*new_ptr;
	uint32_t				i;

	i = -1;
	fat_header = ptr;
	fat_arch = (struct fat_arch*)((void*)ptr + sizeof(struct fat_header));
	while (++i < swap_little_endian(fat_header->nfat_arch))
	{
		new_ptr = (void*)ptr + swap_little_endian(fat_arch->offset);
		if (swap_little_endian(fat_arch->cputype) == CPU_TYPE_X86_64)
		{
			ft_nm(new_ptr, swap_little_endian(fat_arch->size), filename);
			return (1);
		}
		fat_arch = (void*)fat_arch + sizeof(struct fat_arch);
	}
	return (0);
}

int		search_host_arch_64(void *ptr, char *filename)
{
	struct fat_header		*fat_header;
	struct fat_arch_64		*fat_arch;
	void					*new_ptr;
	uint32_t				i;

	i = -1;
	fat_header = ptr;
	fat_arch = (struct fat_arch_64*)((void*)ptr + sizeof(struct fat_header));
	while (++i < swap_little_endian(fat_header->nfat_arch))
	{
		new_ptr = (void*)ptr + swap_little_endian(fat_arch->offset);
		if (swap_little_endian(fat_arch->cputype) == CPU_TYPE_X86_64)
		{
			ft_nm(new_ptr, swap_little_endian(fat_arch->size), filename);
			return (1);
		}
		fat_arch = (void*)fat_arch + sizeof(struct fat_arch_64);
	}
	return (0);
}

void	process_fat_file(void *ptr, char *filename)
{
	struct fat_header		*fat_header;
	t_fat_arch				fat_arch;
	void					*new_ptr;
	uint32_t				i;

	i = -1;
	fat_header = ptr;
	if (fat_header->magic == FAT_CIGAM_64)
		fat_arch.is_64 = (struct fat_arch_64*)((void*)ptr + sizeof(struct fat_header));
	else
		fat_arch.is_32 = (struct fat_arch*)((void*)ptr + sizeof(struct fat_header));
	while (++i < swap_little_endian(fat_header->nfat_arch))
		if (fat_header->magic == FAT_CIGAM_64)
		{
			new_ptr = (void*)ptr + swap_little_endian(fat_arch.is_64->offset);
			ft_nm(new_ptr, swap_little_endian(fat_arch.is_64->size), filename);
			fat_arch.is_64 = (void*)fat_arch.is_64 + sizeof(struct fat_arch_64);
		}
		else
		{
			new_ptr = (void*)ptr + swap_little_endian(fat_arch.is_32->offset);
			ft_nm(new_ptr, swap_little_endian(fat_arch.is_32->size), filename);
			fat_arch.is_32 = (void*)fat_arch.is_32 + sizeof(struct fat_arch);
		}
}

void	handle_fat_file(const void *ptr, char *filename)
{
	struct fat_header		*fat_header;

	fat_header = (struct fat_header*)ptr;
	if (fat_header->magic == FAT_CIGAM_64 &&
			!search_host_arch_64((void*)ptr, filename))
		process_fat_file((void*)ptr, filename);
	else if (fat_header->magic == FAT_CIGAM &&
			!search_host_arch((void*)ptr, filename))
		process_fat_file((void*)ptr, filename);
}

void	handle_arch(void *ptr, int size, char *filename)
{
	struct ar_hdr	*ar;
	unsigned int	size_name;

	(void)filename;
	ar = ptr + 8; // Watch out for corrupted files
	size -= 8 + sizeof(struct ar_hdr) + ft_atoi(ar->ar_size);
	ar = (void*)ar + sizeof(struct ar_hdr) + ft_atoi(ar->ar_size);
	while (size)
	{
		size_name = 0;
		printf("\n%s", filename);
		size -= sizeof(struct ar_hdr) + ft_atoi(ar->ar_size);
		if (!ft_strncmp(ar->ar_name, "#1/", 3))
			size_name = ft_atoi(ar->ar_name + 3);
		if (size_name != 0)
			printf("(%s):\n", ar->ar_name + sizeof(struct ar_hdr));
		else
			printf("(%s):\n", ar->ar_name);
		if (ft_strcmp(ar->ar_name, ""))
			ft_nm((void*)ar + sizeof(struct ar_hdr) + size_name, ft_atoi(ar->ar_size), filename);
		ar = (void*)ar + sizeof(struct ar_hdr) + ft_atoi(ar->ar_size);
	}
}

void	ft_nm(char *ptr, int size, char *filename)
{
	unsigned int			magic_number;

	magic_number = *(int *)ptr;
	if (magic_number == MH_MAGIC_64)
		handle_64(ptr);
	else if (magic_number == MH_MAGIC)
		handle(ptr);
	else if (is_fat_file(ptr))
		handle_fat_file(ptr, filename);
	else if (!ft_memcmp(ptr, ARMAG, SARMAG))
		handle_arch(ptr, size, filename);
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
		//printf("file = %s\n", filename); //TODO
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
	ft_nm(ptr, buf.st_size, filename);
	if ((munmap(ptr, buf.st_size)) == -1)
	{
		printf("ERREUR MUNMAP"); //TODO
		return(1);
	}
}
