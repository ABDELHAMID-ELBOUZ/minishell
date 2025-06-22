/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rediractions2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdelhamid <abdelhamid@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:36:00 by aelbouz           #+#    #+#             */
/*   Updated: 2025/06/22 15:25:46 by abdelhamid       ###   ########.fr       */
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
	t_redirect	*redir;
	int			in_fd;
	int			out_fd;

	in_fd = -1;
	out_fd = -1;
	cmd->file = -1;
	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type == TOKEN_REDIR_OUT || redir->type == TOKEN_REDIR_APPEND)
		{
			if (out_fd != -1)
				close(out_fd);
			out_fd = handle_out_redir(redir);
			if (out_fd == -1)
				return (1);
		}
		else if (redir->type == TOKEN_REDIR_IN)
		{
			if (in_fd != -1)
				close(in_fd);
			in_fd = handle_in_redir(redir);
			if (in_fd == -1)
				return (1);
		}
		redir = redir->next;
	}
	if (in_fd != -1)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
			return (perror("minishell: dup2"), 1);
		close(in_fd);
	}
	if (out_fd != -1)
	{
		if (dup2(out_fd, STDOUT_FILENO) == -1)
			return (perror("minishell: dup2"), 1);
		close(out_fd);
	}
	return (0);
}

void	free_redirects(t_redirect *redir)
{
	t_redirect	*temp;

	while (redir)
	{
		temp = redir;
		redir = redir->next;
		if (temp->file)
			free(temp->file);
		free(temp);
	}
}

void	free_cmd(t_command *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	if (cmd->redirects)
		free_redirects(cmd->redirects);
	free(cmd);
}
