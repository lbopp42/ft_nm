/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_add_next_list_cir.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/15 11:16:50 by lbopp             #+#    #+#             */
/*   Updated: 2017/02/15 11:16:52 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_add_next_list_cir(t_list_cir **cour, t_list_cir *new)
{
	if (*cour == NULL)
		*cour = new;
	else
	{
		(*cour)->next->prev = new;
		new->next = (*cour)->next;
		new->prev = *cour;
		(*cour)->next = new;
	}
}
