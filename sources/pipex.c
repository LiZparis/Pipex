/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 16:53:47 by lzhang2           #+#    #+#             */
/*   Updated: 2024/10/08 16:53:47 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void ft_free_split(char **split)
{
    int i = 0;
    while (split[i])
    {
        free(split[i]);
        i++;
    }
    free(split);
}

int ft_count_words(char *str, char c)
{
	int count = 0;
	while(*str)
	{
		while(*str == c)
			str++;
		if(*str && *str != c)
		{
			count++;
			while(*str && *str != c)
				str++;
		}
	}
	return(count);
}

char *fill_word(char **sstr, char c)
{
	char *str = *sstr;
	while(*str == c)
		str++;
	int len = 0;
	while(str[len] && str[len] != c)
		len++;
	char *word = malloc((len + 1) * sizeof(char));
	if (!word)
		return(NULL);
	word[len] = '\0';
	int i = 0;
	while(i < len)
	{
		word[i] = str[i];
		i++;
	}
	
	*sstr = str + len;
	return(word);
}

char    **ft_split(char *str, char c)
{
	int count_words = ft_count_words(str, c);
	char **tab = malloc((count_words + 1) * sizeof(char *));
	if(!tab)
		return(NULL);
	tab[count_words] = NULL;
	int i = 0;
	while(i < count_words)
	{
		tab[i] = fill_word(&str, c);
		if(!tab[i])
		{
			while(--i)
				free(tab[i]);
			free(tab);
			return(NULL);
		}
		i++;
	}  
	return(tab);                      
}

int ft_check_param(int argc, char **argv, int *infile, int *outfile)
{
	if (argc != 5)
    {
        fprintf(stderr, "Usage: %s infile cmd1 cmd2 outfile\n", argv[0]);
        exit (EXIT_FAILURE);
    }

    *infile = open(argv[1], O_RDONLY);
    if (*infile < 0)
    {
        perror("Failed to open infile");
        return (EXIT_FAILURE);
    }
    *outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (*outfile < 0)
    {
        perror("Failed to open outfile");
        close(*infile);
        return (EXIT_FAILURE);
    }
	return(0);
}


char *find_executable(char *command, char **envp)
{
    char *path_env = NULL;
    char **paths;
    char *full_path;
    int i = 0;

    // 获取 PATH 环境变量的值
    while (envp[i])
    {
        if (strncmp(envp[i], "PATH=", 5) == 0)
        {
            path_env = envp[i] + 5;  // 跳过 "PATH="
            break;
        }
        i++;
    }

    // 如果没有找到 PATH 环境变量
    if (!path_env)
    {
		perror("can't find env path");
        return NULL;
    }

    // 将 PATH 环境变量分割成各个路径
    paths = ft_split(path_env, ':');  // 假设你已经有一个 split 函数，分割字符串

    if (!paths)
    {
        return NULL;
    }

    // 遍历每个路径，构造完整的命令路径并检查它是否可执行
    i = 0;
    while (paths[i])
    {
        // 为完整路径分配空间，并构造路径
        full_path = malloc(strlen(paths[i]) + strlen(command) + 2);
        if (!full_path)
        {
            return NULL;
        }
        strcpy(full_path, paths[i]);
        strcat(full_path, "/");
        strcat(full_path, command);

         // 使用 access() 检查文件是否存在并且具有可执行权限
        if (access(full_path, X_OK) == 0) {
            // 找到可执行文件，返回完整路径
            return full_path;
        }

        free(full_path);  // 释放当前分配的路径内存
        i++;
    }

    // 如果没有找到，释放内存，返回 NULL
    ft_free_split(paths);
    return NULL;
}

void execute_command(char *cmd, char **envp)
{
    char **args;
    char *path;

    // 假设有一个 split 函数可以将命令和参数分割
    args = ft_split(cmd, ' ');  // 将命令分割成命令和参数
    if (args == NULL)
    {
        perror("Failed to split command");
        exit(EXIT_FAILURE);
    }

    // 假设有一个函数 find_executable，可以通过 PATH 环境变量找到命令的可执行路径
    path = find_executable(args[0], envp);  
   if (path == NULL)
{
    ft_free_split(args);  // 释放分配的内存
    perror("Command not found");
    exit(EXIT_FAILURE);
}


    // 使用 execve 执行命令
    execve(path, args, envp);
    
    // 如果 execve 返回，说明出错了
    perror("execve failed");
    exit(EXIT_FAILURE);
}



int main(int argc, char **argv, char **envp)
{
    int pipefd[2];
    int infile, outfile;
    pid_t pid1, pid2;
	ft_check_param(argc, argv, &infile, &outfile);
    if (pipe(pipefd) == -1)
    {
        perror("pipe failed");
        return (EXIT_FAILURE);
    }

    // 创建第一个子进程来执行 cmd1
    pid1 = fork();
    if (pid1 == 0)
    {
        dup2(infile, STDIN_FILENO);       // 将标准输入重定向到 infile
        dup2(pipefd[1], STDOUT_FILENO);   // 将标准输出重定向到管道写端
        close(infile);
        close(pipefd[0]);
        close(pipefd[1]);
		execute_command(argv[2], envp);   // 执行第一个命令 cmd1
    }

    // 创建第二个子进程来执行 cmd2
    pid2 = fork();
    if (pid2 == 0)
    {
        dup2(pipefd[0], STDIN_FILENO);    // 将标准输入重定向到管道读端
        dup2(outfile, STDOUT_FILENO);     // 将标准输出重定向到 outfile
        close(outfile);
        close(pipefd[0]);
        close(pipefd[1]);
		execute_command(argv[3], envp);   // 执行第二个命令 cmd2
    }

    // 父进程关闭不需要的文件描述符
    close(infile);
    close(outfile);
    close(pipefd[0]);
    close(pipefd[1]);

    // 等待两个子进程结束
    waitpid(pid1, NULL, 0);  // 等待第一个子进程
    waitpid(pid2, NULL, 0);  // 等待第二个子进程

    return (EXIT_SUCCESS);
}

/*调试建议
编译代码： 使用 gcc 编译你的程序，并开启警告信息：

bash
Copier le code
gcc -Wall -Wextra -Werror -o pipex pipex.c
运行程序： 确保你有一个有效的输入文件、两个命令和一个输出文件，运行程序：

bash
Copier le code
./pipex infile "ls -l" "wc -l" outfile
使用 gdb 调试： 如果程序崩溃或表现不如预期，可以使用 gdb 调试：

bash
Copier le code
gdb ./pipex
run infile "ls -l" "wc -l" outfile
打印调试信息： 在关键位置添加 printf 语句，打印变量的值和程序的执行状态，以便于调试。

使用 strace： strace 可以帮助你查看系统调用和信号：

bash
Copier le code
strace ./pipex infile "ls -l" "wc -l" outfile
边界条件测试： 测试各种边界条件，如输入文件不存在、命令不可执行、权限不足等情况，以确保代码健壮性。*/
/*以下是一些可以用来测试 `pipex` 项目的边界条件和错误情况的测试案例。这些测试将帮助你确保程序的健壮性和可靠性。

### 边界条件测试案例

1. **缺少输入参数**
   ```bash
   ./pipex
   ```
   - 期望输出：`Usage: ./pipex infile cmd1 cmd2 outfile`

2. **输入参数不足**
   ```bash
   ./pipex infile cmd1
   ```
   - 期望输出：`Usage: ./pipex infile cmd1 cmd2 outfile`

3. **输入文件不存在**
   ```bash
   ./pipex nonexistent.txt "ls -l" "wc -l" outfile
   ```
   - 期望输出：`Failed to open infile: No such file or directory`

4. **输出文件不可写**
   - 尝试写入一个只读文件或没有写权限的目录：
   ```bash
   ./pipex infile "ls -l" "wc -l" /etc/passwd
   ```
   - 期望输出：`Failed to open outfile: Permission denied`

5. **第一个命令不可执行**
   ```bash
   ./pipex infile "nonexistent_cmd" "wc -l" outfile
   ```
   - 期望输出：`Command not found: nonexistent_cmd`

6. **第二个命令不可执行**
   ```bash
   ./pipex infile "ls -l" "nonexistent_cmd" outfile
   ```
   - 期望输出：`Command not found: nonexistent_cmd`

7. **使用空命令**
   ```bash
   ./pipex infile "" "wc -l" outfile
   ```
   - 期望输出：`Command not found: `

8. **命令参数错误**
   - 例如，使用无效选项：
   ```bash
   ./pipex infile "ls -invalid_option" "wc -l" outfile
   ```
   - 期望输出：根据具体命令的实现，可能会输出命令的错误信息。

9. **多个管道**
   - 在 `pipex` 中实现多个命令，例如：
   ```bash
   ./pipex infile "ls -l" "grep '.c'" "wc -l" outfile
   ```
   - 期望输出：文件中以 `.c` 结尾的行数。

10. **测试空文件**
    - 测试处理空输入文件：
    ```bash
    touch empty.txt
    ./pipex empty.txt "cat" "wc -l" outfile
    ```
    - 期望输出：输出文件中的行数应为 `0`。

11. **命令输出较大**
    - 测试命令输出较大的情况，确保不会导致缓冲区溢出或其他错误：
    ```bash
    ./pipex infile "seq 1 100000" "wc -l" outfile
    ```
    - 期望输出：输出文件中的行数应为 `100000`。

12. **特殊字符处理**
    - 测试命令中含有特殊字符：
    ```bash
    echo "Hello World" > infile
    ./pipex infile "grep 'Hello'" "wc -l" outfile
    ```
    - 期望输出：输出文件中的行数应为 `1`。

13. **测试权限**
    - 创建一个没有执行权限的文件并测试：
    ```bash
    touch no_exec
    chmod 444 no_exec  # 只读权限
    ./pipex infile "no_exec" "wc -l" outfile
    ```
    - 期望输出：`Command not found: no_exec`。

### 扩展测试建议

- **内存泄漏检测**：
  使用工具（如 `valgrind`）检测内存泄漏：
  ```bash
  valgrind --leak-check=full ./pipex infile "ls -l" "wc -l" outfile
  ```

- **并发测试**：
  在多个进程中同时执行 `pipex`，确保没有竞争条件：
  ```bash
  for i in {1..10}; do
      ./pipex infile "ls -l" "wc -l" outfile &
  done
  wait
  ```

- **长文件名或路径**：
  测试超长路径或文件名：
  ```bash
  ./pipex a_long_file_name.txt "ls -l" "wc -l" outfile
  ```

通过这些边界条件和错误情况的测试，能够有效地验证你的 `pipex` 项目的稳定性和可靠性。如果遇到错误，仔细检查错误信息，调整代码以处理这些情况。*/