/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pirabaud <pirabaud@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 09:59:03 by pirabaud          #+#    #+#             */
/*   Updated: 2022/06/07 15:00:59 by pirabaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_sons(t_sons *sons)
{
	int	i;

	i = 0;
	if (sons->file != NULL)
		free(sons->file);
	if (sons->cmd != NULL)
	{	
		while(sons->cmd[i] != NULL)
		{
			free(sons->cmd[i]);
			sons->cmd[i++] = NULL;
		}
		free(sons->cmd);
		sons->cmd = NULL;
	}
	if (sons->path != NULL)
	{
		free(sons->path);
		sons->path = NULL;
	}
	free(sons);
}

void	print_error(char *str, t_sons *first, t_sons *second)
{
	if (first != NULL)
		free_sons(first);
	if (second != NULL)
		free_sons(second);
	(void)str;
	perror(str);
	exit(1);
}

void	free_dpointer(char **str)
{
	int	i;

	i = 0;
	while (str[i] != NULL)
		free(str[i++]);
	free(str);
}

char *init_res(char *path, char *cmd)
{
	char *tmp;
	char *res;

	tmp = ft_strjoin(path, "/");
	res = ft_strjoin(tmp, cmd);
	free(tmp);
	return(res);
}

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
	res = init_res(path[i++], cmd);
	while (access(res, F_OK) != 0 && path[i] != NULL)
	{
		free(res);
		res = init_res(path[i++], cmd);
	}
	free_dpointer(path);
	if (access(res, F_OK) == 0)
		return (res);
	free(res);
	if (access(cmd, F_OK) == 0)
		return (ft_strdup(cmd));
	else
		return (NULL);
}

void	first_call(int *pipexfd, char **env, t_sons *first)
{
	int		fd;

	fd = open(first->file, O_RDONLY);
	if (fd == -1)
	print_error(strerror(errno), first, NULL);
	close(pipexfd[0]);
	dup2(pipexfd[1], 1);
	dup2(fd, 0);
	close(fd);
	execve(first->path, first->cmd, env);
}

void	second_call(t_sons *second, int *pipexfd, char **env, t_sons *first)
{
	int fd;

	fd = open(second->file,O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU);
	if (fd == -1)
		print_error(strerror(errno), first, second);	
	waitpid(first->son, NULL, 0);
	close(pipexfd[1]);
	dup2(fd, 1);
	dup2(pipexfd[0], 0);
	close(fd);
	execve(second->path, second->cmd, env);
}

t_sons	*init_null(void)
{
	t_sons *init;

	init = malloc(sizeof(t_sons));
	if (init == NULL)
	{
		perror(strerror(errno));
		exit (1);
	}
	init->file = NULL;
	init->cmd = NULL;
	init->path = NULL;
	
	return (init);
}


t_sons	*init_first(char **argv, char **env, t_sons *init)
{
	t_sons *first;
	
	first = init;
	first->file = ft_strdup(argv[1]);
	if (first->file == NULL)
		print_error(strerror(errno), first, NULL);
	first->cmd = ft_split(argv[2], ' ');
	if (first->cmd == NULL)
		print_error(strerror(errno), first, NULL);
	first->path = check_path(first->cmd[0], env);
	if (first->path == NULL)
		print_error(strerror(errno), first, NULL);
	first->son = fork();
	if (first->son == -1)
		print_error(strerror(errno), first, NULL);
	return (first);
}

t_sons	*init_second(char **argv, char **env, t_sons *init, t_sons *first)
{
	t_sons *second;
	
	second = init;
	second->file = ft_strdup(argv[4]);	
	if (second->file == NULL)
		print_error(strerror(errno), second, first);
	second->cmd	= ft_split(argv[3], ' ');
	if (second->cmd == NULL)
		print_error(strerror(errno), second, first);
	second->path = check_path(second->cmd[0], env);
	if (second->path == NULL)
		print_error(strerror(errno), second, first);
	second->son = fork();
	if (second->son == -1)
		print_error(strerror(errno), second, first);
	return (second);
}

int	main(int argc, char **argv, char **env)
{
	t_sons	*first;
	t_sons	*second;
	int		pipexfd[2];

	if (argc != 5)
	{
		perror("bad count argc");
		return(0);
	}
	pipe(pipexfd);
	first =	init_null();
	first = init_first(argv, env, first);
	if (first->son == 0)
		first_call(pipexfd, env, first);
	second = init_null();
	second = init_second(argv, env, second, first);
	if (second->son == 0)
		second_call(second, pipexfd, env, first);
	close(pipexfd[1]);
	close(pipexfd[0]);
	waitpid(first->son, NULL, 0);
	waitpid(second->son, NULL, 0);
	free_sons(first);
	free_sons(second);
}
