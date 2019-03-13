/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/12 14:34:14 by lbopp             #+#    #+#             */
/*   Updated: 2016/11/12 16:47:44 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list		*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list	*tmp3;
	t_list	*tmp2;
	void	*tmp;

	if (!lst)
		return (0);
	tmp3 = (*f)(lst);
	if ((tmp = ft_lstnew(tmp3->content, tmp3->content_size)))
	{
		tmp2 = (t_list *)tmp;
		lst = lst->next;
		while (lst)
		{
			tmp3 = (*f)(lst);
			if (!(tmp2->next = ft_lstnew(tmp3->content, tmp3->content_size)))
				return (NULL);
			tmp2 = tmp2->next;
			lst = lst->next;
		}
	}
	return ((t_list *)tmp);
}
