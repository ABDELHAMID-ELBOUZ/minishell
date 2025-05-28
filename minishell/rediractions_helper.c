/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rediractions_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 14:36:32 by abdelhamid        #+#    #+#             */
/*   Updated: 2025/05/28 12:52:31 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_cmd(char **args, int *cmd_count)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], "|") == 0)
			(*cmd_count)++;
		i++;
	}
	*cmd_count = (*cmd_count) + 1;
	return (0);
}

t_command	**allocat_cmds(int cmd_count, char **args)
{
	t_command	**cmds;
	int			i;

	cmds = malloc(sizeof(t_command *) * (cmd_count + 1));
	if (!cmds)
		return (NULL);
	i = 0;
	while (i < cmd_count)
	{
		cmds[i] = init_command(args);
		if (!cmds[i])
			return (NULL);
		i++;
	}
	cmds[i] = NULL;
	return (cmds);
}

void	parse_singl_command(char **args, t_command *cmd, t_parse_info *info)
{
	int	next_i;

	while (args[info->j] && ft_strcmp(args[info->j], "|") != 0)
	{
		next_i = parse_rediraction(args, info->j, cmd->redir_info);
		if (next_i > 0)
			info->j = next_i;
		else
		{
			cmd->args[info->k] = ft_strdup(args[info->j]);
			if (!cmd->args[info->k])
			{
				while (info->k > 0)
				{
					free(cmd->args[info->k]);
					(info->k)--;
				}
				free(cmd->args);
				cmd->args = NULL;
				return ;
			}
			(info->k)++;
			(info->j)++;
		}
	}
}

void	parse_cmd_pipes(char **args, t_command *cmd, t_parse_info *info)
{
	if (args[info->j] && ft_strcmp(args[info->j], "|") == 0)
	{
		args[info->j] = NULL;
		if (cmd->redir_info->redir_type == REDIR_NON)
			cmd->redir_info->redir_type = REDIR_PIPE;
	}
	cmd->args[info->k] = NULL;
	info->start = info->j + 1;
}

void	parse_full_cmd(char **args, t_command **cmds, int cmd_count)
{
	t_parse_info	info;

	info.i = 0;
	info.j = 0;
	info.start = 0;
	info.cmd_count = cmd_count;
	while (info.i < cmd_count)
	{
		info.j = info.start;
		info.k = 0;
		parse_singl_command(args, cmds[info.i], &info);
		if (!cmds[info.i]->args)
		{
			while (info.i > 0)
			{
				free_cmd(cmds[info.i]);
				(info.i)--;
			}
			free(cmds);
			cmds = NULL;
			return (free_arr(args));
		}
		parse_cmd_pipes(args, cmds[info.i], &info);
		info.i++;
	}
}
