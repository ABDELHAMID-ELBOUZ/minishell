/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 10:48:26 by aelbouz           #+#    #+#             */
/*   Updated: 2025/05/16 10:46:10 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset_var(char *key, t_env **env)
{
	t_env	*tmp;
	t_env	*current;
	t_env	*previous;

	if (!key || !is_valide_args(key))
		return (1);
	current = *env;
	previous = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (previous)
				previous->next = current->next;
			else
				*env = current->next;
			tmp = current;
			current = current->next;
			return (free(tmp->key), free(tmp->value), free(tmp), 0);
		}
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
		if (unset_var(args[i], env) == 1)
		{
			ft_putstr_fd("minishell: unset: ", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': invalid parameter name\n", 2);
			status = 1;
		}
		i++;
	}
	return (status);
}
