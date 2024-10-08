/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_decimal_10.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 17:06:15 by lzhang2           #+#    #+#             */
/*   Updated: 2024/09/16 12:59:48 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	print_decimal_10(int n)
{
	size_t		len;
	long long	nb;

	nb = (long long)n;
	len = 0;
	if (nb < 0)
	{
		len += print_char('-');
		nb = -nb;
	}
	if (nb >= 10)
		len += print_decimal_10(nb / 10);
	len += print_char(nb % 10 + '0');
	return (len);
}
