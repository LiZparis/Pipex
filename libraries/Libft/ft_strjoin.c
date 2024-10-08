/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 10:42:32 by lzhang2           #+#    #+#             */
/*   Updated: 2024/09/21 12:44:51 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*join;
	size_t	len_s1;
	size_t	len_s2;
	size_t	len;

	if (!s1 && !s2)
		return (NULL);
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	len = len_s1 + len_s2;
	join = (char *)malloc((len + 1) * sizeof(char));
	if (join == NULL)
		return (NULL);
	ft_strlcpy(join, s1, len_s1 + 1);
	ft_strlcat(join, s2, len + 1);
	return (join);
}
