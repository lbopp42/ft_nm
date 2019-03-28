/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_32.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 13:59:00 by lbopp             #+#    #+#             */
/*   Updated: 2019/03/27 14:00:53 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	browse_load_commands(void* ptr, struct load_command *lc, int ncmds, int mode)
{
	struct symtab_command	*symtab;
	int						i;

	i = 0;
	printf("ncmds: %d\n",ncmds);
	while (i < ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			symtab = (struct symtab_command*)lc;
			if (mode == 1)
				browse_symtab(symtab->symoff, symtab->nsyms, symtab->stroff, (void*)ptr);
			else
			{
				printf("ON PASSE BIEN LA\n");
				browse_symtab_64(symtab->symoff, symtab->nsyms, symtab->stroff, (void*)ptr);
			}
			break;
		}
		lc = (void*)lc + lc->cmdsize;
		i++;
	}
}

void	handle(void *ptr)
{
	struct load_command		*lc;

	lc = (void*)ptr + sizeof(struct mach_header);
	browse_load_commands(ptr, lc, get_ncmds(ptr), 1);
}
