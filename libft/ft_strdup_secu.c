/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup_secu.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 15:52:26 by lbopp             #+#    #+#             */
/*   Updated: 2019/05/09 15:53:26 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup_secu(const char *s1, void *eos)
{
	size_t	size;
	int		i;
	char	*dst;

	i = 0;
	if ((size = ft_strlen_secu(s1, eos)) == 0)
		return (NULL);
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
