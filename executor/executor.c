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
        // if (check_if_file_or_dir(argv) == 1)
        // {
        //     errno = 126;
        //     return (NULL);
        // }
        // if (access(argv, X_OK) == 0)
        //     return (argv);
		// else
		// 	return(NULL);
    }
    while (path[i])
    {
        correct_path = join_strings(path[i], "/", argv);
        if (check_if_file_can_be_opened(correct_path) == 0)
            return (correct_path);
        else
            free (correct_path);
        // if (check_if_file_or_dir(correct_path) == 1)
        // {
        //     errno = 126;
        //     return (NULL);
        // }
        // if (access(correct_path, X_OK) == 0)
        //     return (correct_path);
        // else
        //     free(correct_path);
        i++;
    }
    return (NULL);
}

// char	*valid_path(char **path, char *argv)
// {
//     int		i;
//     char	*correct_path;

//     i = 0;
//     correct_path = NULL;
//     if (argv != NULL && argv[0] == '/')
//     {
//         if (access(argv, X_OK) == 0)
//             return (argv);

//     }
//     // if (argv != NULL && argv[0] == '.' && argv[1] == '/')
//     // {
//     //     argv +=2;
//     //     if (access(argv, X_OK) == 0)
//     //         return (argv);
//     // }
//     // else
//     // if (argv != NULL && argv[0] == '/')
//     // {
//     //     if (access(argv, X_OK) == 0)
//     //     {
//     //         printf("This runs\n");
//     //         return (argv);
//     //     }
//     //     else
//     //         return(NULL);
//     // }
    
//         while (path[i])
//         {
//             correct_path = join_strings(path[i], "/", argv);
//             if (access(correct_path, X_OK) == 0)
//             {
//                 //g_exit_code = errno;
//                 return (correct_path);
//             }
//             else
//             {
//                 free(correct_path);
//             }
//             i++;
//         }
//         // if (errno == ENOENT)
//         //     perror("File does not exist.\n");
//         // else if (errno == EACCES)  
//         //     perror("Permission denied to execute the file\n");
//         // else
//         //     perror("Error!\n");
//     //g_exit_code = errno;
//     return (NULL);
// }
  
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

// void redirect_heredoc(char *filename)
// {
//     ssize_t fd;

//     printf("Filename is %s\n", filename);
//     fd = open(filename, O_RDONLY); // S_IRUSR | S_IWUSR
//     if (fd == -1) 
//         error_handling_and_exit("Couldn't open file");
//     // {
//     //     perror("Error opening file");
//     //     exit(EXIT_FAILURE);
//     // }
//     if (dup2(fd, STDIN_FILENO) == -1)
//         error_handling_and_exit("I/O redirection failed");
//     // {
//     //     perror("Couldnt redirect heredocs to STDIN\n");
//     //     close(fd);
//     //     exit(EXIT_FAILURE);
//     // }
//     close(fd);
//     if_file_exists_delete(filename);
//     // if (access(filename, R_OK | W_OK) == 0)
//     // {
//     //     if (unlink(filename) == 0)
//     //         printf("File '%s' removed successfully.\n", filename);
//     //     else 
//     //         perror("Error removing file");
//     // }
// }

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

int output_redirect(t_minishell *m, t_command *cmd)
{
    t_list *tmp;
    int     fd;
    // int original_stdout = dup(STDOUT_FILENO);


    tmp = cmd->out_file;
    while (tmp != NULL)
    {
        if (tmp->next == NULL)
        {
            if (tmp->is_append == 0)
            {
                if_file_exists_delete(tmp->value);
                cmd->out_redirects.fd_write = open(tmp->value, O_CREAT | O_RDWR, 0777);
                if (cmd->out_redirects.fd_write == -1)
                    perror("Could not open output file\n");
                dup2(cmd->out_redirects.fd_write, STDOUT_FILENO);
                close(cmd->out_redirects.fd_write);

                // dup2(original_stdout, STDOUT_FILENO); //
                // close(original_stdout);
                

            }
            if (tmp->is_append == 1)
            {
                write(1, "Output redirection function running\n", 37);
                cmd->out_redirects.fd_write = open(tmp->value, O_CREAT | O_RDWR | O_APPEND, 0777);
                if (cmd->out_redirects.fd_write == -1)
                    perror("Could not open output file\n");
                dup2(cmd->out_redirects.fd_write, STDOUT_FILENO);
                close(cmd->out_redirects.fd_write);
                //dup2(1, STDOUT_FILENO);
            }
            

        }
        else
        {
            if_file_exists_delete(tmp->value);
            fd = open(tmp->value, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR | S_IRWXO); // S_IRUSR | S_IWUSR
            if (fd == -1) 
            {
                perror("Error opening file");
                //m->status_code2 = errno;
                free_m(m);
                free_pipes(m);
                exit(errno);
            }
            close(fd);
        }
        tmp = tmp->next;
    }
    free_filename(cmd->out_redirects.file_name);
    return(0);
}

