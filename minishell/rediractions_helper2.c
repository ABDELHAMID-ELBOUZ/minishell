/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rediractions_helper2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:30:56 by aelbouz           #+#    #+#             */
/*   Updated: 2025/05/26 17:36:01 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_single_command(t_command *cmd, t_env **env)
{
	t_execution_info	info;
	int					status;
	char				*env_path;

	info.status = -1;
	info.stdout_save = -1;
	info.stdin_save = -1;
	if (handle_redir(cmd) != 0)
		return (1);
	if (cmd->outfile != -1 && dup2(cmd->outfile, STDOUT_FILENO) == -1)
		return (perror("minishell: dup2"), 1);
	if (cmd->infile != -1 && dup2(cmd->infile, STDIN_FILENO) == -1)
		return (perror("minishell: dup2"), 1);
	env_path = getenv("PATH");
	if (env_path)
		status = is_builtin(cmd->args[0], cmd->args, env_path, env);
	else
		status = 1;
	info.status = status;
	if (cmd->outfile != -1)
		close(cmd->outfile);
	if (cmd->infile != -1)
		close(cmd->infile);
	return (info.status);
}

int	execute_multiple_commands(t_command **cmds, t_env **env, int cmd_count, \
	t_execution_info *info)
{
	char	*env_path;

	env_path = getenv("PATH");
	info->i = -1;
	info->cmd_count = cmd_count;
	info->status = -1;
	info->env = env;
	while (++(info->i) < info->cmd_count)
	{
		if (execute_with_setup(cmds, cmds[info->i], info, env_path) != 0)
			return (1);
		if (info->i > 0)
			close(cmds[info->i - 1]->redir_info->fd[0]);
		if (info->i < info->cmd_count - 1)
			close(cmds[info->i]->redir_info->fd[1]);
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
