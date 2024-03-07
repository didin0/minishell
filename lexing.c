/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabbadi <mabbadi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 17:00:39 by mabbadi           #+#    #+#             */
/*   Updated: 2024/03/07 09:40:35 by rsainas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
			Split the user input into multiple node and put it in a linked list.
			The user input is splited in a node when it occur an escape ' '.
			If the user input contain chars : '|' or '>' or '<' or '>>' or '<<'
			The input is splitted and the token is placed in a separated node.
@general	If the token size is known then we use the is_token() else
			we use splitting_lexer()
*/

int	is_token(char *c, int i)
{
	if (c[i] == '|')
		return (1);
	else if (c[i] == '<' || c[i] == '>')
	{
		if (c[i] == '<' && c[i + 1] == '<')
			return (2);
		if (c[i] == '>' && c[i + 1] == '>')
			return (2);
		else
			return (1);
	}
	else if (c[i] == '$' && c[i + 1] == '?')
		return (2);
	return (0);
}

int	add_substr_to_list(t_lexer **lexer_list, char *buff, char *line, int i,
		int ibis)
{
	buff = ft_substr(line, ibis, i - ibis);
	if (!buff)
		return (1);
	ft_lstlex_add_back(lexer_list, ft_lstlex_new(buff));
	return (0);
}

/*
@glance			loop leading space, loop each char in line.
				return list of tokens.
@var			ibis is the first unporcessed character and i the last 
 				char of a word.
@is_token		token here is a redirection or a pipe
@if is_token	makes list nodes from redirections and pipe
				ibis !=i ......TODO
				if redir, store substring to buffer
@ft_lstlex_		append node with buffer string to the list
@elseif ' '		space inidcates next token, add to list, deal wiht line end.
@elseif '"'		makes list node from double quoted token.
				first and second quote found, 
				update i to the second quote char.
@ft_strchr_from	look for second char " in line, return char pos + 1.
*/

t_lexer	*splitting_lexer(char *line, t_lexer **lexer_list)
{
	int		i;
	int		ibis;
	char	*buff;
	int		aff;

	i = 0;
	ibis = 0;
	buff = NULL;

	while (line && line[i] == ' ')
		i++;
	while (line && line[i])
	{
		if (is_token(line, i) || line[i] == '\0')
		{
			if (ibis != i)
			{
				printf("node created by ibis != i ???? \", from ibis %d to i %d \n", ibis, i);
				if (add_substr_to_list(lexer_list, buff, line, i, ibis) != 0)
					return (NULL);
			}
			if (is_token(line, i) == 2)
			{
				buff = ft_substr(line, i, 2);
				if (!buff)
					return (NULL);
				i++;
			}
			else
				buff = ft_substr(line, i, 1);
			if (!buff)
				return (NULL);
			printf("node created by is_token, return 1 2, from ibis %d to i %d\n", ibis, i);
			ft_lstlex_add_back(lexer_list, ft_lstlex_new(buff));
			while (line[i + 1] == ' ')
				i++;
			ibis = i + 1;
		}
		else if (line[i] == ' ' || line[i + 1] == '\0') 
		{
			while (line[i + 1] == ' ')
				i++;
			if (line[i + 1] == '\0')
			{
				i++;
				printf("node created by terminator \\0, from ibis %d to i %d \n", ibis, i);
				if (add_substr_to_list(lexer_list, buff, line, i, ibis) != 0)
					return (NULL);
				break ;
			}
			printf("node created by space, from ibis %d to i %d \n", ibis, i);
			if (add_substr_to_list(lexer_list, buff, line, i, ibis) != 0)
				return (NULL);
			ibis = i + 1;
		}
		else if (line[i] == '"' && ft_strchr_from(line, '"', i) != 0)
		{	
			i = ft_strchr_from(line, '"', i);
			printf("node created by double \", from ibis %d i %d \n", ibis + 1, i);
			if (add_substr_to_list(lexer_list, buff, line, i, ibis + 1) != 0)
				return (NULL);
//			if (line[i] != '"')
//			{
				i++;
//			}
			ibis = i + 1;
		}
		else if (line[i] == '\'' && ft_strchr_from(line, '\'', i) != 0)
		{
			printf("second single quote found on i %d\n", i);
			i = ft_strchr_from(line, '\'', i);
			printf("node created by single \', from ibis %d to i %d \n", ibis, i - 1);
			if (add_substr_to_list(lexer_list, buff, line, i, ibis + 1) != 0)
				return (NULL);
//			i++;
//			ibis = i + 1;
		}
		else if (line[i] == '$' && line[i + 1] != '?')
		{
			printf("Dollar sign found on i %d\n", i);
			i = ft_strchr_from(line, ' ', i);
			printf("node created by dollar sign for env var, from ibis %d to i %d \n", ibis, i - 1);
			if (add_substr_to_list(lexer_list, buff, line, i, ibis + 1) != 0)
				return (NULL);
//			i++;
//			ibis = i + 1;
		}
		i++;
	}
	return (*lexer_list);
}

void	lexing(t_data *data)
{
	data->lexer_list = ft_calloc(sizeof(t_lexer), 1);
	if (!data->lexer_list)
		ft_error(data);
	data->lexer_list = splitting_lexer(data->line, &data->lexer_list);
	if (!data->lexer_list)
		ft_error(data);
}
