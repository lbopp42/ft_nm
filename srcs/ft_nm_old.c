/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 10:03:10 by lbopp             #+#    #+#             */
/*   Updated: 2019/03/20 15:52:35 by lbopp            ###   ########.fr       */
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

void	print_name_64(int symoff, int nsyms, int stroff, void *ptr)
{
	char		*stringtable;
	int		i;
	struct nlist_64	*array;
	char		*section;
	struct mach_header_64		*header;

	header = (struct mach_header_64*)ptr;
	stringtable = ptr + stroff;
	array = ptr + symoff;
	i = 0;
	while (i < nsyms)
	{
		printf("%s\n", stringtable + array[i].n_un.n_strx);
		if (array[i].n_type & N_PEXT)
		{
			printf("PEXTERNAL\n"); // TODO Quand c'est active, mettre lettre en minuscule (non-external)
		}
		if (array[i].n_type & N_TYPE)
		{
			printf("============================\n");
			if (array[i].n_type & N_UNDF && array[i].n_sect == NO_SECT)
				printf("UNDEF\n");
			else if (array[i].n_type & N_ABS && array[i].n_sect == NO_SECT)
				printf("ABSOLUTE\n"); // TODO semble etre le 'A'
			else if (array[i].n_type & N_SECT)
			{
				section = get_section(ptr + sizeof(struct mach_header_64), array[i].n_sect, header->ncmds);
				printf("SECTION = %s\n", section);
				free(section);
			}
			if (array[i].n_type & N_PBUD && array[i].n_sect == NO_SECT)
				printf("PBUD\n");
			if (array[i].n_type & N_INDR)
			{
				//TODO A comprendre
				printf("INDR\n");
				printf("n_value = %llu\n", array[i].n_value);
			}
		}
		if (array[i].n_type & N_EXT)
		{
			printf("EXTERNAL\n"); // SEMBLE ETRE 'U'
		}
		i++;
	}

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

void	handle_64(const void *ptr)
{
	int				ncmds;
	struct mach_header_64		*header;
	struct load_command		*lc;
	int				i;
	struct symtab_command	*symtab;

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
			print_name_64(symtab->symoff, symtab->nsyms, symtab->stroff, (void*)ptr);
			break;
		}
		lc = (void*)lc + lc->cmdsize;
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
			print_name_64(symtab->symoff, symtab->nsyms, symtab->stroff, (void*)ptr);
			break;
		}
		lc = (void*)lc + lc->cmdsize;
		i++;
	}

}

void	handle(const void *ptr)
{
	int				ncmds;
	struct mach_header		*header;
	struct load_command		*lc;
	int				i;
	struct symtab_command	*symtab;

	header = (struct mach_header*)ptr;
	lc = (void*)ptr + sizeof(struct mach_header);
	ncmds = header->ncmds;
	printf("ncmds: %d\n",ncmds);
	i = 0;
	while (i < ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			symtab = (struct symtab_command*)lc;
			print_name(symtab->symoff, symtab->nsyms, symtab->stroff, (void*)ptr);
			break;
		}
		lc = (void*)lc + lc->cmdsize;
		i++;
	}

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
	if (fat_header->magic == FAT_MAGIC_64 || fat_header->magic == FAT_CIGAM_64)
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
	while (i < fat_header->nfat_arch)
	{
		fat_arch = (struct fat_arch_64*)((void*)ptr + sizeof(struct fat_header));
		new_ptr = (void*)ptr + sizeof(struct fat_header) + swap_little_endian(fat_arch->offset);
		magic_number = *(int*)new_ptr;
		if (magic_number == MH_MAGIC || magic_number == MH_MAGIC_64 || magic_number == MH_CIGAM || magic_number == MH_CIGAM_64)
		{
			handle_64_big(new_ptr);
			break;
		}
		i++;
	}
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
	while (i < swap_little_endian(fat_header->nfat_arch))
	{
		fat_arch = (struct fat_arch*)((void*)ptr + sizeof(struct fat_header));
		magic_number = *(int*)((void*)fat_header + swap_little_endian(fat_arch->offset));
		new_ptr = (void*)ptr + swap_little_endian(fat_arch->offset);
		magic_number = *(int*)new_ptr;
		if (magic_number == MH_MAGIC || magic_number == MH_MAGIC_64 || magic_number == MH_CIGAM || magic_number == MH_CIGAM_64)
		{
			printf("On trouve une architecture mach-o\n");
			//handle_64_big(new_ptr);
		}
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
		process_64_fat_file(ptr);
	else
		process_fat_file((void*)ptr);
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

void	ft_nm(char *ptr)
{
	unsigned int			magic_number;

	magic_number = *(int *)ptr;
	if (magic_number == MH_MAGIC_64)
	{
		handle_64(ptr);
	}
	else if (is_fat_file(ptr))
	{
		handle_fat_file(ptr);
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
