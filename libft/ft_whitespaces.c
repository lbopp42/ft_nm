/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_whitespaces.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <lbopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/07 12:23:02 by lbopp             #+#    #+#             */
/*   Updated: 2017/02/07 12:31:32 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		ft_word(char const *s)
{
	int	i;
	int	word;

	i = 0;
	word = 0;
	while (s[i])
	{
		if (s[i] && !ft_isspace(s[i]))
			word++;
		while (s[i] && !ft_isspace(s[i]))
			i++;
		s[i] ? i++ : 0;
	}
	return (word);
}

static	char	**ft_fill(char *s, char **array)
{
	int i;
	int letter;
	int word;

	i = 0;
	word = 0;
	while (s[i])
	{
		letter = 0;
		while (s[i] && !ft_isspace(s[i]))
		{
			array[word][letter] = s[i];
			letter++;
			i++;
		}
		while (s[i] && ft_isspace(s[i]))
			i++;
		word++;
	}
	free(s);
	return (array);
}

static	char	**ft_letter(char const *s, char **array)
{
	int i;
	int letter;
	int word;

	i = 0;
	word = 0;
	while (s[i])
	{
		letter = 0;
		while (s[i] && !ft_isspace(s[i]))
		{
			letter++;
			i++;
		}
		while (s[i] && ft_isspace(s[i]))
			i++;
		if (!(array[word] = (char*)ft_memalloc(letter + 1)))
			return (NULL);
		word++;
	}
	return (array);
}

char			**ft_whitespaces(char const *s)
{
	int		i;
	int		word;
	char	**array;
	char	*new;

	i = 0;
	word = 0;
	if (!(s))
		return (NULL);
	if (!(new = ft_strtrim(s)))
		return (0);
	word = ft_word(new);
	if (!(array = (char**)ft_memalloc(sizeof(char*) * (word + 1))))
		return (0);
	if (!(array = ft_letter(new, array)))
		return (0);
	return (ft_fill(new, array));
}
