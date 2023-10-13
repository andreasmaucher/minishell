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
    bool    new_cmd;

    new_cmd = true;
    tmp_tlist = m.tlist;
    while (tmp_tlist != NULL)
    {
        tmp_token = (t_token *)tmp_tlist->value;
        if (tmp_token->type == PIPE)
            cmd_pipe(&m.clist, &new_cmd);
        else if (tmp_token->type == REDIRECT_IN ||tmp_token->type == REDIRECT_HEREDOC)
            cmd_input_redirection(&tmp_tlist, m.clist);
        else if (tmp_token->type == WORD)
            cmd_word(tmp_tlist, m.clist, &new_cmd);
        else if (tmp_token->type == REDIRECT_OUT || tmp_token->type == REDIRECT_APPEND)
            cmd_output_redirection(&tmp_tlist, m.clist);
        tmp_tlist = tmp_tlist->next;
    }
}

t_list *parser(t_minishell m)
{
    if (check_parser_input(m.tlist) == true && m.tlist != NULL)
    {
        m.clist = setup_command_list(&m.clist, m.tlist);
        if (m.clist == NULL)
            return (NULL);
        add_attributes_to_command_list(m);
        m.n_pipes = command_count(m.tlist);
        print_command_list(m.clist); //!only for testing
        return(m.clist);
    }
   else
   {
        exit_shell(m);
        return (NULL); //!exit code? */
   }
}