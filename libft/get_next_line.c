/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/28 08:47:47 by lbopp             #+#    #+#             */
/*   Updated: 2017/01/21 21:14:26 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft.h"

int		get_next_line(const int fd, char **line)
{
	static char	*content;
	int			ret;
	char		buf[BUFF_SIZE + 1];
	char		*newline;

	if (!line || fd < 0)
		return (-1);
	!content ? (content = ft_strnew(0)) : 0;
	while ((ret = read(fd, buf, BUFF_SIZE)) > 0)
	{
		buf[ret] = '\0';
		content = ft_stradd(content, buf);
		if (ft_strchr(buf, '\n'))
			break ;
	}
	if (ret == -1 || !(*line = ft_strcdup(content, '\n')))
		return (-1);
	if (ft_strlen(content) == 0)
		return (0);
	newline = ft_strchr(content, '\n');
	content = ft_memmove(content, newline ?
			newline + 1 : content + ft_strlen(content),
			newline ? ft_strlen(newline) : ft_strlen(content));
	return (1);
}
