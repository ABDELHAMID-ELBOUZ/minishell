/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 10:15:39 by aelbouz           #+#    #+#             */
/*   Updated: 2025/06/13 09:52:20 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env	*get_env(t_env *new_env)
{
	static t_env	*env;

	if (new_env)
		env = new_env;
	return (env);
}

t_env	*new_node(char *envp, t_env *env)
{
	char	*tmp;
	t_env	*new;
	char	*eq;

	eq = ft_strchr(envp, '=');
	if (eq)
	{
		new = malloc(sizeof(t_env));
		if (!new)
			return (free_env(env), NULL);
		tmp = ft_substr(envp, 0, (ft_strlen(envp) - ft_strlen(eq)));
		if (!tmp)
			return (NULL);
		new->key = ft_strjoin(tmp, "=");
		free(tmp);
		new->value = ft_substr(eq + 1, 0, ft_strlen(eq + 1));
		if (!new->key || !new->value)
			return (free(new->key), free(new->value), \
			free(new), free_env(env), NULL);
		new->next = env;
	}
	return (new);
}

void	init_env1(char **envp)
{
	int		i;
	t_env	*env;

	env = NULL;
	i = 0;
	while (envp[i])
	{
		if (envp[i])
			env = new_node(envp[i], env);
		if (!env)
			return (free_env(env));
		i++;
	}
	get_env(env);
}
