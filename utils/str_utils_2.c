/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 14:04:04 by amaucher          #+#    #+#             */
/*   Updated: 2023/08/23 14:04:07 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* find the first occurence of a substring 'needle' wihtin a given string*/
char	*ft_strstr(const char *haystack, const char *needle)
{
	char	*tmpstack;
	size_t	needle_len;

	tmpstack = (char *)haystack;
	needle_len = ft_strlen(needle);
	if (needle_len == 0)
	{
		return (tmpstack);
	}
	while (*tmpstack)
	{
		if (*tmpstack == *needle)
		{
			if (ft_strncmp(needle, tmpstack, needle_len) == 0)
			{
				return (tmpstack);
			}
		}
		tmpstack++;
	}
	return (NULL);
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	a;
	size_t	b;

	a = 0;
	b = 0;
	if (little[a] == '\0')
		return ((char *) big);
	while (big[a] != '\0' && a < len)
	{
		while (big[a + b] == little[b] && big[a + b] != '\0' && a + b < len)
		{
			b++;
		}
		if (little[b] == '\0')
			return ((char *) big + a);
		a++;
		b = 0;
	}
	return (0);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while ((s1[i] != '\0' || s2[i] != '\0') && i < n)
	{
		if (s1[i] != s2[i])
			return ((unsigned char) s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

bool	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (true);
	return (false);
}

bool	ft_is_digit(int c)
{
	if (c >= '0' && c <= '9')
		return (true);
	return (false);
}
