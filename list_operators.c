#include "minishell.h"

t_list	*ft_lstnew(int content)
{
	t_list	*new_node;
	new_node = (t_list *)malloc(sizeof(*new_node));
	if (!new_node)
	{
		free(new_node);
		return (NULL);
	}
	new_node->content = content;
	new_node->next = NULL;
	return (new_node);
}

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*temp;
	temp = *lst;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	if (new == NULL)
		return ;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
}

void	printlist(t_list *head)
{
	t_list	*temp;
	temp = head;
	while (temp != NULL)
	{
		printf("%d         ", temp->content);
		printf("%d         ", temp->index);
		printf("%d\n", temp->local_index);
		temp = temp->next;
	}
}

int	ft_lstsize(t_list *lst)
{
	int		i;
	t_list	*temp;
	i = 0;
	temp = lst;
	if (lst == NULL)
		return (0);
	while (temp->next != NULL)
	{
		i++;
		temp = temp->next;
	}
	if (temp->next == NULL)
		i++;
	return (i);
}

void	free_list(t_list	*head)
{
	t_list	*current;
	t_list	*next;
	current = head;
	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}
}

/* determine the size of a list */
/* int	lst_size(t_list *head)
{
	int		lsize;
	t_list	*current_node_pos;

	lsize = 0;
	current_node_pos = head;
	while (current_node_pos != NULL)
	{
		current_node_pos = current_node_pos->next;
		lsize++;
	}
	return (lsize);
}

t_list	*return_tail_value(t_list *head)
{
	t_list	*current_node_pos;
	int		lsize;

	current_node_pos = head;
	lsize = lst_size(head);
	while (lsize > 1)
	{
		current_node_pos = current_node_pos->next;
		lsize--;
	}
	return (current_node_pos);
}

t_list	*create_new_node(void *value)
{
	t_list	*newnode;

	newnode = malloc(sizeof(t_list));
	if (newnode == NULL)
		return (NULL);
	newnode->value = value;
	newnode->next = NULL;
	return (newnode);
}

void	insert_at_tail(t_list *head, t_list *new_value)
{
	t_list	*current;

	if (head == 0 || new_value == 0)
		return ;
	current = head;
	while (current->next != NULL)
		current = current->next;
	current->next = new_value;
}

t_list *add_token_to_list(t_list **token_list, char *str_with_all_tokens, t_type token_type)
{
	t_list *new_node;
	t_token *data;

	data = add_token_type_and_str(str_with_all_tokens, token_type);
	if (!data)
		return (NULL);
	new_node = NULL;
	new_node = create_new_node(data);
	if (!*token_list)
		*token_list = new_node;
	else
		insert_at_tail(*token_list, new_node);
	return (*token_list);
}*/