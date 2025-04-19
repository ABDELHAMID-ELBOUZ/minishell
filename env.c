/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 10:15:39 by aelbouz           #+#    #+#             */
/*   Updated: 2025/04/19 13:02:30 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env *get_env(t_env *new_env)
{
	static t_env	*env;
	env = NULL;
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

void init_env(char **envp)
{
    int i = 0;
    t_env *env = NULL;

    while (envp[i])
    {
        char *eq = ft_strchr(envp[i], '=');
        if (eq)
        {
            t_env *new = malloc(sizeof(t_env));
            if (!new)
            {
                ft_putstr_fd("minishell: malloc failed\n", 2);
                free_env(env);
                return;
            }
            new->key = ft_substr(envp[i], 0, eq - envp[i]);
            new->value = ft_substr(eq + 1, 0, ft_strlen(eq + 1));
            if (!new->key || !new->value)
            {
                ft_putstr_fd("minishell: substr failed\n", 2);
                free(new->key);
                free(new->value);
                free(new);
                free_env(env);
                return;
            }
            new->next = env;
            env = new;
        }
        i++;
    }
    get_env(env);
}