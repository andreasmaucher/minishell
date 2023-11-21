#include "../minishell.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t destsize)
{
    size_t	src_len;
    size_t	i;

    src_len = ft_strlen(src);
    if (destsize)
    {
        i = 0;
        while (src[i] != '\0' && i < (destsize - 1))
        {
            dest[i] = src[i];
            i++;
        }
        dest[i] = '\0';
    }
    return (src_len);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
    char		*d;
    const char	*s;

    if (!(dest || src))
        return (NULL);
    d = dest;
    s = src;
    while (n-- > 0)
        *d++ = *s++;
    return (dest);
}

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
    size_t	len_d;
    size_t	len_s;

    len_d = ft_strlen(dst);
    len_s = ft_strlen(src);
    if (!size || len_d >= size)
        return (size + len_s);
    if (len_s < size - len_d)
        ft_memcpy(dst + len_d, src, len_s + 1);
    else
    {
        ft_memcpy(dst + len_d, src, size - len_d - 1);
        dst[size - 1] = '\0';
    }
    return (len_d + len_s);
}

char	*join_strings(const char *str1, const char *str2, const char *str3)
{
    size_t	len1;
    size_t	len2;
    size_t	len3;
    char	*result;

    len1 = ft_strlen(str1);
    len2 = ft_strlen(str2);
    len3 = ft_strlen(str3);
    result = malloc((len1 + len2 + len3 + 1) * sizeof(char));
    if (result == NULL)
        return (NULL);
    ft_strlcpy(result, str1, len1 + len2 + len3 + 1);
    ft_strlcat(result, str2, len1 + len2 + len3 + 1);
    ft_strlcat(result, str3, len1 + len2 + len3 + 1);
    return (result);
}

char	**find_path_executor(t_list *envp)
{
	char	*path;
	char	**path_buf;
	t_list	*tmp;
	t_dict	*dict;

	tmp = envp;
	dict = (t_dict *)tmp->value;
	while (tmp != NULL && ft_strnstr(dict->value, "PATH", ft_strlen("PATH")) == NULL)
	{
		dict = tmp->value;
        tmp = tmp->next;
	}
	path = ft_strstr(dict->value, "=");
	if (path == NULL)
		return (NULL);
	path_buf = ft_split(++path, ':');
	return (path_buf);
}

int check_if_file_or_dir(char *path)
{
    struct stat fileStat;

    if (stat(path, &fileStat) == 0) 
    {
        if (S_ISREG(fileStat.st_mode)) 
            return(0);
        else if (S_ISDIR(fileStat.st_mode)) 
            return(1);
        else 
            return(2);
    } 
    return(3);
}

int check_if_file_can_be_opened(char *file)
{
    if (check_if_file_or_dir(file) == 1)
    {
        errno = 126;
        return (1);
    }
    if (access(file, X_OK) == 0)
        return (0);
	else
		return(1);
}


char	*valid_path(char **path, char *argv)
{
    int		i;
    char	*correct_path;

    i = 0;
    correct_path = NULL;
    if (argv != NULL && (argv[0] == '/' || (argv[0] == '.' && argv[1] == '/')))
    {
        if (check_if_file_can_be_opened(argv) == 0)
            return (argv);
        else
            return (NULL);
    }
    while (path[i])
    {
        correct_path = join_strings(path[i], "/", argv);
        if (check_if_file_can_be_opened(correct_path) == 0)
            return (correct_path);
        else
            free (correct_path);
        i++;
    }
    return (NULL);
}
  
int wait_processes(t_minishell *m)
{
    int i;
    int wstatus;
    pid_t pid;
    
    wstatus = 0;
    i = 0;
    pid = 0;
    while (i <= m->pipe_n && m->forked == 1)
    {
        pid = wait(&wstatus);
        if (pid == m->child_id[m->pipe_n])
        {
            m->status_code2 = wstatus;
            if (WIFEXITED(wstatus)) 
                m->status_code2 = WEXITSTATUS(wstatus);
            if (WIFSIGNALED(wstatus)) 
            {
                m->status_code2 = WTERMSIG(wstatus);
                m->status_code2 = WIFSIGNALED(wstatus);
            }
        }
        m->status_code2 = m->status_code2 % 255;
        i++;
    }
    return(m->status_code2);
}

void kill_process(t_minishell *m, int process_id)
{
    int i;
    i = 0;
    while (i <= process_id)
    {
        kill(m->child_id[i], SIGTERM);
        i++;
    }
}

void ft_heredoc_exit_signal(t_minishell *m, int fd)
{
    if (g_signal_switch == 1)
	{
		free_m(m);
		free_pipes(m);
		close(fd);
		exit (errno);
	} 
}

