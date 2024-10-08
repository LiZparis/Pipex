/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_pointer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 19:25:26 by lzhang2           #+#    #+#             */
/*   Updated: 2024/09/16 13:00:04 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	print_hexadecimal_p(uintptr_t nb)
{
	size_t	len;
	char	*base;

	len = 0;
	base = "0123456789abcdef";
	if (nb >= 16)
		len += print_hexadecimal_p(nb / 16);
	len += print_char(base[nb % 16]);
	return (len);
}

size_t	print_pointer(void *ptr)
{
	size_t		len;
	uintptr_t	addr;

	len = 0;
	if (ptr == NULL)
		return (print_string("(nil)"));
	else
	{
		addr = (uintptr_t)ptr;
		len += print_string("0x");
		if (addr == 0)
			len += print_char('0');
		else
			len += print_hexadecimal_p(addr);
	}
	return (len);
}
