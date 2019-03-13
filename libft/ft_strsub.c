/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/07 10:17:22 by lbopp             #+#    #+#             */
/*   Updated: 2016/11/12 15:15:24 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strsub(char	const *s, unsigned int start, size_t len)
{
	char			*news;
	unsigned int	i;

	i = 0;
	if (!(s))
		return (0);
	if (!(news = (char*)malloc(sizeof(char) * (len + 1))))
		return (0);
	while (i < (unsigned int)len)
	{
		news[i] = s[start];
		start++;
		i++;
	}
	news[i] = '\0';
	return (news);
}
