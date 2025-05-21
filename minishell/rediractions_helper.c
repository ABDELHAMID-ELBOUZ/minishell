/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rediractions_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdelhamid <abdelhamid@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 14:36:32 by abdelhamid        #+#    #+#             */
/*   Updated: 2025/05/21 17:15:15 by abdelhamid       ###   ########.fr       */
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

t_command **allocat_cmds(int cmd_count, char **args)
{
    t_command **cmds;
    int i;

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

void	parse_singl_command(char **args, t_command *cmd, int *j, int *k)
{
	int	next_i;
	while (args[*j] && ft_strcmp(args[*j], "|") != 0)
	{
		next_i = parse_rediraction(args, *j, cmd->redir_info);
		if (next_i > 0)
			*j = next_i;
		else
		{
			cmd->args[*k] = ft_strdup(args[*j]);
			if (!cmd->args[*k])
				return ;
			(*k)++;
			(*j)++;
		}
	}
}

void	parse_cmd_pipes(char **args, t_command *cmd, int i, int *j, int *k, int *start, int cmd_count)
{
    if (args[*j] && ft_strcmp(args[*j], "|") == 0)
    {
        args[*j] = NULL;
        if (i < cmd_count - 1)
            cmd->redir_info->redir_type = REDIR_PIPE;
    }
    cmd->args[*k] = NULL;
    *start = *j + 1;
}

int	parse_full_cmd(char **args, t_command **cmds, int cmd_count)
{
	int	i;
	int	j;
	int	k;
	int start;

	i = 0;
	start = 0;
	while (i < cmd_count)
	{
		j = start;
		k = 0;
		parse_singl_command(args, cmds[i], &j, &k);
		parse_cmd_pipes(args, cmds[i], i, &j, &k, &start, cmd_count);
		i++;
	}
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
