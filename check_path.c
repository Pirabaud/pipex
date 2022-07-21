/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pirabaud <pirabaud@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:39:15 by pirabaud          #+#    #+#             */
/*   Updated: 2022/06/25 15:11:24 by pirabaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**ft_tabdup(char **env)
{
	int		i;
	int		j;
	char	**res;

	i = 0;
	j = 0;
	while (env[i] != NULL)
		i++;
	res = malloc((i + 1) * sizeof(char *));
	if (res == NULL)
		return (NULL);
	while (j < i)
	{
		res[j] = ft_strdup(env[j]);
		if (res[j] == NULL)
		{
			free_dpointer(res);
			return (NULL);
		}
		++j;
	}
	res[j] = NULL;
	return (res);
}

char	*init_res(char *path, char *cmd)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(path, "/");
	res = ft_strjoin(tmp, cmd);
	free(tmp);
	return (res);
}

char	*check_path(char *cmd, char **env)
{
	int		i;
	char	*res;
	char	**path;

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
