#include "../minishell.h"

void error_handling_and_exit(char *error_msg)
{
    (void)error_msg;
    //error_msg = strerror(errno);
    // errno = g_exit_code;
    //m->status_code;
    //perror(error_msg);
    write(STDERR_FILENO, strerror(errno), ft_strlen(strerror(errno)));
    //printf("Error code: %d, Error message: %s\n", errno, strerror(errno));
    exit(errno);
}

void edge_cases(char *line)
{
    if (line[0] == '/')
        errno = 126;
    if (line[0] == '.' && line[1] == '/')
        errno = 126;
    if (line[0] == '.' && line[1] == ' ')
        errno = 2;
    // g_exit_cod = errno;
}