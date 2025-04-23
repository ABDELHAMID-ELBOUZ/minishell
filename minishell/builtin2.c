/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 12:21:49 by aelbouz           #+#    #+#             */
/*   Updated: 2025/04/23 12:46:09 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(char **args)
{
	int	no_new_line;
	int	i;

	if (!args)
		return (1);
	i = 1;
	no_new_line = 1;
	while (args[i] && ft_strcmp(args[i], "-n") == 0)
	{
		no_new_line = 0;
		i++;
	}
	while (args[i])
	{
		write(1, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (no_new_line == 1)
		write(1, "\n", 1);
	return (0);
}

int ft_env()
{
    char **envp;
    int i;

    envp = env_to_array(get_env(NULL));
    if (!envp)
        return (1);
    i = 0;
    while (envp[i])
    {
        printf("%s\n", envp[i]);
        i++;
    }
    free_arr(envp);
    return (0);
}

int	is_builtin(char *cmd, char **args, char *env_path, t_env **env)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (ft_echo(args));
	if (ft_strcmp(cmd, "pwd") == 0)
		return (ft_pwd());
	if (ft_strcmp(cmd, "exit") == 0)
		return (ft_exit(args));
	if (ft_strcmp(cmd, "cd") == 0)
		return (ft_cd(args, env));
	if (ft_strcmp(cmd, "env") == 0)
		return (ft_env());
	return (execute_command(args, env_path));
}
