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

/*char **init_path(char **env)
{
	char	**res;
	int		i;

	i = 0;
			++i;
	i = 0;
	while( res != NULL)
		ft_printf("%s\n", res[i++]); 
	return (res);
}*/

char	*check_path(char *cmd, char **env)
{
	int	i;
	char *res;
	char **path;

	i = 0;
	while (ft_memcmp(env[i], "PATH", 4) != 0)
		i++;
	path = ft_split(env[i], ':');
	i = 0;
	res = ft_strjoin(path[i], cmd);
	while (ft_printf("%d\n", access(res, F_OK | R_OK | W_OK | X_OK) != 0))
	{
		free(res);
		res = ft_strjoin(path[i], "/");
		res = ft_strjoin(res, cmd);
		ft_printf("%s\n", res);
		i++;
	}
	if (access(res, F_OK | R_OK | W_OK | X_OK) == 0)
		return (res);
	else if (access(cmd, F_OK | R_OK | W_OK | X_OK) == 0)
		return (cmd);
	else
		return (NULL);
}

void	first_call(int *pipexfd, char **env, t_sons *first)
{
	int		fd;

	fd = open(first->file, O_RDONLY);
	close(pipexfd[0]);
	dup2(pipexfd[1], 1);
	dup2(fd, 0);
	close(fd);
	execve(first->path, first->cmd, env);
}

void	second_call(char *file, char **cmd, int *pipexfd, char **env, pid_t first)
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


t_sons	*init_first(char **argv, char **env)
{
	t_sons *first;
	
	first = malloc(sizeof(t_sons));
	first->file = ft_strdup(argv[1]);
	first->cmd = ft_split(argv[2], ' ');
	first->path = check_path(first->cmd[0], env);
	first->son = fork();
	return (first);
}  

int	main(int argc, char **argv, char **env)
{
	t_sons *first;
	pid_t second;
	char **cmd;
	int pipexfd[2];

	if (argc != 5)
	{
		ft_printf("error");
		return (0);
	}
	pipe(pipexfd);
	first = init_first(argv, env);
	if (first == 0)
		first_call(pipexfd, env, first);
	cmd = ft_split(argv[3], ' ');
	second = fork();
	if (second == 0)
		second_call(argv[4],cmd, pipexfd, env, first->son);
	close(pipexfd[1]);
	close(pipexfd[0]);
	waitpid(first->son, NULL, 0);
	waitpid(second, NULL, 0);
	free_split(cmd);
}
