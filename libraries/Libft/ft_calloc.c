/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 15:42:09 by lzhang2           #+#    #+#             */
/*   Updated: 2024/05/27 11:52:57 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t num, size_t size)
{
	size_t			i;
	unsigned char	*p;

	if (size != 0 && num > ((size_t) -1 / size))
		return (NULL);
	p = (unsigned char *)malloc(num * size);
	if (p == NULL)
		return (NULL);
	i = 0;
	while (i < num * size)
		p[i++] = 0;
	return (p);
}
