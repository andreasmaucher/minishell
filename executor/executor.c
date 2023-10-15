
# include "../minishell.h"
// MR added
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
        waitpid(-1, &status, 0);
        i++;
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
        //manually filling args to test how a single cmd works
        cmd->args = malloc(sizeof(char *) * 3);
        cmd->args[0] = ft_strdup("ls");
        cmd->args[1] = ft_strdup("-la");
        cmd->args[2] = "\0";
        //manually filling args to test how a single cmd works
        cmd->path = valid_path(m->path_buf, cmd->args[0]);
        execute_program(cmd->args, cmd->path);
    }
    return (0);
}

int multiple_cmd(t_minishell *m)
{
    int current_process_id;

    current_process_id = 0;

    t_command *cmd = NULL;
    while(current_process_id <= m->pipe_n && m->clist != NULL)
    {
        cmd = (t_command *)m->clist->value;
        m->child_id[current_process_id] = fork();
        if (m->child_id[current_process_id] == 0)
        {
            printf("------Child process N %d running---------\n", current_process_id);
            //manually filling args to test how a single cmd works
            cmd->args = malloc(sizeof(char *) * 3);
            cmd->args[0] = ft_strdup("ls");
            cmd->args[1] = ft_strdup("-la");
            cmd->args[2] = "\0";
            //manually filling args to test how a single cmd works
            cmd->path = valid_path(m->path_buf, cmd->args[0]);
            execute_program(cmd->args, cmd->path);
            current_process_id++;
            printf("------Child process N %d finished---------\n", current_process_id);

        }


        current_process_id++;
    }

//    int current_process_id;
//    current_process_id = 0;
//    m->child_id = malloc(sizeof(int*) * m->pipe_n + 2);
//    m->child_id[m->pipe_n + 2] = '\0';
//    while(current_process_id != m->pipe_n + 1 )
//    {
//        t_command *cmd = (t_command *)m->clist->value;
//        m->child_id[current_process_id] = fork();
//        printf("------ERROR HERE---------\n");
//        //manually filling args to test how a single cmd works
//        cmd->args = malloc(sizeof(char *) * 3);
//        cmd->args[0] = ft_strdup("ls");
//        cmd->args[1] = ft_strdup("-la");
//        cmd->args[2] = '\0';
//        //manually filling args to test how a single cmd works
//        if (m->child_id[current_process_id] == 0)
//        {
//            cmd->path = valid_path(m->path_buf, cmd->args[0]);
//            execute_program(cmd->args, cmd->path);
//        }
//       // waitpid(m->child_id[current_process_id], NULL, 0);
//        m->clist->value = m->clist->next;
//        current_process_id++;
//    }
    return (0);
}


int	execute_program(char **arg_vec, char *path)
{
    if (execve(path, arg_vec, NULL) == -1)
    {
        perror("Could not execute");
        return (1);
    }
    return (0);
}

void executor(t_minishell m, char **envp)
{
    //t_list *current = m.clist;
    m.pipe_n = 1; // hardcoding to run with a single command
    m.child_id = malloc(sizeof(int) * (m.pipe_n +1));
    m.path_buf = find_path_executor(envp);
    printf("\n------Start---------\n");
    printf("---Executor starts here---\n");
    //t_command *cmd = (t_command *)m.clist->value;
    if (m.pipe_n == 0)
    {
        single_cmd(&m);
        //waitpid(m.child_id[0], NULL, 0);
//        cmd->path = valid_path(m.path_buf, cmd->args[0]);
//        execute_program(cmd->args, cmd->path);
    }
    if (m.pipe_n > 0)
    {
        multiple_cmd(&m);
//        cmd->path = valid_path(m.path_buf, cmd->args[0]);
//        execute_program(cmd->args, cmd->path);
    }
//    free(cmd->args[0]);
//    free(cmd->args[1]);
//    free(cmd->args[2]);
//    free(cmd->args);
    /*
    while (current != NULL) {
        t_command *cmd = (t_command *)current->value;
        t_list *arguments_list = cmd->arguments;

        printf("This line is to silence a warning - line was -, %s\n", m.line);
        //m.path_to_check = find_path_executor(envp);

        printf("Is this before pipe : %s\n", cmd->before_pipe ? "true" : "false");

        while (arguments_list != NULL)
        {
            printf("Argument is : %s\n", (char *)arguments_list->value);

            arguments_list = arguments_list->next;
        }
        current = current->next;
    }
//    pipex.path1 = valid_path(m.path_to_check, pipex.arg_vec1[ls 0]);
//m.clist
     */
    wait_processes(&m);
    printf("------End---------\n");
}

// MR added end