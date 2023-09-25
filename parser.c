#include "minishell.h"

//used to print out the token types (only for testing purposes)
const char *token_type_names2[] = 
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

/* counts the amount of commands (len) in a token list, determined by the amount of PIPES;
iteration by 1 at the end to account for the fact, that a PIPE is always splitting two parts */
int command_count(t_list *tlist) 
{
    t_list *tmp_head;
    t_token *token;
    int len;
    
    tmp_head = tlist;
    len = 0;
    while (tmp_head != NULL) 
    {
        token = (t_token *)tmp_head->value;
        if (token->type == PIPE)
            len++;
        tmp_head = tmp_head->next;
    }
    len++;
    printf("CMD Count: %d\n", len);
    return(len);
}

//!
t_command	*ft_create_cmd(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->arguments = NULL;
	cmd->type = PATH;
	cmd->before_pipe = false;
	cmd->after_pipe = false;
	cmd->input_redir_or_heredoc = 0;
	cmd->out_redir_type = 0;
	cmd->inred_file = NULL;
	cmd->outred_file = NULL;
	return (cmd);
}

/* adds a new node to a list; in case the list is empty, the new node becomes the head, else 
it is added at the end of the list */
t_list *create_command_list(t_list **clist, t_command *tmp_cmd)
{
	t_list *new_node;
	
	new_node = create_new_node(tmp_cmd);
	if (!*clist)
		*clist = new_node;
	else
		insert_at_tail(*clist, new_node);
	return (*clist);
}

void print_command_list(t_list *clist) {
    t_list *current = clist;

    while (current != NULL) {
        t_command *cmd = (t_command *)current->value;

        printf("Command Type: %d\n", cmd->type);
        printf("Before Pipe: %s\n", cmd->before_pipe ? "true" : "false");
        printf("After Pipe: %s\n", cmd->after_pipe ? "true" : "false");
        printf("Input redir or heredoc: %s\n", token_type_names2[cmd->input_redir_or_heredoc]);
        //printf("list[%d]: %s type: %s\n", i, token->str, token_type_names[token->type]);
        
        // Check and print the inred_file and outred_file attributes
        //printf("In Redir File: %s\n", cmd->inred_file != NULL ? (char *)(cmd->inred_file) : "None");
        /* t_list *inred_list = cmd->inred_file;
        while (inred_list != NULL) {
            printf("In Redir File: %s\n", (char *)(inred_list->value));
            inred_list = inred_list->next;
        } */
        t_list *inred_list = cmd->inred_file;
        while (inred_list != NULL)
    {
        t_file *file = (t_file *)inred_list->value;
            printf("--- Input Redirections or Heredoc ---\n");
            printf("File Descriptor (fd): %d\n", file->fd);
            printf("Text to File: %s\n", file->text_to_file != NULL ? file->text_to_file : "None");
            printf("Stop Heredoc: %s\n", file->stop_heredoc != NULL ? file->stop_heredoc : "None");
            printf("Redirection Type: %s\n", token_type_names2[file->redirection_type]);
            printf("New Heredoc File: %s\n", file->new_heredoc_file != NULL ? file->new_heredoc_file : "None");
            inred_list = inred_list->next;
    }
        printf("Out Redir Type: %s\n", token_type_names2[cmd->out_redir_type]);
        //printf("Out Redir File: %s\n", cmd->outred_file != NULL ? (char *)(cmd->outred_file) : "None");
        t_list *outredir_list = cmd->outred_file;
        while (outredir_list != NULL) {
            t_file *file = (t_file *)outredir_list->value;
            printf("--- Out Redirection --- \n");
            printf("File Descriptor (fd): %d\n", file->fd);
            printf("Text to File: %s\n", file->text_to_file != NULL ? file->text_to_file : "None");
            printf("Stop Heredoc: %s\n", file->stop_heredoc != NULL ? file->stop_heredoc : "None");
            printf("Redirection Type: %s\n", token_type_names2[file->redirection_type]);
            printf("New Heredoc File: %s\n", file->new_heredoc_file != NULL ? file->new_heredoc_file : "None");
            outredir_list = outredir_list->next;
        }
        ///printf("Arguments: %s\n", (char *)cmd->arguments);
        t_list *arguments_list = cmd->arguments;
        while (arguments_list != NULL) {
            printf("Argument: %s\n", (char *)(arguments_list->value));
            arguments_list = arguments_list->next;
        }
        //printf("Arguments: %s\n", cmd->arguments != NULL ? (char *)(cmd->arguments) : "None");

        // Move to the next node
        current = current->next;
    }
}

