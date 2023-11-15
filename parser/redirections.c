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

/*
generates a unique filename for a heredoc file by using a static index 
that increments with each call;
converting the index to a string so that it can be joined with the path;
each iteration creates a unique filename
*/

static char *create_heredoc_file(void)
{
    static int index = 0;
    char        *filename;
    char        *str;

    str = ft_itoa(index++);
    filename = ft_strjoin(".heredoc_", str);
	free(str); //forgot to clear this malloc
    return(filename);
}

/* e.g. < for input redirection: wc -l < file2 returns the amount of words 
in one file
e.g. << redirect heredoc: cat <<EOF >> output.txt in this case the heredoc 
allows to write directly in the terminal
until 'EOF' is typed; in a 2nd step it is then appended to the output.txt file;
it's necessary to have a separate structure for file to be able to create as 
many structures as needed */
void	cmd_input_redirection(t_list **tlist, t_list *clist)
{
	t_command	*tmp_cmd;
	t_token		*tmp_token;
	t_list		*new_node;
	char		*file_name;
	char		*eof;

	eof = NULL;
	tmp_cmd = (t_command *) clist->value;
	tmp_token = (t_token *)(*tlist)->value;
	new_node = NULL;
	tmp_cmd->input_redir_type = tmp_token->type;
	if (*tlist != NULL)
		*tlist = (*tlist)->next;
	tmp_token = (t_token *)(*tlist)->value;
	tmp_cmd->in_redirects.fd = -1;
	if (tmp_cmd->input_redir_type == REDIRECT_HEREDOC)
	{
		free_to_null(tmp_cmd->in_redirects.stop_heredoc);
		free_to_null(tmp_cmd->in_redirects.new_heredoc_file);
		// tmp_cmd->in_redirects.stop_heredoc = ft_strdup(tmp_token->str);
		// tmp_cmd->in_redirects.new_heredoc_file = create_heredoc_file();
		tmp_cmd->in_redirects.file_name = NULL;
		// file_name = ft_strdup(tmp_token->str);
		file_name = create_heredoc_file();
		eof = ft_strdup(tmp_token->str);
		printf("HEREDOCS filename is %s\n", file_name);

		new_node = create_new_filename_node(file_name, eof);
		//new_node->value = file_name;
		//new_node->eof = ft_strdup(tmp_token->str);
		new_node->is_heredoc = 1;
		if (!tmp_cmd->in_file)
			tmp_cmd->in_file = new_node;
		else
			insert_at_tail(tmp_cmd->in_file, new_node);
		//free(file_name);
		//free(eof);
	}
	else if (tmp_cmd->input_redir_type == REDIRECT_IN)
	{
		//free_to_null(tmp_cmd->in_redirects.file_name);
		//tmp_cmd->in_redirects.file_name = ft_strdup(tmp_token->str);
		tmp_cmd->in_redirects.stop_heredoc = NULL;
		tmp_cmd->in_redirects.new_heredoc_file = NULL;
		file_name = ft_strdup(tmp_token->str);
		//new_node->value = ft_strdup(tmp_token->str);
		//free(eof);
		eof = NULL;
		new_node = create_new_filename_node(file_name, eof);
		//new_node->is_heredoc = 0;
		if (!tmp_cmd->in_file)
			tmp_cmd->in_file = new_node;
		else
			insert_at_tail(tmp_cmd->in_file, new_node);
		//free(file_name);
	}
}

/* 
for output rexedirections a new file needs to be created to store whatever
is entered before the redirection sign;
e.g. > for output redirection: wc -l < file2 > file1 creates a new file1
and writes the wordcount of file2 in file1
e.g. > for output redirection: ls -al > listings creates a new file listings
with the ls -al information
e.g. >> append: echo "hello again" >> file2 this adds hello again to file 2 
> is used to create or replace a file with new output.
>> is used to append output to an existing file or create a new file and
add output to it without overwriting existing content.
Overwriting is the main differentiation between > & >>;
*/
void	cmd_output_redirection(t_list **tlist, t_list *clist)
{
	t_command	*tmp_cmd;
	t_token		*tmp_token;
	t_list		*new_node;
	char		*file_name;

	new_node = NULL;
	tmp_cmd = (t_command *) clist->value;
	tmp_token = (t_token *)(*tlist)->value;
	tmp_cmd->output_redir_type = tmp_token->type;
	tmp_cmd->out_redirects.fd = -1;
	if (*tlist != NULL)
		*tlist = (*tlist)->next;
	tmp_token = (t_token *)(*tlist)->value;
	file_name = ft_strdup(tmp_token->str);
	new_node = create_new_append_node(file_name);
	printf("type: %d\n", tmp_token->type);
	if (tmp_cmd->output_redir_type == REDIRECT_APPEND)
		new_node->is_append = 1;
	if (tmp_cmd->output_redir_type == REDIRECT_OUT)
		new_node->is_append = 0;
	if (!tmp_cmd->out_file)
		tmp_cmd->out_file = new_node;
	else
		insert_at_tail(tmp_cmd->out_file, new_node);
	//free(file_name);
}
