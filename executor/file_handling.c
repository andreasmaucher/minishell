#include "../minishell.h"

void if_file_exists_delete(void *filename)
{
    if (access(filename, R_OK | W_OK) != 0)
    {
        g_exit_code = errno;
        // error_handling_and_exit("Error removing file");
        
    }
    else
    if (unlink(filename) != 0)
            error_handling_and_exit("Error removing file");
}