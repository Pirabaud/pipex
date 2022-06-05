/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pirabaud <pirabaud@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 09:59:03 by pirabaud          #+#    #+#             */
/*   Updated: 2022/06/05 15:55:05 by pirabaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i] != NULL)
		free(split[i++]);
	free(split);
}

char	*check_path(char **path, char *cmd)
{
	int	i;
	char *res;

	i = 0;
	res = ft_strjoin(path[i], cmd);
	while (access(res, F_OK | R_OK | W_OK | X_OK) != 0 || path[i] != NULL)
	{
		free(res);
		res = ft_strjoin(path[i], cmd);
		i++;
	}
	if (access(res, F_OK | R_OK | W_OK | X_OK) == 0)
		return (res);
	else if (access(cmd, F_OK | R_OK | W_OK | X_OK) == 0)
		return (cmd);
	else
		return (NULL);
}

void	first_call(int *pipexfd, t_fils first)
{
	int		fd;
	char	*cmd;

	cmd = check_path(
	fd = open(first.file, O_RDONLY);
	close(pipexfd[0]);
	dup2(pipexfd[1], 1);
	dup2(fd, 0);
	close(fd);
	execve(cmd[0], cmd, env);
}

void	second_call(int *pipexfd, t_fils second, pid_t first)
{
	int fd;

	fd = open(file,O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU);
	waitpid(first, NULL, 0);
	close(pipexfd[1]);
	dup2(fd, 1);
	dup2(pipexfd[0], 0);
	close(fd);
	execve(cmd[0], cmd, env);
}

char **init_path(char **env)
{
	char	**res;
	int		i;

	i = 0;
	while (ft_memcmp(env[i], "PATH", 4) != 0)
			++i; 
}

int	main(int argc, char **argv, char **env)
{
	t_fils first;
	t_fils second;
	char **cmd;
	int pipexfd[2];

	if (argc != 5)
	
		ft_printf("error");
		return (0);
	}
	pipe(pipexfd);
	first.cmd = ft_split(argv[2], ' ');
	first.fils = fork();
	first.path = init_path(env);
	if (first == 0)
		first_call(argv[1], cmd, pipexfd, env);
	cmd = ft_split(argv[3], ' ');
	second = fork();
	if (second == 0)
		second_call(argv[4],cmd, pipexfd, env, first);
	close(pipexfd[1]);
	close(pipexfd[0]);
	waitpid(first, NULL, 0);
	waitpid(second, NULL, 0);
	free_split(cmd);
}
