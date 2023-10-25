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

char	**find_path_executor(char **envp)
{
    int		i;
    char	*path;
    char	**path_buf;

    i = 0;
    while (ft_strnstr(envp[i], "PATH=", 5) == NULL)
        i++;
    path = ft_strstr(envp[i], "/");
    path_buf = ft_split(path, ':');
    return (path_buf);
}

char	*valid_path(char **path, char *argv)
{
    int		i;
    char	*correct_path;

    i = 0;
    while (path[i])
    {
        correct_path = join_strings(path[i], "/", argv);
        if (access(correct_path, X_OK) == 0)
            return (correct_path);
        else
        {
            free(correct_path);
        }
        i++;
    }
    return (NULL);
}

int wait_processes(t_minishell *m)
{
    int i;
    int status;
    (void)m;

    i = 0;
    while (i <= m->pipe_n)
    {
        //waitpid(-1, &status, 0); //another option for waiting for all processes
        wait(&status);
        i++;
    }
    return(0);
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

void ft_heredoc(char *delimiter)
{
    pid_t   pid;
    int     fd[2];
    // Prompt the user for input until the heredoc delimiter is entered
    char *line;
    if (pipe(fd) == -1)
        exit(42);
    pid = fork();
    if (pid == 0)
    {
        close(fd[0]);
        line = NULL;
        while (1)
        {
            line = readline("heredoc> ");
            line = ft_strjoin(line, "\n");
            if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0 &&
                ft_strlen(delimiter) == ft_strlen(line) - 1)
            {
                free(line);
                exit(EXIT_SUCCESS);
            }
            write(fd[1], line, ft_strlen(line));
            free(line);
        }
    }
    close(fd[1]);
    dup2(fd[0], STDIN_FILENO);
    wait(NULL);
}

void here_docs(t_command *cmd, t_minishell *m)
{
    int     fd[2];
    int status;
    (void)status;
    pid_t pid;
    char *tmp_str;

    if (pipe(fd) == -1)
    {
        //add free memory here
        exit(42);
    }
    pid = fork();
    if (pid == 0)
    {
        tmp_str = NULL;
        close(fd[0]);
        while (1)
        {
            m->here_docs = readline("> ");
            if (ft_strcmp(m->here_docs, cmd->in_redirects.stop_heredoc) == 0)
            {
                printf("Closing file\n");
                free(m->here_docs);
                close(fd[1]);
                exit(EXIT_SUCCESS);
            }
            tmp_str = ft_strjoin(m->here_docs, "\n");
            write(fd[1], tmp_str, ft_strlen(tmp_str));
            free(m->here_docs);
            free(tmp_str);
        }
    }
    else
    {
        close(fd[1]);
        if (dup2(fd[0], STDIN_FILENO))
        {
            perror("Input IN-redirection isn't working\n");
            // free memory here
            exit(1);
        }
        close(fd[0]);
        waitpid(pid, NULL, 0); // probably have to add exit status here
        //wait(NULL);
    }
}

//alternative function for output redirection
int out_redirections(t_minishell *m)
{
    t_command *cmd;
    t_list *temp;

    temp = m->clist;
    while(temp)
    {

        cmd = (t_command *) temp->value;
        (void)cmd;
        if (cmd->output_redir_type == REDIRECT_OUT)
        {
            write(1, "Issue here\n", 11);
            printf("error here3\n");
            // check for access
//            if (check_file_rights(cmd->in_redirects.file_name) != 0)
//                perror("Cant read from input file, permission denied\n");
            cmd->out_redirects.fd_read = open(cmd->out_redirects.file_name, O_CREAT | O_WRONLY, 0777);
            if (cmd->out_redirects.fd_read == -1)
                perror("Cant open the output file\n");
            if (dup2(cmd->out_redirects.fd_read, STDIN_FILENO) == -1)
                perror("Output redirection isn't working\n");
            close(cmd->out_redirects.fd_read);
        }
        temp = temp->next;
        printf("error here\n");
    }
    return (0);
}

int output_redirect(t_command *cmd)
{
    if (cmd->output_redir_type == REDIRECT_OUT)
    {
        write(1, "Output redirection function running\n", 25);
        cmd->out_redirects.fd_write = open(cmd->out_redirects.file_name, O_CREAT | O_RDWR, 0777);
        if (cmd->out_redirects.fd_write == -1)
            write(1, "Could not open output file\n", 25);
        dup2(cmd->out_redirects.fd_write, STDOUT_FILENO);
        close(cmd->out_redirects.fd_write);
    }
    if (cmd->output_redir_type == REDIRECT_APPEND)
    {
        write(1, "Output redirection function running\n", 25);
        cmd->out_redirects.fd_write = open(cmd->out_redirects.file_name, O_CREAT | O_RDWR | O_APPEND, 0777);
        if (cmd->out_redirects.fd_write == -1)
            write(1, "Could not open output file\n", 25);
        dup2(cmd->out_redirects.fd_write, STDOUT_FILENO);
        close(cmd->out_redirects.fd_write);
    }
    return(0);
}

