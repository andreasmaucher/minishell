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
        //waitpid(-1, &status, 0);
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

int single_cmd(t_minishell *m)
{
    t_command *cmd = (t_command *)m->clist->value;
    printf("error here4\n");

    m->child_id[0] = fork();
//    if (m->child_id[0] != 0)
//    {
//        printf("Child PID is :%d\n", getpid() )
//    }
    printf("error here5\n");

    if (m->child_id[0] == 0)
    {
        printf("------Child process N %d running---------\n", m->pipe_n);
        cmd->path = valid_path(m->path_buf, cmd->args[0]);
        if (cmd->path == NULL)
        {
//            printf("Going to kill process PID, %d", getpid());
//            kill(getpid(), SIGUSR1); // Luiz suggestion
//              free_env(m->path_buf);
        }

        if (cmd->output_redir_type == REDIRECT_OUT)
        {
            //dup2(m->pipes[current_process_id - 1][0], STDIN_FILENO);
            write(1, "is output redirection working for a single command?\n", 52);
            cmd->out_redirects.fd_write = open(cmd->out_redirects.file_name, O_CREAT | O_RDWR, 0777);
            if (cmd->out_redirects.fd_write == -1)
                write(1, "Could not open output file\n", 25);
            dup2(cmd->out_redirects.fd_write, STDOUT_FILENO);
            //close_pipes(m);
            close(cmd->out_redirects.fd_write);
        }
        execute_program(cmd->args, cmd->path, m, 0);
    }
//    if (m->child_id[0] != 0)
//    {
//        waitpid(m->child_id[0], NULL, 0);
//        printf("Child PID is :%d\n", m->child_id[0]);
//    }
    return (0);
}
int multiple_cmd(t_minishell *m)
{
    int current_process_id;

    current_process_id = 0;
    t_command *cmd = NULL;
    //while(current_process_id <= m->pipe_n && m->clist)
    while(m->clist)
        {
          //wait_processes(m); // Either here or in the executor in the end. Placing wait here helped the processes to run in the right sequence
//        while (m->clist)
//        {
            cmd = (t_command *) m->clist->value;
            m->child_id[current_process_id] = fork();
            if (m->child_id[current_process_id] == 0)
            {
                if (current_process_id == 0)
                {
                    dup2(m->pipes[current_process_id][1], STDOUT_FILENO);
                    close_pipes(m);
                }

                else if (current_process_id == m->pipe_n && cmd->output_redir_type == REDIRECT_OUT)
                {
                    dup2(m->pipes[current_process_id - 1][0], STDIN_FILENO);
                    write(1, "is output redirection working?\n", 32);
                    cmd->out_redirects.fd_write = open(cmd->out_redirects.file_name, O_CREAT | O_RDWR, 0777);
                    if (cmd->out_redirects.fd_write == -1)
                            write(1, "Could not open output file\n", 25);
                    dup2(cmd->out_redirects.fd_write, STDOUT_FILENO);
                    close_pipes(m);
                    close(cmd->out_redirects.fd_write);
                }

                else if (current_process_id == m->pipe_n && cmd->output_redir_type != REDIRECT_OUT)
                {
                    write(1, "WHat about this?\n", 18);

                    dup2(m->pipes[current_process_id - 1][0], STDIN_FILENO);
                    close_pipes(m);
                }
                else
                {
                    dup2(m->pipes[current_process_id - 1][0], STDIN_FILENO);
                    dup2(m->pipes[current_process_id][1], STDOUT_FILENO);
                    close_pipes(m);
                }
                printf("------Child process N %d running---------\n", current_process_id);
                cmd->path = valid_path(m->path_buf, cmd->args[0]);
                free_env(m->path_buf);
                execute_program(cmd->args, cmd->path, m, current_process_id);
                current_process_id++; //? can i delete this since it just runs in the child who already finished
            }
//        if (m->child_id[current_process_id] != 0)
//        {
//            waitpid(m->child_id[0], NULL, 0);
//            printf("Child PID is :%d\n", m->child_id[0]);
//        }

        printf("------Child process N %d finished---------\n", current_process_id);
        m->clist = m->clist->next;
        current_process_id++;
    }
    close_pipes(m);
    return (0);
}

