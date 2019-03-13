/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/09 09:53:55 by lbopp             #+#    #+#             */
/*   Updated: 2016/11/14 11:10:32 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_positive(const char *str, int i)
{
	int result;

	result = 0;
	if (str[i] == '+' || str[i] == ' ')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - 48);
		i++;
	}
	return (result);
}

static int	ft_negative(const char *str, int i)
{
	int result;

	result = 0;
	i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - 48);
		i++;
	}
	return (-result);
}

int			ft_atoi(const char *str)
{
	int				i;
	unsigned char	*s;

	s = (unsigned char*)str;
	i = 0;
	while (((s[i] != '+' && s[i] != '-' && s[i])
				&& s[i] < '0' && s[i] > '9' && s[i] != '\a')
			|| (s[i] <= ' ' && s[i] != '\a' && s[i]))
	{
		i++;
	}
	if ((((s[i] == '+' || s[i] <= ' ') && (s[i + 1] >= '0' &&
					s[i + 1] <= '9') && s[i])
			|| (s[i] >= '0' && s[i] <= '9' && s[i])))
		return (ft_positive(str, i));
	else if (s[i] == '-' && s[i + 1] >= '0' && s[i + 1] <= '9')
		return (ft_negative(str, i));
	return (0);
}
