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

#include "minishell.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	unsigned int	s_len;
	char			*sub;

	i = 0;
	s_len = ft_strlen(s);
	if (s == NULL)
		return (NULL);
	if (start > s_len)
		return (ft_strdup(""));
	if (s_len <= len)
		len = (s_len - start);
	sub = malloc(sizeof(char) * (len + 1));
	if (sub == NULL)
		return (NULL);
	while (i < len && s[i] != '\0' )
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

t_list	*ft_lstnew(void *content)
{
	t_list	*node;

	node = malloc(sizeof(t_list));
	if (node == 0)
		return (0);
	node->content = content;
	node->next = 0;
	return (node);
}

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*prev;

	if (lst == 0 || new == 0)
		return ;
	if (*lst == 0)
		*lst = new;
	else
	{
		prev = ft_lstlast(*lst);
		prev->next = new;
	}
}