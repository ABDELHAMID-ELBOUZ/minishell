/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 13:26:56 by aelbouz           #+#    #+#             */
/*   Updated: 2025/05/03 14:41:06 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	updat_env(t_env **env, char *key, char *value)
{
	t_env	*tmp;

	if (find_and_update(env, key, value))
		return ;
	tmp = malloc(sizeof(t_env));
	if (!tmp)
		return (free(key));
	tmp->key = ft_strdup(key);
	if (value)
		tmp->value = ft_strdup(value);
	else
		tmp->value = NULL;
	if (value && !tmp->value)
		return (free(tmp->key), free(tmp));
	tmp->next = NULL;
	ft_lstadd_back(env, tmp);
}

int	ft_cd(char **args, t_env **env)
{
	char	*oldpwd;
	char	*newpwd;
	char	*path;

	path = args[1];
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (1);
	if (!path || ft_strcmp(path, "~") == 0)
		path = getenv("HOME");
	if (!path)
		return (free(oldpwd), 1);
	if (chdir(path) == -1)
		return (ft_putstr_fd("cd: ", 2), write(2, path, ft_strlen(path)), \
		ft_putstr_fd(" No such file or directory\n", 2), free(oldpwd), 1);
	newpwd = getcwd(NULL, 0);
	if (!newpwd)
		return (free(oldpwd), 1);
	updat_env(env, "OLDPWD=", oldpwd);
	updat_env(env, "PWD=", newpwd);
	return (free(oldpwd), free(newpwd), 0);
}

int	ft_exit(char **args)
{
	ft_putstr_fd("exit\n", 1);
	if (!args || !args[1])
		exit (0);
	if ((ft_atoi(args[1])) == 1)
	{
		ft_putstr_fd("exit: numeric argument required\n", 2);
		exit (2);
	}
	else if (args[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		exit (1);
	}
	exit (ft_atoi(args[1]) % 256);
}

int	ft_pwd(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
		return (1);
	write(1, path, ft_strlen(path));
	write(1, "\n", 1);
	free(path);
	return (0);
}
