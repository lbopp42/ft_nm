/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/06 15:15:10 by lbopp             #+#    #+#             */
/*   Updated: 2016/11/12 15:03:06 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memalloc(size_t size)
{
	char	*alloc;

	alloc = (char*)malloc(size + 1);
	if (alloc && size < 2147483647)
	{
		ft_memset(alloc, 0, size);
		return (alloc);
	}
	else
		return (0);
}
