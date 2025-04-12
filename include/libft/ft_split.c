/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 14:27:52 by aysadeq           #+#    #+#             */
/*   Updated: 2024/11/14 14:54:01 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(const char *s, char c)
{
	size_t	count;
	int		in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
		{
			in_word = 0;
		}
		s++;
	}
	return (count);
}

static char	*get_next_word(const char *s, char c)
{
	const char	*start;
	size_t		len;

	while (*s == c)
		s++;
	start = s;
	while (*s && *s != c)
		s++;
	len = s - start;
	return (ft_substr(start, 0, len));
}

static void	free_split(char **split, size_t i)
{
	while (i > 0)
		free(split[--i]);
	free(split);
}

char	**ft_split(const char *s, char c)
{
	char	**split;
	size_t	words;
	size_t	i;

	if (!s)
		return (NULL);
	words = count_words(s, c);
	split = (char **)malloc((words + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	i = 0;
	while (i < words)
	{
		split[i] = get_next_word(s, c);
		if (!split[i])
			return (free_split(split, i), NULL);
		while (*s == c)
			s++;
		while (*s && *s != c)
			s++;
		i++;
	}
	split[i] = NULL;
	return (split);
}
