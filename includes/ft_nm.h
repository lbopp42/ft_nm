/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 10:42:04 by lbopp             #+#    #+#             */
/*   Updated: 2019/03/28 13:54:01 by lbopp            ###   ########.fr       */
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

typedef union	u_value
{
	uint32_t	n_value32;
	uint64_t	n_value64;
}				t_value;

typedef union	u_type
{
	int			is_absolute;
	int			is_undef;
}				t_type;

typedef struct	s_data
{
	char		*name;
	t_value		n_value;
	char		*segment;
	char		*section;
	int			is_external;
	int			is_pext;
	int			is_pbud;
	t_type		type;
}				t_data;

#endif
