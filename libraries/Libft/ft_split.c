/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 22:55:22 by lzhang2           #+#    #+#             */
/*   Updated: 2024/08/18 22:55:22 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_count_words(char const *str, char c)
{
	int	count;

	count = 0;
	while (*str)
	{
		while (*str == c)
			str++;
		if (*str && *str != c)
		{
			count++;
			while (*str && *str != c)
				str++;
		}
	}
	return (count);
}

char	*fill_word(char const **sstr, char c)
{
	char				*word;
	char const			*str;
	int					len;
	int					i;

	str = *sstr;
	while (*str == c)
		str++;
	len = 0;
	while (str[len] && str[len] != c)
		len++;
	word = malloc((len + 1) * sizeof(char));
	if (!word)
		return (NULL);
	word[len] = '\0';
	i = 0;
	while (i < len)
	{
		word[i] = str[i];
		i++;
	}
	*sstr = str + len;
	return (word);
}

char	**ft_split(char const *str, char c)
{
	int		count_words;
	char	**tab;
	int		i;

	count_words = ft_count_words(str, c);
	tab = malloc((count_words + 1) * sizeof(char *));
	if (!tab)
		return (NULL);
	tab[count_words] = NULL;
	i = 0;
	while (i < count_words)
	{
		tab[i] = fill_word(&str, c);
		if (!tab[i])
		{
			while (--i)
				free(tab[i]);
			free(tab);
			return (NULL);
		}
		i++;
	}
	return (tab);
}
