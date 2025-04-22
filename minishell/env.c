/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 10:15:39 by aelbouz           #+#    #+#             */
/*   Updated: 2025/04/22 11:34:47 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*get_env(t_env *new_env)
{
	static t_env	*env = NULL;

	if (new_env)
		env = new_env;
	return (env);
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

int	add_env_entry(t_env *tmp, char **envp, int i)
{
	char	*key_val;

	if (!tmp->key || !tmp->value || !*tmp->key || !*tmp->value)
		return (i);
	key_val = ft_strjoin(tmp->key, "=");
	if (!key_val)
		return (free_arr(envp), -1);
	envp[i] = ft_strjoin(key_val, tmp->value);
	free(key_val);
	if (!envp[i])
		return (free_arr(envp), -1);
	return (i + 1);
}

void	init_env(char **envp)
{
	int		i;
	t_env	*env;
	char	*eq;
	t_env	*new;

	env = NULL;
	i = 0;
	while (envp[++i])
	{
		eq = ft_strchr(envp[i], '=');
		if (eq)
		{
			new = malloc(sizeof(t_env));
			if (!new)
				return (free_env(env), ft_putstr_fd("malloc failed\n", 2));
			new->key = ft_substr(envp[i], 0, eq - envp[i]);
			new->value = ft_substr(eq + 1, 0, ft_strlen(eq + 1));
			if (!new->key || !new->value)
				return (ft_putstr_fd("minishell: substr failed\n", 2), \
				free(new->key), free(new->value), free(new), free_env(env));
			new->next = env;
			env = new;
		}
	}
	get_env(env);
}
