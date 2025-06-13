/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dummy.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 02:03:04 by houabell          #+#    #+#             */
/*   Updated: 2025/06/13 10:44:54 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// File: src/shell/shell_commands_dummy.c
// (You can place these in an existing file in src/shell or a new one)

/**
 * @brief Dummy function for creating commands.
 *
 * This function is a placeholder and does not actually parse tokens
 * into commands. It's here to allow the rest of the shell logic
 * (like tokenization and expansion) to be tested without a full parser.
 *
 * @param shell The shell structure, which would normally contain tokens
 *              to be parsed and would be populated with commands.
 */

/**
 * @brief Dummy function for executing commands.
 *
 * This function is a placeholder and does not actually execute any commands.
 * It's here to complete the shell loop structure for testing other phases.
 *
 * @param shell The shell structure, which would normally contain commands
 *              to be executed.
 */

void	free_cmd(t_command *cmd)
{
	t_redirect	*redir;
	t_redirect	*temp;
	int			i;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i++])
			free(cmd->args[i]);
		free(cmd->args);
	}
	if (cmd->redirects)
	{
		redir = cmd->redirects;
		while (redir)
		{
			temp = redir;
			redir = redir->next;
			free(temp->file);
			free(temp);
		}
	}
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

void	setup_and_execute(t_shell *shell, t_command ***cmds, \
int cmd_count, t_execution_info *info)
{
	t_command	*cmd;
	int			i;

	cmd = shell->commands;
	i = 0;
	*cmds = malloc(sizeof(t_command *) * (cmd_count + 1));
	if (!*cmds)
		return ;
	while (i < cmd_count)
	{
		(*cmds)[i] = cmd;
		cmd = cmd->next;
		i++;
	}
	(*cmds)[cmd_count] = NULL;
	info->stdout_save = -1;
	info->stdin_save = -1;
	if (cmd_count == 1)
		shell->exit_status = execute_single_command((*cmds)[0], &shell->env);
	else
		shell->exit_status = \
		execute_multiple_commands(*cmds, &shell->env, cmd_count, info);
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

	count_commands(shell, &cmd_count);
	setup_and_execute(shell, &cmds, cmd_count, &info);
	free_cmds(shell, cmds);
}
