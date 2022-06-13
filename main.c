/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pirabaud <pirabaud@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 09:59:03 by pirabaud          #+#    #+#             */
/*   Updated: 2022/06/13 18:14:54 by pirabaud         ###   ########.fr       */
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
	sons->next = NULL;
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
//	print_error(strerror(errno), first, NULL);
	close(pipexfd[0]);
	dup2(pipexfd[1], 1);
	dup2(fd, 0);
	close(fd);
	execve(first->path, first->cmd, env);
}

void	second_call(t_sons *second, int *pipexfd, char **env, pid_t first, pid_t mid)
{
	int fd;

	fd = open(second->file,O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU);
	if (fd == -1)
//		print_error(strerror(errno), first, second);	
	waitpid(first, NULL, 0);
	waitpid(mid, NULL, 0);
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
	init->next = NULL;
	
	return (init);
}


t_sons	*init_first(char **argv, char **env)
{
	t_sons *first;
	
	first = init_null();
	first->file = ft_strdup(argv[1]);
//	if (first->file == NULL)
//		print_error(strerror(errno), first, NULL);
	first->cmd = ft_split(argv[2], ' ');
//	if (first->cmd == NULL)
//		print_error(strerror(errno), first, NULL);
	first->path = check_path(first->cmd[0], env);
//	if (first->path == NULL)
//		print_error(strerror(errno), first, NULL);
	return (first);
}

t_sons	*init_second(char **argv, char **env, int index, int argc)
{
	t_sons *second;
	
	second = init_null();
	second->file = ft_strdup(argv[argc]);	
//	if (second->file == NULL)
//		print_error(strerror(errno), second, first);
	second->cmd	= ft_split(argv[index], ' ');
//	if (second->cmd == NULL)
//		print_error(strerror(errno), second, first);
	second->path = check_path(second->cmd[0], env);
//	if (second->path == NULL)
//		print_error(strerror(errno), second, first);
//	if (second->son == -1)
//		print_error(strerror(errno), second, first);
	return (second);
}

t_sons	*init_midl(int index, char **argv, char **env)
{
	t_sons	*midl;

	midl = init_null();
	midl->cmd = ft_split(argv[index], ' ');
	midl->path = check_path(midl->cmd[0], env);
	midl->next = NULL;
	return (midl);
}

t_sons	*init_lstcmd(int argc, char **argv, char **env)
{
	t_sons *lstcmd;
	t_sons *tmp;
	int		i;
	int		last_arg;

	last_arg = argc - 1;
	i = 3;
	lstcmd = init_first(argv, env);
	tmp = lstcmd;
	while((argc - 3) < 0)
	{
		tmp->next = init_midl(i++, argv, env);
		tmp = tmp->next;
		--argc;
	}
	tmp->next = init_second(argv, env, i, last_arg);
	return (lstcmd);
	
}

void	mid_call(int	*pipe, int	*pipenext, char **env, t_sons *cmd, pid_t first)
{
	waitpid(first, NULL, 0);
	close(pipenext[0]);
	close(pipe[1]);
	dup2(pipe[1], 0);
	dup2(pipenext[0], 1);
	execve(cmd->path, cmd->cmd, env);
}

int	main(int argc, char **argv, char **env)
{
	t_sons	*lst_cmd;
	pid_t	first;
	pid_t	mid;
	pid_t	last;
	int		pipexfd[3][2];
	int		nb_cmd;
	int		i;

	i = 0;
	nb_cmd = argc - 3;
	if (argc < 5)
	{
		perror("bad count argc");
		return(0);
	}
	pipe(pipexfd[i]);
	lst_cmd = init_lstcmd(argc, argv, env);
	first = fork();
	if (first == 0)
		first_call(pipexfd[i], env, lst_cmd);
	lst_cmd = lst_cmd->next;
	--nb_cmd;
	while (nb_cmd > 1)
	{
		pipe(pipexfd[++i]);
		mid = fork();
		if(mid == 0)
			mid_call(pipexfd[i - 1], pipexfd[i + 1], env, lst_cmd, first);
		lst_cmd = lst_cmd->next;
		close(pipexfd[i][1]);
		close(pipexfd[i][0]);
		--nb_cmd;
	} 
	last = fork();
	if (last == 0)
		second_call(lst_cmd, pipexfd[i], env, first, mid);
	close(pipexfd[i][1]);
	close(pipexfd[i][0]);
	waitpid(first, NULL, 0);
	waitpid(mid, NULL, 0);
	waitpid(last, NULL, 0);
}
