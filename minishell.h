/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabbadi <mabbadi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 16:36:10 by mabbadi           #+#    #+#             */
/*   Updated: 2024/02/29 11:59:59 by mabbadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>

// linked list to handle user input
typedef struct s_lexer
{
	char			*word;
	struct s_lexer	*next;
}					t_lexer;

// GENERAL STRUCT
typedef struct s_data
{
	char			*line;
	t_lexer			*lexer_list;
}					t_data;

// Utils
void	free_array(char **str);
void	init_data(t_data *data, t_lexer *lexer_list);

// List
t_lexer	*ft_lstlex_new(void *word);
void	ft_lstlex_add_back(t_lexer **lst, t_lexer *new);

// Lexer
void	lexing(t_data *data);
t_lexer	*splitting_lexer(char *line, t_lexer **lexer_list);
void add_substr_to_list(t_lexer **lexer_list, char *buff, char *line, int i, int ibis);
int	is_token(char *c, int i);

#endif 