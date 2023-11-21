#include "../minishell.h"

int if_file_exists_delete(void *filename)
{
    if (access(filename, R_OK | W_OK) != 0)
        return (1);
    // else
    if (unlink(filename) != 0)
        return (1);
    else
        return(0);
        // error_handling_and_exit("Error removing file");
}