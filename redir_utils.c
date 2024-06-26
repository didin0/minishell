/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsainas <rsainas@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 15:15:45 by rsainas           #+#    #+#             */
/*   Updated: 2024/05/03 13:16:54 by rsainas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	adv_list_size(t_lexer *list)
{
	int	i;

	i = 0;
	while (list)
	{
		list = list->next;
		i++;
	}
	return (i);
}

/*
@2nd if		in case there are several redirection within one cmd array
@3rd if		expand the exit status for 
@NULL		set the last pointer in cmd pointer array to NULL for
			execve() and safe looping.
*/

static char	**clean_cmd_from_redir(t_data *data, t_lexer *node, char **temp)
{
	int	k;

	k = 0;
	while (node && node->type != PIPE)
	{
		if (is_token(node->word, 0) && node->type != EXP_STATUS)
			node = node->next->next;
		if (((node && is_token(node->word, 0)) && node->type != PIPE)
			&& node->type != EXP_STATUS)
			node = node->next->next;
		if (node && node->type == EXP_STATUS)
		{
			temp[k] = ft_itoa(data->exit_status);
			node = node->next;
			k++;
		}
		if (node && node->type != PIPE)
			temp[k] = node->word;
		if (node)
			node = node->next;
		k++;
	}
	temp[k] = NULL;
	return (temp);
}

/*
@2nd if		in case there is a pipeline, the cur node is not the first node
 */

static char	**change_cmd(t_data *data, int i)
{
	char	**temp;
	t_lexer	*node;
	int		j;

	temp = ft_calloc(adv_list_size(data->lexer_list), sizeof(char *));
	if (!temp)
		ft_error(data, ERR_MALLOC, STDERR_FILENO, FREE_PAR);
//		ft_error(data);//TODO malloc failure
	node = NULL;
	node = keep_cur_node(node, ASK);
	j = 0;
	if (node != data->lexer_list)
	{
		node = data->lexer_list;
		while (node && j < i)
		{
			if (node->type == PIPE)
				j++;
			node = node->next;
		}
		keep_cur_node(node, ASSIGN);
	}
	temp = clean_cmd_from_redir(data, node, temp);
	return (temp);
}

static void	array_contains_redir(t_data *data)
{
	t_lexer	*node;

	node = data->lexer_list;
	node = keep_cur_node(node, ASK);
	while (node && node->type != PIPE)
	{
		if (is_token(node->word, 0))
		{
			if (node->type == REDIR_OUT || node->type == REDIR_OUT_APP
				|| node->type == REDIR_IN)
				redir_fd(data, node);
			else if (node->type == HERE_DOC)
				here_doc_in(data, node);
		}
		node = node->next;
	}
}

/*
@glance		look for redirection tokens, adjust the cmd
*/

char	**look_for_redirs(char **cmd, t_data *data, int i)
{
	t_lexer	*node;

	node = data->lexer_list;
	array_contains_redir(data);
	cmd = change_cmd(data, i);
	return (cmd);
}
