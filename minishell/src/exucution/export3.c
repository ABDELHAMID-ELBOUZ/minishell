/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdelhamid <abdelhamid@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 09:18:01 by aelbouz           #+#    #+#             */
/*   Updated: 2025/06/16 20:02:55 by abdelhamid       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	creat_new_node(t_env **env, char *key, char *value)
{
	t_env	*tmp;

	tmp = malloc(sizeof(t_env));
	if (!tmp)
		return (free(key));
	tmp->key = ft_strdup(key);
	if (value)
		tmp->value = ft_strdup(value);
	else
		tmp->value = NULL;
	if (value && !tmp->value)
		return (free(tmp->key), free(tmp), free(key));
	tmp->next = NULL;
	ft_lstadd_back(env, tmp);
}

void	handl_plus(t_env **env, char *key, char *value)
{
	char	*tmp_key;
	char	*tmp_value;
	t_env	*tmp;

	tmp_key = ft_strtrim(key, "+=");
	if (!tmp_key)
		return ;
	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, tmp_key) == 0)
		{
			tmp_value = tmp->value;
			tmp->value = ft_strjoin(tmp_value, value);
			return (free(tmp_value), free(tmp_key));
		}
		tmp = tmp->next;
	}
	creat_new_node(env, tmp_key, value);
	free(tmp_key);
}