//
//// redirects in parser
//cmd = (t_command *) m->clist->value;
//cmd->input_redir_type == REDIRECT_IN; // check to see if this is in redirect or
//cmd->input_redir_type == REDIRECT_HEREDOC; // check to see if this is in redirect
//
//cmd->output_redir_type == REDIRECT_OUT; // check to see if this is in redirect
//cmd->output_redir_type == REDIRECT_APPEND; // check to see if this is in redirect
//cmd->in_redirects.file_name = "infile.txt";
//cmd->out_redirects.file_name = "outfile.txt";
//cmd->out_redirects.fd= 1;
//
//
//
//if (cmd->input_redir_type == 4 || cmd->input_redir_type == 6)
//{
//printf("--- Input Redirections or Heredoc ---\n");
//printf("Input redirection type: %s\n", token_type_names_parser[cmd->input_redir_type]);
//printf("File Descriptor (fd): %d\n", cmd->in_redirects.fd);
//printf("File Name: %s\n", cmd->in_redirects.file_name != NULL ? cmd->in_redirects.file_name : "None");
//printf("Stop Heredoc: %s\n", cmd->in_redirects.stop_heredoc != NULL ? cmd->in_redirects.stop_heredoc : "None");
//printf("New Heredoc File: %s\n", cmd->in_redirects.new_heredoc_file != NULL ? cmd->in_redirects.new_heredoc_file : "None");
//}
//
//if (cmd->output_redir_type == 5 || cmd->output_redir_type == 7)
//{
//printf("--- Out Redirection --- \n");
//printf("Output redirection type: %s\n", token_type_names_parser[cmd->output_redir_type]);
//printf("File Descriptor (fd): %d\n", cmd->out_redirects.fd);
//printf("File Name: %s\n", cmd->out_redirects.file_name != NULL ? cmd->out_redirects.file_name : "None");
//}
//
////redirects  in lexor for precheck if file exists
//t_token *token;
//t_list	*m.tlist;
//
//token = (t_token *)m.tlist->value;
//
//while (m.tlist != NULL)
//{
//token = (t_token *)m.tlist->value;
//printf("[%d]: filename %s type: %s\n", i, token->str, token_type_names[token->type]); // casted to char since in the first test we want to print a word
//m.tlist = m.tlist->next;
//i++;
//}
//// if token->str == REDIRECT_IN then check access for next token->str
//path is at pwd_path()



int free_execve_fail(t_minishell *m)
{
    free_env(m->path_buf);
    free_env(m->env_lib);
    free_env(m->envp_lib);
    free(m->line);
    return(0);
}

int	execute_program(char **arg_vec, char *path, t_minishell *m, int process_n)
{
    printf("Command to run is: %s\n", path);
 // maybe builtins go here?
 // if (builtins == true)
 //   {
 // find correct builtin and run and free
//    }
// else -> run execve
    int i;
    i = 0;
    while (arg_vec[i])
    {
        printf("Arg_vec : %s\n", arg_vec[i]);

       i++;
    }

    if (execve(path, arg_vec, NULL) == -1)
    {
        perror("Could not execute");
        (void)m;
        (void)process_n;
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
        printf("error here2\n");

        cmd = (t_command *) temp->value;
        (void)cmd;
        if (cmd->input_redir_type == REDIRECT_IN)
        {
            printf("error here3\n");
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
        temp = temp->next;
        printf("error here\n");
    }
    return (0);
}

int out_redirections(t_minishell *m)
{
    t_command *cmd;
    t_list *temp;

    temp = m->clist;
    while(temp)
    {
        printf("error here2\n");

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


int executor(t_minishell m, char **envp)
{
    m.pipe_n = command_count(m.tlist) - 1;
    m.child_id = malloc(sizeof(int) * (m.pipe_n +1));
    m.path_buf = find_path_executor(envp);

    printf("\n------Start---------\n");
    printf("---Executor starts here---\n");
    in_redirections(&m);
    printf("error here3\n");

    if (m.pipe_n == 0)
    {
        printf("error here 0 pipes\n");

        single_cmd(&m);
        //waitpid(m.child_id[0], NULL, 0);
    }
    if (m.pipe_n > 0)
    {
        initialize_pipes(&m);
        multiple_cmd(&m);
    }
//    out_redirections(&m);

    wait_processes(&m); // Here is a traditional way to place wait
    printf("------End---------\n");
    return (0);
}
