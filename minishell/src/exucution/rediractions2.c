/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rediractions2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:36:00 by aelbouz           #+#    #+#             */
/*   Updated: 2025/06/13 09:53:05 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_out_redir(t_redirect *redir_info)
{
	int	fd;

	if (!redir_info || !redir_info->file)
		return (-1);
	if (redir_info->type == TOKEN_REDIR_OUT)
		fd = open(redir_info->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (redir_info->type == TOKEN_REDIR_APPEND)
		fd = open(redir_info->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		return (-1);
	if (fd == -1)
		return (perror("minishell : open"), -1);
	return (fd);
}

int	handle_in_redir(t_redirect *redir_info)
{
	int	fd;

	if (!redir_info || !redir_info->file)
		return (-1);
	fd = open(redir_info->file, O_RDONLY);
	if (fd == -1)
		return (perror("minishell : open"), -1);
	return (fd);
}

int	handle_redir(t_command *cmd)
{
	cmd->file = -1;
	if (cmd->redirects && cmd->redirects->file)
	{
		cmd->file = handle_out_redir(cmd->redirects);
		if (cmd->file == -1)
			return (1);
	}
	if (cmd->redirects && cmd->redirects->type == TOKEN_REDIR_IN)
	{
		cmd->file = handle_in_redir(cmd->redirects);
		if (cmd->file == -1)
			return (1);
	}
	return (0);
}

// int	execute_commands(t_command **cmds, t_env **env, int cmd_count)
// {
// 	t_execution_info	info;

// 	if (!cmds || cmd_count <= 0)
// 		return (1);
// 	info.stdout_save = -1;
// 	info.stdin_save = -1;
// 	if (cmd_count == 1)
// 		return (execute_single_command(cmds[0], env));
// 	return (execute_multiple_commands(cmds, env, cmd_count, &info));
// }
