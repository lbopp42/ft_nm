/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/11 08:35:54 by lbopp             #+#    #+#             */
/*   Updated: 2016/11/14 11:19:42 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_fill(unsigned int div, int size, int i, int n)
{
	unsigned int	nb;
	char			*result;

	result = NULL;
	nb = n;
	if (n < 0 && n != 0)
		nb = -n;
	if (!(result = (char*)malloc(sizeof(char) * (size + 1))))
		return (0);
	if (n < 0 && n != 0)
		result[0] = '-';
	if (n <= 0)
		size--;
	while (size > 0)
	{
		result[i] = nb / div + '0';
		nb %= div;
		div /= 10;
		size--;
		i++;
	}
	result[i] = '\0';
	return (result);
}

char		*ft_itoa(int n)
{
	unsigned int	div;
	int				size;
	unsigned int	nb;
	int				i;

	size = 1;
	i = 0;
	nb = n;
	if (n < 0 && n != 0)
	{
		nb = -n;
		size++;
		i++;
	}
	if (n == 0)
		size++;
	div = 1;
	while (nb / 10 >= div)
	{
		div *= 10;
		size++;
	}
	return (ft_fill(div, size, i, n));
}
