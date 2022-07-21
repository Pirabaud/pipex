/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pirabaud <pirabaud@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 09:59:03 by pirabaud          #+#    #+#             */
/*   Updated: 2022/07/18 15:03:23 by pirabaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	first_pipe(int	**pipexfd, pid_t *son, t_sons *cmd)
{
	pipe(pipexfd[0]);
	son[0] = fork();
	if (son[0] == 0)
		first_call(pipexfd[0], cmd);
	free_cmd(cmd);
}

void	mid_pipe(int **pipexfd, pid_t *son, t_sons *cmd, int i)
{
	pipe(pipexfd[i]);
	son[i] = fork();
	if (son[i] == 0)
		mid_call(pipexfd[i - 1], pipexfd[i], cmd, son[i - 1]);
	close(pipexfd[i - 1][1]);
	close(pipexfd[i - 1][0]);
	free_cmd(cmd);
}

void	last_pipe(int	**pipexfd, pid_t *son, t_sons *cmd, int i)
{
	son[i] = fork();
	if (son[i] == 0)
		last_call(cmd, pipexfd[i - 1], son[i - 1]);
	close(pipexfd[i - 1][1]);
	close(pipexfd[i - 1][0]);
	free_cmd(cmd);
	waitpid(son[i], NULL, 0);
}

int	**malloc_pipe(int argc)
{
	int	**pipexfd;
	int	i;

	i = 0;
	pipexfd = malloc((argc - 3) * sizeof(int *));
	while (i < (argc - 3))
		pipexfd[i++] = malloc(2 * sizeof(int));
	return (pipexfd);
}

int	main(int argc, char **argv, char **env)
{
	t_sons	*cmd;
	pid_t	*son;
	int		**pipexfd;
	int		i;

	i = 0;
	if (ft_memcmp(argv[1], "here_doc", ft_strlen("here_doc")) == 0)
		{
			here_doc(argv, env);
			return (0);
		}
	check_arg(argc, argv, env);
	pipexfd = malloc_pipe(argc);
	son = malloc((argc - 3) * sizeof(int));
	cmd = init_lstcmd(argv, env, argc, i + 1);
	first_pipe(pipexfd, son, cmd);
	while (++i < (argc - 4))
	{
		cmd = init_lstcmd(argv, env, argc, i + 1);
		mid_pipe(pipexfd, son, cmd, i);
	}
	cmd = init_lstcmd(argv, env, argc, argc - 1);
	last_pipe(pipexfd, son, cmd, i);
	i = 0;
	while (i < (argc - 3))
		free(pipexfd[i++]);
	free(pipexfd);
	free(son);
}
