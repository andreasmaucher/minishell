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

/* used for exiting the program and freeing the memory correctly */
void	*set_pt_to_null(void *ptr)
{
	if (ptr)
		free(ptr);
	return (NULL);
}

/* helper function for deleting a token and freeing its memory */
void token_del(void *content) 
{
    if (content == NULL) {
        return;
    }

    t_token *token = (t_token *)content;
    if (token->str != NULL) {
        free(token->str); // Assuming the str field is dynamically allocated.
    }
    free(token); // Free the token structure itself.
}

/* deleting a node in the env list */
void env_del(void *content) 
{
    if (content == NULL) {
        return;
    }
    t_dict *dict = (t_dict *)content;
    if (dict->key != NULL) {
        free(dict->key); // Assuming the str field is dynamically allocated.
    }
    if (dict->value != NULL) {
        free(dict->value); // Assuming the str field is dynamically allocated.
    }
    free(dict); // Free the token structure itself.
}

/*
free args element of command list
*/
void	free_args(char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
	{
		//free (args[i]);
		//printf("FREE\n");
		i++;
	}
	free(args);
}

void	command_del(void *arg)
{
	t_command	*command;

	command = (t_command *)arg;
	if (command->args)
		free_args(command->args);
	/* ft_lstclear(&command->arguments, free);
	ft_lstclear(&command->outred_file, file_del);
	ft_lstclear(&command->inred_file, file_del); */
	command = set_pt_to_null(command);
}

/* delete a node in the env list */
void	del_envp(void *arg)
{
	t_dict	*dict;

	if (!arg)
		return ;
	dict = (t_dict *)arg;
	if (!dict)
		return ;
	dict->value = set_pt_to_null(dict->value);
    dict->key = set_pt_to_null(dict->key);
	dict = set_pt_to_null(dict);
}

void	free_env(char **env)
{
	int i;

	i = 0;
	while (env[i] != NULL)
	{
		free(env[i]);
		i++;
	}
	free(env);
}

//! DO WE NEED TO REPLICATE BASH EXIT CODES?
/* to free up all memory in the end, including memory automatically allocated by readline function */
int	exit_shell(t_minishell m)
{
	if (m.line)
		m.line = set_pt_to_null(m.line);
	if (m.tlist)
		ft_lstclear(&m.tlist, token_del);
	if (m.clist)
		ft_lstclear(&m.clist, command_del);
	if (m.env_lib)
		free_env(m.env_lib);
	if (m.envp_lib)
		free_env(m.envp_lib);
	exit(1);
}