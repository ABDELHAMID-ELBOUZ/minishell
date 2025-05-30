/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 10:48:26 by aelbouz           #+#    #+#             */
/*   Updated: 2025/05/30 09:31:20 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valide_unset(char *key)
{
	int	i;

	if (!*key || !key || ft_isdigit(key[0]))
		return (0);
	i = 0;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	unset_var(char *key, t_env **env)
{
	t_env	*current;
	t_env	*previous;
	char	*clean_key;

	current = *env;
	previous = NULL;
	while (current)
	{
		clean_key = ft_strtrim(current->key, "=");
		if (ft_strcmp(clean_key, key) == 0)
		{
			if (previous)
				previous->next = current->next;
			else
				*env = current->next;
			return (free(current->key), free(current->value), \
			free(current), free(clean_key), 0);
		}
		free(clean_key);
		previous = current;
		current = current->next;
	}
	return (0);
}

int	ft_unset(char **args, t_env **env)
{
	int	i;
	int	status;

	if (!args)
		return (1);
	i = 1;
	status = 0;
	while (args[i])
	{
		if (!args[i] || !is_valide_unset(args[i]))
		{
			ft_putstr_fd("minishell: unset: ", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': invalid parameter name\n", 2);
			status = 1;
		}
		else
			(unset_var(args[i], env));
		i++;
	}
	return (status);
}