void ft_heredoc_exit_eof(t_minishell *m, int fd, char *tmp_line)
{
    free(tmp_line);
    free_m(m);
    free_pipes(m);
    close(fd);
    exit(errno);
}

void ft_heredoc_loop(t_minishell *m, int fd, char *tmp_eof)
{
    char    *tmp_line;
    char    *line;

    line = NULL;
    tmp_line = NULL;
    ft_heredoc_exit_signal(m, fd);
    line = readline("heredoc> ");
    tmp_line = ft_strjoin(line, "\n");
    free(line);
    if (ft_strncmp(tmp_line, tmp_eof, ft_strlen(tmp_eof)) == 0 && ft_strlen(tmp_eof) == ft_strlen(tmp_line) - 1)
        ft_heredoc_exit_eof(m, fd, tmp_line);
    write(fd, tmp_line, ft_strlen(tmp_line));
    free(tmp_line);
}

void ft_launch_heredoc(t_list *tmp, t_minishell *m)
{
    pid_t   pid;
    int     fd;

    pid = fork();
    if (pid == -1) 
        error_handling_and_exit("Fork issue");
    if (pid == 0)
    {
        signal(SIGINT, handle_sigint_child);
        if_file_exists_delete(tmp->value);
        fd = open(tmp->value, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
        if (fd == -1) 
            error_handling_and_exit("Error removing file");
        while (1)
            ft_heredoc_loop(m, fd, tmp->eof);
    }
}

void ft_heredoc(t_minishell *m, t_command *cmd)
{
    t_list *tmp;

    tmp = cmd->in_file;
    while (tmp != NULL)
    {
        if (tmp->is_heredoc == 1)
            ft_launch_heredoc(tmp, m);
        wait(NULL);
    	g_signal_switch = 0;
		signal(SIGINT, handle_sigint);
	tmp = tmp->next;
    }
}

int output_redirect_file_write(t_list *tmp, t_command *cmd)
{
    if (tmp->is_append == 0)
    {
        cmd->out_redirects.fd_write = open(tmp->value, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
        if (cmd->out_redirects.fd_write == -1)
        {
            free(cmd->args);
            perror("Could not open output file\n");
        }
        if (dup2(cmd->out_redirects.fd_write, STDOUT_FILENO) == -1)
            return (1);
        close(cmd->out_redirects.fd_write);
    }
    if (tmp->is_append == 1)
    {
        cmd->out_redirects.fd_write = open(tmp->value, O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR);
        if (cmd->out_redirects.fd_write == -1)
        {
            free(cmd->args);
            perror("Could not open output file\n");
        }
        if (dup2(cmd->out_redirects.fd_write, STDOUT_FILENO) == -1)
            return (1);
        close(cmd->out_redirects.fd_write);
    }
    return (0);
}

int output_redirect(t_minishell *m, t_command *cmd)
{
    t_list *tmp;
    int     fd;

    tmp = cmd->out_file;

    while (tmp != NULL)
    {
        if (tmp->next == NULL)
        {
            if (if_file_exists_delete(tmp->value) != 0 && output_redirect_file_write(tmp, cmd) != 0)
                return (1);
        }
        else
        {
            if_file_exists_delete(tmp->value);
            fd = open(tmp->value, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR); // S_IRUSR | S_IWUSR
            if (fd == -1) 
            {
                free_m(m);
                free_pipes(m);
                return (1);
            }
            close(fd);
        }
        tmp = tmp->next;
    }
    free_filename(cmd->out_redirects.file_name);
    return(0);
}

int free_in_redirects_file(t_minishell *m)
{
    t_command *cmd_free;
    t_list *tmp;
    tmp = m->clist;
    cmd_free= NULL;
    while(tmp)//(m->clist)
    {
        cmd_free = (t_command *) tmp->value;//m->cl
        if (cmd_free->in_redirects.file_name)
            free(cmd_free->in_redirects.file_name);
        tmp = tmp->next;//m->clist = m->clist->next;
    }
return (0);
}

void in_redirections_per_single_bultins_cmd_file_read(t_minishell *m, t_command *cmd, t_list *tmp)
{
    if (check_file_rights((char *)tmp->value) == 0)
        {
            cmd->in_redirects.fd_write = open((char *)tmp->value, O_RDONLY, 0777);
            if (cmd->in_redirects.fd_write == -1)
                perror("Cant open the file\n");
            if (dup2(cmd->in_redirects.fd_write, STDIN_FILENO) == -1)
                perror("Input IN-redirection isn't working\n");
            close(cmd->in_redirects.fd_write);
        }
        else
        {
            perror("Cant read from input file, permission denied\n");
            if (tmp->value)
                {
                    free_m(m);
                    free_pipes(m);
                    m->status_code2 = errno;
                }
        }
}

int in_redirections_per_cmd_single_builtins(t_minishell *m, t_command *cmd)
{
    t_list *tmp;

    tmp = cmd->in_file;
    while (tmp->next != NULL)
    {
        if (check_file_rights((char *)tmp->value) == 0)
        {
            //free_out_file_list(cmd->in_file); //!!! incorrect free, insert correct one
        }
        else
        {
            perror("Cant read from input file, permission denied\n");
            if (tmp->value)
            {
                free_m(m);
                free_pipes(m);
                m->status_code2 = errno;               
            }
        }
        tmp = tmp->next;
    }
    in_redirections_per_single_bultins_cmd_file_read(m, cmd, tmp);
    return (0);
}

void in_redirections_per_cmd_file_read(t_minishell *m, t_command *cmd, t_list *tmp)
{
    if (check_file_rights((char *)tmp->value) == 0)
    {
        cmd->in_redirects.fd_write = open((char *)tmp->value, O_RDONLY, 0777);
        if (cmd->in_redirects.fd_write == -1)
            perror("Cant open the file\n");
        if (dup2(cmd->in_redirects.fd_write, STDIN_FILENO) == -1)
            perror("Input IN-redirection isn't working\n");
        close(cmd->in_redirects.fd_write);
    }
    else
    {
        perror("Cant read from input file, permission denied\n");
        if (tmp->value)
            {
                free_m(m);
                free_pipes(m);
                error_handling_and_exit("Cant read from input file, permission denied");
            }
    }
}

int in_redirections_per_cmd(t_minishell *m, t_command *cmd)
{
    t_list *tmp;

    tmp = cmd->in_file;
    while (tmp->next != NULL)
    {
        if (check_file_rights((char *)tmp->value) == 0)
        {
            //free_out_file_list(cmd->in_file); //!!! incorrect free, insert correct one
        }
        else
        {
            perror("Cant read from input file, permission denied\n");
            if (tmp->value)
            {
                free_m(m);
                free_pipes(m);
                m->status_code = errno;
                error_handling_and_exit("Cant read from input file, permission denied");
            }
        }
        tmp = tmp->next;
    }
    in_redirections_per_cmd_file_read(m, cmd, tmp);
    return (0);
}

void no_cmd(t_command *cmd, t_minishell *m)
{
    if (cmd->path == NULL)
    {
        free_m(m);
        free_pipes(m);
        error_handling_and_exit("No valid command to run\n");
    }
}

void free_all_filenames(t_command *cmd)
{
    free_filename(cmd->out_redirects.file_name);
}

int single_cmd(t_minishell *m, t_command *cmd, char **envp)
{   
    m->forked = 1;
    m->child_id[0] = fork();
    if (m->child_id[0] == -1) 
        error_handling_and_exit("Fork failed\n");
    if (m->child_id[0] == 0)
    {
        cmd->path = NULL;
        if (cmd->input_redir_type == REDIRECT_IN || cmd->input_redir_type == REDIRECT_HEREDOC)
            in_redirections_per_cmd(m, cmd);
        if (cmd->output_redir_type == REDIRECT_OUT || cmd->output_redir_type == REDIRECT_APPEND)
            if (output_redirect(m, cmd) != 0)
                cmd->args = NULL; //output_redirect(m, cmd);
        if (cmd->args)
            cmd->path = valid_path(m->path_buf, cmd->args[0]);
        if (cmd->path == NULL)
            no_cmd(cmd, m);
        if (cmd->type != BUILTIN)
            execute_program(cmd->args, cmd, m, envp);
    }
    free_all_filenames(cmd);
    return (0);
}

void multiple_cmd_piping(t_minishell *m, t_command *cmd)
{
    cmd->path = NULL;
    if (m->current_process_id == 0 )
        dup2(m->pipes[m->current_process_id][1], STDOUT_FILENO);
    else if (m->current_process_id == m->pipe_n )
        dup2(m->pipes[m->current_process_id - 1][0], STDIN_FILENO);
    else
    {
        dup2(m->pipes[m->current_process_id - 1][0], STDIN_FILENO);
        dup2(m->pipes[m->current_process_id][1], STDOUT_FILENO);
    }
    if (cmd->input_redir_type == REDIRECT_IN || cmd->input_redir_type == REDIRECT_HEREDOC)
        in_redirections_per_cmd(m, cmd);
    if (cmd->output_redir_type == REDIRECT_OUT || cmd->output_redir_type == REDIRECT_APPEND)
            if (output_redirect(m, cmd) != 0)
                cmd->args = NULL;
    close_pipes(m);
}


int multiple_cmd(t_minish
// // can delete or rewrite
// int free_execve_fail(t_minishell *m)
// {
//     free_env(m->path_buf);
//     free(m->line);
//     return(0);
// }ell *m, t_command *cmd, char **envp)
{
    m->forked = 1;
    m->child_id[m->current_process_id] = fork();
    if (m->child_id[m->current_process_id] == -1)
        error_handling_and_exit("Fork failed\n");
    if (m->child_id[m->current_process_id] == 0)
    {
        multiple_cmd_piping(m, cmd);
        if (cmd->args)
            cmd->path = valid_path(m->path_buf, cmd->args[0]);
        if (cmd->path == NULL)
            no_cmd(cmd, m);
        if (cmd->type == BUILTIN)
        {
            free_to_null(cmd->path);
            free_all_filenames(cmd);
            execute_builtins(m, cmd);
        }
        if (cmd->type != BUILTIN)
            execute_program(cmd->args, cmd, m, envp);
    }
    free_all_filenames(cmd);
    m->current_process_id++;
    return (0);
}


int	execute_program(char **arg_vec, t_command *cmd, t_minishell *m, char **envp)
{
    int i;

    i = 0;
    while (arg_vec[i])
        i++;
    if (execve(cmd->path, arg_vec, envp) == -1)
    {
        free_m(m);
        if (cmd->in_file)
		    ft_file_name_clear(cmd->in_file);
        free_to_null(cmd->path);
        free_arr_to_null(arg_vec);
        free_all_filenames(cmd);
        free_pipes(m);
        error_handling_and_exit("Could not execute\n");
    }
    return (0);
}

int initialize_pipes(t_minishell *m)
{
    int i;

    i = 0;
    m->pipes = malloc(sizeof (int *) * m->pipe_n * 2);
    while(i <= m->pipe_n)
    {
        m->pipes[i] = malloc(sizeof(int) * 2);
        if (pipe(m->pipes[i]) < 0)
            return (1);
        i++;
    }
    return (0);
}

int free_pipes(t_minishell *m)
{
    int i;

    i = 0;
    if (m->pipe_n == 0)
        return(0);
    while(i <= m->pipe_n)
    {
        if (m->pipes[i])
            free(m->pipes[i]);
        i++;
    }
    if (m->pipes)
        free(m->pipes);
    return (0);
}

int close_pipes(t_minishell *m)
{
    int i;

    i = 0;
    while(i <= m->pipe_n)
    {
        close(m->pipes[i][0]);
        close(m->pipes[i][1]);
        i++;
    }
    return (0);
}

int check_file_rights(char *filename)
{
   if (access(filename, R_OK | W_OK) != 0)
   {
       return(1);
   }
   return(0);
}


void	term_processes(t_minishell *m)
{
	int	j;

	j = 0;
	if (m->forked == 1)
	{
		while (j <= m->pipe_n)
		{
			kill(m->child_id[j], SIGTERM);
			j++;
		}
        m->forked = 0;
	}
}

int init_executor(t_minishell *m)
{
    m->current_process_id = 0;
    m->pipe_n = command_count(m->tlist) - 1;
    m->child_id = malloc(sizeof(int) * (m->pipe_n +1));
    m->path_buf = find_path_executor(m->envp);
    if (m->pipe_n > 0)
    {
        initialize_pipes(m);
        m->forked = 1;
    }
    return (0);
}

int exit_executor(t_minishell *m)
{
    if (m->pipe_n > 0)
    {
        close_pipes(m);
        free_pipes(m);
    }
    m->status_code2 = wait_processes(m);
   	free_arr_to_null(m->path_buf);
    free_intp_to_null(m->child_id);
    m->forked = 0;
    return (m->status_code2);
}

int executor(t_minishell m, t_command *cmd, char **envp)
{
    t_list *tmp;

    init_executor(&m);
    tmp = m.clist;
    while(tmp)
    {
        cmd = (t_command *) tmp->value;
        if (cmd->input_redir_type == REDIRECT_HEREDOC || cmd->input_redir_type == REDIRECT_IN)
            ft_heredoc(&m, cmd);
        if (cmd->type == BUILTIN && m.pipe_n == 0)
            m.status_code2 = execute_single_builtin_logic(&m, cmd);
        if (m.pipe_n == 0 && cmd->type != BUILTIN)
            single_cmd(&m, cmd, envp);
        if (m.pipe_n > 0)
            multiple_cmd(&m, cmd, envp);
        tmp = tmp->next;
    }
    m.status_code2 = exit_executor(&m);
    return (m.status_code2);
}