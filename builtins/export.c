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

bool	invalid_identifier(char **args, int i)
{
	printf("export '%s' : not a valid identifier\n", args[i]);
	return (false);
}

/*
first letter needs to be _ or a character;
in the key only characters, digits & _ is allowed;
*/
bool	check_valid_export_input(char **args)
{
	int		i;
	int		j;

	i = 1;
	j = 0;
	while (args[i] != NULL)
	{
		j = 0;
		while (args[i][j] != '\0' && args[i][j] != '=')
		{
			if (args[i][0] != '_' && ft_isalpha(args[i][0]) == false)
				return (invalid_identifier(args, i));
			else if (args[i][j] == '=')
				break ;
			else if (ft_isalpha(args[i][j]) == false
				&& ft_is_digit(args[i][j]) == false && args[i][j] != '_')
				return (invalid_identifier(args, i));
			j++;
		}
		i++;
	}
	return (true);
}

/*
function extracts the part until '=' to later compare it with m.env_lib
*/
char	*extract_search_str(t_command *cmd, int i)
{
	int		search_len;
	char	*search_str;
	int		j;

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

bool	check_equal_sign(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (true);
		i++;
	}
	return (false);
}

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
int	export(t_minishell *m, t_command *cmd)
{
	if (check_valid_export_input(cmd->args) == true)
	{
		delete_double_envs(m, cmd);
		add_new_envs(m, cmd);
		return (0);
	}
	return (1);
}
