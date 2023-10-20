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

# include "../minishell.h"

char **create_env_library(char **envp)
{
    char **buf = NULL;
    char target = '=';
    int len = 0;
    int substr_len;

    while (envp[len] != NULL)
        len++;
    buf = malloc(sizeof(char *) * (len + 1));
    if (!buf)
        return (NULL);
    len = 0; //! Don't get that logic fully 
    while (envp[len] != NULL)
    {
        char *target_pos = strchr(envp[len], target); //! LIBFT
        if (target_pos != NULL)
        {
            substr_len = target_pos - envp[len];
            buf[len] = malloc(substr_len + 1);
            if (!buf[len])
                return (NULL);
            strncpy(buf[len], envp[len], substr_len); //! LIBFT
            buf[len][substr_len] = '\0';
            //printf("SUBSTR: %s\n", buf[len]);
        }
        else
        {
            buf[len] = NULL;
        }
        len++;
    }
    buf[len] = NULL;
    return (buf);
}

char **create_envp_library(char **envp)
{
    char **buf = NULL;
    char target = '\0';
    int len = 0;
    int substr_len;

    while (envp[len] != NULL)
        len++;
    buf = malloc(sizeof(char *) * (len + 1));
    if (!buf)
        return (NULL);
    len = 0;
    while (envp[len] != NULL)
    {
        char *target_pos = strchr(envp[len], target); //! LIBFT
        if (target_pos != NULL)
        {
            substr_len = target_pos - envp[len];
            buf[len] = malloc(substr_len + 1);
            if (!buf[len])
                return (NULL);
            strncpy(buf[len], envp[len], substr_len); //! LIBFT
            buf[len][substr_len] = '\0';
            //printf("SUBSTR: %s\n", buf[len]);
        }
        //! not needed?!?!??!?!?!?
        /* else
        {
            buf[len] = NULL;
        } */
        len++;
    }
    buf[len] = NULL;
    return (buf);
}

char    *extract_key_from_envp(char *envp)
{
    int i;
    char    *tmp;

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

t_dict *add_key_and_value(char *envp)
{
	t_dict *lib;
    char    *key_str;

    lib = malloc(sizeof(t_dict));
	if (!lib)
		return(NULL);
    key_str = extract_key_from_envp(envp);
	lib->key = key_str;
    lib->value = ft_strdup(envp);
    //free(key_str); //! NEEDED?!
	return(lib);
}

/* adds a new node to a list; in case the list is empty, the new node becomes the head, else 
it is added at the end of the list */
t_list *add_token_to_envp(t_list **token_list, char *envp)
{
	t_list *new_node;
	t_dict *lib;

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


//only for testing purposes, prints a list
void	printlist_envp(t_list *head)
{
	t_list	*temporary;
	t_dict *token;
	int i;

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

t_list *create_envp_list(char **envp)
{
    int i;
    t_list *list;
    
    list = NULL;
    i = 0;
    while (envp[i] != NULL)
    {
        add_token_to_envp(&list, envp[i]);
        i++;
    }
    return (list);
}