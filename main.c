/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 10:13:39 by amaucher          #+#    #+#             */
/*   Updated: 2023/08/23 10:13:42 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//used to print out the token types (only for testing purposes)
const char *token_type_names[] = 
{
	"WORD",
	"WHITESPACE",
    "PIPE",
    "REDIRECT_HEREDOC",
    "REDIRECT_APPEND",
    "REDIRECT_IN",
    "REDIRECT_OUT",
	"DOUBLE_QUOTES",
    "SINGLE_QUOTES"
};

//only for testing purposes, prints a list
void	printlist(t_list *head)
{
	t_list	*temporary;
	t_token *token;
	int i;

	i = 0;
	temporary = head;
	while (temporary != NULL)
	{
		token = (t_token *)temporary->value;
		printf("list[%d]: %s type: %s\n", i, token->str, token_type_names[token->type]); // casted to char since in the first test we want to print a word
		temporary = temporary->next;
		i++;
	}
}

/* determine the size of a list */
int	lst_size(t_list *head)
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

/* returns the value of the last node in a list */
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

/* creates a new node (that is to be added to a list in subsequent function) */
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

/* inserts a node at the end of a list */
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


t_token *add_token_type_and_str(char *str_with_all_tokens, t_type token_type)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	if (!token)
		return(NULL);
	token->str = str_with_all_tokens;
	token->type = token_type;
	return(token);
}

/* adds a new node to a list; in case the list is empty, the new node becomes the head, else 
it is added at the end of the list */
t_list *add_token_to_list(t_list **token_list, char *str_with_all_tokens, t_type token_type)
{
	t_list *new_node;
	t_token *data;

	data = add_token_type_and_str(str_with_all_tokens, token_type);
	if (!data)
		return (NULL);
	new_node = create_new_node(data);
	if (!*token_list)
		*token_list = new_node;
	else
		insert_at_tail(*token_list, new_node);
	return (*token_list);
}

bool	check_for_metacharacter(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '|' || c == '<' || c == '>')
		return (true);
	else
		return (false);
}

/* checks if c is a double or single quote */
bool	check_for_quotes(char c)
{
	if (c == '"' || c == '\'')
		return (true);
	else
		return (false);
}

/* function that gets called in case a redirection token was encountered in the input string;
determines the redirection type depending on the input and returns a duplicate */
char *redirection_token(char *line, int *i, t_type *token_type)
{
	if (line[*i] == '<' && line[*i + 1] == '<')
	{
		*token_type = REDIRECT_HEREDOC;
		(*i) += 2;
		return (ft_strdup("<<"));
	}
	else if (line[*i] == '>' && line[*i + 1] == '>')
	{
		*token_type = REDIRECT_APPEND;
		(*i) += 2;
		return (ft_strdup(">>"));
	}
	else if (line[*i] == '>') //! not detected correctly
	{
		*token_type = REDIRECT_OUT;
		(*i) += 1;
		return (ft_strdup(">"));
	}
	else
	{
		*token_type = REDIRECT_IN;
		(*i) += 1;
		return (ft_strdup("<"));
	}
}

/* function that gets called when a pipe token was detected in the input string; it sets the 
token type and returns a duplicate */
char *pipe_token(int *i, t_type *token_type)
{
	*token_type = PIPE;
	(*i)++;
	return(ft_strdup("|"));
}

/* each substring is malloced! */
char *check_for_word_token(char *line, int *i, t_type *token_type)
{
	int start_index;
	int end_index;

	*token_type = WORD;
	start_index = *i;
	while (check_for_metacharacter(line[*i]) == false && check_for_quotes(line[*i]) == false && line[*i] != '$' && line[*i])
		(*i)++;
	end_index = *i - start_index;
	return(ft_substr(line, start_index, end_index));
}

