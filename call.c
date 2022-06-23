/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pirabaud <pirabaud@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:23:20 by pirabaud          #+#    #+#             */
/*   Updated: 2022/06/23 18:18:59 by pirabaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	first_call(int *pipexfd, char **env, t_sons *first)
{
	int		fd;

	fd = open(first->file, O_RDONLY);
	if (fd == -1)
		print_error(strerror(errno), first);	
	close(pipexfd[0]);
	dup2(pipexfd[1], 1);
	dup2(fd, 0);
	close(fd);
	execve(first->path, first->cmd, env);
}
void	last_call(t_sons *second, int *pipexfd, char **env, pid_t prev)
{
	int fd;
	waitpid(prev, NULL, 0);
	fd = open(second->file,O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU);
	if (fd == -1)
		print_error(strerror(errno), second);	
	close(pipexfd[1]);
	dup2(fd, 1);
	dup2(pipexfd[0], 0);
	close(fd);
	execve(second->path, second->cmd, env);
}

void	mid_call(int	*pipe, int	*pipenext, char **env, t_sons *cmd, pid_t first)
{
	waitpid(first, NULL, 0);
	close(pipenext[0]);
	close(pipe[1]);
	dup2(pipe[0], 0);
	dup2(pipenext[1], 1);
	execve(cmd->path, cmd->cmd, env);
}

