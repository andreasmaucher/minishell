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

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*p;

	p = s;
	while (n > 0)
	{
		*p = c;
		p++;
		n--;
	}
	return (s);
}

/* first initialize all fields in the main data structure t_minishell to zero by using memset;
create libaries for the envp fields, one containing the full information and one only the 
parameter name, which is later being used as a search keyword */
void	init_minishell_struct_and_signals(t_minishell *m, char **envp)
{
	init_signals();
	ft_memset(m, 0, sizeof(t_minishell));
	m->envp = create_envp_list(envp);
}