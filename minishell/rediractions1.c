/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rediractions1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdelhamid <abdelhamid@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 11:36:15 by aelbouz           #+#    #+#             */
/*   Updated: 2025/06/09 21:27:14 by abdelhamid       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

t_redirect	*new_redir_info(void)
{
	t_redirect	*redir_info;

	redir_info = malloc(sizeof(t_redirect));
	if (!redir_info)
		return (NULL);
	redir_info->fd[0] = -1;
	redir_info->fd[1] = -1;
	redir_info->file = NULL;
	redir_info->type = TOKEN_WORD;
	return (redir_info);
}

t_command	*init_command(char **args)
{
	t_command	*cmd;
	int			i;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->redirects = new_redir_info();
	if (!cmd->redirects)
		return (free(cmd), NULL);
	i = 0;
	if (args)
	{
		while (args[i])
			i++;
	}
	cmd->args = malloc(sizeof(char *) * (i + 1));
	if (!cmd->args)
		return (free(cmd->redirects), free(cmd), NULL);
	return (cmd);
}

int	parse_rediraction(char **args, int i, t_redirect *redire_info)
{
	if (ft_strcmp(args[i], ">") == 0)
	{
		handle_out_redir(redire_info);
		free(redire_info->file);
		redire_info->file = ft_strdup(args[i + 1]);
		redire_info->type = TOKEN_REDIR_OUT;
	}
	else if (ft_strcmp(args[i], ">>") == 0)
	{
		handle_out_redir(redire_info);
		redire_info->file = ft_strdup(args[i + 1]);
		redire_info->type = TOKEN_REDIR_APPEND;
	}
	else if (ft_strcmp(args[i], "<") == 0)
	{
		redire_info->file = ft_strdup(args[i + 1]);
		redire_info->type = TOKEN_REDIR_IN;
	}
	// else if (ft_strcmp(args[i], "<<") == 0)
	// {
	// 	redire_info->delimiter = ft_strdup(args[i + 1]);
	// 	redire_info->type = TOKEN_HEREDOC;
	// }
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
	while (cmd->args[i])
	{
		free(cmd->args[i]);
		i++;
	}
	free(cmd->args);
	if (cmd->redirects)
	{
		free(cmd->redirects->file);
		// free(cmd->redir_info->delimiter);
		free(cmd->redirects);
	}
	free (cmd);
}

t_command	**parse_command(char **args, int *cmd_count)
{
	t_command	**cmds;
	int			count;

	count = 0;
	count_cmd(args, &count);
	*cmd_count = count;
	cmds = allocat_cmds(*cmd_count, args);
	if (!cmds)
		return (NULL);
	parse_full_cmd(args, cmds, *cmd_count);
	return (cmds);
}
