/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 16:19:26 by lzhang2           #+#    #+#             */
/*   Updated: 2024/09/16 13:00:27 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	check_format(const char *format, va_list args)
{
	size_t	count;

	count = 0;
	if (*format == 'c')
		count += print_char(va_arg(args, int));
	else if (*format == 's')
		count += print_string(va_arg(args, char *));
	else if (*format == 'd' || *format == 'i')
		count += print_decimal_10(va_arg(args, int));
	else if (*format == 'u')
		count += print_unsigned_10(va_arg(args, unsigned int));
	else if (*format == 'x')
		count += print_hexadecimal_lower(va_arg(args, unsigned int));
	else if (*format == 'X')
		count += print_hexadecimal_upper(va_arg(args, unsigned int));
	else if (*format == 'p')
		count += print_pointer(va_arg(args, void *));
	else if (*format == '%')
		count += print_char('%');
	return (count);
}

int	ft_printf( const char *format, ...)
{
	va_list	args;
	size_t	len;

	va_start(args, format);
	len = 0;
	if (format == NULL)
		return (-1);
	while (*format)
	{
		if (*format != '%')
			len += print_char(*format++);
		else
		{
			format++;
			len += check_format(format, args);
			format++;
		}
	}
	va_end(args);
	return (len);
}
