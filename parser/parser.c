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

/* counts the amount of commands (len) in a token list, determined by the amount of PIPES;
iteration by 1 at the end to account for the fact, that a PIPE is always splitting two parts */
int command_count(t_list *tlist) 
{
    t_list *tmp_head;
    t_token *token;
    int len;
    
    tmp_head = tlist;
    len = 0;
    while (tmp_head != NULL) 
    {
        token = (t_token *)tmp_head->value;
        if (token->type == PIPE)
            len++;
        tmp_head = tmp_head->next;
    }
    len++;
    printf("CMD Count: %d\n", len);
    return(len);
}

//!
t_command	*ft_create_cmd(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->arguments = NULL;
	cmd->type = PATH;
	cmd->before_pipe = false;
	cmd->after_pipe = false;
	cmd->input_redir_or_heredoc = 0;
	cmd->out_redir_type = 0;
	cmd->inred_file = NULL;
	cmd->outred_file = NULL;
	return (cmd);
}

void add_token_to_command_list(t_list **token_list, char *token_info)
{
	t_list *new_node;

	new_node = create_new_node(token_info);
	if (!*token_list)
		*token_list = new_node;
	else
		insert_at_tail(*token_list, new_node);
}

/* function that adds attributes to the command list (m.clist);
- assign the value of a node in the token list to tmp_token
- depending on it's type execute a subsequent function to add the attributes
to the command list
*/
static void add_attributes_to_command_list(t_minishell m)
{
    t_list *tmp_tlist;
    t_token *tmp_token;
    bool    first_word;

    first_word = true;
    tmp_tlist = m.tlist;
    while (tmp_tlist != NULL)
    {
        tmp_token = (t_token *)tmp_tlist->value;
        if (tmp_token->type == PIPE)
            cmd_pipe(&m.clist, &first_word);
        else if (tmp_token->type == REDIRECT_IN ||tmp_token->type == REDIRECT_HEREDOC)
            cmd_input_redirection(&tmp_tlist, m.clist);
        else if (tmp_token->type == WORD)
            cmd_word(tmp_tlist, m.clist, &first_word);
        else if (tmp_token->type == REDIRECT_OUT || tmp_token->type == REDIRECT_APPEND)
            cmd_output_redirection(&tmp_tlist, m.clist);
        tmp_tlist = tmp_tlist->next;
    }
}

t_list *parser(t_minishell m)
{
    //! add a check if input is valid or is this handled at the end of lexer?
    // always needs to be sth. after < & << otherwise segfault
    m.clist = setup_command_list(&m.clist, m.tlist);
    if (!m.clist)
        return (NULL);
    add_attributes_to_command_list(m);
    print_command_list(m.clist); //only for testing to see if attributes changed
    return(m.clist);
}