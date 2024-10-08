/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_hexadecimal_upper.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 18:22:50 by lzhang2           #+#    #+#             */
/*   Updated: 2024/09/16 12:59:59 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	print_hexadecimal_upper(unsigned int nb)
{
	size_t	len;
	char	*base;

	base = "0123456789ABCDEF";
	len = 0;
	if (nb >= 16)
		len += print_hexadecimal_upper(nb / 16);
	len += print_char(base[nb % 16]);
	return (len);
}
