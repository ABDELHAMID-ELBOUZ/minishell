/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 13:27:11 by aelbouz           #+#    #+#             */
/*   Updated: 2025/04/17 11:27:27 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_exit(char **args)
{
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (!args || !args[1])
		exit(0);
	if ((is_numeric(args[1])) == 1)
	{
		ft_putstr_fd("exit: numeric argument required\n", 2);
		exit(2);	
	}
	else if (args[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		exit(1);		
	}
	exit(ft_atoi(args[1]) % 256);
}