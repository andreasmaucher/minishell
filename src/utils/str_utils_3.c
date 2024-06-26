/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 14:04:04 by amaucher          #+#    #+#             */
/*   Updated: 2024/06/04 23:20:22 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t destsize)
{
	size_t	src_len;
	size_t	i;

	src_len = ft_strlen(src);
	if (destsize)
	{
		i = 0;
		while (src[i] != '\0' && i < (destsize - 1))
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
	}
	return (src_len);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char		*d;
	const char	*s;

	if (!(dest || src))
		return (NULL);
	d = dest;
	s = src;
	while (n-- > 0)
		*d++ = *s++;
	return (dest);
}

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	len_d;
	size_t	len_s;

	len_d = ft_strlen(dst);
	len_s = ft_strlen(src);
	if (!size || len_d >= size)
		return (size + len_s);
	if (len_s < size - len_d)
		ft_memcpy(dst + len_d, src, len_s + 1);
	else
	{
		ft_memcpy(dst + len_d, src, size - len_d - 1);
		dst[size - 1] = '\0';
	}
	return (len_d + len_s);
}

char	*join_strings(const char *str1, const char *str2, const char *str3)
{
	size_t	len1;
	size_t	len2;
	size_t	len3;
	char	*result;

	if (str3 == NULL)
		return (NULL);
	len1 = ft_strlen(str1);
	len2 = ft_strlen(str2);
	len3 = ft_strlen(str3);
	result = malloc((len1 + len2 + len3 + 1) * sizeof(char));
	if (result == NULL)
		return (NULL);
	ft_strlcpy(result, str1, len1 + len2 + len3 + 1);
	ft_strlcat(result, str2, len1 + len2 + len3 + 1);
	ft_strlcat(result, str3, len1 + len2 + len3 + 1);
	return (result);
}
