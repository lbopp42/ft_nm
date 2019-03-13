/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_list_cir.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/15 11:13:14 by lbopp             #+#    #+#             */
/*   Updated: 2017/02/15 11:13:17 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list_cir	*ft_create_list_cir(char *content)
{
	t_list_cir	*new;

	if (!(new = (t_list_cir*)ft_memalloc(sizeof(t_list_cir))))
		return (NULL);
	new->next = new;
	new->prev = new;
	new->content = ft_strdup(content);
	return (new);
}
