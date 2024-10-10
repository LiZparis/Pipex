#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include "../libraries/Libft/libft.h"

typedef enum s_flag
{
	true,
	false
}	t_flag;

typedef struct s_prog
{
    int     infile;     // 输入文件描述符（4字节）
    int     outfile;    // 输出文件描述符（4字节）
    int     pipe_fd[2]; // 管道文件描述符（2个int，共8字节）
    pid_t   pid1;       // 第一个子进程的PID（4字节，和int相同）
    pid_t   pid2;       // 第二个子进程的PID（4字节
	t_flag is_1st_cmd;
}   t_prog;

void	ft_check_param(t_prog *prog, int argc, char **argv);
void	close_unneeded_fds(t_prog *prog);
void	ft_free_prog(t_prog *prog);
void	ft_free_split(char **split);
void	init_struct_prog(t_prog *prog);
// char	*ft_get_path_env(char **envp);
// char 	**ft_get_full_path(char *path_env);
char	*find_executable(char *command, char **envp);
// void	execute_command(t_prog *prog, char *cmd, char **envp, t_flag is_1st_cmd);
void	ft_child_1(t_prog *prog, char **argv, char **envp);
void	ft_child_2(t_prog *prog, char **argv, char **envp);
void	wait_for_children(t_prog *prog);
#endif