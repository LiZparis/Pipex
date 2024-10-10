
#include "pipex.h"

void	ft_free_split(char **split)
{
	int i;

	i = 0;
	if (!split)
		return;
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
		if (prog->infile != -1)
			close(prog->infile);
		if (prog->outfile != -1)
			close(prog->outfile);
		close(prog->pipe_fd[0]);
		close(prog->pipe_fd[1]);
		free(prog);
	}
	exit(EXIT_FAILURE);
}

void	ft_check_param(t_prog *prog, int argc, char **argv)
{
	if (argc != 5)
	{
		ft_putstr_fd("argc should be 5\n", 2);
		ft_free_prog(prog);
	}
	prog->infile = open(argv[1], O_RDONLY);
	if (prog->infile < 0)
	{
		ft_putstr_fd("Failed to open infile\n", 2);
		ft_free_prog(prog);
	}
	prog->outfile = open(argv[4], O_WRONLY | O_TRUNC, 0644);
	if (prog->outfile < 0)
	{
		ft_putstr_fd("Failed to open outfile", 2);
		close(prog->infile);
		ft_free_prog(prog);
	}
}

char	*find_executable(char *command, char **envp)
{
	char *path_env;
	char **paths;
	char *full_path;
	int i;

	path_env = NULL;
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_env = envp[i] + 5;
			break;
		}
		i++;
	}
	if (!path_env)
	{
		ft_putstr_fd("can't find env path", 2);
		return (NULL);
	}
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = malloc(strlen(paths[i]) + strlen(command) + 2);
		if (!full_path)
			return (NULL);
		strcpy(full_path, paths[i]);
		ft_printf("%s\n", full_path);
		strcat(full_path, "/");
		ft_printf("%s\n", full_path);
		strcat(full_path, command);
		ft_printf("%s\n", full_path);
		if (access(full_path, X_OK) == 0)
		{
			ft_free_split(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	ft_free_split(paths);
	return (NULL);
}

void	execute_command(t_prog *prog, char *cmd, char **envp, int is_1st_cmd)
{
	char **args;
	char *path;

	args = ft_split(cmd, ' ');
	if (args == NULL)
	{
		ft_putstr_fd("Failed to split command", 2);
		ft_free_prog(prog);
		exit(EXIT_FAILURE);
	}
	path = find_executable(args[0], envp);
	if (path == NULL)
	{
		ft_free_split(args);
		ft_putstr_fd("Command not found", 2);
		ft_free_prog(prog);
		if(prog->is_1st_cmd == true)
        	exit(127); 
		else
			exit(1);
	}
	ft_printf("Executing command: %s\n", path);
	if(execve(path, args, envp) == -1)
	{
		perror("Error");
		ft_putstr_fd("execve failed", 2);
		ft_free_split(args);
		ft_free_prog(prog);
		if(prog->is_1st_cmd == true)
        	exit(127); 
		else
			exit(1);
	}
}

void	init_struct_prog(t_prog *prog)
{
	prog->infile = -1;
	prog->outfile = -1;
	prog->pipe_fd[0] = -1;
	prog->pipe_fd[1] = -1;
	prog->pid1 = -1;
	prog->pid2 = -1;
}

void	ft_child_1(t_prog *prog, char **argv, char **envp)
{
	prog->pid1 = fork();
	if (prog->pid1 < 0)
	{
		ft_putstr_fd("fork failed for child 1", 2);
		ft_free_prog(prog);
		exit(EXIT_FAILURE);
	}
	if (prog->pid1 == 0)
	{
		if (dup2(prog->infile, STDIN_FILENO) < 0)
		{
			ft_putstr_fd("dup2 infile failed", 2);
			ft_free_prog(prog);
			exit(EXIT_FAILURE);
		}
		ft_printf("child1 dup1 ok\n");
		if (dup2(prog->pipe_fd[1], STDOUT_FILENO) < 0)
		{
			ft_putstr_fd("dup2 pipe_fd[1] failed", 2);
			ft_free_prog(prog);
			exit(EXIT_FAILURE);
		}
		ft_printf("child1 dup2 ok\n");
		close(prog->infile);
		close(prog->pipe_fd[0]);
		close(prog->pipe_fd[1]);
		execute_command(prog, argv[2], envp, true);
	}
}

void	ft_child_2(t_prog *prog, char **argv, char **envp)
{
	prog->pid2 = fork();
	if (prog->pid2 < 0)
	{
		ft_putstr_fd("fork failed for child 2", 2);
		ft_free_prog(prog);
	}
	if (prog->pid2 == 0)
	{
		if (dup2(prog->pipe_fd[0], STDIN_FILENO) < 0)
		{
			ft_putstr_fd("dup2 pipe_fd[0] failed", 2);
			ft_free_prog(prog);
			exit(EXIT_FAILURE);
		}
		if (dup2(prog->outfile, STDOUT_FILENO) < 0)
		{
			ft_putstr_fd("dup2 outfile failed", 2);
			ft_free_prog(prog);
			exit(EXIT_FAILURE);
		}
		close(prog->outfile);
		close(prog->pipe_fd[0]);
		close(prog->pipe_fd[1]);
		execute_command(prog, argv[3], envp, false);
	}
}

void	close_unneeded_fds(t_prog *prog) 
{
	close(prog->pipe_fd[0]);
	close(prog->pipe_fd[1]);
}

void	wait_for_children(t_prog *prog) 
{
	waitpid(prog->pid1, NULL, 0);
	waitpid(prog->pid2, NULL, 0);
}

int	main(int argc, char **argv, char **envp)
{
	t_prog *prog;

	prog = malloc(sizeof(t_prog));
	if (!prog)
		return (1);
	init_struct_prog(prog);
	ft_check_param(prog, argc, argv);
	if (pipe(prog->pipe_fd) == -1)
	{
		ft_putstr_fd("pipe failed", 2);
		ft_free_prog(prog);
		return (1);
	}
	ft_child_1(prog, argv, envp);
	ft_child_2(prog, argv, envp);
	close_unneeded_fds(prog);
	wait_for_children(prog);
	ft_free_prog(prog);
	return (0);
}