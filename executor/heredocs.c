/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrizakov <mrizakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 23:47:56 by mrizakov          #+#    #+#             */
/*   Updated: 2023/11/22 18:54:21 by mrizakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_heredoc_exit_signal(t_minishell *m, int fd)
{
	if (g_signal_switch == 1)
	{
		free_m(m);
		free_pipes(m);
		close(fd);
		exit (errno);
	}
}

void	ft_heredoc_exit_eof(t_minishell *m, int fd, char *tmp_line)
{
	free(tmp_line);
	free_m(m);
	free_pipes(m);
	close(fd);
	exit(errno);
}

void	ft_heredoc_loop(t_minishell *m, int fd, char *tmp_eof)
{
	char	*tmp_line;
	char	*line;

	line = NULL;
	tmp_line = NULL;
	ft_heredoc_exit_signal(m, fd);
	line = readline("heredoc> ");
	if (!line)
		ft_heredoc_exit_eof(m, fd, tmp_line);
	tmp_line = ft_strjoin(line, "\n");
	free(line);
	if (ft_strncmp(tmp_line, tmp_eof, ft_strlen(tmp_eof)) == 0 
		&& ft_strlen(tmp_eof) == ft_strlen(tmp_line) - 1)
		ft_heredoc_exit_eof(m, fd, tmp_line);
	write(fd, tmp_line, ft_strlen(tmp_line));
	free(tmp_line);
}

void	ft_launch_heredoc(t_list *tmp, t_minishell *m)
{
	pid_t	pid;
	int		fd;

	pid = fork();
	if (pid == -1) 
		error_handling_and_exit("Fork issue");
	if (pid == 0)
	{
		signal(SIGINT, handle_sigint_child);
		signal(SIGQUIT, handle_sigint_child);
		if_file_exists_delete(tmp->value);
		fd = open(tmp->value, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
		if (fd == -1) 
			error_handling_and_exit("Error removing file");
		while (1)
			ft_heredoc_loop(m, fd, tmp->eof);
	}
}

void	ft_heredoc(t_minishell *m, t_command *cmd)
{
	t_list	*tmp;

	tmp = cmd->in_file;
	while (tmp != NULL)
	{
		if (tmp->is_heredoc == 1)
			ft_launch_heredoc(tmp, m);
		wait(NULL);
		if (g_signal_switch == 1)
			m->status_code2 = 130;
		signal(SIGINT, handle_sigint);
		tmp = tmp->next;
	}
}
