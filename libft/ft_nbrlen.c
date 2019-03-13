/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nbrlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/13 14:46:59 by lbopp             #+#    #+#             */
/*   Updated: 2017/01/13 09:51:33 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_nbrlen(int n)
{
	size_t			size;
	int				div;
	unsigned int	nb;

	size = 1;
	div = 1;
	nb = n;
	if (n < 0)
	{
		nb = -n;
		size++;
	}
	while (nb / 10 >= (unsigned int)div)
	{
		div *= 10;
		size++;
	}
	return (size);
}
