/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 11:05:44 by aelbouz           #+#    #+#             */
/*   Updated: 2025/05/01 12:36:33 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_arr(char **arr)
{
	int	j;

	if (!arr)
		return ;
	j = 0;
	while (arr[j])
	{
		free(arr[j++]);
	}
	free (arr);
}

int	main(int ac, char **av, char **envp)
{
	char	*input;
	char	**args;
	char	*env_path;
	t_env	*env;

	init_env(envp);
	env = get_env(NULL);
	env_path = getenv("PATH");
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break ;
		if (!*input)
		{
			free(input);
			continue ;
		}
		add_history(input);
		args = ft_split(input, ' ');
		if (args && args[0])
			is_builtin(args[0], args, env_path, &env);
		free_arr(args);
		free(input);
	}
	free_env(env);
	rl_clear_history();
	return (0);
}
