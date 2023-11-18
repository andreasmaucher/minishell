#include "../minishell.h"

void error_handling_and_exit(char *error_msg)
{
    // errno = g_exit_code;
    //m->status_code;
    //perror(error_msg);
    write(STDERR_FILENO, error_msg, ft_strlen(error_msg));
    //printf("Error code: %d, Error message: %s\n", errno, strerror(errno));
    exit(g_exit_code);
}

void edge_cases(char *line)
{
    if (line[0] == '/')
        errno = 126;
    if (line[0] == '.' && line[1] == '/')
        errno = 126;
    if (line[0] == '.' && line[1] == ' ')
        errno = 2;
    g_exit_code = errno;
}