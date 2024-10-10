/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_leak.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 15:25:35 by lzhang2           #+#    #+#             */
/*   Updated: 2024/10/10 18:31:26 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"









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
