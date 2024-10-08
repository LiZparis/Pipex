/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 10:41:46 by lzhang2           #+#    #+#             */
/*   Updated: 2024/05/28 13:04:50 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *ptr, int c, size_t len)
{
	unsigned char	*p1;

	p1 = (unsigned char *)ptr;
	while (len--)
		*p1++ = (unsigned char)c;
	return (ptr);
}
