/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 12:00:39 by lzhang2           #+#    #+#             */
/*   Updated: 2024/05/28 16:02:00 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*p1;

	if (dest == NULL && src == NULL)
		return (NULL);
	p1 = (unsigned char *)dest;
	while (n--)
		*p1++ = *(const unsigned char *)src++;
	return (dest);
}
