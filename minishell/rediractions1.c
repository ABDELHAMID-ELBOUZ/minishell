/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rediractions1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdelhamid <abdelhamid@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 11:36:15 by aelbouz           #+#    #+#             */
/*   Updated: 2025/05/21 17:11:13 by abdelhamid       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redir	*new_redir_info(void)
{
	t_redir	*redir_info;

	redir_info = malloc(sizeof(t_redir));
	if (!redir_info)
		return (NULL);
	redir_info->fd[0] = -1;
	redir_info->fd[1] = -1;
	redir_info->outfile = NULL;
	redir_info->infile = NULL;
	redir_info->redir_type = -1;
	return (redir_info);
}

t_command	*init_command(char **args)
{
	t_command	*cmd;
	int			i;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->redir_info = new_redir_info();
	if (!cmd->redir_info)
		return (free(cmd), NULL);
	i = 0;
	if (args)
	{

		while (args[i])
			i++;
	}
	cmd->args = malloc(sizeof(char *) * (i + 1));
	if (!cmd->args)
		return (free(cmd->redir_info), free(cmd), NULL);
	return (cmd);
}

int	parse_rediraction(char **args, int i, t_redir *redire_info)
{
	if (!args[i] || !args[i + 1])
		return (-1);
	if (ft_strcmp(args[i], ">") == 0)
	{
		redire_info->outfile = ft_strdup(args[i + 1]);
		redire_info->redir_type = REDIR_OUT;
	}
	else if (ft_strcmp(args[i], ">>") == 0)
	{
		redire_info->outfile = ft_strdup(args[i + 1]);
		redire_info->redir_type = REDIR_APPEND;
	}
	else if (ft_strcmp(args[i], "<") == 0)
	{
		redire_info->infile = ft_strdup(args[i + 1]);
		redire_info->redir_type = REDIR_IN;
	}
	else if (ft_strcmp(args[i], "<<") == 0)
	{
		redire_info->delimiter = ft_strdup(args[i + 1]);
		redire_info->redir_type = REDIR_HEREDOC;
	}
	else
		return (0);
	return (i + 2);
}

void	free_cmd(t_command *cmd)
{
	int	i;

	if (!cmd)
		return ;
	i = 0;
	while (cmd->args && cmd->args[i])
	{
		free(cmd->args[i]);
		i++;
	}
	free(cmd->args);
	if (cmd->redir_info)
	{
		free(cmd->redir_info->infile);
		free(cmd->redir_info->outfile);
		free(cmd->redir_info);
	}
	free (cmd);
}

t_command **parse_command(char **args, int *cmd_count)
{
	t_command **cmds;
	int count;

	count = 0;
	if (count_cmd(args, &count) != 0)
		return (NULL);
	*cmd_count = count;
	cmds = allocat_cmds(*cmd_count, args);
	if (!cmds)
		return (NULL);
	parse_full_cmd(args, cmds, *cmd_count);
	return (cmds);
}