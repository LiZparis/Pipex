/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 16:52:21 by lzhang2           #+#    #+#             */
/*   Updated: 2024/05/31 13:55:19 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_node;
	t_list	*new_lst;
	void	*new_content;

	new_lst = NULL;
	while (lst)
	{
		new_content = (*f)(lst->content);
		if (!new_content)
		{
			ft_lstclear(&new_lst, del);
			return (NULL);
		}
		new_node = ft_lstnew(new_content);
		if (!new_node)
		{
			del(new_content);
			ft_lstclear(&new_lst, del);
			return (NULL);
		}
		ft_lstadd_back(&new_lst, new_node);
		lst = lst->next;
	}
	return (new_lst);
}
