/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_unsigned_10.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 18:11:14 by lzhang2           #+#    #+#             */
/*   Updated: 2024/09/16 13:00:14 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	print_unsigned_10(unsigned int nb)
{
	size_t				len;

	len = 0;
	if (nb >= 10)
		len += print_unsigned_10(nb / 10);
	len += print_char((nb % 10) + '0');
	return (len);
}
