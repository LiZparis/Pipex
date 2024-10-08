/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 09:43:50 by lzhang2           #+#    #+#             */
/*   Updated: 2024/09/26 15:13:42 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*dup;
	size_t	len_s;
	size_t	i;

	len_s = ft_strlen(s);
	dup = (char *)malloc((len_s + 1) * sizeof(char));
	if (dup == NULL)
		return (NULL);
	i = 0;
	while (i < len_s)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}
