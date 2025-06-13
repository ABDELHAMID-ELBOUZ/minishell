/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:44:07 by aelbouz           #+#    #+#             */
/*   Updated: 2025/06/13 09:52:24 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env	*ft_lstnew(char *key, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_strdup(key);
	if (value)
		new->value = ft_strdup(value);
	else
		new->value = NULL;
	new->next = NULL;
	return (new);
}

void	init_default_env1(t_env **env)
{
	t_env	*new;
	char	*pwd;

	if (!get_my_env("PWD", *env))
	{
		pwd = getcwd(NULL, 0);
		if (pwd)
		{
			new = ft_lstnew("PWD=", pwd);
			ft_lstadd_back(env, new);
			free(pwd);
		}
	}
	if (!get_my_env("SHLVL", *env))
	{
		new = ft_lstnew("SHLVL=", "1");
		ft_lstadd_back(env, new);
	}
}

t_env	*init_default_env(t_env **env)
{
	t_env	*new;

	init_default_env1(env);
	if (!get_my_env("_", *env))
	{
		new = ft_lstnew("_=", "/usr/bin/env");
		ft_lstadd_back(env, new);
	}
	if (!get_my_env("PATH", *env))
	{
		new = ft_lstnew("PATH=", \
		"/vscode:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
		ft_lstadd_back(env, new);
	}
	if (!get_my_env("OLLPWD", *env))
	{
		new = ft_lstnew("OLDPWD", NULL);
		ft_lstadd_back(env, new);
	}
	return (*env);
}
