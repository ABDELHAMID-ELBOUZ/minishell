/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exucutable.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 10:48:33 by aelbouz           #+#    #+#             */
/*   Updated: 2025/04/19 13:08:03 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_executable(char *cmd, char *env_path)
{
	char	**dirs;
	char	*full_path;
	char	*tmp;
	int		i;

	if (!env_path || !cmd || !*env_path)
		return (NULL);
	dirs = ft_split(env_path, ':');
	if (!dirs)
		return (NULL);
	i = 0;
	while(dirs[++i])
	{
		tmp = ft_strjoin(dirs[i], "/");
		if (!tmp)
			return(free_arr(dirs), NULL);
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!full_path)
			return (free_arr(dirs), NULL);
		if (access(full_path, X_OK) == 0)
			return (free_arr(dirs), full_path);
		free(full_path);
	}
	return (free_arr(dirs), NULL);
}

int	check_executable(char *cmd, char *env_path, char **full_path)
{
	if (!cmd)
		return (127);
	if (cmd[0] == '/' || ft_strncmp(cmd, "../", 3) == 0 || ft_strncmp(cmd, "./", 2) == 0)
	{
		if (access(cmd, F_OK) != 0)
            return (127);
		if (access(cmd, X_OK) == 0)
		{
			*full_path = ft_substr(cmd, 0, ft_strlen(cmd));
		if (!*full_path)
			return (1);
		return (0);
		}
		return (126);
	}
	*full_path = find_executable(cmd, env_path);
	if (!*full_path)
		return(127);
	return (0);
}

int	check_status( char **args, char *env_path, char **full_path)
{
	int status;

	if (!args || !args[0])
		return (1);
	status = check_executable(args[0], env_path, full_path);
	if (status == 127)
	{
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	else if (status == 126)
		ft_putstr_fd("permission denied\n", 2);
	return (status);
}

int	execut_command(char **args, char *env_path)
{
	char	*full_path;
	int		status;
	int		pid;

	full_path = NULL;
	status = check_status(args, env_path, &full_path);
	if (status != 0)
		return (free(full_path), status);
	pid = fork();
	if (pid == -1)
		return (free(full_path), perror("fork"), 1);
	else if (pid == 0)
	{
		execve(full_path, args,__environ);
		return (free(full_path), perror("execve"), 1);
	}
	waitpid(pid, &status, 0);
	free(full_path);
    return (WEXITSTATUS(status));
}