//int in_redirections(t_minishell *m)

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

int in_redirections_per_cmd_single_builtins(t_minishell *m, t_command *cmd)
{
    //1. Looping until the i get to the element previous to the last, checking if the file exists. 
    //If an invalid input file is found - error and exit
    //2. After that, the last file is checked for rights and only it is being run
    //Known issues - cmd->in_file->value is not a string, it is a node so freeing it is a bit tricky. We need a function which will free each node
    //t_command *tmp;
    t_list *tmp;

    tmp = cmd->in_file;
    while (tmp->next != NULL)
    {
        // write(1, (char *)tmp->value, strlen((char *)tmp->value));
        // write(1, "\n", 1);
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
                //free_to_null(cmd->path);
                free_pipes(m);
                //g_exit_code = EXIT_FAILURE;
                m->status_code2 = errno;
                //exit(m->status_code);

                // free_arr_to_null(m->path_buf);
                // free_to_null(m->line);
	            // if (m->tlist)
		        //     ft_lstclear(&m->tlist, delete_token);
	            // if (m->clist)
		        //     ft_lstclear(&m->clist, delete_cmd);
	            // if (m->envp)
		        //     ft_lstclear(&m->envp, delete_envp);
                // free_intp_to_null(m->child_id);
               
            }
        }
        tmp = tmp->next;
    }
    // write(1, (char *)tmp->value, strlen((char *)tmp->value));
    // write(1, "\n", 1);
    // write(1, "\n", 1);
    // write(1, "\n", 1);

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
                    //free_to_null(cmd->path);
                    free_pipes(m);
                    // g_exit_code = EXIT_FAILURE;
                    m->status_code2 = errno;
                    //exit(m->status_code);
                    // free_arr_to_null(m->path_buf);
                    // free_to_null(cmd->path);
                    // free_to_null(m->line);
	                // if (m->tlist)
		            //     ft_lstclear(&m->tlist, delete_token);
	                // if (m->clist)
		            //     ft_lstclear(&m->clist, delete_cmd);
	                // if (m->envp)
		            //     ft_lstclear(&m->envp, delete_envp);
                    // free_pipes(m);
                    // free_intp_to_null(m->child_id);
                    // g_exit_code = EXIT_FAILURE;
                    // exit(g_exit_code);
                }
        }
    return (0);
}


int in_redirections_per_cmd(t_minishell *m, t_command *cmd)
{
    //1. Looping until the i get to the element previous to the last, checking if the file exists. 
    //If an invalid input file is found - error and exit
    //2. After that, the last file is checked for rights and only it is being run
    //Known issues - cmd->in_file->value is not a string, it is a node so freeing it is a bit tricky. We need a function which will free each node
    //t_command *tmp;
    t_list *tmp;

    tmp = cmd->in_file;
    while (tmp->next != NULL)
    {
        // write(1, (char *)tmp->value, strlen((char *)tmp->value));
        // write(1, "\n", 1);
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
                //free_to_null(cmd->path);
                free_pipes(m);
                //g_exit_code = EXIT_FAILURE;
                m->status_code = errno;
                exit(m->status_code);

                // free_arr_to_null(m->path_buf);
                // free_to_null(m->line);
	            // if (m->tlist)
		        //     ft_lstclear(&m->tlist, delete_token);
	            // if (m->clist)
		        //     ft_lstclear(&m->clist, delete_cmd);
	            // if (m->envp)
		        //     ft_lstclear(&m->envp, delete_envp);
                // free_intp_to_null(m->child_id);
               
            }
        }
        tmp = tmp->next;
    }
    // write(1, (char *)tmp->value, strlen((char *)tmp->value));
    // write(1, "\n", 1);
    // write(1, "\n", 1);
    // write(1, "\n", 1);

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
                    //free_to_null(cmd->path);
                    free_pipes(m);
                    // g_exit_code = EXIT_FAILURE;
                    m->status_code = errno;
                    exit(m->status_code);
                    // free_arr_to_null(m->path_buf);
                    // free_to_null(cmd->path);
                    // free_to_null(m->line);
	                // if (m->tlist)
		            //     ft_lstclear(&m->tlist, delete_token);
	                // if (m->clist)
		            //     ft_lstclear(&m->clist, delete_cmd);
	                // if (m->envp)
		            //     ft_lstclear(&m->envp, delete_envp);
                    // free_pipes(m);
                    // free_intp_to_null(m->child_id);
                    // g_exit_code = EXIT_FAILURE;
                    // exit(g_exit_code);
                }
        }
    return (0);
}



