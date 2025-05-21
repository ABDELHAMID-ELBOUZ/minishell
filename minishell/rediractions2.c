/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rediractions2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:36:00 by aelbouz           #+#    #+#             */
/*   Updated: 2025/05/18 13:33:50 by aelbouz          ###   ########.fr       */
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

int	execute_command_core(t_command *cmd, t_env **env)
{
	char	*env_path;

	env_path = getenv("PATH");
	return (is_builtin(cmd->args[0], cmd->args, env_path, env));
}

int execute_command(t_command **cmds, t_env **env, int cmd_count)
{
    int i;
    int stdout_save;
    int stdin_save;
    int status;

    i = 0;
    stdin_save = dup(STDIN_FILENO);
    stdout_save = dup(STDOUT_FILENO);
    if (stdin_save == -1 || stdout_save == -1)
        return (perror("minishell: dup"), 1);
    while (i < cmd_count)
    {
        if (i < cmd_count - 1 && pipe(cmds[i]->redir_info->fd) == -1)
            return (perror("minishell: pipe"), 1);
        if (fork() == 0)
        {
            if (i > 0)
                dup2(cmds[i - 1]->redir_info->fd[0], STDIN_FILENO);
            if (i < cmd_count - 1)
                dup2(cmds[i]->redir_info->fd[1], STDOUT_FILENO);
            if (handle_redir(cmds[i]) != 0)
                exit(1);
            if (cmds[i]->outfile != -1 && dup2(cmds[i]->outfile, STDOUT_FILENO) == -1)
                exit(1);
            if (cmds[i]->infile != -1 && dup2(cmds[i]->infile, STDIN_FILENO) == -1)
                exit(1);
            status = execute_command_core(cmds[i], env);
            exit(status);
        }
        if (i > 0)
            close(cmds[i - 1]->redir_info->fd[0]);
        if (i < cmd_count - 1)
            close(cmds[i]->redir_info->fd[1]);
        i++;
    }
    while (wait(NULL) != -1 || errno != ECHILD);
    dup2(stdout_save, STDOUT_FILENO);
    dup2(stdin_save, STDIN_FILENO);
    close(stdout_save);
    close(stdin_save);
    if (cmds[cmd_count - 1]->redir_info->redir_type == REDIR_HEREDOC)
        unlink("/tmp/herdoc");
    return (0);
}
