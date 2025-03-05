/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akahir <akahir@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:57:10 by akahir            #+#    #+#             */
/*   Updated: 2025/03/05 13:57:10 by akahir           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_pipex.h"

static	void	opening_files(char **argv, int *inf, int *outf)
{
	*inf = open(argv[1], O_RDONLY);
	if (*inf < 0)
	{
		perror("didn't have source file");
		exit(1);
	}
	*outf = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (*outf < 0)
	{
		perror("Error opening output file");
		exit(1);
	}
}

static	void	execute_cmd1(pid_t pid1, int inf, int pipe_fd[2], char **argv)
{
	char	**cmd1;

	if (pid1 == -1)
		exit(1);
	else if (pid1 == 0)
	{
		close(pipe_fd[0]);
		dup2(inf, 0);
		dup2(pipe_fd[1], 1);
		close(inf);
		close(pipe_fd[1]);
		cmd1 = ft_split(argv[2], ' ');
		if (!cmd1)
			exit(1);
		execvp(cmd1[0], cmd1);
		perror("Exec failed");
		free_split(cmd1);
		exit(127);
	}
}

static	void	execute_cmd2(pid_t pid2, int outf, int pipe_fd[2], char **argv)
{
	char	**cmd2;

	if (pid2 == -1)
		exit(1);
	else if (pid2 == 0)
	{
		close(pipe_fd[1]);
		dup2(outf, 1);
		dup2(pipe_fd[0], 0);
		close(outf);
		close(pipe_fd[0]);
		cmd2 = ft_split(argv[3], ' ');
		if (!cmd2)
			exit(1);
		execvp(cmd2[0], cmd2);
		perror("Exec failed");
		free_split(cmd2);
		exit(127);
	}
}

int	main(int argc, char **argv)
{
	pid_t	pid1;
	pid_t	pid2;
	int		inf;
	int		outf;
	int		pipe_fd[2];

	if (argc != 5)
		exit(1);
	opening_files(argv, &inf, &outf);
	if (pipe(pipe_fd) == -1)
		exit(1);
	pid1 = fork();
	if (pid1 == 0)
		execute_cmd1(pid1, inf, pipe_fd, argv);
	else if (pid1 > 0)
	{
		pid2 = fork();
		if (pid2 == 0)
		execute_cmd2(pid2, outf, pipe_fd, argv);
		else if (pid2 > 0)
		{
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			close(inf);
			close(outf);
			waitpid(pid1, NULL, 0);
			waitpid(pid2, NULL, 0);
		}
		else
			exit(1);
	}
	else
		exit(1);
}
