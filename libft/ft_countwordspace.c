/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_countwordspace.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <lbopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/07 11:08:13 by lbopp             #+#    #+#             */
/*   Updated: 2017/02/07 11:14:33 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	verif_quote(char *s, int i, char par)
{
	while (s[i])
	{
		if (s[i] == par)
			return (i);
		i++;
	}
	return (-1);
}

int			ft_countwordspace(char *s)
{
	int i;
	int word;

	i = 0;
	word = 0;
	while (s[i])
	{
		if (!ft_isspace(s[i]))
			word++;
		while (s[i] && !ft_isspace(s[i]))
		{
			if ((s[i] == 34 || s[i] == 39) && verif_quote(s, i + 1, s[i]) != -1)
				i = verif_quote(s, i + 1, s[i]) + 1;
			s[i] ? i++ : 0;
		}
		while (s[i] && ft_isspace(s[i]))
			i++;
	}
	return (word);
}
