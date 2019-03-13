/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/05 10:42:41 by lbopp             #+#    #+#             */
/*   Updated: 2016/11/12 10:49:20 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	size_t	size;
	int		i;
	char	*dst;

	i = 0;
	size = ft_strlen(s1);
	dst = (char *)malloc(sizeof(char) * (size + 1));
	if (dst == NULL)
		return (0);
	else
	{
		while (s1[i] != '\0')
		{
			dst[i] = s1[i];
			i++;
		}
		dst[i] = '\0';
		return (dst);
	}
}
