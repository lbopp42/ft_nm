/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcdup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/22 16:26:05 by lbopp             #+#    #+#             */
/*   Updated: 2016/11/30 09:33:05 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcdup(const char *s1, char n)
{
	size_t	size;
	int		i;
	char	*dst;

	i = 0;
	size = 0;
	while (s1[size] != n && s1[size])
		size++;
	dst = (char *)malloc(sizeof(char) * (size + 1));
	if (dst == NULL)
		return (0);
	else
	{
		while (s1[i] != n && s1[i])
		{
			dst[i] = s1[i];
			i++;
		}
		dst[i] = '\0';
		return (dst);
	}
}
