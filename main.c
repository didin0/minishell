/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabbadi <mabbadi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 16:02:30 by mabbadi           #+#    #+#             */
/*   Updated: 2024/05/03 16:40:31 by rsainas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Debugging function to print a list
void	show_list(t_lexer *list)
{
	int	i;

	i = 0;
	while (list)
	{
		printf("list [%d] type (%d) ft_strlen(list->word) %zu :%s\n", i, list->type, ft_strlen(list->word), list->word);
		list = list->next;
		i++;
	}
}

void	show_env_list(t_env *list)
{
	int	i;

	i = 0;
	while (list)
	{
		printf("key [%s] value (%s)\n", list->key, list->value);
		list = list->next;
		i++;
	}
}

void	show_cmd(char ***cmd, t_data *data)
{
	int i;
	int j;
	int	cmd_count;

	cmd_count = count_token_type(data, BUILTIN, COMMAND);
	i = 0;
	while (i < cmd_count)
	{
		j = 0;
		while (cmd[i][j])
		{
			printf("cmd [%d] [%d] %s\n", i, j, cmd[i][j]);
			j++;
		}
		i++;
	}
}

static int	line_is_empty(t_data *data)
{
	if (!data->line)
			ft_error(data, "exit\n", STDOUT_FILENO, FREE_ENV);
	if (!adv_strncmp(data->line, "") || lexing(data))
	{
		free(data->line);
		return (1);
	}
	add_history(data->line);
	return (0);
}

/*
@if			handle ctrl-D EOF (signal/char) in shell interactive mode
			with exit message.
@2nd if		path for linebreak/enter on line

*/

int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	t_env	*env_list;

	(void)argc;	
	(void)argv;
	init_data(&data);
	env_list = get_env_to_list(&data, envp);
	while (1)
	{
		init_signals(&data);
		data.line = readline("\033[36mminishell :\033[m ");
		if (line_is_empty(&data))
			continue;
		token_type(&data, env_list);
//		parsing(&data, env_list);
		execution(&data, env_list);
	}
	free(data.line);//normal exit, is this at all viable path???
	return (0);
}
