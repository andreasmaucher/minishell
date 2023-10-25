/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 10:13:39 by amaucher          #+#    #+#             */
/*   Updated: 2023/08/23 10:13:42 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*extract_key_from_envp(char *envp)
{
	char	*tmp;
	int		i;

	i = 0;
	while (envp[i] != '\0' && envp[i] != '=')
		i++;
	tmp = malloc(sizeof(char) * i + 1);
	if (!tmp)
		return (NULL);
	i = 0;
	while (envp[i] != '\0' && envp[i] != '=')
	{
		tmp[i] = envp[i];
		i++;
	}
	tmp[i] = '\0';
	return (tmp);
}

/*
fills the key and value components with content;
key = the part before '=';
value = the full path including the key;
*/
t_dict	*add_key_and_value(char *envp)
{
	t_dict	*lib;
	char	*key_str;

	lib = malloc(sizeof(t_dict));
	if (!lib)
		return (NULL);
	key_str = extract_key_from_envp(envp);
	lib->key = key_str;
	lib->value = ft_strdup(envp);
	return (lib);
}

/*
adds a new node to a list; in case the list is empty, 
the new node becomes the head,
else it is added at the end of the list
*/
t_list	*add_token_to_envp(t_list **token_list, char *envp)
{
	t_list	*new_node;
	t_dict	*lib;

	lib = add_key_and_value(envp);
	if (!lib)
		return (NULL);
	new_node = create_new_node(lib);
	if (!*token_list)
		*token_list = new_node;
	else
		insert_at_tail(*token_list, new_node);
	return (*token_list);
}

/*
only for testing purposes, prints a list
*/
void	printlist_envp(t_list *head)
{
	int		i;
	t_list	*temporary;
	t_dict	*token;

	i = 0;
	temporary = head;
	printf("\n--- ENVP LIST ---\n");
	while (temporary != NULL)
	{
		token = (t_dict *)temporary->value;
		printf("[%d]: %s key: %s\n", i, (char *)token->value, token->key);
		i++;
		temporary = temporary->next;
	}
}

/*
creates a list based on the envp variables
*/
t_list	*create_envp_list(char **envp)
{
	int		i;
	t_list	*list;

	list = NULL;
	i = 0;
	while (envp[i] != NULL)
	{
		add_token_to_envp(&list, envp[i]);
		i++;
	}
	return (list);
}
