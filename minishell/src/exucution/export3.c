/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 09:18:01 by aelbouz           #+#    #+#             */
/*   Updated: 2025/07/03 15:20:46 by aelbouz          ###   ########.fr       */
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
	if (!tmp->key)
		return (free(tmp));
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

	tmp_key = ft_strtrim(key, "+");
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

void	updat_env(t_env **env, char *key, char *value)
{
	t_env	*tmp;

	if (ft_strchr(key, '+'))
		return (handl_plus(env, key, value));
	if (find_and_update(env, key, value))
		return ;
	tmp = malloc(sizeof(t_env));
	if (!tmp)
		return (free(key));
	tmp->key = ft_strdup(key);
	if (value)
		tmp->value = ft_strdup(value);
	else
		tmp->value = NULL;
	if (value && !tmp->value)
		return (free(tmp->key), free(tmp));
	tmp->next = NULL;
	ft_lstadd_back(env, tmp);
}

int	is_parent_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0 && cmd[1] != '\0')
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

void	set_interactive_mode(t_shell *shell)
{
	int	is_stdin_terminal;
	int	is_stdout_terminal;

	is_stdin_terminal = isatty(STDIN_FILENO);
	is_stdout_terminal = isatty(STDOUT_FILENO);
	if (is_stdin_terminal && is_stdout_terminal)
		shell->interactive = 1;
	else
		shell->interactive = 0;
}
