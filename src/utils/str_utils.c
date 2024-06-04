/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 14:04:04 by amaucher          #+#    #+#             */
/*   Updated: 2024/06/04 23:20:22 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

size_t	ft_strlen(const char *str)
{
	int	counter;

	counter = 0;
	while (str[counter] != '\0')
	{
		counter++;
	}
	return (counter);
}

char	*ft_strdup(const char *s)
{
	char	*s_new;
	size_t	s_size;

	s_size = ft_strlen(s) + 1;
	s_new = (char *)malloc(sizeof(*s_new) * s_size);
	if (!s_new)
		return (NULL);
	return ((char *)ft_memcpy(s_new, s, s_size));
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*substr;
	unsigned int	i;
	unsigned int	str_len;
	unsigned int	correct_size;

	i = 0;
	str_len = ft_strlen((char *)s);
	correct_size = 1;
	if (start > str_len || len == 0)
		correct_size = 1;
	else if (len <= str_len - start)
		correct_size = len + 1;
	else if (len > str_len - start)
		correct_size = str_len - start + 1;
	substr = malloc(correct_size * sizeof(char));
	if (!substr)
		return ((void *) NULL);
	while (i != correct_size - 1)
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return ((unsigned char) s1[i] - (unsigned char)s2[i]);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char			*new_str;
	unsigned int	i;
	unsigned int	y;

	new_str = malloc (ft_strlen((char *)s1) + ft_strlen((char *)s2) + 1);
	if (!new_str)
		return ((void *) NULL);
	i = 0;
	y = 0;
	while (s1[i] != '\0')
	{
		new_str[i] = s1[i];
		i++;
	}
	while (s2[y] != '\0')
	{
		new_str[i] = s2[y];
		i++;
		y++;
	}
	new_str[i] = '\0';
	return (new_str);
}
