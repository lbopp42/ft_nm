/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 10:42:04 by lbopp             #+#    #+#             */
/*   Updated: 2019/05/13 14:03:13 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

# include <stdio.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <errno.h>
# include <ar.h>
# include "libft.h"
# define ARCH_32 -2
# define ARCH_64 -3

typedef union	u_value
{
	uint32_t	n_value32;
	uint64_t	n_value64;
}				t_value;

typedef union	u_fat_arch
{
	struct fat_arch_64	*is_64;
	struct fat_arch		*is_32;
}				t_fat_arch;

typedef struct	s_info
{
	void		*f_ptr;
	int			size_file;
	char		*filename;
	int			c;
}				t_info;

/*
**	LAUNCH_OTOOL.C
*/
void			ft_otool(char *ptr, int size, int nb_file, t_info *info);

/*
**	HANDLE_ARCH.C
*/
void			handle_arch(void *ptr, int size, char *filename, t_info *i);

/*
**	HANDLE_FAT.C
*/
void			handle_fat_file(const void *ptr, char *filename,
		int nb_file, t_info *info);

/*
**	HANDLE_MACHO.C
*/
void		handle_64(void *ptr, t_info *i, char *filename, int nb_file);
void		handle(void *ptr, t_info *i, char *filename, int nb_file);

/*
**	SEARCH_HOST.C
*/
int			search_host_arch(void *ptr, int nb_file, t_info *i);
int			search_host_arch_64(void *ptr, int nb_file,
		t_info *info);

/*
**	TOOL1.C
*/
uint32_t		swap_little_endian(uint32_t nb);
int				get_ncmds(void *ptr, t_info info);
uint32_t		get_32(void *ptr, uint32_t v);
uint64_t		get_64(void *ptr, uint64_t v);
int				is_macho64(void *ptr);

/*
**	TOOL2.C
*/
int				get_n_strx(void *ptr, void *nlist, int c, t_info i);
int				get_n_value(void *ptr, void *nlist, int i);
int				is_fat_file(const void *ptr, t_info info);

/*
**	GET_CPU_TYPE.C
*/
char			*get_cputype(void *ptr, int i, t_info info);

/*
**	BROWSE_SYMTAB.C
*/
void		browse_symtab(void *array, struct symtab_command *symtab,
		void *ptr, t_info *i);

#endif
