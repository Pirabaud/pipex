/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pirabaud <pirabaud@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:43:13 by pirabaud          #+#    #+#             */
/*   Updated: 2022/07/18 15:03:45 by pirabaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_cmd(t_sons *cmd)
{
	int	i;

	i = 0;
	if (cmd->env != NULL)
		free_dpointer(cmd->env);
	if (cmd->file != NULL)
		free(cmd->file);
	if (cmd->cmd != NULL)
	{	
		while (cmd->cmd[i] != NULL)
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
	free(cmd);
}

void	print_error(char *str, t_sons *first)
{
	if (first != NULL)
		free_cmd(first);
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

void	check_arg(int argc, char **argv, char **env)
{
	char	**test;
	char	*check;
	int		i;

	i = 2;
	if (argc < 5)
		print_error("bad count argc\n", NULL);
	while ((argc - 3) > 0)
	{
		test = ft_split(argv[i++], ' ');
		if (test == NULL)
			print_error(strerror(errno), NULL);
		check = check_path(test[0], env);
		if (check == NULL)
		{
			free_dpointer(test);
			print_error(strerror(errno), NULL);
		}
		else
		{
			free_dpointer(test);
			free(check);
		}
		argc--;
	}
}