/* initialization of clist to NULL, to ensure that it starts as an empty list */
t_list *setup_command_list(t_list **clist, t_list *tlist)
{
    int len;
    t_command   *tmp_cmd;

    *clist = NULL;
    len = command_count(tlist);
    while (len--)
    {
        tmp_cmd = ft_create_cmd();
        *clist = create_command_list(clist, tmp_cmd);
    }
    return(*clist);
}

/* modifies the attributes of the command node in the list that m.clist points to */
void cmd_pipe(t_list **clist, bool *first_word)
{
    t_command *tmp_cmd;
    t_list *tmp_head;

    tmp_head = *clist;
    tmp_cmd = (t_command *) tmp_head->value;
    tmp_cmd->before_pipe = true;
    tmp_head = tmp_head->next;
    tmp_cmd = (t_command *) tmp_head->value;
    tmp_cmd->after_pipe = true;
    *first_word = true;
}

/* generates a unique filename for a heredoc file by using a static index that increments with each call;
converting the index to a string so that it can be joined with the path;
each iteration creates a unique filename */
//! MALLOC
static char *create_heredoc_file(void)
{
    static int index = 0;
    char        *filename;
    char        *str;

    str = ft_itoa(index++);
    filename = ft_strjoin("/tmp/.heredoc_", str);
    return(filename);
}

void add_token_to_command_list(t_list **token_list, char *token_info)
{
	t_list *new_node;

	new_node = create_new_node(token_info);
	if (!*token_list)
		*token_list = new_node;
	else
		insert_at_tail(*token_list, new_node);
}

t_list	*ft_lstlast(t_list *lst)
{
	if (lst == 0)
		return (0);
	while (lst->next != 0)
	{
		lst = lst->next;
	}
	return (lst);
}

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*prev;

	if (lst == 0 || new == 0)
		return ;
	if (*lst == 0)
		*lst = new;
	else
	{
		prev = ft_lstlast(*lst);
		prev->next = new;
	}
}

t_list	*ft_lstnew(void *content)
{
	t_list	*node;

	node = malloc(sizeof(t_list));
	if (node == 0)
		return (0);
	node->value = content;
	node->next = 0;
	return (node);
}

/* e.g. < for input redirection: wc -l < file2 returns the amount of words in one file
e.g. > for output redirection: wc -l < file2 > file1 creates a new file1 and writes the wordcount in it
e.g. >> append: echo "hello again" >> file2 this adds hello again to file 2 
e.g. << redirect heredoc: cat <<EOF >> output.txt in this case the heredoc allows to write directly in the terminal
until 'EOF' is typed; in a 2nd step it is then appended to the output.txt file;
it's necessary to have a separate structure for file to be able to create as many structures as needed */
void cmd_input_redirection(t_list **tlist, t_list *clist)
{
    t_file  *file;
    t_command *tmp_cmd;
    t_token *tmp_token;

    file = malloc(sizeof(t_file));
    if (!file)
        return;
    tmp_cmd = (t_command *) clist->value;
    tmp_token = (t_token *) (* tlist)->value;
    tmp_cmd->input_redir_or_heredoc = tmp_token->type; //! if not used later, delete again
    file->redirection_type = tmp_token->type; //! if not used later, delete again
    if (*tlist != NULL) //! this is to avoid segfaults, but also need to be careful of case without anything after <
        *tlist = (* tlist)->next;
    tmp_token = (t_token *)(* tlist)->value;
    file->fd = -1;
    if (tmp_token->type == REDIRECT_HEREDOC)
    {
        file->stop_heredoc = ft_strdup(tmp_token->str);
        file->new_heredoc_file = create_heredoc_file();
        file->text_to_file = NULL;
    }
    else if (tmp_token->type == REDIRECT_IN)
    {
        file->text_to_file = ft_strdup(tmp_token->str);
        file->stop_heredoc = NULL;
        file->new_heredoc_file = NULL;
    }
    //new_node = create_new_node((void *)file);
    //! NOT WORKING
    add_token_to_command_list(&tmp_cmd->inred_file, (void *)file);
    //ft_lstadd_back(&(tmp_cmd->inred_file), ft_lstnew((void *)file));
    //insert_at_tail(tmp_cmd->inred_file, create_new_node((void *)file)); //! WHATS THE LIST???
}

