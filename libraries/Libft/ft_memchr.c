/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 11:17:16 by lzhang2           #+#    #+#             */
/*   Updated: 2024/05/28 13:03:41 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *ptr, int c, size_t n)
{
	unsigned char	*p1;

	p1 = (unsigned char *)ptr;
	while (n)
	{
		if (*p1 == (unsigned char)c)
			return (p1);
		n--;
		p1++;
	}
	return (NULL);
}
