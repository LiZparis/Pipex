/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 12:23:47 by lzhang2           #+#    #+#             */
/*   Updated: 2024/05/28 15:58:13 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;

	if (!dest && !src)
		return (NULL);
	if (dest < src)
		return (ft_memcpy(dest, src, n));
	d = (unsigned char *)dest;
	s = (const unsigned char *)src;
	while (n--)
		d[n] = s[n];
	return (dest);
}
