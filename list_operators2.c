#include "minishell.h"

//only for testing purposes, prints a list
/* void	printlist(t_list *head)
{
	t_list	*temporary;
	t_token *token;
	int i;

	i = 0;
	temporary = head;
	while (temporary != NULL)
	{
		token = (t_token *)temporary->value;
		printf("list[%d]: %s\n", i, token->str); // casted to char since in the first test we want to print a word
		temporary = temporary->next;
		i++;
	}
	printf("\n");
} */