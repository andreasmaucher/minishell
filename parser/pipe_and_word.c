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

    tmp_cmd = (t_command *) (*clist)->value;
    tmp_cmd->before_pipe = true;
    *clist = (*clist)->next;
    tmp_cmd = (t_command *) (*clist)->value;
    tmp_cmd->after_pipe = true;
    *new_cmd = true;
}

void cmd_word(t_list **tlist, t_list *clist, bool *new_cmd)
{
    t_token *tmp_token;
    t_command *tmp_command;
    int i = 0;
    int tlist_len;
    t_list *tmp_tlist;

    tmp_tlist = NULL;
    tmp_tlist = *tlist;
    tmp_command = (t_command *) clist->value;
    tmp_token = (t_token *) tmp_tlist->value;
    if (*new_cmd == true)
    {
        if (!ft_strcmp(tmp_token->str, "echo") || !ft_strcmp(tmp_token->str, "cd")
		|| !ft_strcmp(tmp_token->str, "pwd") || !ft_strcmp(tmp_token->str, "export")
		|| !ft_strcmp(tmp_token->str, "unset") || !ft_strcmp(tmp_token->str, "env")
		|| !ft_strcmp(tmp_token->str, "exit"))
        tmp_command->type = BUILTIN;
        else
            tmp_command->type = PATH;
        tlist_len = token_count_tlist(tmp_tlist);
        tmp_command->args = malloc(sizeof(char *) * (tlist_len +1 ));
        if (!tmp_command->args)
          return;
         i = 0;
        while (tmp_token->type != PIPE && i <= tlist_len && tmp_tlist != NULL)
        {
            tmp_command->args[i] = ft_strdup((tmp_token->str));
            printf("Arg allocatio : %s\n",  tmp_command->args[i]);
            i++;
            tmp_tlist = tmp_tlist->next;
            if (tmp_tlist != NULL)
             tmp_token = (t_token *) tmp_tlist->value;
        }
        tmp_command->args[i] = NULL;
    }
    *new_cmd = false;
}