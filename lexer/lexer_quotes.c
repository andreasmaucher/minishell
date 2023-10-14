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

char	*append_str(char *str, char *appendix)
{
	char	*appended_str;

	if (!str)
		return(appendix);
	appended_str = ft_strjoin(str, appendix);
	//free(str);
	//free(appendix);
	return(appended_str);
}

char	*char_to_str(char c)
{
	char	*str;
	int		i;

	str = malloc(sizeof(char) * 2); //!MALLOC
	if (str == NULL)
		return (NULL);
	i = 0;
	str[i] = c;
	str[++i] = '\0';
	return(str);
}

char *double_quote_to_string(char *line, int *i, t_minishell m)
{
	char *str_between_quotes;
	char	*search_str;
	char	*env_expanded;

	str_between_quotes = NULL;
	while (line[*i] != '"' && line[*i] != '\0')
	{
		if (line[*i] == '$')
		{	
			(*i)++;
			search_str = extract_env_name(line, i);
			if (check_if_part_of_library(m.env_lib, search_str) == true)
				env_expanded = *find_path(m.envp_lib, search_str);
			else if (check_if_part_of_library(m.env_lib, search_str) == false)
				env_expanded = "";
			str_between_quotes = append_str(str_between_quotes, env_expanded);
			free(search_str);
		}
		else
			str_between_quotes = append_str(str_between_quotes, char_to_str(line[(*i)++]));
	}
	if (line[*i])
		(*i)++;
	return(str_between_quotes);
}

char *single_quote_to_string(char *line, int *i)
{
	char *str_between_quotes;
	int start;
	int length;

	start = *i;
	length = 0;
	while (line[*i] && line[*i] != '\'')
		(*i)++;
	length = *i - start;
	str_between_quotes = malloc(sizeof(char) * (length + 1)); //! MALLOC
	if (!str_between_quotes)
		return (NULL);
	strncpy(str_between_quotes, &line[start], length);
	str_between_quotes[length] = '\0';
	if (line[*i])
		(*i)++;
	return(str_between_quotes);
}

char *single_or_double_quotes_token(char *line, int *i, t_type *token_type, t_minishell m)
{
	if (line[*i] == '"')
	{
		*token_type = WORD;//DOUBLE_QUOTES; //!so in my version i don't even need this?!
		if (line[*i])
			(*i)++;
		return(double_quote_to_string(line, i, m));
	}
	else
	{
		*token_type = WORD;//SINGLE_QUOTES;
		if (line[*i])
			(*i)++;
		return(single_quote_to_string(line, i));
	}
}