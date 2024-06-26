/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabbadi <mabbadi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 16:36:10 by mabbadi           #+#    #+#             */
/*   Updated: 2024/05/03 18:58:38 by mabbadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>//for add_history on linux
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>//open
# include <sys/wait.h>//for waitpid on linux
# include <errno.h>//error number codes
# include <limits.h>//PATH_MAX
# include <signal.h>//signal

#define BUILTIN 0
#define COMMAND 1
#define PIPE 5
#define REDIR_IN 40
#define HERE_DOC 400
#define REDIR_OUT 41
#define REDIR_OUT_APP 411
#define EXP_STATUS 6
#define	F_FILE 10
#define OTHER 3
#define	ASSIGN 0
#define	ASK 1
#define	EMPTY 10000

#define OPEN_FLAGS (O_WRONLY | O_CREAT | O_APPEND)
#define OPEN_RIGHTS (S_IRUSR | S_IWUSR)

// Error defines
#define ERR_WRITE_FAIL "Write call failed\n"
#define ERR_MALLOC "Malloc failed\n"
#define ERR_READLINE "Readline fail of EOF sent to process\n"
#define ERR_UNSET "UNSET needs an argument\n"

#define NO_STDOUT 0
#define STDOUT 1 //duplicate error message to stdout
#define FREE_PAR 10
#define FREE_ENV 3

extern pid_t g_child_pid;
extern void rl_replace_line(const char *str, int i);

// linked list to copy the $ENV variable
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

// linked list to handle user input
// type 0 builtin TODO
//type 1 cmd, 2 cmd option, 3 cmd term/argument grep AAA grep a"asd 1"a
// 31 cmd argument properly double quoted grep "AAA"
// 32 cmd argument properly single quoted grep '<' 
// 40 redirect input 41 redirect output 400 heredoc redirect input between
// 411 redirect output in append mode
// 5 pipe, 6 exit status expansion, 7 variable to be expanded, 10 file

typedef struct s_lexer
{
	char			*word;
	int				type;
	struct s_lexer	*next;
}					t_lexer;

// GENERAL STRUCT
typedef struct s_data
{	
	char				*line;
	t_lexer				*lexer_list;
	int					exit_status;
	char				***cmd;
	int					cmd_count;
	int					pipe_count;
	int					list_size;
	struct sigaction	sa;
	t_env		 		*env_list;
}					t_data;

//Lexing splitting local struct
typedef struct s_stat
{
	int				i;
	int				ibis;
}					t_stat;


// Utils
void	free_array(char **str);
void	free_env_list(t_env *head);
void	init_data(t_data *data);
void	ft_error(t_data *data, const char *msg, int fd, int flag);
void	ft_error_errno(t_data *data, char **cmd);
	
// List
t_lexer	*ft_lstlex_new(void *word);
void	ft_lstlex_add_back(t_lexer **lst, t_lexer *new);
void	create_node_is_token(t_data *data, t_stat *stat, char *buff);
int		create_node_space_term(t_data *data, t_stat *stat, char *buff);
int		create_node_quotes(t_data *data, t_stat *stat, char *buff);
void	str_to_list(t_data *data, t_stat *stat, char *buff);
void	show_list(t_lexer *lexer_list);
void	show_env_list(t_env *list);

// Env
t_env	*get_env_to_list(t_data *data, char **envp);
char	**get_paths(t_data *data, t_env *env_list);
t_env	*create_env_node(t_data *data, char *key, char *value);
void	add_to_end(t_env **head, t_env *new_node);

// Exec
int		check_meaning(t_data *data);
char	***allocate_cmd(t_data *data);
int		count_token_type(t_data *data, int	type1, int type2);
int		**create_pipes(t_data *data);
char	 **organize_good_paths(char ***cmd, t_data *data, t_env *env_list);
int		execution(t_data *data, t_env *env_list);
char    *find_good_path(t_data *data, char *cmd, char **paths);
pid_t	*alloc_pids(t_data *data);
int		adv_strncmp(const char *s1, const char *s2);
void	exec_child(char*** cmd, t_env *env_list, t_data *data, pid_t *pids);
void	parent_close_all_fds(t_data *data, int **pipefd);
void	redirect_close_fds(t_data *data, int **pipefd, int i);
void	close_unused_fds(t_data *data, int **pipefd, int i);
int		count_tokens(t_data *data);
void	stat_from_waitpid(t_data *data, pid_t *pids);
t_lexer	*keep_cur_node(t_lexer *cur_node, int i);
void	update_cur_node(t_data *data, int i);
void	print_str_array(char **array, int len);

// Lexer
int	lexing(t_data *data);
int	add_substr_to_list(t_lexer **lexer_list, char *buff, char *line, int i, int ibis);
int	is_token(char *c, int i);
void	token_type(t_data *data, t_env *env_list);
int	is_cmd(t_data *data, t_lexer *token, t_env *env_list);
int	all_tokens_categorized(t_lexer *temp);
int	ft_strchr_from(char *s, char c, int i);
int	ft_strchr_end(char *s, char c, int i);
int	is_quote_closed(char *s, char c);
int is_builtin(t_data *data, char *word);


// Parser
t_lexer *parsing(t_data *data, t_env *env_list);
char	**ft_new_split(char const *s, char c);
int count_$(char *str);
int check_sq(char *str);
char	*clean_quote(char *str);

//Redirections

char	**look_for_redirs(char **cmd, t_data *data, int i);
void	redir_fd(t_data *data, t_lexer *node);
void	create_empty_file(char *name);
void	here_doc_in(t_data *data, t_lexer *node);
int		check_heredoc_meaning(t_lexer *node);
int		adv_list_size(t_lexer *list);

//Builtins
int	exec_builtin_parent(t_data *data, char **cmd, t_env *env_list);
int	exec_builtin_child(t_data *data, char **cmd, t_env *env_list);
void	pwd_builtin(t_data *data, t_env *env_list);
void	env_builtin(t_data *data, t_env *env_list);
void	cd_builtin(t_data *data, char **cmd, t_env *env_list);
void	export_builtin(t_data *data, char **cmd, t_env *env_list);
void	unset_builtin(t_data *data, char **cmd, t_env *env_list);
int		ft_isdigit_sign(char *str);
void	exit_builtin(t_data *data, char **cmd);
void	expand_status(t_data *data);

//Signals
void	init_signals(t_data *data);
void	sigint_handler(int signum);
void	show_cmd(char ***cmd, t_data *data);
#endif 
