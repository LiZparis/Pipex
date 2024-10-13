/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 17:11:55 by lzhang2           #+#    #+#             */
/*   Updated: 2024/10/13 19:34:31 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	check_infile(t_prog *prog, char *infile)
{
	prog->infile = open(infile, O_RDONLY);
	if (prog->infile < 0)
	{
		if (errno == EACCES)
			perror(infile);
		else if (errno == ENOENT || errno == EISDIR)
			perror(infile);
		ft_free_prog(&prog);
		exit(EXIT_FAILURE);
	}
}

void	check_outfile(t_prog *prog, char *outfile)
{
	prog->outfile = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (prog->outfile < 0)
	{
		if (errno == EACCES)
			perror(outfile);
		else
			perror(outfile);
		ft_free_prog(&prog);
		exit(EXIT_FAILURE);
	}
}

void	is_execvable(t_prog *prog, char *path, char **args, char **envp)
{
	if (execve(path, args, envp) == -1)
	{
		perror("execve");
		free(path);
		ft_free_split(args);
		is_1st_or_2nd(prog);
	}
}

void	execute_command(t_prog *prog, char *cmd, char **envp)
{
	char	**args;
	char	*path;

	args = NULL;
	path = find_abs_cmd(prog, cmd);
	if (!path)
	{
		args = ft_split(cmd, ' ');
		if (!args)
		{
			ft_putstr_fd("Error: Failed to split command\n", 2);
			exit(EXIT_FAILURE);
		}
		path = find_executable(prog, args[0], envp);
		if (!path)
		{
			ft_free_split(args);
			ft_putstr_fd("Error: Command not found\n", 2);
			is_1st_or_2nd(prog);
		}
	}
	is_execvable(prog, path, args, envp);
	free(path);
	ft_free_split(args);
}

int	wait_for_children(pid_t pid1, pid_t pid2)
{
	int		status;
	int		exit_status;
	pid_t	pid;
	int		child_waited;

	exit_status = 0;
	child_waited = 0;
	while (child_waited < 2)
	{
		pid = wait(&status);
		if (pid == pid1 || pid == pid2)
		{
			if (WIFEXITED(status))
				exit_status = WEXITSTATUS(status);
		}
		child_waited++;
	}
	return (exit_status);
}
