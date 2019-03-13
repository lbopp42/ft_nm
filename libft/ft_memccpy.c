/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/11 14:49:24 by lbopp             #+#    #+#             */
/*   Updated: 2016/11/12 10:42:14 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;
	int				i;
	int				end;

	d = (unsigned char*)dst;
	s = (unsigned char*)src;
	i = 0;
	end = (int)n;
	while (i < end)
	{
		d[i] = s[i];
		if (d[i] == (unsigned char)c)
		{
			i++;
			return (d + i);
		}
		i++;
	}
	return (0);
}
