// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   execute.c                                          :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2024/10/10 15:25:44 by lzhang2           #+#    #+#             */
// /*   Updated: 2024/10/10 17:07:56 by lzhang2          ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "pipex.h"

// #include "pipex.h"

// char *get_path_env(char **envp)
// {
//     char *path_env;
//     int i;

//     i = 0;
//     while (envp[i])
//     {
//         if (ft_strncmp(envp[i], "PATH=", 5) == 0)
//         {
//             path_env = envp[i] + 5;
//             return path_env;
//         }
//         i++;
//     }
//     ft_putstr_fd("can't find env path", 2);
//     return NULL;
// }

// char **split_paths(char *path_env)
// {
//     char **paths;

//     paths = ft_split(path_env, ':');
//     if (!paths)
//     {
//         ft_putstr_fd("Error: split failed\n", 2);
//         return NULL;
//     }
//     return paths;
// }

// char *check_paths(char **paths, char *command)
// {
//     char *full_path;
//     int i;

//     i = 0;
//     while (paths[i])
//     {
//         full_path = malloc(strlen(paths[i]) + strlen(command) + 2);
//         if (!full_path)
//             return NULL;
//         strcpy(full_path, paths[i]);
//         strcat(full_path, "/");
//         strcat(full_path, command);
        
//         ft_printf("%s\n", full_path); // 打印完整路径用于调试
//         if (access(full_path, X_OK) == 0)
//         {
//             ft_free_split(paths);
//             return full_path;
//         }
//         free(full_path);
//         i++;
//     }
//     ft_free_split(paths);
//     return NULL;
// }

// char *find_executable(char *command, char **envp)
// {
//     char *path_env;
//     char **paths;
//     char *path;

//     path_env = get_path_env(envp);
//     if (!path_env)
//         return NULL;
    
//     paths = split_paths(path_env);
//     if (!paths)
//         return NULL;

//     path = check_paths(paths, command);
//     return path;
// }
