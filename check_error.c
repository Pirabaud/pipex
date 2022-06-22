/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pirabaud <pirabaud@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:43:13 by pirabaud          #+#    #+#             */
/*   Updated: 2022/06/22 14:43:06 by pirabaud         ###   ########.fr       */
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

void	print_error(char *str, t_sons *first, t_sons *second)
{
	if (first != NULL)
		free_cmd(first);
	if (second != NULL)
		free_cmd(second);
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

