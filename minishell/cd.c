/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:03:01 by aelbouz           #+#    #+#             */
/*   Updated: 2025/04/22 11:35:59 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	updat_env(t_env **env, char *key, char *value)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (tmp->key || ft_strcmp(tmp->key, key) == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			return ;
		}
		tmp = tmp->next;
	}
	tmp = malloc(sizeof(t_env));
	if (!tmp)
		return (ft_putstr_fd("update env: malloc failed", 2));
	tmp->key = ft_strdup(key);
	tmp->value = ft_strdup(value);
	if (!tmp->key || !tmp->value)
		return (free(tmp->key), free(tmp->value), free(tmp));
	tmp->next = *env;
	*env = tmp;
}

int	ft_cd(char **args, t_env **env)
{
	char	*oldpwd;
	char	*newpwd;
	char	*path;

	path = args[1];
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (ft_putstr_fd("cd : getcwd faild\n", 2), 1);
	if (!path || ft_strcmp(path, "~") == 0)
		path = getenv("HOME");
	if (!path)
		return (free(oldpwd), ft_putstr_fd("cd: HOME not set\n", 2), 1);
	if (chdir(path) == -1)
		return (free(oldpwd), ft_putstr_fd("cd: ", 2), ft_putstr_fd(path, 2), \
		ft_putstr_fd(" No such file or directory\n", 2), 1);
	newpwd = getcwd(NULL, 0);
	if (!newpwd)
		return (free(oldpwd), ft_putstr_fd("cd : getcwd faild\n", 2), 1);
	updat_env(env, "OLDPWD", oldpwd);
	updat_env(env, "PWD", newpwd);
	return (free(oldpwd), free(newpwd), 0);
}