char *whitespace_token(char *line, int *i, t_type *token_type)
{
	int start_index;
	int end_index;

	*token_type = WHITESPACE;
	start_index = *i;
	while ((line[*i] == ' ' || line[*i] == '\t') && line[*i])
		(*i)++;
	end_index = *i - start_index;
	return(ft_substr(line, start_index, end_index));
}

//! this should be the expand function check for 
char *env_within_double_quotes(char *line, int *i)
{
	int start;
	int length;
	char *env_string;

	start = ++(*i); // pre-iterate since index is still at '$'
	if (line[*i] == '\0' || line[*i] == '"')
		return(ft_strdup("$"));
	while (line[*i] && check_for_metacharacter(line[*i]) == false && line[*i] != '\'' && line[*i] != '"' && line[*i] != '$')
		(*i)++;
	length = *i - start;
	env_string = malloc(sizeof(char) * (length + 1)); //! MALLOC
	if (!env_string)
		return (NULL);
	strncpy(env_string, &line[start], length);
	env_string[length] = '\0';
	printf("%s", env_string);
	return(env_string);
}

//! case if env is within double quote string
//! i need to have it in the same loop, but how do I get the lenght?!
//! cant use same logic with strncpy need to use append 
char *double_quote_to_string(char *line, int *i)
{
	char *str_between_quotes;
	char *env;
	int start;
	int length;

	start = *i;
	while (line[*i] != '"' && line[*i] != '\0')
	{
		if (line[*i] == '$')
		{	
			env = env_within_double_quotes(line, i);
			printf("%s", env);
		}
		(*i)++;
	}
	length = *i - start;
	str_between_quotes = malloc(sizeof(char) * (length + 1)); //! MALLOC
	if (!str_between_quotes)
		return (NULL);
	strncpy(str_between_quotes, &line[start], length);
	str_between_quotes[length] = '\0';
	if (line[*i])
		(*i)++;
	return(str_between_quotes);
}

char *single_quote_to_string(char *line, int *i)
{
	char *str_between_quotes;
	int start;
	int length;

	start = *i;
	length = 0;
	while (line[*i] && line[*i] != '\'')
		(*i)++;
	length = *i - start;
	str_between_quotes = malloc(sizeof(char) * (length + 1)); //! MALLOC
	if (!str_between_quotes)
		return (NULL);
	strncpy(str_between_quotes, &line[start], length);
	str_between_quotes[length] = '\0';
	if (line[*i])
		(*i)++;
	return(str_between_quotes);
}

char *single_or_double_quotes_token(char *line, int *i, t_type *token_type)
{
	if (line[*i] == '"')
	{
		*token_type = WORD;//DOUBLE_QUOTES; //!so in my version i don't even need this?!
		if (line[*i])
			(*i)++;
		return(double_quote_to_string(line, i));
	}
	else
	{
		*token_type = WORD;//SINGLE_QUOTES;
		if (line[*i])
			(*i)++;
		return(single_quote_to_string(line, i));
	}
}

t_list *find_previous_node(t_list *head, t_list *target_node)
{
    t_list *previous_node;
    t_list *current_node;
	
	current_node = head;
	previous_node = NULL;
    while (current_node != NULL && current_node != target_node)
    {
        previous_node = current_node;
        current_node = current_node->next;
    }
    return (previous_node);
}

void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	if (lst == 0 || del == 0)
		return ;
	(*del)(lst->value);
	free(lst);
}

void	ft_lstremove(t_list **lst, t_list *node, void (*del)(void *))
{
	t_list	*prev;

	if (!lst || !(*lst) || !node)
		return ;
	prev = find_previous_node(*lst, node);
	if (!prev)
		*lst = node->next;
	else
		prev->next = node->next;
	ft_lstdelone(node, del);
}

void token_del(void *content) {
    if (content == NULL) {
        return;
    }

    t_token *token = (t_token *)content;
    if (token->str != NULL) {
        free(token->str); // Assuming the str field is dynamically allocated.
    }
    
    free(token); // Free the token structure itself.
}

