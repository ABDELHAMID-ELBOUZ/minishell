/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_helper2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:48:59 by aelbouz           #+#    #+#             */
/*   Updated: 2025/06/26 08:37:30 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_status(char **args, char *env_path, char **full_path)
{
	int			status;
	struct stat	path_stat;

	status = check_executable(args[0], env_path, full_path);
	if (status != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		if (status == 127)
		{
			if (env_path)
				ft_putstr_fd(": command not found\n", 2);
			else if (env_path == NULL)
				ft_putstr_fd(": No such file or directory\n", 2);
		}
		else if (status == 126)
		{
			if (stat(args[0], &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
				ft_putstr_fd(": is a directory\n", 2);
			else
				ft_putstr_fd(": Permission denied\n", 2);
		}
	}
	return (status);
}

int	is_not_builtin(char **args, char *env_path, t_env *env)
{
	char	*full_path;
	int		status;
	char	**envp;

	full_path = NULL;
	status = check_status(args, env_path, &full_path);
	if (status != 0)
		exit(status);
	envp = env_to_array(env);
	if (!envp)
	{
		free(full_path);
		exit(1);
	}
	execve(full_path, args, envp);
	free(full_path);
	free_arr(envp);
	perror(args[0]);
	exit(126);
}

void	free_cmd(t_command *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	if (cmd->redirects)
		free_redirects(cmd->redirects);
	free(cmd);
}
