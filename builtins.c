/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 12:21:49 by aelbouz           #+#    #+#             */
/*   Updated: 2025/04/17 11:22:12 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_builtin(char *cmd, char **args, char *env_path)
{
	if (!cmd || !args)
        return (1);
	if (ft_strncmp(cmd, "echo", 4) == 0)
			return (ft_echo(args));
	else if (ft_strncmp(cmd, "pwd", 3) == 0)
		return (ft_pwd());
	else if (ft_strncmp(cmd, "exit", 4) == 0)
		return (ft_exit(args));
	else
		return (execut_command(args, env_path));
}
