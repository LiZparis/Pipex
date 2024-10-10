// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   execute_cmd.c                                      :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2024/10/10 17:08:55 by lzhang2           #+#    #+#             */
// /*   Updated: 2024/10/10 17:46:22 by lzhang2          ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "pipex.h"

// char **split_command(char *cmd)
// {
//     char **args = ft_split(cmd, ' ');
//     if (!args)
//     {
//         ft_putstr_fd("Failed to split command", 2);
//         return NULL;
//     }
//     return args;
// }

// void handle_exec_error(t_prog *prog, char **args, char *path, t_flag is_1st_cmd)
// {
//     if (path == NULL)
//     {
//         ft_free_split(args);
//         ft_putstr_fd("Command not found", 2);
//         ft_free_prog(prog);
//         if (is_1st_cmd == true)
//             exit(1);
//         else
//             exit(0);
//     }
// }

// void execute_command(t_prog *prog, char *cmd, char **envp, t_flag is_1st_cmd)
// {
//     char **args;
//     char *path;

//     args = split_command(cmd);
//     if (!args)
//     {
//         ft_free_prog(prog);
//         exit(EXIT_FAILURE);
//     }
//     path = find_executable(args[0], envp);
// 	handle_exec_error(prog, args, path, is_1st_cmd);
//     ft_printf("Executing command: %s\n", path);
//     if (execve(path, args, envp) == -1)
//         handle_exec_error(prog, args, path, is_1st_cmd);
//     ft_free_split(args);
// }
