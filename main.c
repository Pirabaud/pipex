/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pirabaud <pirabaud@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 09:59:03 by pirabaud          #+#    #+#             */
/*   Updated: 2022/06/22 18:21:17 by pirabaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **env)
{
	t_sons	*cmd;
	pid_t	son[argc - 3];
	int		pipexfd[argc - 3][2];
	int		i;

	i = 0;
	if (argc < 5)
	{
		perror("bad count argc\n");
		return (0);
	}
	pipe(pipexfd[i]);
	cmd = init_lstcmd(argv, env, argc, i + 1);
	son[i] = fork();
	if (son[i] == 0)
		first_call(pipexfd[i], env, cmd);
	free_cmd(cmd);
	while (++i < (argc - 4))
	{ 
		pipe(pipexfd[i]);
		cmd = init_lstcmd(argv, env, argc, i + 1);
		son[i] = fork();
		if(son[i] == 0)
			mid_call(pipexfd[i - 1], pipexfd[i], env, cmd, son[i - 1]);
		close(pipexfd[i - 1][1]);
		close(pipexfd[i - 1][0]);
		free_cmd(cmd);
	}
	cmd = init_lstcmd(argv, env, argc, argc - 1);
	son[i] = fork();
	if (son[i] == 0)
		last_call(cmd, pipexfd[i - 1], env, son[i - 1]);
	close(pipexfd[i - 1][1]);
	close(pipexfd[i - 1][0]);
	
	i = 0;
	while (i > 0)
		waitpid(son[i--], NULL, 0);
}
