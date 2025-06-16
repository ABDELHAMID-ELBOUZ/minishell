/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environement_main.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 21:51:40 by houabell          #+#    #+#             */
/*   Updated: 2025/06/16 16:51:50 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_env	*create_env_node_from_string(char *env_string)
{
	char	*key;
	char	*value;
	t_env	*node;

	key = ft_strdup(env_string);
	if (!key)
		return (NULL);
	value = ft_strchr(key, '=');
	if (value)
	{
		*value = '\0';
		value++;
		node = new_env_node(key, value);
		free(key);
		if (!node)
			return (NULL);
		return (node);
	}
	return (NULL);
}

t_env	*init_env(char **envp)
{
	t_env	*env;
	t_env	*node;
	int		i;

	env = NULL;
	i = 0;
	while (envp[i])
	{
		node = create_env_node_from_string(envp[i]);
		if (!node)
		{
			free_env(env);
			return (NULL);
		}
		add_env_node(&env, node);
		i++;
	}
	return (env);
}
