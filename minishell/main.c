/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 11:05:44 by aelbouz           #+#    #+#             */
/*   Updated: 2025/05/18 13:35:16 by aelbouz          ###   ########.fr       */
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

int main(int ac, char **av, char **envp)
{
    char *input;
    char **args;
    t_env *env;
    t_command **cmds;  // Changed from t_command *cmd to t_command **cmds
    int cmd_count;     // Added to store the number of commands
    int last_status;

    init_env(envp);
    env = get_env(NULL);
    last_status = 0;
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
        free(input);
        if (!args || !args[0])
        {
            free(args);
            continue ;
        }
        cmds = parse_command(args, &cmd_count);  // Updated with &cmd_count
        if (cmds)
        {
            execute_command(cmds, &env, cmd_count);  // Updated to use cmds and cmd_count
            // free_cmds(cmds, cmd_count);             // Updated to use cmds and cmd_count
        }
        free_arr(args);
    }
    free_env(env);
    rl_clear_history();
    return (last_status);
}
