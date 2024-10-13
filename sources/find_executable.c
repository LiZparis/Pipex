/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_executable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 17:30:39 by lzhang2           #+#    #+#             */
/*   Updated: 2024/10/13 19:34:49 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*ft_get_path_env(char **envp)
{
	int		i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	ft_putstr_fd("can't find env path", 2);
	return (NULL);
}

char	*build_full_path(const char *dir, const char *command)
{
	char	*full_path;

	full_path = malloc(ft_strlen(dir) + ft_strlen(command) + 2);
	if (!full_path)
		return (NULL);
	ft_strcpy(full_path, dir);
	ft_strcat(full_path, "/");
	ft_strcat(full_path, command);
	return (full_path);
}

char	*is_accessible(t_prog *prog, char *command)
{
	if (access(command, F_OK) == 0)
	{
		if (access(command, X_OK) == -1)
		{
			perror("Permission denied for commande");
			ft_free_prog(&prog);
			exit(126);
		}
		return (command);
	}
	return (NULL);
}

char	*find_executable(t_prog *prog, char *command, char **envp)
{
	int		i;

	prog->exec.path_env = ft_get_path_env(envp);
	if (!prog->exec.path_env)
		return (NULL);
	prog->exec.paths = ft_split(prog->exec.path_env, ':');
	if (!prog->exec.paths)
		return (NULL);
	i = -1;
	while (prog->exec.paths [++i])
	{
		prog->exec.full_path = build_full_path(prog->exec.paths[i], command);
		if (prog->exec.full_path)
		{
			prog->exec.access_ok = is_accessible(prog, prog->exec.full_path);
			if (prog->exec.access_ok)
			{
				ft_free_split(prog->exec.paths);
				return (prog->exec.access_ok);
			}
			free(prog->exec.full_path);
		}
	}
	ft_free_split(prog->exec.paths);
	return (NULL);
}

char	*find_abs_cmd(t_prog *prog, char *command)
{
	char	*access_ok;

	if (command[0] == '.' || command[0] == '/')
	{
		access_ok = is_accessible(prog, command);
		if (access_ok)
			return (access_ok);
	}
	return (NULL);
}
