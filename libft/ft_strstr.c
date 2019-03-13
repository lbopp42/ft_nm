/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/06 14:01:26 by lbopp             #+#    #+#             */
/*   Updated: 2016/11/12 11:02:20 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *big, const char *little)
{
	int i;
	int j;

	i = 0;
	if (!big[i] && !little[i])
		return (&((char*)big)[i]);
	while (big[i])
	{
		j = 0;
		while (big[i + j] == little[j] && big[i] && little[j])
			j++;
		if (!little[j])
			return (&((char*)big)[i]);
		else
			i++;
	}
	return (0);
}
