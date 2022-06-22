/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pirabaud <pirabaud@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:15 by pirabaud          #+#    #+#             */
/*   Updated: 2022/06/22 17:45:49 by pirabaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

t_sons	*init_last(char **argv, char **env, int index)
{
	t_sons *second;
	
	second = init_null();
	second->file = ft_strdup(argv[index]);
//	if (second->file == NULL)
//		print_error(strerror(errno), second, first);
	second->cmd	= ft_split(argv[index - 1], ' ');
//	if (second->cmd == NULL)
//		print_error(strerror(errno), second, first);
	second->path = check_path(second->cmd[0], env);
//	if (second->path == NULL)
//		print_error(strerror(errno), second, first);
//	if (second->son == -1)
//		print_error(strerror(errno), second, first);
	return (second);
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

t_sons	*init_midl(int index, char **argv, char **env)
{
	t_sons	*midl;
	
	midl = init_null();
	midl->cmd = ft_split(argv[index], ' ');
	midl->path = check_path(midl->cmd[0], env);
	midl->next = NULL;
	return (midl);
}

t_sons	*init_lstcmd(char **argv, char **env, int argc, int i)
{
	t_sons *cmd;

	(void)argc;
	if ( i == 1)
	cmd = init_first(argv, env);
	else if ( i < argc - 4)
		cmd = init_midl(i, argv, env);
	else
		cmd = init_last(argv, env, i);
	return (cmd);
}
