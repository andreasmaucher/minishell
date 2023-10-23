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

/* generates a unique filename for a heredoc file by using a static index 
that increments with each call;
converting the index to a string so that it can be joined with the path;
each iteration creates a unique filename */
static char *create_heredoc_file(void)
{
    static int index = 0;
    char        *filename;
    char        *str;

    str = ft_itoa(index++);
    filename = ft_strjoin("/tmp/.heredoc_", str);
    return(filename);
}

/* e.g. < for input redirection: wc -l < file2 returns the amount of words in one file
e.g. << redirect heredoc: cat <<EOF >> output.txt in this case the heredoc allows to write directly in the terminal
until 'EOF' is typed; in a 2nd step it is then appended to the output.txt file;
it's necessary to have a separate structure for file to be able to create as many structures as needed */
void cmd_input_redirection(t_list **tlist, t_list *clist)
{
    t_command *tmp_cmd;
    t_token *tmp_token;

    tmp_cmd = (t_command *) clist->value;
    tmp_token = (t_token *) (* tlist)->value;
    tmp_cmd->input_redir_type = tmp_token->type;
    if (*tlist != NULL)
        *tlist = (* tlist)->next;
    tmp_token = (t_token *)(* tlist)->value;
    tmp_cmd->in_redirects.fd = -1;
    if (tmp_cmd->input_redir_type == REDIRECT_HEREDOC)
    {
        tmp_cmd->in_redirects.stop_heredoc = ft_strdup(tmp_token->str);
        tmp_cmd->in_redirects.new_heredoc_file = create_heredoc_file();
        tmp_cmd->in_redirects.file_name = NULL;
    }
    else if (tmp_cmd->input_redir_type == REDIRECT_IN)
    {
        tmp_cmd->in_redirects.file_name = ft_strdup(tmp_token->str);
        tmp_cmd->in_redirects.stop_heredoc = NULL;
        tmp_cmd->in_redirects.new_heredoc_file = NULL;
    }
}

/* for output redirections a new file needs to be created to store whatever is entered
before the redirection sign;
e.g. > for output redirection: wc -l < file2 > file1 creates a new file1 and writes the wordcount of file2 in file1
e.g. > for output redirection: ls -al > listings creates a new file listings with the ls -al information
e.g. >> append: echo "hello again" >> file2 this adds hello again to file 2 
> is used to create or replace a file with new output.
>> is used to append output to an existing file or create a new file and add output to it without overwriting existing content.
Overwriting is the main differentiation between > & >>;
*/
void    cmd_output_redirection(t_list **tlist, t_list *clist)
{
    t_command *tmp_cmd;
    t_token *tmp_token;

    tmp_cmd = (t_command *) clist->value;
    tmp_token = (t_token *) (* tlist)->value;
    tmp_cmd->output_redir_type = tmp_token->type;
    tmp_cmd->out_redirects.fd = -1;
    if (*tlist != NULL)
        *tlist = (* tlist)->next;
    tmp_token = (t_token *) (* tlist)->value;
    tmp_cmd->out_redirects.file_name = ft_strdup(tmp_token->str);
}