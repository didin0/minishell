/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabbadi <mabbadi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 16:16:49 by abaccari          #+#    #+#             */
/*   Updated: 2024/05/23 17:42:27 by mabbadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Removes a substring from a string starting from a given position
char	*ft_strremove(char *s, int start, int n)
{
	int	len;
	int	i;

	len = 0;
	while (s[len])
		len++;
	if (start >= len)
		return (NULL);
	i = start;
	while (s[i + n] != '\0')
	{
		s[i] = s[i + n];
		i++;
	}
	s[i] = '\0';
	return (s);
}

// Determines the size of a key in a string (used in the expansion function)
int	key_size(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"' || str[i] == '$' || str[i] == ' ')
			break ;
		i++;
	}
	return (i);
}

// Performs variable expansion in a string based on the environment list
char *expen(char *str, t_env *env_list) {
    int size;
    char *remaining;
    char *result;

    if (ft_strlen(str) > 1)
        str++;
    size = key_size(str);

    while (env_list) {
        if (ft_strncmp(str, env_list->key, ft_strlen(env_list->key)) == 0) {
            remaining = ft_strdup(str + ft_strlen(env_list->key));
            result = malloc(ft_strlen(env_list->value) + ft_strlen(remaining) + 1);
            if (!result) {
                free(remaining);
                return NULL;
            }
            ft_strlcpy(result, env_list->value, ft_strlen(env_list->value) + 1);
            ft_strlcat(result, remaining, ft_strlen(result) + ft_strlen(remaining) + 1);
            free(remaining);
            return result;
        }
        env_list = env_list->next;
    }
    return ft_strremove(str, 0, size);
}


// Iterates over a word string and expend variables
void	parsing_loop(char **word, t_env *env_list)
{
	char	*new_str;
	int		before;
	char 	*expended;
	char 	*str;

	str = *word;
	while (*str)
	{
		expended = NULL;
		if (*str == '$')
		{
			before = str - *word + 1;
			expended = expen(str, env_list);
			if (expended)
			{
				new_str = malloc(ft_strlen(*word) + ft_strlen(expended) + 1);
				ft_strlcpy(new_str, *word, before);
				ft_strlcat(new_str, expended, ft_strlen(new_str) + ft_strlen(expended) + 1);
				free(*word);
				*word = new_str;
				free(expended);
				str = *word + before - 1;
			}
			while (*str + 1 == '$')
				str++;
		}
		str++;
	}
}

// Iterates over the list and applies variable expansion for each
t_lexer	*parsing(t_data *data, t_env *env_list)
{
	t_lexer	*lexer_list;

	lexer_list = data->lexer_list;
	while (lexer_list)
	{
		if (!check_sq(lexer_list->word))
			parsing_loop(&(lexer_list->word), env_list);
		lexer_list->word = clean_quote(lexer_list->word);
		lexer_list = lexer_list->next;
	}
	return (data->lexer_list);
}
