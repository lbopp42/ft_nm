/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrimchar.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <lbopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/11 11:02:32 by lbopp             #+#    #+#             */
/*   Updated: 2017/02/03 11:18:03 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		ft_trimsize(char const *s, int start, char c)
{
	int i;

	i = start;
	while (s[i])
		i++;
	while (s[i - 1] == c && (i > 0))
	{
		i--;
	}
	if ((i - start) < 0)
		return (0);
	else
		return (i - start);
}

static char		*ft_trimnews(char const *s, int start, int len, char *news)
{
	int i;

	i = 0;
	while (start < len)
	{
		news[i] = s[start];
		start++;
		i++;
	}
	return (news);
}

char			*ft_strtrimchar(char *s, char c)
{
	char	*news;
	int		i;
	int		start;
	int		j;
	int		size;

	i = 0;
	j = 0;
	start = 0;
	size = 0;
	if (!s)
		return (0);
	while (s[i] && s[i] == c)
		i++;
	start = i;
	size = ft_trimsize(s, start, c);
	if (!(news = (char*)ft_memalloc(size + 1)))
		return (0);
	news = ft_trimnews(s, start, size + start, news);
	return (news);
}
