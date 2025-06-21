/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdelhamid <abdelhamid@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 22:07:58 by houabell          #+#    #+#             */
/*   Updated: 2025/06/21 14:55:12 by abdelhamid       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

volatile int	g_signal_status;

int	exit_status(int new)
{
	static	int status;

	if (status != new)
		status = new;
	return  (status);
}

int	main(int ac, char **av, char **envp)
{
	t_shell	*shell;
	int		exit_status;

	(void)ac;
	(void)av;
	shell = init_shell(envp);
	if (!shell)
		return (ERROR);
	exit_status = minishell_loop(shell);
	free_shell(shell);
	return (exit_status);
}
