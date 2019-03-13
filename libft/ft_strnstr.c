/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/09 16:35:41 by lbopp             #+#    #+#             */
/*   Updated: 2016/11/12 11:03:24 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	int i;
	int j;

	i = 0;
	j = 0;
	if (!big[i] && !little[i])
		return (&((char*)big)[i]);
	while (big[i] && i + j < (int)len)
	{
		j = 0;
		while (big[i + j] == little[j] && big[i] && little[j] &&
				i + j < (int)len)
			j++;
		if (!little[j])
			return (&((char*)big)[i]);
		else
			i++;
	}
	return (0);
}
