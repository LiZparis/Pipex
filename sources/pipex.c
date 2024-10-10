/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 17:19:46 by lzhang2           #+#    #+#             */
/*   Updated: 2024/10/10 21:14:33 by lzhang2          ###   ########.fr       */
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

void	ft_check_param(t_prog *prog, int argc, char **argv)
{
	if (argc != 5)
	{
		ft_putstr_fd("argc should be 5\n", 2);
		ft_free_prog(prog);
		exit(EXIT_FAILURE);
	}
	prog->infile = open(argv[1], O_RDONLY);
	if (prog->infile < 0)
	{
		ft_putstr_fd("Failed to open infile\n", 2);
		ft_free_prog(prog);
		exit(EXIT_FAILURE);
	}
	prog->outfile = open(argv[4], O_WRONLY | O_TRUNC, 0644);
	if (prog->outfile < 0)
	{
		ft_putstr_fd("Failed to open outfile", 2);
		close(prog->infile);
		ft_free_prog(prog);
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
			return(envp[i] + 5);
		i++;
	}
	ft_putstr_fd("can't find env path", 2);
	return (NULL);
}


char *build_full_path(const char *dir, const char *command)
{
    char *full_path;
	
	full_path = malloc(ft_strlen(dir) + ft_strlen(command) + 2);
    if (!full_path)
        return NULL;
    ft_strcpy(full_path, dir);
    ft_strcat(full_path, "/");
    ft_strcat(full_path, command);
    
    return full_path;
}

char *find_abs_cmd(char *command)
{
	if (command[0] == '/' || command[0] == '.')
    {
        if (access(command, X_OK) == 0)
            return (ft_strdup(command)); // 返回命令的副本
	}
    return (NULL); // 绝对路径不可执行
}

char	*find_executable(char *command, char **envp)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	int		i;
	char	*abs_cmd;
	
	abs_cmd = find_abs_cmd(command);
	if(abs_cmd)
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
	if (execve(path, args, envp) == -1)
	{
		perror("execve");
		free(path);
		ft_free_split(args);
		ft_free_prog(prog);
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
		ft_free_prog(prog);
		exit(EXIT_FAILURE);
	}
	path = find_executable(args[0], envp);
	if (!path)
	{
		ft_free_split(args);
		ft_free_prog(prog);
		ft_putstr_fd("Error: Command not found\n", 2);
		if(prog->is_1st_cmd == true)
			exit(0);
		else if(prog->is_1st_cmd == false)
			exit(127);
	}
	is_execvable(prog, path, args, envp);
}

void	ft_child_1(t_prog *prog, char **argv, char **envp)
{
	prog->pid1 = fork();
	if (prog->pid1 == 0) // 子进程 1
	{
		dup2(prog->infile, STDIN_FILENO); // 将 infile 重定向到标准输入
		dup2(prog->pipe_fd[1], STDOUT_FILENO); // 将管道的写端重定向到标准输出
		close(prog->infile); // 关闭不再需要的文件描述符
		close_unneeded_fds(prog); // 关闭所有不需要的文件描述符
		prog->is_1st_cmd = true; // 标记为第一个命令
		execute_command(prog, argv[2], envp); // 执行命令
	}
}

void	ft_child_2(t_prog *prog, char **argv, char **envp)
{
	prog->pid2 = fork();
	if (prog->pid2 == 0) // 子进程 2
	{
		dup2(prog->pipe_fd[0], STDIN_FILENO); // 将管道的读端重定向到标准输入
		dup2(prog->outfile, STDOUT_FILENO); // 将 outfile 重定向到标准输出
		close(prog->outfile); // 关闭不再需要的文件描述符
		close_unneeded_fds(prog); // 关闭所有不需要的文件描述符
		prog->is_1st_cmd = false; // 标记为第二个命令
		execute_command(prog, argv[3], envp); // 执行命令
	}
}

// void	wait_for_children(t_prog *prog) 
// {
//     int status;

//     waitpid(prog->pid1, &status, 0);
//     if (WIFEXITED(status))
//         ft_printf("child 1 exited with status: %d\n", WEXITSTATUS(status));
//     else
//         ft_printf("child 1 did not exit normally\n");

//     waitpid(prog->pid2, &status, 0);
//     if (WIFEXITED(status))
//         ft_printf("child 2 exited with status: %d\n", WEXITSTATUS(status));
//     else
//         ft_printf("child 2 did not exit normally\n");
// }
void wait_for_children(t_prog *prog)
{
    int status1, status2;

    // 等待 child 1 结束
    waitpid(prog->pid1, &status1, 0);
    if (WIFEXITED(status1))
    {
        int exit_status = WEXITSTATUS(status1);
        ft_printf("child 1 exited with status: %d\n", exit_status);
        // 如果 child 1 失败（非零退出码），则退出程序
        if (exit_status != 0)
			exit(exit_status);
    }
    else
    {
        ft_printf("child 1 did not exit normally\n");
        exit(1); // 子进程异常退出，父进程也退出
    }
    waitpid(prog->pid2, &status2, 0);
    if (WIFEXITED(status2))
    {
        int exit_status = WEXITSTATUS(status2);
        ft_printf("child 2 exited with status: %d\n", exit_status);
        // 如果 child 2 失败（非零退出码），则退出程序
        if (exit_status != 0)
            exit(exit_status);
    }
    else
    {
        ft_printf("child 2 did not exit normally\n");
        exit(1); // 子进程异常退出，父进程也退出
    }
}



int	main(int argc, char **argv, char **envp)
{
	t_prog	*prog;
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
	wait_for_children(prog);
	ft_free_prog(prog);
	return (0);
}