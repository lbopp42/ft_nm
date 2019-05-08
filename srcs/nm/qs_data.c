/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   qs_data.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 10:03:26 by lbopp             #+#    #+#             */
/*   Updated: 2019/05/08 10:03:28 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	print_tab(int tab[], int size)
{
	int i;

	i = 0;
	while (i < size)
	{
		printf("%d ", tab[i]);
		i++;
	}
	printf("\n");
}

int		partitionnement(t_data **data, int premier, int dernier)
{
	int		j;
	int		i;
	t_data	tmp;

	j = premier;
	i = premier;
	while (i <= dernier - 1)
	{
		if (ft_strcmp((*data)[i].name, (*data)[dernier].name) < 0 ||
			(!ft_strcmp((*data)[i].name, (*data)[dernier].name) &&
			(*data)[i].n_value.n_value64 < (*data)[dernier].n_value.n_value64))
		{
			tmp = (*data)[i];
			(*data)[i] = (*data)[j];
			(*data)[j] = tmp;
			j++;
		}
		i++;
	}
	tmp = (*data)[dernier];
	(*data)[dernier] = (*data)[j];
	(*data)[j] = tmp;
	return (j);
}

t_data	*qs_data(t_data *data, int premier, int dernier)
{
	int pivot;

	if (premier < dernier)
	{
		pivot = partitionnement(&data, premier, dernier);
		data = qs_data(data, premier, pivot - 1);
		data = qs_data(data, pivot + 1, dernier);
	}
	return (data);
}
