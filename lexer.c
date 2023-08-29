#include "minishell.h"

//testing mr_testbranch
//test
//added this line for testing

/* typedef struct s_data
{
    char *input;


} t_data; */




void read_input(char *input)
{
    while (1) {
        input = readline("Myshell%");

        // Check for end-of-input or Ctrl+D
        if (input == NULL) {
            printf("\nExiting...\n");
            break;
        }

        // If the input is not empty, add it to the history
        if (strlen(input) > 0) {
            add_history(input);
        }

        printf("You entered: %s\n", input);

        // Free the allocated input buffer
        free(input);
    }
}

/* char **lexor(char *input)
{

} */


void free_all(char *input)
{
    free(input);
    while(tokens[i])
        free(tokens[i]);
    free(tokens);
}
int main(void)
{
    char *input;
    char **tokens;
    read_input(input);

    lexor(input);
    free_all();
    return 0;
}