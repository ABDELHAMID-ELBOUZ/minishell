/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_helper3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:57:01 by aelbouz           #+#    #+#             */
/*   Updated: 2025/07/06 15:00:46 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_fork_failure(t_pipeline_data *data)
{
	perror("minishell: fork");
	if (*data->in_fd != STDIN_FILENO)
		close(*data->in_fd);
	if (*data->i < data->cmd_count - 1)
	{
		close(data->pipe_fd[0]);
		close(data->pipe_fd[1]);
	}
	return ;
}
