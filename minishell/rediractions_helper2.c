/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rediractions_helper2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdelhamid <abdelhamid@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:30:56 by aelbouz           #+#    #+#             */
/*   Updated: 2025/06/09 11:52:33 by abdelhamid       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_io(t_execution_info *info)
{
	info->stdin_save = dup(STDIN_FILENO);
	info->stdout_save = dup(STDOUT_FILENO);
	if (info->stdin_save == -1 || info->stdout_save == -1)
		return (perror("minishell: dup"), 1);
	return (0);
}

int	execute_single_command(t_command *cmd, t_env **env)
{
	t_execution_info	info;
	char				*env_path;

	info.status = -1;
	info.stdout_save = dup(STDOUT_FILENO);
	info.stdin_save = dup(STDIN_FILENO);
	if (handle_redir(cmd) != 0)
		return (1);
	if (cmd->file != -1 && dup2(cmd->file, STDOUT_FILENO) == -1)
		return (perror("minishell: dup2"), 1);
	else if (cmd->file != -1 && dup2(cmd->file, STDIN_FILENO) == -1)
		return (perror("minishell: dup2"), 1);
	env_path = get_my_env("PATH", *env);
	if (env_path == NULL)
		env_path = NULL;
	info.status = is_builtin(cmd->args[0], cmd->args, env_path, env);
	dup2(info.stdout_save, STDOUT_FILENO);
	dup2(info.stdin_save, STDIN_FILENO);
	if (cmd->redirects && cmd->redirects->type == TOKEN_HEREDOC)
		unlink("/tmp/herdoc");
	close_fds(cmd, info.stdout_save, info.stdout_save);
	return (info.status);
}

int	execute_multiple_commands(t_command **cmds, t_env **env, int cmd_count, \
	t_execution_info *info)
{
	char	*env_path;

	env_path = get_my_env("PATH", *env);
	if (env_path == NULL)
		env_path = NULL;
	info->i = 0;
	info->cmd_count = cmd_count;
	info->status = -1;
	info->env = env;
	while (info->i < info->cmd_count)
	{
		if (execute_with_setup(cmds, cmds[info->i], info, env_path) != 0)
			return (1);
		if (info->i > 0)
			close(cmds[info->i - 1]->redirects->fd[0]);
		if (info->i < info->cmd_count - 1)
			close(cmds[info->i]->redirects->fd[1]);
		(info->i)++;
	}
	cleanup_execution(cmds, cmd_count, info);
	return (0);
}

void	free_cmds(t_command **cmds)
{
	int	i;

	i = 0;
	while (cmds[i])
	{
		free_cmd(cmds[i]);
		i++;
	}
	free(cmds);
}
