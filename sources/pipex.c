/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 17:19:46 by lzhang2           #+#    #+#             */
/*   Updated: 2024/10/12 18:25:53 by lzhang2          ###   ########.fr       */
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
		{
			perror(infile);
			ft_free_prog(&prog);
			exit(EXIT_FAILURE);
		}
		else if (errno == ENOENT || errno == EISDIR)
		{
			perror(infile);
			ft_free_prog(&prog);
			exit(EXIT_FAILURE);
		}
	}
}
void	check_outfile(t_prog *prog, char *outfile)
{
	prog->outfile = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (prog->outfile < 0)
	{
		if (errno == EACCES)
		{
			perror(outfile);
			ft_free_prog(&prog);
			exit(EXIT_FAILURE);
		}
		else
		{
			perror(outfile);
			ft_free_prog(&prog);
			exit(EXIT_FAILURE);
		}
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

char	*find_abs_cmd(char *command)
{
	if (command[0] == '/' || command[0] == '.')
	{
		if (access(command, X_OK) == 0)
			return (command);
	}
	return (NULL);
}

char	*find_executable(char *command, char **envp)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	int		i;
	char	*abs_cmd;

	abs_cmd = find_abs_cmd(command);
	if (abs_cmd)
		return (abs_cmd);
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
		if (access(full_path, X_OK) == 0)
		{
			ft_free_split(paths);
			return (full_path);
		}
		free(full_path);
	}
	ft_free_split(paths);
	return (NULL);
}

void is_execvable(t_prog *prog, char *path, char **args, char **envp)
{
	(void)prog;
	if (execve(path, args, envp) == -1)
	{
		perror("execve");
		free(path);
		ft_free_split(args);
		exit(126);
	}
	free(path);
	ft_free_split(args);
}

void	execute_command(t_prog *prog, char *cmd, char **envp)
{
	char	**args;
	char	*path;

	args = ft_split(cmd, ' ');
	if (!args)
	{
		ft_putstr_fd("Error: Failed to split command\n", 2);
		exit(EXIT_FAILURE);
	}
	path = find_executable(args[0], envp);
	if (!path)
	{
		ft_free_split(args);
		ft_putstr_fd("Error: Command not found\n", 2);
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
	is_execvable(prog, path, args, envp);
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

int	wait_for_children(pid_t pid2)
{
	int		status;
	pid_t	pid;
	int		exit_status;

	exit_status = 0;
	while ((pid = waitpid(-1, &status, 0)) > 0)
	{
		// if (WIFEXITED(status))
		// 	ft_printf("Child %d exited with status %d\n", pid, WEXITSTATUS(status));
		// else if (WIFSIGNALED(status))
		// 	ft_printf("Child %d was killed by signal %d\n", pid, WTERMSIG(status));

		if (pid == pid2 && WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
	}
	return (exit_status);
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
	int exit_status = wait_for_children(prog->pid2);
    
    ft_free_prog(&prog);  // 确保释放 prog
	free(prog);

    return (exit_status);
}

/*strace -e signal=SIGPIPE -o trace.log < input /usr/bin/cat | /usr/bin/wcgd > output
*/
/*valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./pipex input /usr/bin/cat dls output

*/