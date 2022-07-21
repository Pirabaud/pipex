/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pirabaud <pirabaud@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 11:27:46 by pirabaud          #+#    #+#             */
/*   Updated: 2022/07/21 17:03:58 by pirabaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <string.h>

void	here_doc(char **argv,char **env)
{
	int 	fd;
	char *line;
	int		i;
	pid_t	son;
	pid_t	file;
	char	**cmd;
	

	(void) env;
	fd = open("here_doc", O_WRONLY | O_CREAT, S_IRWXU);
	line = get_next_line(0, 1);
	line[ft_strlen(line) - 1] = 0;
	while (ft_strcmp(line, argv[2]))
	{
		i = 0;
		while (line[i])
		{
			write(fd, &line[i++], 1);
		}
		write(fd, "\n", 1);
		free(line);
		line = NULL;
		line = get_next_line(0, 1);
		line[ft_strlen(line) - 1] = 0;
	}
	write(fd, '\0', 1);
	ft_split(argv[3]);
	son = fork();
	if (son == 0)
		first_call(pipexfd, fd, cmd, env);
	file = fork();
	if (file == 0)
	{
		wait_pid(son);
		execve(check_path("rm", env), ft_split("rm here_doc"), env);
	}
	son2 = fork();
	if (son2 == 0)
	{
		waitpid(son);
		last_call(cmd, pipexfd, file);
	}
}
