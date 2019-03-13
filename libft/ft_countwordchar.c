/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_countwordchar.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <lbopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/03 09:40:30 by lbopp             #+#    #+#             */
/*   Updated: 2017/02/03 16:50:58 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	verif_quote(char *s, int i, char par)
{
	while (s[i])
	{
		if (s[i] == par)
			return (i);
		i++;
	}
	return (-1);
}

int	ft_countwordchar(char *s, char c)
{
	int i;
	int word;

	i = 0;
	word = 0;
	while (s[i])
	{
		if (s[i] != c)
			word++;
		while (s[i] && s[i] != c)
		{
			if ((s[i] == 34 || s[i] == 39) && verif_quote(s, i + 1, s[i]) != -1)
				i = verif_quote(s, i + 1, s[i]) + 1;
			s[i] ? i++ : 0;
		}
		while (s[i] && s[i] == c)
			i++;
	}
	return (word);
}
