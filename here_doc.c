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

void	here_doc_call(int *pipexfd, t_sons *first)
{
	int		fd;

	fd = open("here_doc", O_RDONLY);
	if (fd == -1)
		print_error("error first", first);
	ft_printf("salut1\n");
	close(pipexfd[0]);
	ft_printf("salut2\n");
	dup2(pipexfd[1], 1);
	dup2(fd, 0);
	close(fd);
	execve(first->path, first->cmd, first->env);
}
t_sons	*initcmd_heredoc(char *str, char *cmd, char **env)
{
	t_sons *here_doc;
	
	here_doc = init_null();
	here_doc->env = ft_tabdup(env);
	here_doc->file = ft_strdup(str);
	here_doc->cmd = ft_split(cmd, ' ');
	here_doc->path = check_path(here_doc->cmd[0], env);
	return (here_doc);
}

void	here_doc(char **argv, char **env)
{
	int 	fd;
	char *line;
	int	i;
	pid_t	son;
	pid_t	file;
	pid_t	son2;
	int	*pipexfd;
	t_sons	*here_doc;
	t_sons	*last_cmd;
	

	(void) env;
	fd = open("here_doc", O_WRONLY | O_CREAT, S_IRWXU);
	line = get_next_line(0, 1);
	line[ft_strlen(line) - 1] = 0;
	pipexfd = malloc(2 * sizeof(int));
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
	pipe(pipexfd);
	close(fd);
	here_doc = initcmd_heredoc("here_doc", argv[3], env);
	son = fork();
	if (son == 0)
		here_doc_call(pipexfd, here_doc);
	last_cmd = init_last(argv,env, 5);
	son2 = fork();
	if (son2 == 0)
	{
		last_call(last_cmd, pipexfd, son);
	}
	ft_printf("salut4\n");
	close(pipexfd[1]);
	close(pipexfd[0]);
	waitpid(son2, NULL, 0);
	file = fork();
	if (file == 0)
	{
		execve(check_path("rm", env), ft_split("rm here_doc", ' '), env);
	}
	waitpid(file, NULL, 0);

}