/* used for joining two consecutive word tokens; the function takes in a pointer to the head of the
list and two pointers to the current node and the previous node;
if the token type of the previous node is WORD the string contents of the two consecutive tokens are joined 
into one and one token is freed */
void join_str_and_del_old_node(t_list *tlist, t_list *current_node, t_list *previous_node)
{
	char	*new_joined_str;
	t_token *previous_token;

	previous_token = previous_node->value;
	if (previous_token->type == WORD)
	{
		new_joined_str = ft_strjoin(previous_token->str, ((t_token *)current_node->value)->str); 
		free(((t_token *)previous_node->value)->str);
		((t_token *)previous_node->value)->str = new_joined_str;
		ft_lstremove(&tlist, current_node, token_del);
	}
}

/* function that merges two consecutive word tokens (without spaces in between!)
e.g. l"s"; otherwise the parser would process this as two separate tokens */
t_list	*merge_words(t_list **tlist, t_list *current_node)
{
	t_list *previous_node;
	t_token	*current_token;

	previous_node = NULL;
	current_token = current_node->value;
	if (current_token->type == WORD)
	{
		previous_node = find_previous_node(*tlist, current_node);
		if (previous_node != NULL)
			join_str_and_del_old_node(*tlist, current_node, previous_node);
	}
	return (*tlist);
}

t_list	*delete_whitespace(t_list **tlist, t_list *current_node)
{
	t_token	*tmp_token;

	tmp_token = current_node->value;
	if (tmp_token->type == WHITESPACE)
		ft_lstremove(tlist, current_node, token_del);
	return(*tlist);
}

t_list	*modify_list(t_list **tlist, t_list *(*f)(t_list **tlist, t_list *current_node))
{
	t_list	*current_node;
	t_list	*next;

	current_node = *tlist;
	while (current_node)
	{
		next = current_node->next;
		*tlist = (*f)(tlist, current_node);
		current_node = next;
	}
	return (*tlist);
}

t_list	*cleanup_token_list(t_list *tlist)
{
	tlist = modify_list(&tlist, merge_words);
	tlist = modify_list(&tlist, delete_whitespace);
	return(tlist);
}

//! does not work because dict list is not set!!!
char	*get_env_value(t_list *dict, char *key, char **envp)
{
	///t_dict	*item;
	int	i;
	(void)dict;
	i = 0;
	if (!key)
		return (NULL);
/* 	while (dict)
	{
		//item = (t_dict *)dict->value;
 		if (!ft_strcmp(item->key, key))
			return (item->value);
		dict = dict->next;
	} */
	while (envp[i] != NULL)
	{
		if (envp[i] == key)
			return(envp[i]);
		i++;
	}
	return (NULL);
}

char **create_env_library(char **envp)
{
    char **buf = NULL;
    char target = '=';
    int len = 0;
    int substr_len;

    while (envp[len] != NULL)
        len++;
    buf = malloc(sizeof(char *) * (len + 1));
    if (!buf)
        return (NULL);
    len = 0; //! Don't get that logic fully 
    while (envp[len] != NULL)
    {
        char *target_pos = strchr(envp[len], target); //! LIBFT
        if (target_pos != NULL)
        {
            substr_len = target_pos - envp[len];
            buf[len] = malloc(substr_len + 1);
            if (!buf[len])
                return (NULL);
            strncpy(buf[len], envp[len], substr_len); //! LIBFT
            buf[len][substr_len] = '\0';
            printf("SUBSTR: %s\n", buf[len]);
        }
        else
        {
            buf[len] = NULL;
        }
        len++;
    }
    buf[len] = NULL;
    return (buf);
}


