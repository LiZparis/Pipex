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

#endif