/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rediractions2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:36:00 by aelbouz           #+#    #+#             */
/*   Updated: 2025/05/15 11:48:05 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int handle_out_redir(t_redir *redir_info)
{
	int	fd;

	if (!redir_info || !redir_info->outfile)
		return (-1);
	if (redir_info->redir_type == REDIR_OUT)
		fd = open(redir_info->outfile,  O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (redir_info->redir_type == REDIR_APPEND)
		fd = open(redir_info->outfile,  O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		return (-1);
	if (fd == -1)
		return (perror("minishell : open"), -1);
	return (fd);
}

int handle_in_redir(t_redir *redir_info)
{
	int	fd;

	if (!redir_info || !redir_info->infile)
		return (-1);
	fd = open(redir_info->infile, O_RDONLY);
	if (fd == -1)
		return (perror("minishell : open"), -1);
	return (fd);
}

int handle_redir(t_command *cmd, int *out_fd, int *in_fd)
{
	*out_fd = -1;
	*in_fd = -1;
    if (cmd->redir_info && cmd->redir_info->outfile)
    {
        *out_fd = handle_out_redir(cmd->redir_info);
		if (*out_fd == -1)
			return (1);
    }
    if (cmd->redir_info && cmd->redir_info->infile)
    {
    	*in_fd = handle_in_redir(cmd->redir_info);
        if (*in_fd == -10)
			return (1);
    }
    return (0);
}

int	execute_command_core(t_command *cmd, t_env **env)
{
	char	*env_path;

	env_path = getenv("PATH");
	return (is_builtin(cmd->args[0], cmd->args, env_path, env));
}

int	execute_command(t_command *cmd, t_env **env)
{
	int	out_fd;
	int	in_fd;
	int	stdout_save;
	int	stdin_save ;
	int status;

	out_fd = -1;
	in_fd = -1;
	stdin_save = dup(STDIN_FILENO);
	stdout_save = dup(STDOUT_FILENO);
	if (stdout_save == -1 || stdin_save == -1)
		return (1);
	if (handle_redir(cmd, &out_fd, &in_fd) != 0)
		return (1);
	if (out_fd != -1 && dup2(out_fd, STDOUT_FILENO) == -1)
		return (perror("minishell: dup2"), 1);
	if (in_fd != -1 && dup2(in_fd, STDIN_FILENO) == -1)
		return (perror("minishell: dup2"), 1);
	status = execute_command_core(cmd, env);
	dup2(stdout_save, STDOUT_FILENO);
	dup2(stdin_save, STDIN_FILENO);
	if (status != 0)
		return (1);
	return (0);
}
