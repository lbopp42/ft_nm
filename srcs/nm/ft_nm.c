/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 10:03:10 by lbopp             #+#    #+#             */
/*   Updated: 2019/05/10 16:58:55 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	print_addr_hex(uint64_t	n_value, int arch)
{
	char	*addr;
	size_t	len;
	char	*s;

	(void)arch;
	addr = ft_itoa_hex(n_value);
	len = ft_strlen(addr);
	if (arch == ARCH_32 && (s = malloc(sizeof(char) * (8 - len))))
	{
		ft_memset(s, '0', 8 - len);
		write(1, s, 8 - len);
	}
	else if (arch == ARCH_64 && (s = malloc(sizeof(char) * (16 - len))))
	{
		ft_memset(s, '0', 16 - len);
		write(1, s, 16 - len);
	}
	else
	{
		free(addr);
		return;
	}
	free(s);
	ft_putstr(addr);
	ft_putchar(' ');
}

char	get_char_sym(t_data elem)
{
	char	c;

	if (elem.section && !ft_strcmp(elem.section, "__text"))
		c = 't';
	else if (elem.section && !ft_strcmp(elem.section, "__bss"))
		c = 'b';
	else if (elem.section && !ft_strcmp(elem.section, "__data"))
		c = 'd';
	else if (elem.section)
		c = 's';
	else if (elem.is_abs)
		c = 'a';
	else if (elem.is_ind)
		c = 'i';
	else if (elem.is_undef && elem.n_value.n_value32 != 0)
		c = 'c';
	else
		c = 'u';
	if (elem.is_external)
		c = ft_toupper(c);
	return (c);
}

void	print_addr(unsigned int *ptr, t_data elem)
{
	if ((*ptr == MH_MAGIC_64 || *ptr == MH_CIGAM_64)
			&& elem.is_undef && !elem.n_value.n_value64)
		ft_putstr("                 ");
	else if ((*ptr == MH_MAGIC || *ptr == MH_CIGAM) && elem.is_undef
			&& !elem.n_value.n_value32)
		ft_putstr("         ");
	else if (*ptr == MH_MAGIC_64 || *ptr == MH_CIGAM_64)
		print_addr_hex(elem.n_value.n_value64, ARCH_64);
	else
		print_addr_hex(elem.n_value.n_value32, ARCH_32);
}

void	print_data(void *ptr, t_data *array, int size)
{
	int		i;

	i = -1;
	while (++i < size)
	{
		if (array[i].is_stab)
		{
			if (array[i].segment)
				free(array[i].segment);
			continue;
		}
		print_addr((unsigned int*)ptr, array[i]);
		ft_putchar(get_char_sym(array[i]));
		ft_putchar(' ');
		ft_putendl(array[i].name);
		if (array[i].segment)
			free(array[i].segment);
		if (array[i].section)
			free(array[i].section);
	}
}

void	init_string_array(t_data **string_array, int c)
{
	(*string_array)[c].is_ind = 0;
	(*string_array)[c].is_external = 0;
	(*string_array)[c].is_stab = 0;
	(*string_array)[c].is_pext = 0;
	(*string_array)[c].is_pbud = 0;
	(*string_array)[c].is_abs = 0;
	(*string_array)[c].is_undef = 0;
}

void	fill_data_64(struct nlist_64 nlist, void *ptr,
		t_info *info, int c)
{
	struct mach_header_64		*header;

	header = (struct mach_header_64*)ptr;
	init_string_array(&(*info).data, c);
	if (nlist.n_type & N_STAB)
		(*info).data[c].is_stab = 1;
	if (nlist.n_type & N_PEXT)
		(*info).data[c].is_pext = 1;
	if ((nlist.n_type & N_TYPE) == N_UNDF)
		(*info).data[c].is_undef = 1;
	else if ((nlist.n_type & N_TYPE) == N_ABS)
		(*info).data[c].is_abs = 1;
	else if ((nlist.n_type & N_TYPE) == N_SECT)
	{
		(*info).c = c;
		get_section_64(ptr, ptr + sizeof(struct mach_header_64), nlist.n_sect
				, info);
	}
	else if ((nlist.n_type & N_TYPE) == N_PBUD)
		(*info).data[c].is_pbud = 1;
	else if ((nlist.n_type & N_TYPE) == N_INDR)
		(*info).data[c].is_ind = 1;
	if (nlist.n_type & N_EXT)
		(*info).data[c].is_external = 1;
}

void	fill_data_32(struct nlist nlist, void *ptr,
		t_info *info, int c)
{
	struct mach_header		*header;

	header = (struct mach_header*)ptr;
	init_string_array(&(*info).data, c);
	if (nlist.n_type & N_STAB)
		(*info).data[c].is_stab = 1;
	if (nlist.n_type & N_PEXT)
		(*info).data[c].is_pext = 1;
	if ((nlist.n_type & N_TYPE) == N_UNDF)
		(*info).data[c].is_undef = 1;
	else if ((nlist.n_type & N_TYPE) == N_ABS)
		(*info).data[c].is_abs = 1;
	else if ((nlist.n_type & N_TYPE) == N_SECT)
	{
		(*info).c = c;
		get_section(ptr, ptr + sizeof(struct mach_header), nlist.n_sect, info);
	}
	else if ((nlist.n_type & N_TYPE) == N_PBUD)
		(*info).data[c].is_pbud = 1;
	else if ((nlist.n_type & N_TYPE) == N_INDR)
		(*info).data[c].is_ind = 1;
	if (nlist.n_type & N_EXT)
		(*info).data[c].is_external = 1;
}

int		is_macho64(void *ptr)
{
	if (*(unsigned int*)ptr == MH_CIGAM_64
			|| *(unsigned int*)ptr == MH_MAGIC_64)
		return (1);
	else
		return (0);
}

int		get_n_strx(void *ptr, void *nlist, int c, t_info i)
{
	if (*(unsigned int*)ptr == MH_MAGIC || *(unsigned int*)ptr == MH_CIGAM)
	{
		if (i.f_ptr > (void*)((struct nlist*)nlist + c)
				|| i.f_ptr + i.size_file < (void*)((struct nlist*)nlist + c))
			return (0);
	}
	else if (is_macho64(ptr))
		if (i.f_ptr > (void*)((struct nlist_64*)nlist + c)
				|| i.f_ptr + i.size_file < (void*)((struct nlist*)nlist + c))
			return (0);
	if (*(unsigned int*)ptr == MH_MAGIC)
		return (((struct nlist*)nlist)[c].n_un.n_strx);
	else if (*(unsigned int*)ptr == MH_MAGIC_64)
		return (((struct nlist_64*)nlist)[c].n_un.n_strx);
	else if (*(unsigned int*)ptr == MH_CIGAM)
		return (swap_little_endian(((struct nlist*)nlist)[c].n_un.n_strx));
	else
		return (swap_little_endian(((struct nlist_64*)nlist)[c].n_un.n_strx));
}

int		get_n_value(void *ptr, void *nlist, int i)
{
	if (*(unsigned int*)ptr == MH_MAGIC)
		return (((struct nlist*)nlist)[i].n_value);
	else if (*(unsigned int*)ptr == MH_MAGIC_64)
		return (((struct nlist_64*)nlist)[i].n_value);
	else if (*(unsigned int*)ptr == MH_CIGAM)
		return (swap_little_endian(((struct nlist*)nlist)[i].n_value));
	else
		return (swap_little_endian(((struct nlist_64*)nlist)[i].n_value));
}

void	fill_name(char *s, t_info *i, int c)
{
		(*i).data[c].segment = NULL;
		(*i).data[c].section = NULL;
		if ((*i).f_ptr > (void*)s || (*i).f_ptr + (*i).size_file < (void*)s)
		{
			(*i).data[c].name = NULL;
			return ;
		}
		(*i).data[c].name = ft_strdup_secu(s, (*i).f_ptr + (*i).size_file);
}

void	sort_print_data(void *ptr, t_data *data, int nsyms)
{
	qs_data(data, 0, nsyms - 1);
	print_data(ptr, data, nsyms);
}

int		check_symtab(void *ptr, void *array, t_info i)
{
	if (is_macho64(ptr)
		&& (i.f_ptr <= (void*)((struct nlist_64*)array + i.c)
			&& i.f_ptr + i.size_file >= (void*)((struct nlist_64*)array + i.c)))
		return (1);
	else if (!is_macho64(ptr)
		&& (i.f_ptr <= (void*)((struct nlist*)array + i.c)
			&& i.f_ptr + i.size_file >= (void*)((struct nlist*)array + i.c)))
		return (1);
	else
		return (0);
}

void	browse_symtab(void *array, int nsyms, int stroff, void *ptr, t_info *i)
{
	char		*name;

	if (!((*i).data = (t_data*)malloc(nsyms * sizeof(t_data))))
		return ;
	(*i).c = -1;
	while (++(*i).c < nsyms)
	{
		name = ptr + stroff + get_n_strx(ptr, array, (*i).c, *i);
		fill_name(name, i, (*i).c);
		if (is_macho64(ptr) && check_symtab(ptr, array, (*i)))
		{
			(*i).data[(*i).c].n_value.n_value64 =
				get_64(ptr, ((struct nlist_64*)array)[(*i).c].n_value);
			fill_data_64(((struct nlist_64*)array)[(*i).c], ptr, i, (*i).c);
		}
		else if (check_symtab(ptr, array, (*i)))
		{
			(*i).data[(*i).c].n_value.n_value32 =
				get_32(ptr, ((struct nlist*)array)[(*i).c].n_value);
			fill_data_32(((struct nlist*)array)[(*i).c], ptr, i, (*i).c);
		}
	}
	sort_print_data(ptr, (*i).data, nsyms);
}

int		get_symtab_symoff(void *ptr, struct symtab_command *symtab)
{
	if (*(unsigned int*)ptr == MH_MAGIC_64 || *(unsigned int*)ptr == MH_MAGIC)
		return (symtab->symoff);
	else
		return (swap_little_endian(symtab->symoff));
}

int		get_symtab_nsyms(void *ptr, struct symtab_command *symtab)
{
	if (*(unsigned int*)ptr == MH_MAGIC_64 || *(unsigned int*)ptr == MH_MAGIC)
		return (symtab->nsyms);
	else
		return (swap_little_endian(symtab->nsyms));
}

int		get_symtab_stroff(void *ptr, struct symtab_command *symtab)
{
	if (*(unsigned int*)ptr == MH_MAGIC_64 || *(unsigned int*)ptr == MH_MAGIC)
		return (symtab->stroff);
	else
		return (swap_little_endian(symtab->stroff));
}

void	browse_load_commands(void *ptr, struct load_command *lc, t_info *i)
{
	struct symtab_command	*symtab;
	int						c;
	int						ncmds;
	void					*array;

	c = -1;
	ncmds = get_ncmds(ptr, *i);
	while (++c < ncmds)
	{
		if (get_32(ptr, lc->cmd) == LC_SYMTAB)
		{
			symtab = (struct symtab_command*)lc;
			array = ptr + get_symtab_symoff(ptr, symtab);
			if ((*i).f_ptr > (void*)array
					|| (*i).f_ptr + (*i).size_file < (void*)array)
				return ;
			browse_symtab(array, get_symtab_nsyms(ptr, symtab),
					get_symtab_stroff(ptr, symtab), (void*)ptr, i);
			break;
		}
		lc = (void*)lc + get_32(ptr, lc->cmdsize);
		if ((*i).f_ptr > (void*)lc || (*i).f_ptr + (*i).size_file < (void*)lc)
			break;
	}
}

void	handle(void *ptr, t_info *i, char *filename, int nb_file)
{
	struct load_command		*lc;

	if (nb_file > 1)
	{	
		ft_putchar('\n');
		ft_putstr(filename);
		ft_putendl(":");
	}
	lc = (void*)ptr + sizeof(struct mach_header);
	if ((*i).f_ptr <= (void*)lc && (*i).f_ptr + (*i).size_file >= (void*)lc)
		browse_load_commands(ptr, lc, i);
}

void	handle_64(void *ptr, t_info *i, char *filename, int nb_file)
{
	struct load_command		*lc;

	if (nb_file > 1)
	{	
		ft_putchar('\n');
		ft_putstr(filename);
		ft_putendl(":");
	}
	lc = (void*)ptr + sizeof(struct mach_header_64);
	if ((*i).f_ptr <= (void*)lc && (*i).f_ptr + (*i).size_file >= (void*)lc)
		browse_load_commands(ptr, lc, i);
}

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

char	*get_cputype_64(void *ptr, int i, t_info info)
{
	struct fat_arch_64	*fat_arch;

	fat_arch = (struct fat_arch_64*)((void*)ptr +
			sizeof(struct fat_header)) + i;
	if (info.f_ptr > (void*)fat_arch
			|| info.f_ptr + info.size_file < (void*)fat_arch)
		return ("undefined");
	if (swap_little_endian(fat_arch->cputype) == CPU_TYPE_POWERPC)
		return ("ppc");
	else if (swap_little_endian(fat_arch->cputype) == CPU_TYPE_X86)
		return ("i386");
	else
		return ("undefined");
}

char	*get_cputype_32(void *ptr, int i, t_info info)
{
	struct fat_arch	*fat_arch;

	fat_arch = (struct fat_arch*)((void*)ptr + sizeof(struct fat_header)) + i;
	if (info.f_ptr > (void*)fat_arch
			|| info.f_ptr + info.size_file < (void*)fat_arch)
		return ("undefined");
	if (swap_little_endian(fat_arch->cputype) == CPU_TYPE_POWERPC)
		return ("ppc");
	else if (swap_little_endian(fat_arch->cputype) == CPU_TYPE_X86)
		return ("i386");
	else
		return ("undefined");
}

char	*get_cputype(void *ptr, int i, t_info info)
{
	struct fat_header		*fat_header;

	fat_header = ptr;
	if (fat_header->magic == FAT_CIGAM_64)
		return(get_cputype_64(ptr, i, info));
	else
		return(get_cputype_32(ptr, i, info));
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
