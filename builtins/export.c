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

/*
function that mimics the behavior of the 'export' command;
export is used to set or export environment variables;
once an environment variable is exported it can be accessed by other programs
within the same session or shell;
environment variables that were set using export are available to the current
shell session and andy child processes spawned from it;
by closing the terminal or shell session these variables are discarded;

Usage examples:
export MY_VARIABLE=new -> creates a new var and sets its value to 'new'
export USER=new_user -> this will change the already existing env 'USER' and 
                        set it to 'new_user'
export MY_VARIABLE= -> will be added, even though not path specified
export KEY8=m KEYFAI KEY7=true -> 7+8 will be added, even though the middle var
                                    is in the wrong format and will not be added
export KEY12=m KEYFAI. KEY11=true -> no variables added, because of invalid
                                    input for the middle var
export LOVE ME= -> will only add ME= but show no error

Function below only adds variables if all variables have the format MY_VAR=
*/

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

/*
first letter needs to be _ or a character;
in the key only characters, digits & _ is allowed;
*/
bool    check_valid_export_input(char **args)
{
    int i;
    int j;

    i = 1;
    j = 0;
    while (args[i] != NULL)
    {   
        j= 0;
        while (args[i][j] != '\0' && args[i][j] != '=')
        {
            printf("ARGS IJ: %c\n", args[i][j]);
            if (args[i][0] != '_' && ft_isalpha(args[i][0]) == false)
            {
                printf("export '%s' : not a valid identifier\n", args[i]);
                return(false);
            }
            if (args[i][j] == '=')
                break;
            if (ft_isalpha(args[i][j]) == false && ft_is_digit(args[i][j]) == false && args[i][j] != '_')
            {
                printf("export '%s' : not a valid identifier\n", args[i]);
                return(false);
            }
            j++;
        }
        i++;
    }
    return(true);
}

int calculate_old_env_lib_size(t_minishell m)
{
    int i;

    i = 0;
    while (m.envp_lib[i] != NULL)
        i++;
    return(i - 1);
}

/*
function extracts the part until '=' to later compare it with m.env_lib
*/
char *extract_search_str(t_command *cmd, int i)
{
    int search_len;
    char *search_str;
    int j;

    j = 0;
    search_len = 0;
    while (cmd->args[i][j] != '\0' && cmd->args[i][j] != '=')
    {
        search_len++;
        j++;
    }
    search_str = malloc(sizeof(char) * (search_len + 1));
    if (search_str == NULL)
        return (0);
    j = 0;
    while (cmd->args[i][j] != '\0' && cmd->args[i][j] != '=')
    {
        search_str[j] = cmd->args[i][j];
        j++;
    }
    search_str[j] = '\0';
    return (search_str);
}

bool    check_equal_sign(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == '=')
            return (true);
        i++;
    }
    return (false);
}

void add_new_envs(t_minishell *m, t_command *cmd)
{
    int i;
    t_list *new;
    t_dict *dict;

    i = 0;
    new = NULL;
    dict = NULL;
    while (cmd->args[i])
    {
        if (check_equal_sign(cmd->args[i]) == true)
        {
            new = create_new_node(NULL);
            dict = malloc(sizeof(t_dict));
            dict->value = ft_strdup(cmd->args[i]);
            dict->key = extract_key_from_envp(cmd->args[i]);
            new->value = dict;
            insert_at_tail(m->envp, new);
        }
        i++;
    }
}

/* 
checks if the current key matches with the key in the user input;
if true the corresponding node is deleted and free 
*/
bool    check_for_key_doubles(t_minishell *m, char *search_str, t_list *tmp)
{
    t_dict *dict;

    dict = (t_dict *)tmp->value;
    if (ft_strcmp(dict->key, search_str) == 0)
    {
        ft_lstremove(&m->envp, tmp, del_envp);
        return (true);
    }
    return (false);
}

t_list *delete_double_envs(t_minishell *m, t_command *cmd)
{
    int i;
    t_list *tmp;
    char *search_str;

    i = 0;
    tmp = m->envp;
    search_str = NULL;
    while(cmd->args[i] != NULL)
    {
        tmp = m->envp;
        //if (check_equal_sign(cmd->args[i]) == true)
        search_str = extract_key_from_envp(cmd->args[i]);
        if (check_if_part_of_library(m->envp, search_str) == true)
        {
            while (tmp != NULL)
	        {
                if (check_for_key_doubles(m, search_str, tmp) == true)
                    break;
                tmp = tmp->next;
	        }
        }
        free(search_str);
        i++;
    }
    return (tmp);
}

int export(t_minishell *m, t_command *cmd)
{
    if (check_valid_export_input(cmd->args) == true)
    {
        delete_double_envs(m, cmd);
        add_new_envs(m, cmd);
        printlist_envp(m->envp);
        return (0);
    }
    return (1);
}