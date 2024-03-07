/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabbadi <mabbadi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 15:49:53 by mabbadi           #+#    #+#             */
/*   Updated: 2024/03/02 12:16:22 by rsainas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Add a node to the end of the linked list type 't_lexer'
void	ft_lstlex_add_back(t_lexer **lst, t_lexer *new)
{
	t_lexer	*tmp;

	if (new == NULL)
		return ;
	if ((*lst)->word == NULL)
		*lst = new;
	else
	{
		tmp = *lst;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new;
	}
}

// Create a new node
t_lexer	*ft_lstlex_new(void *word)
{
	t_lexer *result;

	result = (t_lexer *)ft_calloc(sizeof(t_lexer), 1);
	result->word = malloc(sizeof(char *) + 1);
	if (!result)
		return (NULL);
	result->word = word;
	result->type = 0;
	result->next = NULL;
	return (result);
}
