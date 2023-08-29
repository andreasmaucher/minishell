#include "minishell.h"

char *redirection_token(char *line, int *i, t_type *token_type)
{
	(*i)++;
	if (line[*i] == '<' && line[*i++] == '<')
	{
		*token_type = REDIRECT_HEREDOC;
		(*i)++;
		return (ft_strdup("<<"));
	}
	else if (line[*i] == '>' && line[*i++] == '>')
	{
		*token_type = REDIRECT_APPEND;
		(*i)++;
		return (ft_strdup(">>"));
	}
	else if (line[*i] == '>') //! not detected correctly
	{
		*token_type = REDIRECT_OUT;
		return (ft_strdup(">"));
	}
	else
	{
		*token_type = REDIRECT_IN;
		return (ft_strdup("<"));
	}
}

char *pipe_token(int *i, t_type *token_type)
{
	*token_type = PIPE;
	(*i)++;
	return(ft_strdup("|"));
}

char *check_for_word_token(char *line, int *i, t_type *token_type)
{
	int token_start;

	token_start = *i;
	while (check_for_metacharacter(line[*i]) == false && check_for_quotes(line[*i]) == false && line[*i] != '$')
		(*i)++;
	*token_type = WORD;
	return(ft_substr(line, token_start, *i - token_start));
}