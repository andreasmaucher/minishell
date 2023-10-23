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

    m->child_id[0] = fork();
//    if (m->child_id[0] != 0)
//    {
//        printf("Child PID is :%d\n", getpid() )
//    }
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
        execute_program(m, cmd, 0);
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
                else if (current_process_id == m->pipe_n)
                {
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
                execute_program(m, cmd, current_process_id);
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

int free_execve_fail(t_minishell *m)
{
    free_env(m->path_buf);
    free_env(m->env_lib);
    free_env(m->envp_lib);
    free(m->line);
    return(0);
}

int	execute_program(t_minishell *m, t_command *cmd, int process_n)
{
    //! FREE MEMORY?! IF TYPE == BUILTIN
    if (cmd->type == BUILTIN)
    { 
        printf("EXECUTED\n");
        execute_builtins(m, cmd);
    }
    else
    (void)m;
    {
        printf("Command to run is: %s\n", cmd->path);

        if (execve(cmd->path, cmd->args, NULL) == -1)
        {
            perror("Could not execute");
            (void)cmd;
            (void)process_n;
            exit(1);
        }
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

int executor(t_minishell m, char **envp)
{
    m.pipe_n = command_count(m.tlist) - 1;
    m.child_id = malloc(sizeof(int) * (m.pipe_n +1));
    m.path_buf = find_path_executor(envp);

    printf("\n------Start---------\n");
    printf("---Executor starts here---\n");
    if (m.pipe_n == 0)
    {
        single_cmd(&m);
        //waitpid(m.child_id[0], NULL, 0);
    }
    if (m.pipe_n > 0)
    {
        initialize_pipes(&m);
        multiple_cmd(&m);
    }
    wait_processes(&m); // Here is a traditional way to place wait
    printf("------End---------\n");
    return (0);
}