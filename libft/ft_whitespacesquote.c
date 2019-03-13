/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_whitespacesquote.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <lbopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/07 11:15:36 by lbopp             #+#    #+#             */
/*   Updated: 2017/02/07 12:31:50 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		ft_word(char *s, int i, int word)
{
	int		tmp;
	char	quote;

	while (s[i] != '\0')
	{
		if (!ft_isspace(s[i]) && s[i] != '\0')
			word++;
		if (s[i++] == 34 || s[i++] == 39)
		{
			quote = s[i - 1];
			tmp = i;
			while (s[i] != quote && s[i])
				i++;
			if (s[i] == '\0')
			{
				word--;
				i = tmp;
			}
		}
		else
			while (!ft_isspace(s[i]) && s[i] != '\0' && s[i])
				i++;
		s[i] != '\0' ? i++ : 0;
	}
	return (word);
}

void			ft_fill(char const *s, char ***array)
{
	int		i;
	int		letter;
	int		word;
	char	quote;

	i = 0;
	word = 0;
	while (s[i])
	{
		letter = 0;
		if (s[i] == 34 || s[i] == 39)
		{
			quote = s[i++];
			while (s[i] && s[i] != quote)
				(*array)[word][letter++] = s[i++];
			if (s[i] == quote)
				i++;
		}
		else
			while (!ft_isspace(s[i]) && s[i])
				(*array)[word][letter++] = s[i++];
		word++;
		while (ft_isspace(s[i]) && s[i])
			i++;
	}
}

void			ft_letter(char const *s, char ***array, int i, int word)
{
	int		letter;
	char	quote;

	while (s[i])
	{
		letter = 0;
		if (s[i] == 34 || s[i] == 39)
		{
			quote = s[i++];
			while (s[i++] != quote && s[i])
				letter++;
			(s[i] == quote) ? i++ : 0;
		}
		else
			while (!ft_isspace(s[i]) && s[i])
			{
				letter++;
				i++;
			}
		while (ft_isspace(s[i]) && s[i])
			i++;
		if (!((*array)[word++] =
			(char*)ft_memalloc(sizeof(char) * (letter + 1))))
			return ;
	}
}

char			**ft_whitespacesquote(char const *s)
{
	int		word;
	char	**array;
	char	*string;

	if (!(s))
		return (NULL);
	if (!(string = ft_strtrim(s)))
		return (0);
	word = ft_word(string, 0, 0);
	if (!(array = (char**)ft_memalloc(sizeof(char*) * (word + 1))))
		return (0);
	ft_letter(string, &array, 0, 0);
	ft_fill(string, &array);
	free(string);
	return (array);
}