//! needs addition in case of command within string
void cmd_word(t_list *tlist, t_list *clist, bool *first_word)
{
    t_token *tmp_token;
    t_command *tmp_command;

    tmp_command = (t_command *) clist->value;
    tmp_token = (t_token *) tlist->value;
    add_token_to_command_list(&tmp_command->arguments, ft_strdup(tmp_token->str));
    *first_word = false;
}

/* for output redirections a new file needs to be created to store whatever is entered
before the redirection sign; */
void    cmd_output_redirection(t_list **tlist, t_list *clist)
{
    t_file  *file;
    t_command *tmp_cmd;
    t_token *tmp_token;

    file = malloc(sizeof(t_file));
    if (!file)
        return;
    tmp_cmd = (t_command *) clist->value;
    tmp_token = (t_token *) (* tlist)->value;
    tmp_cmd->out_redir_type = tmp_token->type;
    file->redirection_type = tmp_token->type; //! CHANGE FOR INPUT
    if (*tlist != NULL) //! NECESSARY?
        *tlist = (* tlist)->next;
    tmp_token = (t_token *)(* tlist)->value;
    file->fd = -1;
    file->text_to_file = ft_strdup(tmp_token->str);
    file->stop_heredoc = NULL;
    file->new_heredoc_file = NULL;
    add_token_to_command_list(&tmp_cmd->outred_file, (void *)file);
}

/* function that adds attributes to the command list (m.clist);
- assign the value of a node in the token list to tmp_token
- depending on it's type execute a subsequent function to add the attributes
to the command list
*/
static void add_attributes_to_command_list(t_minishell m)
{
    t_list *tmp_tlist;
    t_token *tmp_token;
    bool    first_word;

    first_word = true;
    tmp_tlist = m.tlist;
    while (tmp_tlist != NULL)
    {
        tmp_token = (t_token *)tmp_tlist->value;
        if (tmp_token->type == PIPE)
            cmd_pipe(&m.clist, &first_word);
        else if (tmp_token->type == REDIRECT_IN ||tmp_token->type == REDIRECT_HEREDOC)
            cmd_input_redirection(&tmp_tlist, m.clist);
        else if (tmp_token->type == WORD)
            cmd_word(tmp_tlist, m.clist, &first_word);
        else if (tmp_token->type == REDIRECT_OUT || tmp_token->type == REDIRECT_APPEND)
            cmd_output_redirection(&tmp_tlist, m.clist);
        tmp_tlist = tmp_tlist->next;
    }
}

t_list *parser(t_minishell m)
{
    //! add a check if input is valid or is this handled at the end of lexer?
    // always needs to be sth. after < & << otherwise segfault
    m.clist = setup_command_list(&m.clist, m.tlist);
    if (!m.clist)
        return (NULL);
    add_attributes_to_command_list(m);
    print_command_list(m.clist); //only for testing to see if attributes changed
    return(m.clist);
}