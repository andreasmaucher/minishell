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

/* modifies the attributes of the command node in the list that m.clist points to */
void cmd_pipe(t_list **clist, bool *new_cmd)
{
    t_command *tmp_cmd;
    t_list *tmp_head;

    tmp_head = *clist;
    tmp_cmd = (t_command *) tmp_head->value;
    tmp_cmd->before_pipe = true;
    tmp_head = tmp_head->next;
    tmp_cmd = (t_command *) tmp_head->value;
    tmp_cmd->after_pipe = true;
    *new_cmd = true;
}

void cmd_word(t_list *tlist, t_list *clist, bool *new_cmd)
{
    t_token *tmp_token;
    t_command *tmp_command;

    tmp_command = (t_command *) clist->value;
    tmp_token = (t_token *) tlist->value;
    printf("TMP TOKEN STR %s\n", tmp_token->str);
    printf("NEW CMD %s \n", new_cmd ? "true" : "false");
    if (*new_cmd == true)
    {
        if (!ft_strcmp(tmp_token->str, "echo") || !ft_strcmp(tmp_token->str, "cd")
		|| !ft_strcmp(tmp_token->str, "pwd") || !ft_strcmp(tmp_token->str, "export")
		|| !ft_strcmp(tmp_token->str, "unset") || !ft_strcmp(tmp_token->str, "env")
		|| !ft_strcmp(tmp_token->str, "exit"))
        tmp_command->type = BUILTIN;
        else
            tmp_command->type = PATH;
        printf("ONE LOOP \n");
    }
    *new_cmd = false;
    printf("NEW CMD AFTER %s \n", new_cmd ? "true" : "false");
    add_token_to_command_list(&tmp_command->arguments, ft_strdup(tmp_token->str));
}