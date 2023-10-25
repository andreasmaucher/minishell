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

/* extracts the path starting AFTER = */
char    *extract_value_from_env(char *path)
{
    int i;
    char    *tmp;
    int path_len;
    int j;

    i = 0;
    while (path[i] != '=')
        i++;
    i++;
    path_len = ft_strlen(path) - i;
    tmp = malloc(sizeof(char) * path_len + 1);
    if (!tmp)
        return (NULL);
    j = 0;
    while (path[i] != '\0')
    {
        tmp[j] = path[i];
        j++;
        i++;
    }
    tmp[j] = '\0';
    printf("PATH AFTER: %s\n", tmp);
    return (tmp);
}

/*
searches for the HOME directory path in the env list;
return value is the path after '=';
*/
char    *get_path(t_minishell *m, char *search_path)
{
    t_list *tmp;
    t_dict  *dict;
    char    *return_path;
    char    *return_value;

    tmp = m->envp;
    return_value = NULL;
    dict = (t_dict *)tmp->value;
    return_path = NULL;
    while (tmp != NULL)
    {
        dict = (t_dict *)tmp->value;
        if (ft_strcmp(dict->key, search_path) == 0)
        {
            return_path = ft_strdup(dict->value);
            break;
        }
        tmp = tmp->next;
    }
    return_value = extract_value_from_env(return_path);
    //free(return_path);
    return(return_value);
}

void    delete_specific_envs(t_minishell *m, t_command *cmd, char *search_key)
{
    int i;
    t_list *tmp;

    i = 0;
    tmp = m->envp;
    while(cmd->args[i] != NULL)
    {
        tmp = m->envp;
        if (check_if_part_of_library(m->envp, search_key) == true)
        {
            while (tmp != NULL)
	        {
                if (check_for_key_doubles(m, search_key, tmp) == true)
                    break;
                tmp = tmp->next;
	        }
        }
        i++;
    }
}

void add_specific_envs(t_minishell *m, char *path_incl_key)
{
    t_list *new;
    t_dict *dict;

    new = NULL;
    dict = NULL;
    new = create_new_node(NULL);
    dict = malloc(sizeof(t_dict));
    dict->value = ft_strdup(path_incl_key);
    dict->key = extract_key_from_envp(path_incl_key);
    new->value = dict;
    insert_at_tail(m->envp, new);
}

/*
first current working directory is saved in in old_dir;
chdir function is used to change the current working directory of a process to new_path;
chdir returns -1 if there was an error and 0 on success;
*/
int update_paths(char *new_path, t_minishell *m, t_command *cmd)
{
    char    cwd[PATH_MAX];
    char    *old_dir;
    char    *new_path_returned_from_cwd;

    (void)m;
    (void)cmd;
    getcwd(cwd, PATH_MAX);
    old_dir = ft_strdup(cwd);
    printf("OLD DIR: %s\n", old_dir);
    printf("NEW PATH TO SEND INTO CHDIR: %s\n", new_path);
    if (chdir(new_path))
    {
        free(old_dir);
        free(new_path);
        printf("No such file or directory!\n");
        return(1);
    }
    getcwd(cwd, PATH_MAX);
    new_path_returned_from_cwd = ft_strdup(cwd);
    printf("NEW PATH RETURNED FROM CWD: %s\n", new_path_returned_from_cwd);
    //! PART BELOW STILL NEEDS TESTING WHEN MORE FUNCTIONALITY
    // this is supposed to change PWD & OLDPWD --- NOT HOME!
    delete_specific_envs(m, cmd, "PWD");
    add_specific_envs(m, new_path_returned_from_cwd);
    delete_specific_envs(m, cmd, "OLDPWD");
    add_specific_envs(m, old_dir);
    new_path = set_pt_to_null(new_path);
    return(0);
}

/*
function that replicates the behavior of the cd command;
Example behavior:
cd : back to home
cd - : back to the last folder we've been in
cd ~ : back to home
cd -- : back to home
For multiple arguments it checks first if all folders are valid and if so,
enters the last one. If one is invalid it returns:
"cd: N: No such file or directory"
*/
int cd(t_minishell *m, t_command *cmd)
{
    char *path;

    (void)m;
    path = NULL;
    if ((arg_count(cmd->args) > 2))
    {
        printf("Too many arguments\n");
        return (1);
    }
    else if ((arg_count(cmd->args) == 2) && ft_strcmp(cmd->args[1], "-") == 0)
    {
        path = get_path(m, "OLDPWD");
        if (!path)
        {
            printf(("Error OLDPW not set!\n"));
            return (1);
        }
    }
    else if ((arg_count(cmd->args)) == 1 || ft_strcmp(cmd->args[1], "--") == 0)
    {
        path = get_path(m, "HOME");
        printf("HOME PATH %s \n", path);
        if (!path)
        {
            printf(("Error HOME not set!\n"));
            return (1);
        }
    }
 /*    else
        path = standard_path(m, cmd); */
    update_paths(path, m, cmd);
    return(0);
}