/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 12:57:50 by lzhang2           #+#    #+#             */
/*   Updated: 2024/05/25 13:42:45 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*p;
	size_t	len_s;
	size_t	i;

	if (!s)
		return (NULL);
	len_s = ft_strlen(s);
	p = (char *)malloc((len_s + 1) * sizeof(char));
	if (!p)
		return (NULL);
	i = 0;
	while (i < len_s)
	{
		p[i] = f(i, s[i]);
		i++;
	}
	p[i] = '\0';
	return (p);
}
