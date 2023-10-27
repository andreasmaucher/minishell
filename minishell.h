/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 10:16:54 by amaucher          #+#    #+#             */
/*   Updated: 2023/08/23 10:16:56 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <limits.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <fcntl.h>

extern int global_exit_code;

//token type (index starts at 0 e.g. word = 0, pipe = 2)
typedef enum
{
	NOT_SET,
	WORD,
	WHITESPACE,
    PIPE,
    REDIRECT_HEREDOC,
    REDIRECT_APPEND,
    REDIRECT_IN,
    REDIRECT_OUT,
	ENV,
	ENV_FAIL,
}	t_type;

typedef struct s_dict
{
	char	*key;
	void	*value;
}	t_dict;
typedef struct s_list
{
	void *value; //value needs to be void since we want to be able to store multiple different types
	struct s_list	*next;
}	t_list;

typedef struct s_token
{
	int		type;
	char	*str;
}	t_token;

//parser
typedef enum cmd_type
{
	BUILTIN,
	PATH,
}	t_cmd_type; //!

/*
- fd: to indicate whether a file is open
- file_name: name of the new file e.g. hello > friend -> friend would be the file name
in thie output redirection
- stop_heredoc: after typing this word the heredoc writing process stops
- redirection_type: stores the tyype of redirection e.g. heredoc, append etc.
- new_heredoc_file: name of the newly created heredoc file (stores the full path)
*/
typedef struct s_file
{
	int		fd;
	int		fd_read;
	int		fd_write;
	char	*file_name;
	char	*stop_heredoc;
	char	*new_heredoc_file;
}	t_file;
typedef struct s_command
{
	t_cmd_type		type; // BUILTIN OR PATH
	bool			before_pipe;
	bool			after_pipe;
	t_type	        input_redir_type;
	t_type			output_redir_type;
	char			**args;
	t_file			out_redirects;
	t_file			in_redirects;
	char            *path;
}					t_command;

typedef struct s_minishell
{
    int tokens;
    t_list *tlist; // for lexer, token list
    t_list *clist; // for parser, command list
	t_list	*envp;
    char *line;
    char *string_between_quotes;
	char 	*token_str;
    t_type token_type;
    t_token token;
	struct sigaction sa;
	int	pipe_n;
	int *child_id;
    int **pipes;
	char **path_to_check;
    char **path_buf;
	char *here_docs;
    int stdin_original;
	int stdout_original;
}   t_minishell;

//str_utils
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strdup(const char *s);
int	    ft_strcmp(const char *s1, const char *s2);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strstr(const char *haystack, const char *needle);
char	*ft_strnstr(const char *big, const char *little, size_t len);
void	*ft_memset(void *s, int c, size_t n);
size_t	ft_strlen(const char *s);
int		ft_strncmp(const char *s1, const char *s2, size_t n);

//list operators
int	lst_size(t_list *head);
t_list	*create_new_node(void *value);
void	insert_at_tail(t_list *head, t_list *new_value);
t_list	*return_tail_value(t_list *head);
t_list *add_token_to_list(t_list **token_list, char *token_str, t_type token_type);
t_list *find_previous_node(t_list *head, t_list *target_node);
void	ft_lstremove(t_list **lst, t_list *node, void (*del)(void *));
t_token *add_token_type_and_str(char *token_str, t_type token_type);
void env_del(void *content);
void	del_envp(void *arg);

//parser
t_list *parser(t_minishell m);

//itoa
char	*ft_itoa(int n);

//initialization
void	init_minishell_struct_and_signals(t_minishell *m, char **envp);
void	*ft_memset(void *s, int c, size_t n);
void	init_signals();

//testing
void	printlist(t_list *head);
void print_command_list(t_list *clist);
void	printlist_envp(t_list *head);

//lexer
t_list *split_line_into_tokens(t_minishell m);

//env_library
char **create_envp_library(char **envp);
char **create_env_library(char **envp);
t_list *create_envp_list(char **envp);
char    *extract_key_from_envp(char *envp);

