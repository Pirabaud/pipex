/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pirabaud <pirabaud@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/27 16:46:38 by pirabaud          #+#    #+#             */
/*   Updated: 2022/06/22 14:34:32 by pirabaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>
# include <stdio.h>
# include <string.h>

typedef struct	s_sons
{
	char 			*file;
	char 			**cmd;
	char 			*path;
	struct s_sons	*next;
	pid_t			*son;
	int				nb_cmd;
	char			**env;
}	t_sons;

void	free_cmd(t_sons *cmd);
void	free_dpointer(char **str);
void	print_error(char *str, t_sons *first, t_sons *second);

void	first_call(int *pipexfd, char **env, t_sons *first);
void	mid_call(int *pipe, int *pipenext, char **env, t_sons *cmd, pid_t first);
void	last_call(t_sons *second, int *pipexfd, char **env, pid_t prev);

t_sons	*init_lstcmd(char **argv, char **env, int argc, int i);
char	*check_path(char *cmd, char **env);

#endif
