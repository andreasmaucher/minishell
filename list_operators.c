#include "minishell.h"

//only for testing purposes, prints a list
/* void	printlist(t_list *head)
{
	t_list	*temporary;

	temporary = head;
	while (temporary != NULL)
	{
		printf("Stored in list: %s - ", (char *) temporary->value); // casted to char since in the first test we want to print a word
		temporary = temporary->next;
	}
	printf("\n");
} */