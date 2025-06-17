/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dummy.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 02:03:04 by houabell          #+#    #+#             */
/*   Updated: 2025/06/17 11:41:25 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void free_redirects(t_redirect *redir)
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
		return;
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

void	count_commands(t_shell *shell, int *cmd_count)
{
	t_command	*cmd;

	cmd = shell->commands;
	*cmd_count = 0;
	if (!shell || !shell->commands)
		return ;
	while (cmd)
	{
		(*cmd_count)++;
		cmd = cmd->next;
	}
}

t_command	**setup_and_execute(t_shell *shell, \
int cmd_count, t_execution_info *info)
{
	t_command	*cmd;
	t_command	**cmds;
	int			i;

	cmd = shell->commands;
	i = 0;
	cmds = malloc(sizeof(t_command *) * (cmd_count + 1));
	if (!cmds)
		return (NULL);
	while (i < cmd_count)
	{
		(cmds)[i] = cmd;
		cmd = cmd->next;
		i++;
	}
	(cmds)[cmd_count] = NULL;
	info->stdout_save = -1;
	info->stdin_save = -1;
	if (cmd_count == 1)
		shell->exit_status = execute_single_command(cmds[0], &shell->env);
	else
		shell->exit_status = \
		execute_multiple_commands(cmds, &shell->env, cmd_count, info);
	return (cmds);
}

void	free_cmds(t_shell *shell, t_command **cmds)
{
	t_command	*cmd;
	t_command	*temp;

	cmd = shell->commands;
	while (cmd)
	{
		temp = cmd;
		cmd = cmd->next;
		free_cmd(temp);
	}
	shell->commands = NULL;
	free(cmds);
}

void	execute_commands(t_shell *shell)
{
	int					cmd_count;
	t_execution_info	info;
	t_command			**cmds;
	
	cmds = NULL;
	count_commands(shell, &cmd_count);
	cmds = setup_and_execute(shell, cmd_count, &info);
	free_cmds(shell, cmds);
}
