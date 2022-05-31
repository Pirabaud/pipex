void	free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i] != NULL)
		free(split[i++])
	free(split);
}

void	first_call(char *file, char **cmd, int *pipexfd, char **env)
{
	int fd;

	fd = open(file, O_RDONLY);
	dup2(fd, 0);
	dup2(pipexfd[1], 1);
	close(pipexfd[0]);
	close(fd);
	execve(cmd[0], cmd + 1, env);
}

void	second_call(char *file, char **cmd, int *pipexfd, char **env)
{
	int fd;

	fd = open(file, O_CREATE | O_WRONLY | S_IRWXU);
	dup2(fd, 1);
	dup2(pipexfd[0], 0);
	close(pipexfdfd[1]);
	close fd;
	execve(cmd[0], cmd + 1, env);
}

int	main(int argc, char **argv, char **env)
{
	pid_t first;
	pid_t second;
	char **cmd;
	int pipexfd[2];

	if (argc != 5)
	{
		ft_printf("error");
		return (0);
	}
	cmd = ft_split(argv[2], ' ');
	first = fork();
	if (first == 0)
		first_call(argv[1], cmd, pipexfd, env);
	free_split(cmd);
	cmd = ft_split(argv[3], ' ');
	second = fork();
	if (second == 0)
		second_call(argv[4],cmd, pipexfd, env);
	free_split(cmd);
	waidpid(first, NULL, 0);
	waidpid(second, NULL, 0);
}
