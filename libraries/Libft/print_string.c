/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_string.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 16:54:53 by lzhang2           #+#    #+#             */
/*   Updated: 2024/09/16 13:00:09 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	print_string(char *str)
{
	size_t	len;

	len = 0;
	if (!str)
		str = "(null)";
	while (*str)
	{
		len += print_char(*str);
		str++;
	}
	return (len);
}