int single_cmd(t_minishell *m)
{
    t_command *cmd = (t_command *)m->clist->value;

    m->child_id[0] = fork();
    if (m->child_id[0] == 0)
    {
        printf("------Child process N %d running---------\n", m->pipe_n);
        cmd->path = valid_path(m->path_buf, cmd->args[0]);
        if (cmd->path == NULL)
        {
            //Should I do anything here?
            //Probably freeing memory
        }
        if (cmd->output_redir_type == REDIRECT_OUT || cmd->output_redir_type == REDIRECT_APPEND)
        {
            output_redirect(cmd);
        }
        execute_program(cmd->args, cmd->path);
    }
    return (0);
}
int multiple_cmd(t_minishell *m)
{
    int current_process_id;

    current_process_id = 0;
    t_command *cmd;
    cmd = NULL;
    while(m->clist)
    {
        cmd = (t_command *) m->clist->value;
        m->child_id[current_process_id] = fork();
        if (m->child_id[current_process_id] == 0)
        {
            if (current_process_id == 0)
            {
                dup2(m->pipes[current_process_id][1], STDOUT_FILENO);
                close_pipes(m);
            }
            else if (current_process_id == m->pipe_n && (cmd->output_redir_type == REDIRECT_OUT || cmd->output_redir_type == REDIRECT_APPEND))
            {
                dup2(m->pipes[current_process_id - 1][0], STDIN_FILENO);
                output_redirect(cmd);
                close_pipes(m);
            }
			//! CHECK CONDITIONS, ERROR WARNING ALL CONDITIONS TRUE
           /*  else if (current_process_id == m->pipe_n && (cmd->output_redir_type != REDIRECT_OUT &&  cmd->output_redir_type != REDIRECT_APPEND))
            {
                dup2(m->pipes[current_process_id - 1][0], STDIN_FILENO);
                close_pipes(m);
            } */
            else
            {
                dup2(m->pipes[current_process_id - 1][0], STDIN_FILENO);
                dup2(m->pipes[current_process_id][1], STDOUT_FILENO);
                close_pipes(m);
            }
            printf("------Child process N %d running---------\n", current_process_id);
            cmd->path = valid_path(m->path_buf, cmd->args[0]);
            free_env(m->path_buf);
            execute_program(cmd->args, cmd->path);
            current_process_id++; //? can i delete this since it just runs in the child who already finished
        }
        printf("------Child process N %d finished---------\n", current_process_id);
        m->clist = m->clist->next;
        current_process_id++;
    }
    close_pipes(m);
    return (0);
}
// can delete or rewrite
int free_execve_fail(t_minishell *m)
{
    free_env(m->path_buf);
    free_env(m->env_lib);
    free_env(m->envp_lib);
    free(m->line);
    return(0);
}

int	execute_program(char **arg_vec, char *path)
{
    int i;

    i = 0;
    printf("Command to run is: %s\n", path);
    while (arg_vec[i])
    {
        printf("Arg_vec : %s\n", arg_vec[i]);
        i++;
    }
    if (execve(path, arg_vec, NULL) == -1)
    {
        perror("Could not execute");
        exit(1);
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

//fix this function
//int check_file_rights(char *filename)
//{
//    char *filename_path;
//
//    filename_path = ft_strjoin(pwd_path(), filename);
//    if (access(filename_path, R_OK) != 0)
//    {
//        free(filename_path);
//        return(1);
//    }
//    free(filename_path);
//    return(0);
//    //Should i also free filename?
//}

int in_redirections(t_minishell *m)
{
    t_command *cmd;
    t_list *temp;

    temp = m->clist;
    while(temp)
    {
        cmd = (t_command *) temp->value;
        if (cmd->input_redir_type == REDIRECT_IN)
        {
            // check for access
//            if (check_file_rights(cmd->in_redirects.file_name) != 0)
//                perror("Cant read from input file, permission denied\n");
            cmd->in_redirects.fd_write = open(cmd->in_redirects.file_name, O_RDONLY, 0777);
            if (cmd->in_redirects.fd_write == -1)
                perror("Cant open the file\n");
            if (dup2(cmd->in_redirects.fd_write, STDIN_FILENO) == -1)
                perror("Input IN-redirection isn't working\n");
            close(cmd->in_redirects.fd_write);
        }
        if (cmd->input_redir_type == REDIRECT_HEREDOC)
        {
            here_docs(cmd, m);
        }
        temp = temp->next;
    }
    return (0);
}

int executor(t_minishell m, char **envp)
{
    m.pipe_n = command_count(m.tlist) - 1;
    m.child_id = malloc(sizeof(int) * (m.pipe_n +1));
    m.path_buf = find_path_executor(envp);

    printf("\n------Start---------\n");
    printf("---Executor starts here---\n");
    in_redirections(&m);
    if (m.pipe_n == 0)
    {
        single_cmd(&m);
    }
    if (m.pipe_n > 0)
    {
        initialize_pipes(&m);
        multiple_cmd(&m);
    }
//  out_redirections(&m);
//should I move out_redirections here instead of having them in single and mutliple_cmd?
//test running commands following a command with redirections

    wait_processes(&m); // Here is a traditional way to place wait
    printf("------End---------\n");
    return (0);
}