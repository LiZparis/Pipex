/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strappend.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:13:02 by lzhang2           #+#    #+#             */
/*   Updated: 2024/10/08 14:17:03 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// char	*ft_strappend(char *s1, const char *s2)
// {
// 	char	*new_str;

// 	new_str = ft_strjoin(s1, s2);
// 	if (*s1)
// 		free(s1);
// 	s1 = new_str;
// 	return (new_str);
// }
char	*ft_strappend(char **s1, const char *s2)
{
	char	*new_str;

	new_str = ft_strjoin(*s1, s2);
	if (*s1)
		free(*s1);
	*s1 = new_str;
	return (new_str);
}
/*if (*s1)
		free(*s1);确保在每次新分配内存前释放旧的*/