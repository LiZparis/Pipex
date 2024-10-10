/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prog.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 16:53:47 by lzhang2           #+#    #+#             */
/*   Updated: 2024/10/08 16:53:47 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
void close_unneeded_fds(t_prog *prog) 
{
    // close(prog->infile);
    // close(prog->outfile);
    close(prog->pipe_fd[0]);
    close(prog->pipe_fd[1]);
}
void	ft_exit(t_prog *prog)
{
	if (prog)
	{
		if (prog->infile != -1)
			close(prog->infile);
		if (prog->outfile != -1)
			close(prog->outfile);
		close_unneeded_fds(prog);
		free(prog);
	}
	exit(EXIT_FAILURE);
}

int	ft_check_param(t_prog *prog, int argc, char **argv)
{
	if (argc != 5)
    {
        fprintf(stderr, "Usage: %s infile cmd1 cmd2 outfile\n", argv[0]);
		ft_exit(prog);
    }
	prog->infile = open(argv[1], O_RDONLY);
    if (prog->infile < 0)
    {
        perror("Failed to open infile");
		ft_exit(prog);
    }
    // prog->outfile = open(argv[4], O_WRONLY | O_CREAT | O_APPEND, 0644);
	prog->outfile = open(argv[4], O_WRONLY | O_TRUNC, 0644);
    if (prog->outfile < 0)
    {
        perror("Failed to open outfile");
        close(prog->infile);
		ft_exit(prog);
    }
	return(0);
}

char	*find_executable(char *command, char **envp)
{
    char *path_env;
    char **paths;
    char *full_path;
    size_t i;

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
		perror("can't find env path");
        return (NULL);
    }
    paths = ft_split(path_env, ':');
    if (!paths)
        return (NULL);
    i = 0;
    while (paths[i])
    {
        full_path = malloc(ft_strlen(paths[i]) + ft_strlen(command) + 2);
        if (!full_path)
            return (NULL);
      	strcpy(full_path, paths[i]);
		strcat(full_path, "/");
		strcat(full_path, command);
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

void	execute_command(t_prog *prog, char *cmd, char **envp)
{
    char **args;
    char *path;

    args = ft_split(cmd, ' ');
    if (args == NULL)
    {
        perror("Failed to split command");
		ft_exit(prog);
    }
    path = find_executable(args[0], envp);  
	if (path == NULL)
	{
    	ft_free_split(args);
    	perror("Command not found");
		ft_exit(prog);
	}
	ft_printf("%s %s", path, args);
    execve(path, args, envp);
	perror("execve failed");
	free(path);
	ft_free_split(args);
    ft_exit(prog);
}


void init_struct_prog(t_prog *prog)
{
	prog->infile = -1;    // 表示输入文件尚未打开
    prog->outfile = -1;   // 表示输出文件尚未打开
    prog->pipe_fd[0] = -1; // 初始化管道文件描述符
	prog->pipe_fd[1] = -1; 
	prog->pid1 = -1; //子进程尚未创建,表示子进程尚未创建
	prog->pid2 = -1;
  
}

void ft_child_1(t_prog *prog, char **argv, char **envp)
{
    prog->pid1 = fork();
    if (prog->pid1 < 0)
    {
        perror("fork failed for child 1");
        ft_exit(prog);
    }
    if (prog->pid1 == 0)
    { 
        if (dup2(prog->infile, STDIN_FILENO) < 0)
        {
            perror("dup2 infile failed");
            ft_exit(prog);
        }
        if (dup2(prog->pipe_fd[1], STDOUT_FILENO) < 0)
        {
            perror("dup2 pipe_fd[1] failed");
            ft_exit(prog);
        }
		ft_printf("dup2ok");
        close(prog->infile);
        close_unneeded_fds(prog);
		execute_command(prog, argv[2], envp);
    }
}

void ft_child_2(t_prog *prog, char **argv, char **envp)
{
    prog->pid2 = fork();
    if (prog->pid2 < 0)
    {
        perror("fork failed for child 2");
        ft_exit(prog);
    }
    if (prog->pid2 == 0)
    {
        if (dup2(prog->pipe_fd[0], STDIN_FILENO) < 0)
        {
            perror("dup2 pipe_fd[0] failed");
            ft_exit(prog);
        }
        if (dup2(prog->outfile, STDOUT_FILENO) < 0)
        {
            perror("dup2 outfile failed");
            ft_exit(prog);
        }
        close(prog->outfile);
        close_unneeded_fds(prog);
        ft_printf("Executing command: %s\n", argv[3]);
        execute_command(prog, argv[3], envp);   // 执行第二个命令 cmd2
    }
}


void wait_for_children(t_prog *prog) 
{
    waitpid(prog->pid1, NULL, 0);
    waitpid(prog->pid2, NULL, 0);
}

int	main(int argc, char **argv, char **envp)
{
	t_prog *prog;

	prog = malloc(sizeof(t_prog));
	if(!prog)
		return(0);
	init_struct_prog(prog);
	ft_check_param(prog, argc, argv);
    if (pipe(prog->pipe_fd) == -1)
    {
        perror("pipe failed");
        ft_exit(prog);
    }
	ft_child_1(prog, argv, envp);
	ft_child_2(prog, argv, envp);
    close_unneeded_fds(prog);
	wait_for_children(prog);
	free(prog);
    return (EXIT_SUCCESS);
}

/*调试建议
编译代码： 使用 gcc 编译你的程序，并开启警告信息：

bash
Copier le code
gcc -Wall -Wextra -Werror -o prog prog.c
运行程序： 确保你有一个有效的输入文件、两个命令和一个输出文件，运行程序：

bash
Copier le code
./prog infile "ls -l" "wc -l" outfile
使用 gdb 调试： 如果程序崩溃或表现不如预期，可以使用 gdb 调试：

bash
Copier le code
gdb ./prog
run infile "ls -l" "wc -l" outfile
打印调试信息： 在关键位置添加 printf 语句，打印变量的值和程序的执行状态，以便于调试。

使用 strace： strace 可以帮助你查看系统调用和信号：

bash
Copier le code
strace ./prog infile "ls -l" "wc -l" outfile
边界条件测试： 测试各种边界条件，如输入文件不存在、命令不可执行、权限不足等情况，以确保代码健壮性。*/
/*以下是一些可以用来测试 `prog` 项目的边界条件和错误情况的测试案例。这些测试将帮助你确保程序的健壮性和可靠性。

### 边界条件测试案例

1. **缺少输入参数**
   ```bash
   ./prog
   ```
   - 期望输出：`Usage: ./prog infile cmd1 cmd2 outfile`

2. **输入参数不足**
   ```bash
   ./prog infile cmd1
   ```
   - 期望输出：`Usage: ./prog infile cmd1 cmd2 outfile`

3. **输入文件不存在**
   ```bash
   ./prog nonexistent.txt "ls -l" "wc -l" outfile
   ```
   - 期望输出：`Failed to open infile: No such file or directory`

4. **输出文件不可写**
   - 尝试写入一个只读文件或没有写权限的目录：
   ```bash
   ./prog infile "ls -l" "wc -l" /etc/passwd
   ```
   - 期望输出：`Failed to open outfile: Permission denied`

5. **第一个命令不可执行**
   ```bash
   ./prog infile "nonexistent_cmd" "wc -l" outfile
   ```
   - 期望输出：`Command not found: nonexistent_cmd`

6. **第二个命令不可执行**
   ```bash
   ./prog infile "ls -l" "nonexistent_cmd" outfile
   ```
   - 期望输出：`Command not found: nonexistent_cmd`

7. **使用空命令**
   ```bash
   ./prog infile "" "wc -l" outfile
   ```
   - 期望输出：`Command not found: `

8. **命令参数错误**
   - 例如，使用无效选项：
   ```bash
   ./prog infile "ls -invalid_option" "wc -l" outfile
   ```
   - 期望输出：根据具体命令的实现，可能会输出命令的错误信息。

9. **多个管道**
   - 在 `prog` 中实现多个命令，例如：
   ```bash
   ./prog infile "ls -l" "grep '.c'" "wc -l" outfile
   ```
   - 期望输出：文件中以 `.c` 结尾的行数。

10. **测试空文件**
    - 测试处理空输入文件：
    ```bash
    touch empty.txt
    ./prog empty.txt "cat" "wc -l" outfile
    ```
    - 期望输出：输出文件中的行数应为 `0`。

11. **命令输出较大**
    - 测试命令输出较大的情况，确保不会导致缓冲区溢出或其他错误：
    ```bash
    ./prog infile "seq 1 100000" "wc -l" outfile
    ```
    - 期望输出：输出文件中的行数应为 `100000`。

12. **特殊字符处理**
    - 测试命令中含有特殊字符：
    ```bash
    echo "Hello World" > infile
    ./prog infile "grep 'Hello'" "wc -l" outfile
    ```
    - 期望输出：输出文件中的行数应为 `1`。

13. **测试权限**
    - 创建一个没有执行权限的文件并测试：
    ```bash
    touch no_exec
    chmod 444 no_exec  # 只读权限
    ./prog infile "no_exec" "wc -l" outfile
    ```
    - 期望输出：`Command not found: no_exec`。

### 扩展测试建议

- **内存泄漏检测**：
  使用工具（如 `valgrind`）检测内存泄漏：
  ```bash
  valgrind --leak-check=full ./prog infile "ls -l" "wc -l" outfile
  ```

- **并发测试**：
  在多个进程中同时执行 `prog`，确保没有竞争条件：
  ```bash
  for i in {1..10}; do
      ./prog infile "ls -l" "wc -l" outfile &
  done
  wait
  ```

- **长文件名或路径**：
  测试超长路径或文件名：
  ```bash
  ./prog a_long_file_name.txt "ls -l" "wc -l" outfile
  ```

通过这些边界条件和错误情况的测试，能够有效地验证你的 `prog` 项目的稳定性和可靠性。如果遇到错误，仔细检查错误信息，调整代码以处理这些情况。*/