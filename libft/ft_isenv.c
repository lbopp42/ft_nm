/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isenv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <lbopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/29 12:51:27 by lbopp             #+#    #+#             */
/*   Updated: 2017/02/05 16:00:48 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isenv(t_lst *env_lst, char *var)
{
	while (env_lst != NULL)
	{
		if (!ft_strcmp(env_lst->name, var))
			return (1);
		env_lst = env_lst->next;
	}
	return (0);
}