// int in_redirections(t_minishell *m)
// {
//     t_command *cmd;
//     t_list *temp;
//     temp = NULL;

//     temp = m->clist;
//     while(temp)
//     {
//         cmd = (t_command *) temp->value;
//         // while  (cmd->input_redir_type == REDIRECT_IN && cmd->args == NULL) // add heredocs -this is a case for <file |echo hallo
//         // {
//         //     printf("Byebye!\n");
//         //     //exit(42);
//         // }
//         //printf("------Entered in redir loop for process N %d---------\n", m->pipe_n);
//         //write(1, "Entered in_redir function\n", 27);

//         while (cmd->input_redir_type == REDIRECT_IN && cmd->args == NULL) 
//         {
//             //printf("------Running in redir loop for process N %d---------\n", m->pipe_n);
//             //write(1, "Running input redirection\n", 27);
//             if (check_file_rights(cmd->in_redirects.file_name) == 0)
//             {
//                 cmd->in_redirects.fd_write = open(cmd->in_redirects.file_name, O_RDONLY, 0777);
//                 if (cmd->in_redirects.fd_write == -1)
//                     perror("Cant open the file\n");
//                 if (dup2(cmd->in_redirects.fd_write, STDIN_FILENO) == -1)
//                     perror("Input IN-redirection isn't working\n");
//                 close(cmd->in_redirects.fd_write);
//                 //printf("cmd->input_redir_type == REDIRECT_IN && cmd->args == NULL loop\n");
//             }
//             else
//             {
//                 perror("Cant read from input file, permission denied\n");
//                 free_filename(cmd->in_redirects.file_name);
//             }
//             temp = temp->next;
//         }
//         if (cmd->input_redir_type == REDIRECT_IN && cmd->args != NULL) 
//         {

//             //printf("------Running in redir loop for process N %d---------\n", m->pipe_n);
//             //write(1, "Running input redirection\n", 27);
//             if (check_file_rights(cmd->in_redirects.file_name) == 0)
//             {
//                 cmd->in_redirects.fd_write = open(cmd->in_redirects.file_name, O_RDONLY, 0777);
//                 if (cmd->in_redirects.fd_write == -1)
//                     perror("Cant open the file\n");
//                 if (dup2(cmd->in_redirects.fd_write, STDIN_FILENO) == -1)
//                     perror("Input IN-redirection isn't working\n");
//                 close(cmd->in_redirects.fd_write);
//                 //printf("cmd->input_redir_type == REDIRECT_IN && cmd->args != NULL loop\n");
//             }
//             else
//             {
//                 perror("Cant read from input file, permission denied\n");
//                 free_filename(cmd->in_redirects.file_name);
//             }
//             temp = temp->next;
//         }
//         if (cmd->input_redir_type == REDIRECT_HEREDOC)
//         {
//         }
//         // temp = temp->next;
//     }
//     return (0);
// }

// void no_cmd_no_exit(t_command *cmd, t_minishell *m)
// {
//     if (cmd->path == NULL)
//     {
//         free_filename(cmd->in_redirects.file_name);
//         free_filename(cmd->out_redirects.file_name);
//         free_filename(cmd->in_redirects.new_heredoc_file);
//         free_filename(cmd->in_redirects.stop_heredoc);
//         if (m->path_buf)
//             free_env(m->path_buf);
//         free_to_null(cmd->path);
//         free_to_null(m->line);
// 	    if (m->tlist)
// 	    	ft_lstclear(&m->tlist, delete_token);
// 	    if (m->clist)
// 		    ft_lstclear(&m->clist, delete_cmd);
// 	    if (m->envp)
// 		    ft_lstclear(&m->envp, delete_envp);
//         free(m->child_id);
//     }
// }

