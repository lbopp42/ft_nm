/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 10:42:04 by lbopp             #+#    #+#             */
/*   Updated: 2019/05/10 18:00:46 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach-o/fat.h>
#include <errno.h>
#include <ar.h>
#include "libft.h"
#define ARCH_32 32
#define ARCH_64 64

typedef union	u_value
{
	uint32_t	n_value32;
	uint64_t	n_value64;
}				t_value;

typedef union	u_fat_arch
{
	struct fat_arch_64	*is_64;
	struct fat_arch	*is_32;
}				t_fat_arch;

typedef struct	s_data
{
	char		*name;
	t_value		n_value;
	char		*segment;
	char		*section;
	int			is_external;
	int			is_pext;
	int			is_pbud;
	int			is_stab;
	int			is_ind;
	int			is_abs;
	int			is_undef;
}				t_data;

typedef struct	s_info
{
	void	*f_ptr;
	int		size_file;
	t_data	*data;
	int		c;
}				t_info;

void		ft_nm(char *ptr, int size, char *filename, int nb_file, t_info *info);

t_data		*qs_data(t_data *data, int premier, int dernier);

/*
**	TOOL1.C
*/
uint32_t	swap_little_endian(uint32_t nb);
int			get_ncmds(void *ptr, t_info info);
uint32_t	get_32(void *ptr, uint32_t v);
uint64_t	get_64(void *ptr, uint64_t v);
int			is_macho64(void *ptr);

/*
**	TOOL2.C
*/
int		get_n_strx(void *ptr, void *nlist, int c, t_info i);
int		get_n_value(void *ptr, void *nlist, int i);

/*
**	FILL_SECTION.C
*/
void		get_section(void *ptr, struct load_command *lc, int n_sect,
		t_info *i);
void		get_section_64(void *ptr, struct load_command *lc,
		int n_sect, t_info *i);

/*
**	PRINT.C
*/
void		print_data(void *ptr, t_data *array, int size);

/*
**	FILL_DATA.C
*/
void		fill_data_64(struct nlist_64 nlist, void *ptr,
		t_info *info, int c);
void		fill_data_32(struct nlist nlist, void *ptr,
		t_info *info, int c);

/*
**	BROWSE_SYMTAB.C
*/
void		browse_symtab(void *array, struct symtab_command *symtab,
		void *ptr, t_info *i);

/*
**	HANDLE_MACHO.C
*/
void		handle(void *ptr, t_info *i, char *filename, int nb_file);
void		handle_64(void *ptr, t_info *i, char *filename, int nb_file);

/*
**	GET_CPU_TYPE.C
*/
char		*get_cputype(void *ptr, int i, t_info info);

#endif
