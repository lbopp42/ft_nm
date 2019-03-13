/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/05 14:31:57 by lbopp             #+#    #+#             */
/*   Updated: 2016/11/14 11:26:11 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	int	i;
	int	travel;
	int	j;

	j = 0;
	travel = 0;
	while (dst[travel] && travel < (int)size)
	{
		travel++;
	}
	i = ft_strlen(dst);
	while ((i + j + 1) < (int)size && (i + j) < ((int)ft_strlen(src) + travel))
	{
		dst[i + j] = src[j];
		j++;
	}
	dst[i + j] = '\0';
	return (travel + ft_strlen(src));
}
