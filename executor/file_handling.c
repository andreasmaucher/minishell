#include "../minishell.h"

void if_file_exists_delete(void *filename)
{
    // int status_code;
    if (access(filename, R_OK | W_OK) != 0)
    {
        // status_code = errno;
        // error_handling_and_exit("Error removing file");
        
    }
    else
    if (unlink(filename) != 0)
            error_handling_and_exit("Error removing file");
    // status_code = errno;
    // return 
}