//lexer_tokens
char *pipe_token(int *i, t_type *token_type);
char *check_for_word_token(char *line, int *i, t_type *token_type);
char *whitespace_token(char *line, int *i, t_type *token_type);
bool	check_for_metacharacter(char c);
bool	check_for_quotes(char c);
char *redirection_token(char *line, int *i, t_type *token_type);

//lexer_quotes
char *quotes(char *line, int *i, t_type *token_type, t_minishell m);
char *single_quote_to_string(char *line, int *i);
char *double_quote_to_string(char *line, int *i, t_minishell m);
char	*char_to_str(char c);
char	*append_str(char *str, char *appendix);

//lexer_cleanup
t_list	*cleanup_token_list(t_list *tlist);
t_list	*apply_function_to_list(t_list **tlist, t_list *(*f)(t_list **tlist, t_list *current_node));
t_list	*delete_whitespace(t_list **tlist, t_list *current_node);
t_list	*merge_tokens(t_list **tlist, t_list *current_node);
void join_str_and_del_old_node(t_list *tlist, t_list *current_node, t_list *previous_node);

//free_memory
int	exit_shell(t_minishell m);
void	command_del(void *arg);
void	*set_pt_to_null(void *ptr);
void token_del(void *content);
void	ft_lstdelone(t_list *lst, void (*del)(void*));
void	ft_lstclear(t_list **lst, void (*del)(void*));

//env
char	*env_token(char *line, int *i, t_type *token_type, t_list *env_list);
char	*extract_env_name(char *line, int *i);
char 	*env_within_double_quotes(char *line, int *i);
bool check_if_part_of_library(t_list *envp, char *search_str);
char *env_within_double_quotes(char *line, int *i);
char	*extract_env_name(char *line, int *i);
char    *extract_key_from_envp(char *envp);
void add_new_envs(t_minishell *m, t_command *cmd);
bool    check_for_key_doubles(t_minishell *m, char *search_str, t_list *tmp);
char	**find_path_after_key(t_list *envp, char *search_str);
char	**find_path(char **envp, char *search_str);
char	*find_env_value(t_list *envp, char *search_str);

//ft_split
char	**ft_split(char const *s, char c);

//parser
void cmd_input_redirection(t_list **tlist, t_list *clist);
void    cmd_output_redirection(t_list **tlist, t_list *clist);
void cmd_pipe(t_list **clist, bool *new_cmd);
void cmd_word(t_list **tlist, t_list *clist, bool *new_cmd);
t_list *create_command_list(t_list **clist, t_command *tmp_cmd);
t_list *setup_command_list(t_list **clist, t_list *tlist);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *new);
t_list	*ft_lstnew(void *content);
int command_count(t_list *tlist);
t_command	*ft_create_cmd(void);
void add_token_to_command_list(t_list **token_list, char *token_info);
bool check_parser_input(t_list *tlist);
int token_count_tlist(t_list *tlist);

//builtins
int arg_count(char **args);
char    *pwd_path(void);
int    pwd(void);
int unset(t_minishell *m, t_command *cmd);
int export(t_minishell *m, t_command *cmd);
int env(t_minishell *m);
int echo(t_minishell m, t_command *cmd);
int    execute_builtins(t_minishell *m, t_command *cmd);
char *extract_search_str(t_command *cmd, int i);
bool    check_if_existing_env(t_minishell *m, t_command *cmd, int len);
int calc_length_of_new_env_arr(t_minishell *m, t_command *cmd);
void    update_env_lib(t_minishell *m, t_command *cmd);
bool    check_for_key(t_minishell *m, t_command *cmd, int i, t_list *tmp);
int cd(t_minishell *m, t_command *cmd);

//execution
int executor(t_minishell m, char **envp);
int	execute_program(char **arg_vec, char *path);
int initialize_pipes(t_minishell *m);
int close_pipes(t_minishell *m);
void kill_process(t_minishell *m, int process_id);
void	free_env(char **env);
int     free_execve_fail(t_minishell *m);

char *ft_last_substring(const char *haystack, const char *needle);
char	*find_path_pwd(char **envp);
char *validate_path(char **path, char *argv, char **envp);




#endif