void no_cmd(t_command *cmd, t_minishell *m)
{
    if (cmd->path == NULL)
    {
        free_m(m);
        //free_all_filenames(cmd);
        // free_arr_to_null(m->path_buf); 
        //free_to_null(cmd->path);
        // free_to_null(m->line);
	    // if (m->tlist)
	    // 	ft_lstclear(&m->tlist, delete_token);
	    // if (m->clist)
		//     ft_lstclear(&m->clist, delete_cmd);
	    // if (m->envp)
		//     ft_lstclear(&m->envp, delete_envp);
        free_pipes(m);
        // free_intp_to_null(m->child_id);
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
        printf("Single cmd child process is running\n");
        cmd->path = NULL;
        if (cmd->input_redir_type == REDIRECT_IN || cmd->input_redir_type == REDIRECT_HEREDOC)
            in_redirections_per_cmd(m, cmd);
        if (cmd->output_redir_type == REDIRECT_OUT || cmd->output_redir_type == REDIRECT_APPEND)
            output_redirect(m, cmd);
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

int multiple_cmd(t_minishell *m, t_command *cmd, char **envp)
{
    m->forked = 1;
    m->child_id[m->current_process_id] = fork();
    if (m->child_id[m->current_process_id] == -1)
        error_handling_and_exit("Fork failed\n");
    if (m->child_id[m->current_process_id] == 0)
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
        output_redirect(m, cmd);
        close_pipes(m);
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

// can delete or rewrite
int free_execve_fail(t_minishell *m)
{
    free_env(m->path_buf);
    //! need to add new env-list
    free(m->line);
    return(0);
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
        // free_on_exit(m, cmd);
        if (cmd->in_file)
		    ft_file_name_clear(cmd->in_file);
        free_to_null(cmd->path);
        free_arr_to_null(arg_vec);
        // free_all(*m);
        // free_arr_to_null(m->path_buf);
        // free_intp_to_null(m->child_id);
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
        // if (m->pipes[i][0])
            close(m->pipes[i][0]);
        // if (m->pipes[i][1])
            close(m->pipes[i][1]);
        i++;
    }
    return (0);
}

//fix this function
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
    //m->stdin_original = dup(STDIN_FILENO);
    // m->stdout_original = dup(STDOUT_FILENO);
    if (m->pipe_n > 0)
    {
        initialize_pipes(m);
        m->forked = 1;
    }
    return (0);
}

int exit_executor(t_minishell *m)
{
    //int status;
    if (m->pipe_n > 0)
    {
        close_pipes(m);
        free_pipes(m);
    }
    // if (m->pipe_n != 0)
    // {
    m->status_code2 = wait_processes(m); // Here is a traditional way to place wait
	//printf("m->status_code2 after_wait process in exit)executor  is %d\n", m->status_code2);

    //}
   	free_arr_to_null(m->path_buf);

    // if (m->path_buf)
	// 	free_env(m->path_buf);
    free_intp_to_null(m->child_id);
    // free(m->child_id);
    m->forked = 0;
    return (m->status_code2);
}


int executor(t_minishell m, t_command *cmd, char **envp)
{
    t_list *tmp;
    //int original_stdout = dup(STDOUT_FILENO);

  
    init_executor(&m);
    tmp = m.clist;
    while(tmp)
    {
        cmd = (t_command *) tmp->value;
        if (cmd->input_redir_type == REDIRECT_HEREDOC || cmd->input_redir_type == REDIRECT_IN)
            ft_heredoc(&m, cmd);
        if (cmd->type == BUILTIN && m.pipe_n == 0)
        {
            // if (cmd->input_redir_type == REDIRECT_IN || cmd->input_redir_type == REDIRECT_HEREDOC)
            //     in_redirections_per_cmd(&m, cmd);
            // if (cmd->output_redir_type == REDIRECT_OUT || cmd->output_redir_type == REDIRECT_APPEND)
            //     output_redirect(&m, cmd);
            m.status_code2 = execute_single_builtin_logic(&m, cmd);
            //restore_stdin_stdout(&m);
            // restore_stdin_stdout_main();

            // dup2(original_stdout, STDOUT_FILENO); //
            // close(original_stdout);

            // if (restore_stdin_stdout() != 0)
			//     exit(42);
        	//printf("m->status_code2 in executor after running single_bulitins() is = %d\n", m.status_code2);

        }
        if (m.pipe_n == 0 && cmd->type != BUILTIN)
            single_cmd(&m, cmd, envp);
        if (m.pipe_n > 0)
            multiple_cmd(&m, cmd, envp);
        tmp = tmp->next;
    }
    m.status_code2 = exit_executor(&m);
    //printf("m->status_code after exit_executor in main is %d\n", m.status_code2);

    return (m.status_code2);
}