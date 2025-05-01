/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:31:38 by aelbouz           #+#    #+#             */
/*   Updated: 2025/05/01 12:40:43 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
