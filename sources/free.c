/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 15:14:34 by lzhang2           #+#    #+#             */
/*   Updated: 2024/10/10 20:36:46 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_free_split(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	ft_free_prog(t_prog *prog)
{
	if (prog)
	{
		close_unneeded_fds(prog);
		free(prog);
		prog = NULL;
	}
}

void	close_unneeded_fds(t_prog *prog)
{
	if (prog)
	{
		if (prog->infile != -1)
			close(prog->infile);
		if (prog->outfile != -1)
			close(prog->outfile);
		if (prog->pipe_fd[0] != -1)
			close(prog->pipe_fd[0]);
		if (prog->pipe_fd[1] != -1)
			close(prog->pipe_fd[1]);
	}
}




