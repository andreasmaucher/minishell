/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrizakov <mrizakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 10:16:54 by amaucher          #+#    #+#             */
/*   Updated: 2023/11/22 00:07:01 by mrizakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdarg.h>
# include <string.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/ioctl.h>
# include <limits.h>
# include <sys/ioctl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <errno.h>

extern int	g_exit_code;
extern int	g_signal_switch;
typedef enum type
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
	char	*value;
}	t_dict;

/* value needs to be void since we want to be 
able to store multiple different types */
typedef struct s_list
{
	void			*value;
	struct s_list	*next;
	int				is_heredoc;
	int				is_append;
	char			*eof;
}	t_list;

typedef struct s_token
{
	int		type;
	char	*str;
}	t_token;

typedef enum cmd_type
{
	BUILTIN,
	PATH,
}	t_cmd_type;

/*
- fd: to indicate whether a file is open
- file_name: name of the new file e.g. hello > friend -> friend would 
be the file name in the output redirection
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
	t_cmd_type			type;
	bool				before_pipe;
	bool				after_pipe;
	t_type				input_redir_type;
	t_type				output_redir_type;
	char				**args;
	t_file				out_redirects;
	t_file				in_redirects;
	char				*path;
	t_list				*out_file;
	t_list				*in_file;
}					t_command;

typedef struct s_minishell
{
	int					tokens;
	t_list				*tlist;
	t_list				*clist;
	t_list				*envp;
	char				*line;
	char				*string_between_quotes;
	char				*token_str;
	t_type				token_type;
	t_token				token;
	struct sigaction	sa;
	int					pipe_n;
	int					*child_id;
	int					**pipes;
	char				**path_to_check;
	char				**path_buf;
	char				*here_docs;
	int					stdin_original;
	int					stdout_original;
	int					forked;
	int					current_process_id;
	int					status_code;
	int					status_code2;

}	t_minishell;

/* str_utils */
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strdup(const char *s);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strstr(const char *haystack, const char *needle);
char	*ft_strnstr(const char *big, const char *little, size_t len);
void	*ft_memset(void *s, int c, size_t n);
size_t	ft_strlen(const char *s);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
bool	ft_isalpha(int c);
bool	ft_is_digit(int c);
size_t	ft_strlcpy(char *dest, const char *src, size_t destsize);
void	*ft_memcpy(void *dest, const void *src, size_t n);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
char	*join_strings(const char *str1, const char *str2, const char *str3);





/* list operators */
int		lst_size(t_list *head);
t_list	*create_new_node(void *value);
void	insert_at_tail(t_list *head, t_list *new_value);
t_list	*return_tail_value(t_list *head);
t_list	*add_token_to_list(t_list **token_list, char *token_str,
			t_type token_type);
t_list	*find_previous_node(t_list *head, t_list *target_node);
void	ft_lstremove(t_list **lst, t_list *node, void (*del)(void *));
t_token	*add_token_type_and_str(char *token_str, t_type token_type);
void	delete_envp(void *arg);

/* parser */
t_list	*parser(t_minishell m);

/* itoa */
char	*ft_itoa(int n);

/* initialization */
void	*ft_memset(void *s, int c, size_t n);
void	init_signals(void);
void	*ft_memcpy(void *dest, const void *src, size_t n);

/* testing */
void	printlist(t_list *head);
void	print_command_list(t_list *clist);
void	printlist_envp(t_list *head);

/* lexer */
t_list	*split_line_into_tokens(t_minishell m);

/* env_library */
char	**create_envp_library(char **envp);
char	**create_env_library(char **envp);
t_list	*create_envp_list(char **envp);
char	*extract_key_from_envp(char *envp);
t_list	*delete_env_fail(t_list **tlist, t_list *current_node);

/* lexer_tokens */
char	*pipe_token(int *i, t_type *token_type);
char	*check_for_word_token(char *line, int *i, t_type *token_type);
char	*whitespace_token(char *line, int *i, t_type *token_type);
bool	check_for_metacharacter(char c);
bool	check_for_quotes(char c);
char	*redirection_token(char *line, int *i, t_type *token_type);

/* lexer_quotes */
char	*quotes(char *line, int *i, t_type *token_type, t_minishell m);
char	*single_quote_to_string(char *line, int *i);
char	*double_quote_to_string(char *line, int *i, t_minishell m);
char	*char_to_str(char c);
char	*append_str(char *str, char *appendix);

/* lexer_cleanup */
t_list	*cleanup_token_list(t_list *tlist);
t_list	*apply_function_to_list(t_list **tlist, t_list *(*f)(t_list **tlist,
				t_list *current_node));
t_list	*delete_whitespace(t_list **tlist, t_list *current_node);
t_list	*merge_tokens(t_list **tlist, t_list *current_node);
void	join_str_and_del_old_node(t_list *tlist, t_list *current_node,
			t_list *previous_node);

/* free_memory */
int		exit_shell(t_minishell m);
void	delete_cmd(void *arg);
void	*set_pt_to_null(void *ptr);
void	delete_token(void *content);
void	ft_lstdelone(t_list *lst, void (*del)(void*));
void	ft_lstclear(t_list **lst, void (*del)(void*));
void	free_all(t_minishell m);
void	delete_file(void *content);
void	free_out_file_list(t_list *out_file);
void	free_to_null(char *var);
void	free_intp_to_null(int *var);
void	free_all_the_og(t_minishell m);
void	free_cmd_the_og(t_command *cmd);

/* env */
char	*env_token(int *i, t_type *token_type,
			t_list *env_list, t_minishell *m);
