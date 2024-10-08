/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:42:02 by lzhang2           #+#    #+#             */
/*   Updated: 2024/05/28 16:13:46 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	len_little;

	if (*little == 0)
		return ((char *)big);
	len_little = ft_strlen(little);
	while (*big && len_little <= len)
	{
		if (ft_strncmp(big, little, len_little) == 0)
			return ((char *)big);
		++big;
		--len;
	}
	return (NULL);
}
