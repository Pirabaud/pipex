/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pirabaud <pirabaud@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:15 by pirabaud          #+#    #+#             */
/*   Updated: 2022/06/25 15:15:36 by pirabaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

t_sons	*init_null(void)
{
	t_sons	*init;

	init = malloc(sizeof(t_sons));
	if (init == NULL)
	{
		perror(strerror(errno));
		exit (1);
	}
	init->env = NULL;
	init->file = NULL;
	init->cmd = NULL;
	init->path = NULL;
	return (init);
}

t_sons	*init_last(char **argv, char **env, int index)
{
	t_sons	*second;

	second = init_null();
	second->env = ft_tabdup(env);
	if (second->env == NULL)
		print_error(strerror(errno), second);
	second->file = ft_strdup(argv[index]);
	if (second->file == NULL)
		print_error(strerror(errno), second);
	second->cmd = ft_split(argv[index - 1], ' ');
	if (second->cmd == NULL)
		print_error(strerror(errno), second);
	second->path = check_path(second->cmd[0], env);
	if (second->path == NULL)
		print_error(strerror(errno), second);
	return (second);
}

t_sons	*init_first(char **argv, char **env)
{
	t_sons	*first;

	first = init_null();
	first->env = ft_tabdup(env);
	if (first->env == NULL)
		print_error(strerror(errno), first);
	first->file = ft_strdup(argv[1]);
	if (first->file == NULL)
		print_error(strerror(errno), first);
	first->cmd = ft_split(argv[2], ' ');
	if (first->cmd == NULL)
		print_error(strerror(errno), first);
	first->path = check_path(first->cmd[0], env);
	if (first->path == NULL)
		print_error(strerror(errno), first);
	return (first);
}

t_sons	*init_midl(int index, char **argv, char **env)
{
	t_sons	*midl;

	midl = init_null();
	midl->env = ft_tabdup(env);
	if (midl->env == NULL)
		print_error(strerror(errno), midl);
	midl->cmd = ft_split(argv[index + 1], ' ');
	if (midl->cmd == NULL)
		print_error(strerror(errno), midl);
	midl->path = check_path(midl->cmd[0], env);
	if (midl->path == NULL)
		print_error(strerror(errno), midl);
	return (midl);
}

t_sons	*init_lstcmd(char **argv, char **env, int argc, int i)
{
	t_sons	*cmd;

	if (i == 1)
		cmd = init_first(argv, env);
	else if (i < argc - 3)
		cmd = init_midl(i, argv, env);
	else
		cmd = init_last(argv, env, i);
	return (cmd);
}