char	*extract_env_name(char *line, int *i);
char	*env_within_double_quotes(char *line, int *i);
bool	check_if_part_of_library(t_list *envp, char *search_str);
char	*env_within_double_quotes(char *line, int *i);
char	*extract_env_name(char *line, int *i);
char	*extract_key_from_envp(char *envp);
void	add_new_envs(t_minishell *m, t_command *cmd);
char	*find_path_after_key(t_list *envp, char *search_str);
char	*ft_strncpy(char *desti, char *src, unsigned int n);

/* ft_split */
char	**ft_split(char const *s, char c);

/* parser */
void	cmd_input_redirection(t_list **tlist, t_list *clist);
void	cmd_output_redirection(t_list **tlist, t_list *clist);
void	cmd_pipe(t_list **clist, bool *new_cmd);
void	cmd_word(t_list **tlist, t_list *clist, bool *new_cmd);
t_list	*create_command_list(t_list **clist, t_command *tmp_cmd);
t_list	*setup_command_list(t_list **clist, t_list *tlist);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *new);
t_list	*ft_lstnew(void *content);
int		command_count(t_list *tlist);
void	add_token_to_command_list(t_list **token_list, char *token_info);
bool	check_parser_input(t_list *tlist);
int		token_count_tlist(t_list *tlist);

/* builtins */
int		arg_count(char **args);
char	*pwd_path(void);
int		pwd(void);
int		unset(t_minishell *m, t_command *cmd);
int		export(t_minishell *m, t_command *cmd);
int		env(t_minishell *m);
int		echo(t_minishell m, t_command *cmd);
int		execute_builtins(t_minishell *m, t_command *cmd);
char	*extract_search_str(t_command *cmd, int i);
bool	check_if_existing_env(t_minishell *m, t_command *cmd, int len);
int		calc_length_of_new_env_arr(t_minishell *m, t_command *cmd);
void	update_env_lib(t_minishell *m, t_command *cmd);
bool	check_for_key(t_minishell *m, t_command *cmd, int i, t_list *tmp);
int		cd(t_minishell *m, t_command *cmd);
int		exit_builtin(t_minishell *m, t_command *cmd);
t_list	*delete_double_envs(t_minishell *m, t_command *cmd);
void	add_new_envs(t_minishell *m, t_command *cmd);
bool	check_equal_sign(char *str);
bool	check_for_key_doubles(t_minishell *m, char *search_str, t_list *tmp);
char	*go_back_to_home(t_minishell *m, char *path);
char	*go_back_to_last_directory(t_minishell *m, char *path);
void	delete_node(t_minishell *m, char *search_key);
void	add_specific_envs(t_minishell *m, char *path, char *key);
char	*get_path(t_minishell *m, char *search_path);
int		count_cd_args(t_minishell *m, t_command *cmd);
bool	invalid_identifier(char *args);
int		execute_single_builtin_logic(t_minishell *m, t_command *cmd);

/* execution */
int		executor(t_minishell m, t_command *cmd, char **envp);
int		single_cmd(t_minishell *m, t_command *cmd, char **envp);
int		multiple_cmd(t_minishell *m, t_command *cmd, char **envp);
int		execute_program(char **arg_vec, t_command *cmd, t_minishell *m,
			char **envp);
int		initialize_pipes(t_minishell *m);
int		close_pipes(t_minishell *m);
void	kill_process(t_minishell *m, int process_id);
void	free_env(char **env);
int		free_execve_fail(t_minishell *m);
void	term_processes(t_minishell *m);
int		execute_single_builtins(t_minishell *m, t_command *cmd);
int		free_pipes(t_minishell *m);
void	free_args(char **args);
int		in_redirections_per_cmd(t_minishell *m, t_command *cmd);
int		check_file_rights(char *filename);
int		free_in_redirects_file(t_minishell *m);
void	free_all_the_og(t_minishell m);
void	free_cmd_the_og(t_command *cmd);
int		restore_stdin_stdout(void);
void	ft_heredoc(t_minishell *m, t_command *cmd);
t_list	*create_new_filename_node(void *value, char *eof);
t_list	*create_new_append_node(void *value);
int		output_redirect(t_minishell *m, t_command *cmd);
void	handle_sigint_child(int signum);
void	handle_sigint_switch(int signum);
void	handle_sigint_parent(int signum);
void	handle_sigint(int signal);
int		in_redirections_per_cmd_single_builtins(t_minishell *m, t_command *cmd);
int		check_if_file_can_be_opened(char *file);
int		init_executor(t_minishell *m);
int		initialize_pipes(t_minishell *m);
char	*valid_path(char **path, char *argv);
char	**find_path_executor(t_list *envp);
int		init_executor(t_minishell *m);
int		exit_executor(t_minishell *m);
int		close_pipes(t_minishell *m);
int		free_pipes(t_minishell *m);
int		initialize_pipes(t_minishell *m);
int		init_executor(t_minishell *m);
void	kill_process(t_minishell *m, int process_id);
int		wait_processes(t_minishell *m);
void	no_cmd(t_command *cmd, t_minishell *m);
void	free_all_filenames(t_command *cmd);








/* freeing */ 
void	free_intp_to_null(int *var);
void	free_to_null(char *var);
void	free_arr_to_null(char **arr);
void	free_all_filenames(t_command *cmd);
void	free_filename(char *filename);
void	ft_file_name_clear(t_list *lst);
void	free_m(t_minishell *m);
int		free_pipes(t_minishell *m);


/* exiting and error handling */
void	error_handling_and_exit(char *error_msg);

/* file_handling */
int	if_file_exists_delete(void *filename);
int output_redirect_file_write(t_list *tmp, t_command *cmd);


#endif
