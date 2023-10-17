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

/* //! does not work because dict list is not set!!!
char	*get_env_value(t_list *dict, char *key, char **envp)
{
	///t_dict	*item;
	int	i;
	(void)dict;
	i = 0;
	if (!key)
		return (NULL);
	while (dict)
	{
		//item = (t_dict *)dict->value;
 		if (!ft_strcmp(item->key, key))
			return (item->value);
		dict = dict->next;
	} 
	while (envp[i] != NULL)
	{
		if (envp[i] == key)
			return(envp[i]);
		i++;
	}
	return (NULL);
} */

//! this should be the expand function check for 
char *env_within_double_quotes(char *line, int *i)
{
	int start;
	int length;
	char *env_string;

	start = ++(*i); // pre-iterate since index is still at '$'
	if (line[*i] == '\0' || line[*i] == '"')
		return(ft_strdup("$"));
	while (line[*i] && check_for_metacharacter(line[*i]) == false && line[*i] != '\'' && line[*i] != '"' && line[*i] != '$')
		(*i)++;
	length = *i - start;
	env_string = malloc(sizeof(char) * (length + 1)); //! MALLOC
	if (!env_string)
		return (NULL);
	strncpy(env_string, &line[start], length);
	env_string[length] = '\0';
	//printf("%s", env_string);
	return(env_string);
}

bool check_if_part_of_library(char **env_lib, char *search_str)
{
	int	i;

	i = 0;
	while (env_lib[i] != NULL)
	{
		if (ft_strcmp(env_lib[i], search_str) == 0)
			return(true);
		i++;
	}
	return(false);
}

/* searches for the corresponding env to search_str within envp;
when assigning to the buffer path is iterated by one to skip the equal sign*/
char	**find_path(char **envp, char *search_str)
{
	int		i;
	char	*path;
	char	**path_buf;
	
	i = 0;
	while (ft_strnstr(envp[i], search_str, ft_strlen(search_str)) == NULL)
		i++;
	path = ft_strstr(envp[i], "=");
	if (path == NULL)
		return (NULL);
	path_buf = ft_split(++path, '\0');
	return (path_buf);
}

/* search_str is the string that needs to be found */
char	*extract_env_name(char *line, int *i)
{
	int	start;
	int length;
	char	*search_str;

	start = *i;
	if (!line[*i] || line[*i] == '"')
		return(ft_strdup("$"));
	//! add g exit code
	while(line[*i] != '\0' && check_for_metacharacter(line[*i]) == false &&
	check_for_quotes(line[*i]) == false && line[*i] != '$')
		(*i)++;
	length = *i - start;
	search_str = ft_substr(line, start, length);
	return (search_str);
}

/* extract the env_str from the input */
char	*env_token(char *line, int *i, t_type *token_type, char **env_lib, char **envp)
{
	char	*search_str;
	char	*env_final;
	//char	**env_lib;

	(*i)++;
	*token_type = WORD;//ENV;
	search_str = extract_env_name(line, i);
	printf("SEARCH: %s", search_str);
	//env_lib = create_env_library(envp);
	if (check_if_part_of_library(env_lib, search_str) == false)
	{
		*token_type = ENV_FAIL;
		if (ft_strcmp(search_str, "$") == 0) // if theres a single dollar sign it's supposed to be printed
		{
			*token_type = WORD;//not needed anymore
			free(search_str);
			return(ft_substr("$", 0, 1));
		}
		free(search_str);
		return (NULL); //! does this make sense? how should I process it?
		//! if no matching ENV nothing happens, meaning I could do if type ENV_FAIL = skip that command
	}
	else 
	{
		env_final = *find_path(envp, search_str);
		free(search_str);
	}
	return (env_final);
}
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

/* //! does not work because dict list is not set!!!
char	*get_env_value(t_list *dict, char *key, char **envp)
{
	///t_dict	*item;
	int	i;
	(void)dict;
	i = 0;
	if (!key)
		return (NULL);
	while (dict)
	{
		//item = (t_dict *)dict->value;
 		if (!ft_strcmp(item->key, key))
			return (item->value);
		dict = dict->next;
	} 
	while (envp[i] != NULL)
	{
		if (envp[i] == key)
			return(envp[i]);
		i++;
	}
	return (NULL);
} */

//! this should be the expand function check for 
char *env_within_double_quotes(char *line, int *i)
{
	int start;
	int length;
	char *env_string;

	start = ++(*i); // pre-iterate since index is still at '$'
	if (line[*i] == '\0' || line[*i] == '"')
		return(ft_strdup("$"));
	while (line[*i] && check_for_metacharacter(line[*i]) == false && line[*i] != '\'' && line[*i] != '"' && line[*i] != '$')
		(*i)++;
	length = *i - start;
	env_string = malloc(sizeof(char) * (length + 1)); //! MALLOC
	if (!env_string)
		return (NULL);
	strncpy(env_string, &line[start], length);
	env_string[length] = '\0';
	//printf("%s", env_string);
	return(env_string);
}

bool check_if_part_of_library(char **env_lib, char *search_str)
{
	int	i;

	i = 0;
	while (env_lib[i] != NULL)
	{
		if (ft_strcmp(env_lib[i], search_str) == 0)
			return(true);
		i++;
	}
	return(false);
}

/* searches for the corresponding env to search_str within envp;
when assigning to the buffer path is iterated by one to skip the equal sign*/
char	**find_path(char **envp, char *search_str)
{
	int		i;
	char	*path;
	char	**path_buf;
	
	i = 0;
	while (ft_strnstr(envp[i], search_str, ft_strlen(search_str)) == NULL)
		i++;
	path = ft_strstr(envp[i], "=");
	if (path == NULL)
		return (NULL);
	path_buf = ft_split(++path, '\0');
	return (path_buf);
}

/* search_str is the string that needs to be found */
char	*extract_env_name(char *line, int *i)
{
	int	start;
	int length;
	char	*search_str;

	start = *i;
	if (!line[*i] || line[*i] == '"')
		return(ft_strdup("$"));
	//! add g exit code
	while(line[*i] != '\0' && check_for_metacharacter(line[*i]) == false &&
	check_for_quotes(line[*i]) == false && line[*i] != '$')
		(*i)++;
	length = *i - start;
	search_str = ft_substr(line, start, length);
	return (search_str);
}

/* extract the env_str from the input */
char	*env_token(char *line, int *i, t_type *token_type, char **env_lib, char **envp)
{
	char	*search_str;
	char	*env_final;
	//char	**env_lib;

	(*i)++;
	*token_type = WORD;//ENV;
	search_str = extract_env_name(line, i);
	printf("SEARCH: %s", search_str);
	//env_lib = create_env_library(envp);
	if (check_if_part_of_library(env_lib, search_str) == false)
	{
		*token_type = ENV_FAIL;
		if (ft_strcmp(search_str, "$") == 0) // if theres a single dollar sign it's supposed to be printed
		{
			*token_type = WORD;//not needed anymore
			free(search_str);
			return(ft_substr("$", 0, 1));
		}
		free(search_str);
		return (NULL); //! does this make sense? how should I process it?
		//! if no matching ENV nothing happens, meaning I could do if type ENV_FAIL = skip that command
	}
	else 
	{
		env_final = *find_path(envp, search_str);
		free(search_str);
	}
	return (env_final);
}