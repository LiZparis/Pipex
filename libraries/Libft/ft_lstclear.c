/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 09:37:57 by lzhang2           #+#    #+#             */
/*   Updated: 2024/05/27 11:46:02 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*current;
	t_list	*next_node;

	current = *lst;
	while (current)
	{
		next_node = current -> next;
		(*del)(current -> content);
		free (current);
		current = next_node;
	}
	*lst = NULL;
}
