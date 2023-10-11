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

/* generates a unique filename for a heredoc file by using a static index that increments with each call;
converting the index to a string so that it can be joined with the path;
each iteration creates a unique filename */
//! MALLOC
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
e.g. > for output redirection: wc -l < file2 > file1 creates a new file1 and writes the wordcount in it
e.g. >> append: echo "hello again" >> file2 this adds hello again to file 2 
e.g. << redirect heredoc: cat <<EOF >> output.txt in this case the heredoc allows to write directly in the terminal
until 'EOF' is typed; in a 2nd step it is then appended to the output.txt file;
it's necessary to have a separate structure for file to be able to create as many structures as needed */
void cmd_input_redirection(t_list **tlist, t_list *clist)
{
    t_file  *file;
    t_command *tmp_cmd;
    t_token *tmp_token;

    file = malloc(sizeof(t_file));
    if (!file)
        return;
    tmp_cmd = (t_command *) clist->value;
    tmp_token = (t_token *) (* tlist)->value;
    tmp_cmd->input_redir_or_heredoc = tmp_token->type; //! if not used later, delete again
    file->redirection_type = tmp_token->type; //! if not used later, delete again
    printf("TEST1: %s\n", tmp_token->str);
    if (*tlist != NULL) //! this is to avoid segfaults, but also need to be careful of case without anything after <
        *tlist = (* tlist)->next;
    tmp_token = (t_token *)(* tlist)->value;
    printf("TEST2: %s\n", tmp_token->str); //! does not print because of whitespace!!!
    file->fd = -1;
    if (tmp_cmd->input_redir_or_heredoc == REDIRECT_HEREDOC)
    {
        file->stop_heredoc = ft_strdup(tmp_token->str);
        file->new_heredoc_file = create_heredoc_file();
        file->text_to_file = NULL;
    }
    else if (tmp_cmd->input_redir_or_heredoc == REDIRECT_IN)
    {
        file->text_to_file = tmp_token->str;//ft_strdup(tmp_token->str);
        file->stop_heredoc = NULL;
        file->new_heredoc_file = NULL;
    }
    //new_node = create_new_node((void *)file);
    //! NOT WORKING
    add_token_to_command_list(&tmp_cmd->inred_file, (void *)file);
    //ft_lstadd_back(&(tmp_cmd->inred_file), ft_lstnew((void *)file));
    //insert_at_tail(tmp_cmd->inred_file, create_new_node((void *)file)); //! WHATS THE LIST???
}

/* for output redirections a new file needs to be created to store whatever is entered
before the redirection sign; */
void    cmd_output_redirection(t_list **tlist, t_list *clist)
{
    t_file  *file;
    t_command *tmp_cmd;
    t_token *tmp_token;

    file = malloc(sizeof(t_file));
    if (!file)
        return;
    tmp_cmd = (t_command *) clist->value;
    tmp_token = (t_token *) (* tlist)->value;
    tmp_cmd->out_redir_type = tmp_token->type;
    file->redirection_type = tmp_token->type;
    if (*tlist != NULL) //! NECESSARY?
        *tlist = (* tlist)->next;
    tmp_token = (t_token *)(* tlist)->value;
    file->fd = -1;
    file->text_to_file = ft_strdup(tmp_token->str);
    file->stop_heredoc = NULL;
    file->new_heredoc_file = NULL;
    add_token_to_command_list(&tmp_cmd->outred_file, (void *)file);
}