/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_list_cir.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/15 11:23:19 by lbopp             #+#    #+#             */
/*   Updated: 2017/02/15 11:23:31 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_print_list_cir(t_list_cir *list_cir)
{
	t_list_cir	*tmp;

	if (list_cir == NULL)
		ft_putstr_fd("List is empty", 2);
	else
	{
		ft_putendl(list_cir->content);
		tmp = list_cir->prev;
		while (tmp != list_cir)
		{
			ft_putendl(tmp->content);
			tmp = tmp->prev;
		}
	}
}
