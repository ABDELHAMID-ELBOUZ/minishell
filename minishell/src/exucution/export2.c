/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:31:38 by aelbouz           #+#    #+#             */
/*   Updated: 2025/06/13 09:52:32 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	update_existing_key(t_env *tmp, char *key, char *value, char *clean_key)
{
	if (!value && (!tmp->value || !tmp->value[0]))
		return (free(clean_key), 1);
	if (tmp->value && !value)
		return (free(clean_key), 1);
	free(tmp->value);
	free(tmp->key);
	tmp->key = ft_strdup(key);
	if (!tmp->key)
		return (free(clean_key), 0);
	if (value)
		tmp->value = ft_strdup(value);
	else
		tmp->value = NULL;
	free(clean_key);
	return (1);
}

int	find_and_update(t_env **env, char *key, char *value)
{
	t_env	*tmp;
	char	*tmp_key;
	char	*clean_key;

	tmp = *env;
	clean_key = ft_strtrim(key, "=");
	if (!clean_key)
		return (0);
	while (tmp)
	{
		tmp_key = ft_strtrim(tmp->key, "=");
		if (tmp_key && ft_strcmp(tmp_key, clean_key) == 0)
		{
			free(tmp_key);
			return (update_existing_key(tmp, key, value, clean_key));
		}
		free(tmp_key);
		tmp = tmp->next;
	}
	return (free(clean_key), 0);
}

t_env	*new_copie(t_env *env)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_strdup(env->key);
	if (!new->key)
		return (free(new), NULL);
	if (env->value)
		new->value = ft_strdup(env->value);
	else
		new->value = NULL;
	if (env->value && !new->value)
		return (free(new->key), free(new), NULL);
	new->next = NULL;
	return (new);
}

t_env	*copie_env(t_env	*env)
{
	t_env	*copie;
	t_env	*new;
	t_env	*last;

	last = NULL;
	copie = NULL;
	while (env)
	{
		new = new_copie(env);
		if (!copie)
			copie = new;
		else
			last->next = new;
		last = new;
		env = env->next;
	}
	return (copie);
}

t_env	*env_sorted(t_env *env)
{
	t_env	*current;
	char	*tmp_key;
	char	*tmp_value;

	current = env;
	while (current && current->next)
	{
		if (strcmp(current->key, current->next->key) > 0)
		{
			tmp_key = current->key;
			current->key = current->next->key;
			current->next->key = tmp_key;
			tmp_value = current->value;
			current->value = current->next->value;
			current->next->value = tmp_value;
			current = env;
		}
		else
			current = current->next;
	}
	return (env);
}
