/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_arch.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/11 11:19:52 by lbopp             #+#    #+#             */
/*   Updated: 2019/05/11 11:41:38 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void	print_ar_name(int size_name, struct ar_hdr *ar, t_info info)
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

static void	launch_nm(struct ar_hdr *ar, unsigned int size_name, t_info *info)
{
	if ((*info).f_ptr > (void*)ar + sizeof(struct ar_hdr) + size_name
			|| (*info).f_ptr + (*info).size_file <
			(void*)ar + sizeof(struct ar_hdr) + size_name)
		return ;
	ft_nm((void*)ar + sizeof(struct ar_hdr) + size_name,
			ft_atoi(ar->ar_size), -1, info);
}

void		handle_arch(void *ptr, int size, char *filename, t_info *i)
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
			launch_nm(ar, size_name, i);
		ar = (void*)ar + sizeof(struct ar_hdr) + ft_atoi(ar->ar_size);
		if ((*i).f_ptr > (void*)ar || (*i).f_ptr + (*i).size_file < (void*)ar)
			return ;
	}
}
