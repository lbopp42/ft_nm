/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_hex.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 16:19:35 by lbopp             #+#    #+#             */
/*   Updated: 2019/05/01 17:05:50 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static char	*ft_fill(uint64_t div, int size, int i, uint64_t n)
{
	uint64_t	nb;
	char		*result;
	const char	array[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

	result = NULL;
	nb = n;
	if (!(result = (char*)malloc(sizeof(char) * (size + 1))))
		return (0);
	while (size > 0)
	{
		result[i] = array[nb / div];
		nb %= div;
		div /= 16;
		size--;
		i++;
	}
	result[i] = '\0';
	return (result);
}

char		*ft_itoa_hex(uint64_t n)
{
	uint64_t		div;
	int				size;
	uint64_t		nb;
	int				i;

	size = 1;
	i = 0;
	nb = n;
	div = 1;
	while (nb / 16 >= div)
	{
		div *= 16;
		size++;
	}
	return (ft_fill(div, size, i, n));
}
