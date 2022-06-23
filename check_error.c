/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pirabaud <pirabaud@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:43:13 by pirabaud          #+#    #+#             */
/*   Updated: 2022/06/23 15:59:09 by pirabaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_cmd(t_sons *cmd)
{
	int	i;

	i = 0;
	if (cmd->file != NULL)
		free(cmd->file);
	if (cmd->cmd != NULL)
	{	
		while(cmd->cmd[i] != NULL)
		{
			free(cmd->cmd[i]);
			cmd->cmd[i++] = NULL;
		}
		free(cmd->cmd);
		cmd->cmd = NULL;
	}
	if (cmd->path != NULL)
	{
		free(cmd->path);
		cmd->path = NULL;
	}
	cmd->next = NULL;
	free(cmd);
}

void	print_error(char *str, t_sons *first)
{
	free_cmd(first);
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

