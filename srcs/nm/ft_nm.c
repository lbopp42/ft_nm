/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 10:03:10 by lbopp             #+#    #+#             */
/*   Updated: 2019/03/28 15:10:24 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

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
		printf("%016llx ", array[i].n_value.n_value64);
		if (array[i].is_pext && !array[i].section)
			printf("- ");
		else if (array[i].section && !ft_strcmp(array[i].section, "__text"))
			printf("T ");
		else if (!array[i].section)
			printf("U ");
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
		if (nlist.n_type & N_INDR)
		{
			//TODO A comprendre
			printf("INDR\n");
		}
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
	{
		string_array->is_external = 1;
		//printf("EXTERNAL\n"); // SEMBLE ETRE 'U'
	}
}

void	browse_symtab(int symoff, int nsyms, int stroff, void *ptr)
{
	int			i;
	void		*array;
	t_data		*string_array;

	array = ptr + symoff;
	i = 0;
	printf("On creer un tableau de %d\n", nsyms);
	string_array = (t_data*)malloc(nsyms * sizeof(t_data));
	while (i < nsyms)
	{
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

int		is_64_fat_file(const void *ptr)
{
	struct fat_header		*fat_header;

	fat_header = (struct fat_header*)ptr;
	if (fat_header->magic == FAT_CIGAM_64)
		return (1);
	return (0);
}

void	process_64_fat_file(const void *ptr)
{
	struct fat_header		*fat_header;
	struct fat_arch_64		*fat_arch;
	void					*new_ptr;
	unsigned int			magic_number;
	uint32_t				i;

	i = 0;
	fat_header = (struct fat_header*)ptr;
	printf("ICI c'est OK\n");
	while (i < fat_header->nfat_arch)
	{
		fat_arch = (struct fat_arch_64*)((void*)ptr + sizeof(struct fat_header));
		new_ptr = (void*)ptr + sizeof(struct fat_header) + swap_little_endian(fat_arch->offset);
		magic_number = *(int*)new_ptr;
		if (magic_number == MH_MAGIC || magic_number == MH_MAGIC_64 || magic_number == MH_CIGAM || magic_number == MH_CIGAM_64)
		{
			printf("ON ENTRE\n");
			handle_64(new_ptr);
			break;
		}
		i++;
	}
}
int		search_host_arch(void *ptr)
{
	struct fat_header		*fat_header;
	struct fat_arch			*fat_arch;
	void					*new_ptr;
	unsigned int			magic_number;
	uint32_t				i;

	i = 0;
	fat_header = ptr;
	printf("On entre\n");
	fat_arch = (struct fat_arch*)((void*)ptr + sizeof(struct fat_header));
	while (i < swap_little_endian(fat_header->nfat_arch))
	{
		printf("OFFSET = %d\n", swap_little_endian(fat_arch->offset));
		new_ptr = (void*)ptr + swap_little_endian(fat_arch->offset);
		magic_number = *(int*)new_ptr;
		printf("MAGIC = %x\n", swap_little_endian(magic_number));
		if (magic_number == MH_CIGAM_64 || magic_number == MH_MAGIC_64)
		{
			printf("On trouve une architecture mach-o\n");
			handle_64(new_ptr);
			return (1);
		}
		else if (magic_number == MH_CIGAM || magic_number == MH_MAGIC)
		{
			printf("On manipule du 32bits\n");
			handle(new_ptr);
		}
		else
			printf("Ok\n");
		fat_arch = (void*)fat_arch + sizeof(struct fat_arch);
		i++;
	}
	return (0);
}

void	process_fat_file(void *ptr)
{
	struct fat_header		*fat_header;
	struct fat_arch			*fat_arch;
	void					*new_ptr;
	unsigned int			magic_number;
	uint32_t				i;

	i = 0;
	fat_header = ptr;
	printf("On entre\n");
	fat_arch = (struct fat_arch*)((void*)ptr + sizeof(struct fat_header));
	while (i < swap_little_endian(fat_header->nfat_arch))
	{
		printf("OFFSET = %d\n", swap_little_endian(fat_arch->offset));
		new_ptr = (void*)ptr + swap_little_endian(fat_arch->offset);
		magic_number = *(int*)new_ptr;
		printf("MAGIC = %x\n", swap_little_endian(magic_number));
		if (magic_number == MH_CIGAM_64 || magic_number == MH_MAGIC_64)
		{
			printf("On trouve une architecture mach-o\n");
			//handle_64(new_ptr);
		}
		else if (magic_number == MH_CIGAM || magic_number == MH_MAGIC)
		{
			printf("On manipule du 32bits\n");
			//handle(new_ptr);
		}
		else
			printf("Ok\n");
		fat_arch = (void*)fat_arch + sizeof(struct fat_arch);
		i++;
	}
}

void	handle_fat_file(const void *ptr)
{
	struct fat_header		*fat_header;
	struct fat_arch			*fat_arch;
	unsigned int			magic_number;
	struct mach_header_64	*mach_header_64;

	fat_header = (struct fat_header*)ptr;
	if (is_64_fat_file(ptr))
	{
		process_64_fat_file(ptr);
	}
	else
	{
		printf("C'est une arch 64 bits\n");
		process_fat_file((void*)ptr);
	}
	printf("Il y a %x arch\n", fat_header->nfat_arch);
	printf("Il y a %x arch\n", swap_little_endian(fat_header->nfat_arch));
	fat_arch = (struct fat_arch*)((void*)ptr + sizeof(struct fat_header));
	if (swap_little_endian(fat_arch->cputype) == CPU_TYPE_X86_64)
		printf("x86_64\n");
	printf("offset = [%d]\n", swap_little_endian(fat_arch->offset));
	printf("size = [%d]\n", swap_little_endian(fat_arch->size));
	magic_number = *(int*)((void*)fat_header + swap_little_endian(fat_arch->offset));
	printf("magic_number = [%08x]\n", swap_little_endian(magic_number));
	mach_header_64 = (struct mach_header_64 *)((char *)fat_header + swap_little_endian(fat_arch->offset));
	printf("Magic = %08x\n", mach_header_64->magic);
}

void	handle_arch(void *ptr)
{
	struct ar_hdr	*ar;
	struct ar_hdr	*next;

	ar = ptr + 8;
	printf("ar_name = [%s]\n", ar->ar_name);
	printf("ar_size = [%s]\n", ar->ar_size);
	printf("sizeof ar_hdr = %lu\n", sizeof(struct ar_hdr));
	next = ptr + 8 + sizeof(struct ar_hdr) + ft_atoi(ar->ar_size);
	printf("next name = [%s]\n", next->ar_name);
	printf("MAGIC = %x\n", *(int*)((void*)next + sizeof(struct ar_hdr) + 20));
}

void	ft_nm(char *ptr)
{
	unsigned int			magic_number;

	magic_number = *(int *)ptr;
	if (magic_number == MH_MAGIC_64)
		handle_64(ptr);
	else if (magic_number == MH_MAGIC)
		handle(ptr);
	else if (is_fat_file(ptr))
	{
		printf("C'est un fat\n");
		handle_fat_file(ptr);
	}
	else if (!ft_memcmp(ptr, ARMAG, SARMAG))
	{
		printf("Nous avons ici une archive\n");
		handle_arch(ptr);
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
