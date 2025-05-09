/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rediractions2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:36:00 by aelbouz           #+#    #+#             */
/*   Updated: 2025/05/09 11:49:55 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int handle_out_redir(t_redir *redir_info)
{
	int	fd;
	int	flags;

	if (!redir_info || !redir_info->outfile)
		return (-1);
	if (redir_info->redir_type == REDIR_OUT)
		flags = O_CREAT | O_WRONLY | O_TRUNC;
	else if (redir_info->redir_type == REDIR_APPEND)
		flags = O_CREAT | O_WRONLY | O_APPEND;
	else
		return (-1);
	fd = open(redir_info->outfile, flags, 0644);
	if (fd == -1)
		return (perror("minishell : open"), -1);
	redir_info->fd[0] = dup(STDOUT_FILENO);
	if (redir_info->fd[0] == -1)
		return (close(fd), -1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (close(fd), -1);
	close(fd);
	return (redir_info->fd[0]);
}

int handle_in_redir(t_redir *redir_info)
{
	int	fd;

	if (!redir_info || !redir_info->infile)
		return (-1);
	fd = open(redir_info->infile, O_RDONLY);
	if (fd == -1)
		return (perror("minishell : open"), -1);
	redir_info->fd[1] = dup(STDIN_FILENO);
	if (redir_info->fd[1] == -1)
		return (close(fd), -1);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (close(fd), -1);
	close(fd);
	return (redir_info->fd[1]);
}

int	handle_redir(t_command *cmd, int *out_fd, int *in_fd)
{
	if (cmd->redir_info && cmd->redir_info->outfile)
		*out_fd = handle_out_redir(cmd->redir_info);
	if (*out_fd == -1 && cmd->redir_info->outfile)
		return (1);
	if (cmd->redir_info && cmd->redir_info->infile)
		*in_fd = handle_in_redir(cmd->redir_info);
	if (*in_fd == -1 && cmd->redir_info->infile)
		return (1);
	return (0);
}

int	execute_command_core(t_command *cmd, t_env **env, int out_fd, int in_fd)
{
	int		status;
	char	*env_path;

	env_path = getenv("PATH");
	status = is_builtin(cmd->args[0], cmd->args, env_path, env);
	if (in_fd != -1)
	{
		close(STDIN_FILENO);
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (out_fd != -1)
	{
		close(STDOUT_FILENO);
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	return (status);
}

int	execute_command(t_command *cmd, t_env **env)
{
	int	out_fd;
	int	in_fd;
	int	redir_status;

	in_fd = -1;
	out_fd = -1;
	redir_status = handle_redir(cmd, &out_fd, &in_fd);
	if (redir_status != 0)
		return (redir_status);
	return (execute_command_core(cmd, env, out_fd, in_fd));
}
