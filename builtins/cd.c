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

/* extracts the path starting AFTER = */
char	*extract_value_from_env(char *path)
{
	int		i;
	char	*tmp;
	int		path_len;
	int		j;

	i = 0;
	while (path[i] != '=' && path[i] != '\0')
		i++;
	i++;
	path_len = ft_strlen(path) - i;
	tmp = malloc(sizeof(char) * (path_len + 1));
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
	return (tmp);
}

/*
searches for the HOME directory path in the env list;
return value is the path after '=';
*/
char	*get_path(t_minishell *m, char *search_path)
{
	t_list	*tmp;
	t_dict	*dict;
	char	*return_path;
	char	*return_value;

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
			break ;
		}
		tmp = tmp->next;
	}
	return_value = extract_value_from_env(return_path);
	free(return_path);
	return (return_value);
}

/*
In case of input "cd directory_name" this function creates the path that 
needs to be set as the new PDW;
'cd ..' -> go one directory level up;
'cd .' -> the dot represents the current directory, so essentially nothing
happens;
'cd /' -> change current directory to the root directory;
*/
static char	*standard_path(t_minishell *m, t_command *cmd)
{
	char	*current_dir;
	char	*path_with_slash;
	char	*final_path;

	if (ft_strcmp(cmd->args[1], ".") == 0 || ft_strcmp(cmd->args[1], "..") == 0
		|| ft_strcmp(cmd->args[1], "/") == 0)
		final_path = ft_strdup(cmd->args[1]);
	else
	{
		current_dir = get_path(m, "PWD");
		path_with_slash = ft_strjoin(current_dir, "/");
		final_path = ft_strjoin(path_with_slash, cmd->args[1]);
		free(current_dir);
		free(path_with_slash);
	}
	return (final_path);
}

/*
this function updates the PWD and OLDPWD variables;
first current working directory is saved in in old_dir;
chdir function is used to change the current working directory
of a process to new_path;
chdir returns -1 if there was an error and 0 on success;
*/
int	update_paths(char *new_path, t_minishell *m)
{
	char	cwd[PATH_MAX];
	char	*old_dir;
	char	*new_path_returned_from_cwd;

	getcwd(cwd, PATH_MAX);
	old_dir = ft_strdup(cwd);
	if (chdir(new_path))
	{
		free(old_dir);
		free(new_path);
		printf("No such file or directory!\n");
		return (1);
	}
	getcwd(cwd, PATH_MAX);
	new_path_returned_from_cwd = ft_strdup(cwd);
	printf("PWD: %s\n", new_path_returned_from_cwd);
	delete_node(m, "PWD");
	delete_node(m, "OLDPWD");
	add_specific_envs(m, new_path_returned_from_cwd, "PWD");
	add_specific_envs(m, old_dir, "OLDPWD");
	new_path = set_pt_to_null(new_path);
	free(old_dir);
	free(new_path_returned_from_cwd);
	return (0);
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
int	cd(t_minishell *m, t_command *cmd)
{
	char	*path;

	path = NULL;
	if ((arg_count(cmd->args) > 2))
	{
		printf("Too many arguments\n");
		return (1);
	}
	else if (arg_count(cmd->args) == 1 && ft_strcmp(cmd->args[0], "cd") == 0)
		path = go_back_to_home(m, path);
	else if (arg_count(cmd->args) == 2)
	{
		if (ft_strcmp(cmd->args[1], "-") == 0)
			path = go_back_to_last_directory(m, cmd->args[1]);
		else if (ft_strcmp(cmd->args[1], "--") == 0
			|| ft_strcmp(cmd->args[1], "~") == 0)
			path = go_back_to_home(m, path);
		else
		{
			path = standard_path(m, cmd);
			printf("path after standard path %s\n", path);
		}
	}
	update_paths(path, m);
	return (0);
}