/* searches for the corresponding env to search_str within envp;
when assigning to the buffer path is iterated by one to skip the equal sign*/
char	**find_path(char **envp, char *search_str)
{
	int		i;
	char	*path;
	char	**path_buf;
	char	**env_lib;

	//(void)search_str;
	env_lib = create_env_library(envp);
	(void)env_lib;
	i = 0;
	while (ft_strnstr(envp[i], search_str, ft_strlen(search_str)) == NULL)
		i++;
	path = ft_strstr(envp[i], "=");
	if (path == NULL)
		return (NULL);
	path_buf = ft_split(++path, '\0');
	return (path_buf);
}

/* search_str is the string that needs to be found */
char	*extract_env_name(char *line, int *i, t_list *env, char **envp)
{
	int	start;
	int length;
	char	*search_str;
	char	*env_str = NULL; //!

	(void)env;
	start = *i;
	if (!line[*i] || line[*i] == '"')
		return(ft_strdup("$"));
	//! add g exit code
	while(line[*i] != '\0' && check_for_metacharacter(line[*i]) == false &&
	check_for_quotes(line[*i]) == false && line[*i] != '$')
		(*i)++;
	length = *i - start;
	search_str = ft_substr(line, start, length);
	env_str = *find_path(envp, search_str);
	free(search_str);
	return (ft_strdup(env_str));
}

/* extract the env_str from the input */
char	*env_token(char *line, int *i, t_type *token_type, t_list *env, char **envp)
{
	char	*env_name;

	(*i)++;
	*token_type = ENV;
	env_name = extract_env_name(line, i, env, envp);
	if (!ft_strcmp(env_name, "$"))
		*token_type = WORD;
	return (env_name);
}

// add multiple checks for all kind of delimiters e.g. parameter, quotes, whitespaces
t_list *split_line_into_tokens(t_minishell m, char **envp)
{
	int 	i;

	i = 0;
	m.tlist = NULL;
	while (m.line[i])
	{
		if (m.line[i] == '|')
			m.str_with_all_tokens = pipe_token(&i, &m.token_type);
		else if (m.line[i] == '<' || m.line[i] == '>')
			m.str_with_all_tokens = redirection_token(m.line, &i, &m.token_type);
		else if (m.line[i] == '$')
			m.str_with_all_tokens = env_token(m.line, &i, &m.token_type, m.env, envp);
		else if (m.line[i] == ' ' || m.line[i] == '\t')
			m.str_with_all_tokens = whitespace_token(m.line, &i, &m.token_type);
		else if (m.line[i] == '\'' || m.line[i] == '\"')
			m.str_with_all_tokens = single_or_double_quotes_token(m.line, &i, &m.token_type);
		else
			m.str_with_all_tokens = check_for_word_token(m.line, &i, &m.token_type);
		m.tlist = add_token_to_list(&m.tlist, m.str_with_all_tokens, m.token_type);
	}
	cleanup_token_list(m.tlist);
	return(m.tlist);
}

//! SEGFAULTS AT EXIT
/* to free up all memory in the end, including memory automatically allocated by readline function */
void	freememory(t_minishell m)
{
	t_list	*delete;

	while (lst_size(m.tlist) > 0)
	{
		delete = m.tlist;
		m.tlist = m.tlist->next;
		free(delete);
	}
	free(m.line);
}

int main(int ac, char **av, char **envp)
{
	t_minishell m;

	(void)av;
	//(void)envp;
	if (ac == 1)
	{
		while(1)
		{
			m.line = readline("Myshell: ");
			//m.env = envp;
			// Check for end-of-input or Ctrl+D
			if (m.line == NULL || ft_strcmp(m.line, "exit") == 0) {
				printf("\nExiting...\n");
				freememory(m);
				exit(1);
			}
			m.tlist = split_line_into_tokens(m, envp); // line that holds all the tokens
			printlist(m.tlist); //only for testing
			m.clist = parser(m);
			add_history(m.line);
		}
	}
	//! if ac != 1 terminate program immediately, we don't want any command line arguments except executable name
}
