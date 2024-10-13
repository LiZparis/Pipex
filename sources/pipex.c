/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 17:19:46 by lzhang2           #+#    #+#             */
/*   Updated: 2024/10/13 17:06:54 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	init_struct_prog(t_prog *prog)
{
	prog->infile = -1;
	prog->outfile = -1;
	prog->pipe_fd[0] = -1;
	prog->pipe_fd[1] = -1;
	prog->pid1 = -1;
	prog->pid2 = -1;
	prog->is_1st_cmd = false;
}

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

void	ft_check_param(t_prog *prog, int argc)
{
	if (argc != 5)
	{
		ft_putstr_fd("argc should be 5\n", 2);
		ft_free_prog(&prog);
		exit(EXIT_FAILURE);
	}
}

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

char	*find_executable(t_prog *prog, char *command, char **envp)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	int		i;
	char	*access_ok;

	path_env = ft_get_path_env(envp);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		full_path = build_full_path(paths[i], command);
		if (full_path)
		{
			access_ok = is_accessible(prog, full_path);
			if (access_ok)
			{
				ft_free_split(paths);
				return (access_ok);
			}
			free(full_path);
		}
	}
	ft_free_split(paths);
	return (NULL);
}

void	is_1st_or_2nd(t_prog *prog)
{
	if (prog->is_1st_cmd == true)
	{
		ft_free_prog(&prog);
		exit(EXIT_FAILURE);
	}
	else if (prog->is_1st_cmd == false)
	{
		ft_free_prog(&prog);
		exit(127);
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

void	ft_child_1(t_prog *prog, char **argv, char **envp)
{
	prog->pid1 = fork();
	if (prog->pid1 == 0)
	{
		check_infile(prog, argv[1]);
		if ((dup2(prog->infile, STDIN_FILENO) == -1)
			|| (dup2(prog->pipe_fd[1], STDOUT_FILENO) == -1))
		{
			ft_putstr_fd("Failed to duplicate infile to stdin", 2);
			ft_free_prog(&prog);
			exit(EXIT_FAILURE);
		}
		close_unneeded_fd(prog);
		close_unneeded_pipe(prog);
		prog->is_1st_cmd = true;
		execute_command(prog, argv[2], envp);
	}
}

void	ft_child_2(t_prog *prog, char **argv, char **envp)
{

	prog->pid2 = fork();
	if (prog->pid2 == 0)
	{
		check_outfile(prog, argv[4]);
		if ((dup2(prog->pipe_fd[0], STDIN_FILENO) == -1)
			|| dup2(prog->outfile, STDOUT_FILENO) == -1)
		{
			ft_putstr_fd("Failed to duplicate infile to stdout", 2);
			ft_free_prog(&prog);
			exit(EXIT_FAILURE);
		}
		close_unneeded_fd(prog);
		close_unneeded_pipe(prog);
		prog->is_1st_cmd = false;
		execute_command(prog, argv[3], envp);
	}
}

int wait_for_children(pid_t pid1, pid_t pid2)
{
    int status;
    int exit_status = 0;

    for (int i = 0; i < 2; i++)
    {
        pid_t pid = wait(&status);
        if (pid == pid1 || pid == pid2)
        {
            if (WIFEXITED(status))
                exit_status = WEXITSTATUS(status);
        }
    }
    return exit_status;
}

int	main(int argc, char **argv, char **envp)
{
	t_prog	*prog;
	prog = malloc(sizeof(t_prog));
	if (!prog)
		return (1);
	init_struct_prog(prog);
	ft_check_param(prog, argc);
	if (pipe(prog->pipe_fd) == -1)
	{
		ft_putstr_fd("pipe failed", 2);
		ft_free_prog(&prog);
		return (1);
	}
	ft_child_1(prog, argv, envp);
	ft_child_2(prog, argv, envp);
	close_unneeded_pipe(prog);
	int exit_status = wait_for_children(prog->pid1, prog->pid2);
    ft_free_prog(&prog);  // 确保释放 prog
	printf("Program exiting with status %d\n", exit_status);
    return (exit_status);
}

/*strace -e signal=SIGPIPE -o trace.log < input /usr/bin/cat | /usr/bin/wcgd > output
*/
/*valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./pipex input /usr/bin/cat dls output

*/