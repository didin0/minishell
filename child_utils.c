/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsainas <rsainas@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:34:33 by rsainas           #+#    #+#             */
/*   Updated: 2024/04/30 07:52:50 by rsainas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parent_close_all_fds(t_data *data, int **pipefd)
{
	int	j;

	j = 0;
	while (j < data->pipe_count)
	{
		close(pipefd[j][0]);
		close(pipefd[j][1]);
		free(pipefd[j]);
		j++;
	}
	free(pipefd);
}

void	redirect_close_fds(t_data *data, int **pipefd, int i)
{
	if (i > 0)
	{
		dup2(pipefd[i - 1][0], STDIN_FILENO);
		close(pipefd[i - 1][0]);
	}
	if (i < data->pipe_count)
	{
		dup2(pipefd[i][1], STDOUT_FILENO);
		close(pipefd[i][1]);
	}
}

/*
Exmaple to illustrate the pattern of fd-s in a pipeline.

Child		0		1	 	2		3		4
pipefd[0]  r W	   R w	   r w	   r w 	   r w
pipefd[1]  r w	   r W	   R w	   r w 	   r w
pipefd[2]  r w	   r w	   r W	   R w 	   r w
pipefd[3]  r w	   r w	   r w	   r W 	   R w

So close all lowercase fd-s, duplicate and then close uppercase fd-s.
*/

void	close_unused_fds(t_data *data, int **pipefd, int i)
{
	int	j;

	j = 0;
	while (j < data->pipe_count)
	{
		if (i - j != 1)
			close(pipefd[j][0]);
		if (i != j)
			close(pipefd[j][1]);
		j++;
	}
}
