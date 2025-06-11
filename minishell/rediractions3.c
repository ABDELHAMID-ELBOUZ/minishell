/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rediractions3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdelhamid <abdelhamid@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 09:47:56 by aelbouz           #+#    #+#             */
/*   Updated: 2025/06/09 11:53:07 by abdelhamid       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_fds(t_command *cmd, int stdout_save, int stdin_save)
{
	if (cmd->file != -1)
		close(cmd->file);
	if (stdin_save != -1)
		close(stdin_save);
	if (stdout_save != -1)
		close(stdout_save);
}

char	*get_my_env(char *name, t_env *env)
{
	t_env	*tmp;
	char	*key;

	if (!name || !env)
		return (NULL);
	tmp = env;
	while (tmp)
	{
		key = ft_strtrim(tmp->key, "=");
		if (ft_strcmp(key, name) == 0)
			return (free(key), tmp->value);
		tmp = tmp->next;
		free(key);
	}
	return (NULL);
}

// int	handle_herdoc(t_redirect *redir_info)
// {
// 	int		fd;
// 	char	*line;

// 	fd = open("/tmp/herdoc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
// 	if (fd == -1)
// 		return (perror("minishell: open"), -1);
// 	while (1)
// 	{
// 		line = readline(">");
// 		if (!line || ft_strcmp(line, redir_info->delimiter) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		write(fd, line, ft_strlen(line));
// 		write(fd, "\n", 1);
// 		free(line);
// 	}
// 	close(fd);
// 	fd = open("/tmp/herdoc", O_RDONLY);
// 	if (fd == -1)
// 		return (perror("minishell: open"), -1);
// 	return (fd);
// }

int	execute_with_setup(t_command **cmds, t_command *cmd, \
	t_execution_info *info, char *env_path)
{
	if (setup_io(info) != 0)
		return (1);
	if (info->i < info->cmd_count - 1 && pipe(cmd->redirects->fd) == -1)
		return (perror("minishell: pipe"), 1);
	if (fork() == 0)
	{
		if (info->i > 0)
			dup2(cmds[info->i - 1]->redirects->fd[0], STDIN_FILENO);
		if (info->i < info->cmd_count - 1)
			dup2(cmd->redirects->fd[1], STDOUT_FILENO);
		if (handle_redir(cmd) != 0)
			exit(1);
		if (cmd->file != -1 && dup2(cmd->file, STDOUT_FILENO) == -1)
			exit(1);
		if (cmd->file != -1 && dup2(cmd->file, STDIN_FILENO) == -1)
			exit(1);
		if (!env_path)
			exit(1);
		info->status = is_builtin(cmd->args[0], cmd->args, env_path, info->env);
		exit(EXIT_SUCCESS);
	}
	waitpid(-1, &info->status, 0);
	return (0);
}

int	cleanup_execution(t_command **cmds, int cmd_count, t_execution_info *info)
{
	if (info->stdout_save != -1)
		dup2(info->stdout_save, STDOUT_FILENO);
	if (info->stdin_save != -1)
		dup2(info->stdin_save, STDIN_FILENO);
	if (info->stdout_save != -1)
		close(info->stdout_save);
	if (info->stdin_save != -1)
		close(info->stdin_save);
	if (cmd_count > 0 && \
			cmds[cmd_count - 1]->redirects->type == TOKEN_HEREDOC)
		unlink("/tmp/herdoc");
	return (0);
}
