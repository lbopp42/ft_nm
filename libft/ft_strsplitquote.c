/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplitquote.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <lbopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/03 09:45:43 by lbopp             #+#    #+#             */
/*   Updated: 2017/02/07 11:01:57 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		valid_quote(char *s, int i, char par)
{
	while (s[i])
	{
		if (s[i] == par)
			return (i);
		i++;
	}
	return (-1);
}

void	ft_fillsplit(char *s, char ***array, char c)
{
	int	var[4];

	ft_bzero(var, sizeof(int) * 4);
	while (s[var[0]])
	{
		var[1] = 0;
		while (s[var[0]] != c && s[var[0]])
		{
			if ((s[var[0]] == 34 || s[var[0]] == 39) &&
				valid_quote(s, var[0] + 1, s[var[0]]) != -1)
			{
				var[3] = var[0];
				var[0]++;
				while (s[var[0]] != s[var[3]])
					(*array)[var[2]][var[1]++] = s[var[0]++];
				var[0]++;
			}
			else
				(*array)[var[2]][var[1]++] = s[var[0]++];
		}
		while (s[var[0]] == c && s[var[0]])
			var[0]++;
		if (s[var[0] - 1] == c || s[var[0]] == '\0')
			var[2]++;
	}
}

void	count_letter(char *s, int *i, int *letter)
{
	int tmp;

	if ((s[*i] == 34 || s[*i] == 39) && valid_quote(s, *i + 1, s[*i]) != -1)
	{
		tmp = *i;
		(*i)++;
		while (s[*i] != s[tmp])
		{
			(*i)++;
			(*letter)++;
		}
		(*i)++;
	}
	else
	{
		(*i)++;
		(*letter)++;
	}
}

void	ft_lettersplit(char *s, char ***array, int i, char c)
{
	int letter;
	int word;

	word = 0;
	while (s[i])
	{
		letter = 0;
		while (s[i] != c && s[i])
			count_letter(s, &i, &letter);
		while (s[i] == c && s[i])
			i++;
		if (s[i - 1] == c || s[i] == '\0')
			if (!((*array)[word++] = (char*)ft_memalloc(letter + 1)))
				return ;
	}
}

char	**ft_strsplitquote(char *s, char c)
{
	int		word;
	char	**array;
	char	*string;

	if (!(s))
		return (NULL);
	string = NULL;
	if (!(string = ft_strtrimchar(s, c)))
		return (0);
	word = ft_countwordchar(string, c);
	if (!(array = (char**)ft_memalloc(sizeof(char*) * (word + 1))))
		return (0);
	ft_lettersplit(string, &array, 0, c);
	ft_fillsplit(string, &array, c);
	free(string);
	return (array);
}
