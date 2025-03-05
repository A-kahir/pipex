#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>

int		main(int argc, char **argv);
int		ft_strlen(char *str);
char	**ft_split(const char *s, char c);
void	free_split(char **split);

#endif