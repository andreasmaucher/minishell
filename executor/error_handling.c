#include "../minishell.h"

void error_handling_and_exit(char *error_msg)
{
    g_exit_code = errno;

    perror(error_msg);
    write(STDERR_FILENO, error_msg, ft_strlen(error_msg));
    printf("Error code: %d, Error message: %s\n", errno, strerror(errno));
    exit(g_exit_code);
}