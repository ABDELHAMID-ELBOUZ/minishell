/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rediractions2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:36:00 by aelbouz           #+#    #+#             */
/*   Updated: 2025/05/29 11:26:14 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_out_redir(t_redir *redir_info)
{
	int	fd;

	if (!redir_info || !redir_info->outfile)
		return (-1);
	if (redir_info->redir_type == REDIR_OUT)
		fd = open(redir_info->outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (redir_info->redir_type == REDIR_APPEND)
		fd = open(redir_info->outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		return (-1);
	if (fd == -1)
		return (perror("minishell : open"), -1);
	return (fd);
}

int	handle_in_redir(t_redir *redir_info)
{
	int	fd;

	if (!redir_info || !redir_info->infile)
		return (-1);
	fd = open(redir_info->infile, O_RDONLY);
	if (fd == -1)
		return (perror("minishell : open"), -1);
	return (fd);
}

int	handle_redir(t_command *cmd)
{
	cmd->outfile = -1;
	cmd->infile = -1;
	if (cmd->redir_info && cmd->redir_info->outfile)
	{
		cmd->outfile = handle_out_redir(cmd->redir_info);
		if (cmd->outfile == -1)
			return (1);
	}
	if (cmd->redir_info && cmd->redir_info->redir_type == REDIR_IN)
	{
		cmd->infile = handle_in_redir(cmd->redir_info);
		if (cmd->infile == -1)
			return (1);
	}
	else if (cmd->redir_info && cmd->redir_info->redir_type == REDIR_HEREDOC)
	{
		cmd->infile = handle_herdoc(cmd->redir_info);
		if (cmd->infile == -1)
			return (1);
	}
	return (0);
}

int	execute_commands(t_command **cmds, t_env **env, int cmd_count)
{
	t_execution_info	info;

	if (!cmds || cmd_count <= 0)
		return (1);
	info.stdout_save = -1;
	info.stdin_save = -1;
	if (cmd_count == 1)
		return (execute_single_command(cmds[0], env));
	return (execute_multiple_commands(cmds, env, cmd_count, &info));
}
