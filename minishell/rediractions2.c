/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rediractions2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdelhamid <abdelhamid@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:36:00 by aelbouz           #+#    #+#             */
/*   Updated: 2025/05/24 14:36:43 by abdelhamid       ###   ########.fr       */
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

int execute_command(t_command **cmds, t_env **env, int cmd_count)
{
    t_execution_info info;

    info.i = -1;
    if (cmd_count <= 0)
        return (1);
    info.cmd_count = cmd_count;
    info.status = -1;
    info.stdout_save = -1;
    info.stdin_save = -1;
    info.env = env;
    while (info.i++ < info.cmd_count)
    {
        if (execute_with_setup(cmds, cmds[info.i], &info) != 0)
            return (1);
        if (info.i > 0)
            close(cmds[info.i - 1]->redir_info->fd[0]);
        if (info.i < info.cmd_count - 1)
            close(cmds[info.i]->redir_info->fd[1]);
    }
    cleanup_execution(cmds, cmd_count, &info);
    return (0);
}
