/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 22:07:58 by houabell          #+#    #+#             */
/*   Updated: 2025/07/05 10:14:13 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

volatile int	g_signal_status = 0;

int	main(int ac, char **av, char **envp)
{
	t_shell	*shell;
	int		exit_status;

	(void)ac;
	(void)av;
	shell = init_shell(envp);
	if (!shell)
		return (ERROR);
	if (shell->interactive == 0)
	{
		free_shell(shell);
		return (0);
	}
	exit_status = minishell_loop(shell);
	free_shell(shell);
	return (exit_status);
}
