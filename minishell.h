/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skaraosm <skaraosm@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 12:36:01 by iogul             #+#    #+#             */
/*   Updated: 2026/03/15 13:22:31 by skaraosm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <sys/wait.h> 
# define _XOPEN_SOURCE 700

typedef struct s_gc
{
	void			*ptr;
	struct s_gc		*next;
}	t_gc;

typedef enum e_type
{
	WORD,
	PIPE,
	INPUT,
	TRUNC,
	HEREDOC,
	APPEND
}	t_type;

typedef struct s_token
{
	char			*value;
	t_type			type;
	int				is_expandable;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef enum e_redir_type
{
	R_INPUT,
	R_TRUNC,
	R_APPEND,
	R_HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*file;
	int				is_expandable;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			*cmd;
	char			**args;
	t_redir			*redirs;
	int				infile;
	int				outfile;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}	t_cmd;

void	*gc_malloc(size_t size);
void	gc_add_node(void *ptr);
void	gc_free_all(void);
t_gc	**get_gc_list(void);
t_token	*lexer(char *input, int *exit_status);
t_cmd	*parser(t_token *tokens, int *exit_status);
int		check_syntax(t_token *tokens, int *exit_status);
t_token	*ft_token_new(char *value, t_type type);
t_token	*ft_token_last(t_token *tokens);
void	ft_token_add_back(t_token **tokens, t_token *new);
t_env	*new_env_node(char *env_str);
t_env	*init_env(char **envp);
char	*get_env_value(char *key, t_env *env_list);
void	expander(t_token *tokens, t_env *env_list, int last_exit_status);
void	update_env_val(t_env **env, char *key, char *new_val);
t_env	*find_env_node(t_env *lst_env, char *search_key);
int		ft_cd(char **args, t_env *env);
int		ft_env(t_env *env);
int		ft_pwd(void);
int		executer(t_cmd *cmd, t_env *env, int lst_sts);
char	*find_path(char *command, char **envp);
char	**env_list_to_array(t_env *envp);
int		count_env_vars(t_env *envp);
char	*expand_string(char *str, t_env *env_list, int last_exit_status);
int		run_heredoc(t_redir *node, t_env *env, int index, int last_exit_status);
int		is_builtin(char *command);
int		exec_builtin(t_cmd *tmp, t_env *env, int *last_exit_status);
char	**get_paths_from_env(t_env *envp);
void	init_signals(void);
void	handle_sigint(int sig);
void	reset_signals(void);
void	ignore_signal(void);
void	merge_tokens(t_token *tokens);
void	ft_echo(char **args);
int		ft_export(char **args, t_env *env_list);
void	ft_unset(char **args, t_env **env_list);
int		finish_execution(t_cmd *cmd, int lst_sts, pid_t last_pid);
int		ft_exit(char **args, int *last_exit_status);
int		check_heredocs(t_cmd *cmd, t_env *env, int last_exit_status);
int		setup_file_descriptors(t_cmd *cmd);
void	init_shell_env(t_env **env_list);
void	add_new_env_helper(t_env **env, char *key, char *new_val);
int		count_env_vars(t_env *envp);
void	handle_exec_errors(char *arg, char *full_path);
int		exec_single_builtin(t_cmd *cmd, t_env *env, int *lst_sts);
char	*remove_quotes(char *str);
char	**split_unquoted(char *str);
char	*get_var_name(char *str, int *i);
char	*process_var(char *str, int *i, t_env *env_list, int status);
int		count_unquoted_words(char *str);
char	*expand_string(char *str, t_env *env_list, int last_exit_status);
void	expander(t_token *tokens, t_env *env_list, int last_exit_status);
void	handle_exec_errors(char *arg, char *full_path);
int		exec_single_builtin(t_cmd *cmd, t_env *env, int *lst_sts);
char	**get_paths_from_env(t_env *envp);
char	*find_path(char *command, char **paths);
int		is_builtin(char *command);
int		exec_builtin(t_cmd *tmp, t_env *env, int *last_exit_status);
int		run_heredoc(t_redir *node, t_env *env, int index, int lst_sts);
void	unlink_heredocs(t_cmd *cmd);
int		finish_execution(t_cmd *cmd, int lst_sts, pid_t last_pid);
int		check_heredocs(t_cmd *cmd, t_env *env, int last_exit_status);
int		setup_file_descriptors(t_cmd *cmd);
int		handle_word(char *input, int i, t_token **tokens);
int		handle_operator(char *input, int i, t_token **tokens);
int		handle_quotes(char *input, int i, t_token **tokens);
int		is_operator(char c);
int		check_unclosed_quotes(char *input, int *exit_status);
t_cmd	*parse_single_command(t_token **curr_token);
int		count_args(t_token *tokens);
void	handle_redirection(t_cmd *cmd, t_token **curr_token);
void	heredoc_loop(t_redir *node, t_env *env, int lst_sts, int fd);

#endif