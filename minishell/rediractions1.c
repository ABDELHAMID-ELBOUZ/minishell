/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rediractions1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 11:36:15 by aelbouz           #+#    #+#             */
/*   Updated: 2025/05/12 10:01:40 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redir	*new_redir_info()
{
	t_redir	*redir_info;
	
	redir_info = malloc(sizeof(t_redir));
	if (!redir_info)
		return (NULL);
	redir_info->fd[0] = -1;
	redir_info->fd[1] = -1;
	redir_info->outfile = NULL;
	redir_info->infile = NULL;
	return	(redir_info);
}

t_command	*init_command(char **args)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->redir_info = new_redir_info();
	if (!cmd->redir_info)
		return (free(cmd), NULL);
	cmd->args = malloc(sizeof(char *) * (count_args(args) + 1));
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
		redire_info->infile = ft_strdup(args[i + 1]);
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

t_command	*parse_command(char **args)
{
	t_command	*cmd;
	int			i;
	int			j;
	int			next_i;

	cmd = init_command(args);
	if (!cmd)
		return (NULL);
	i = 0;
	j = 0;
	while (args[i])
	{
		next_i = parse_rediraction(args, i, cmd->redir_info);
		if (next_i > 0)
			i = next_i;
		else
		{
			cmd->args[j++] = ft_strdup(args[i]);
			if (!cmd->args[j - 1])
				return (free_cmd(cmd), NULL);
			i++;
		}
	}
	cmd->args[j] = NULL;
	return (cmd);
}
