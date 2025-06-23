/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rediractions_helper2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdelhamid <abdelhamid@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:30:56 by aelbouz           #+#    #+#             */
/*   Updated: 2025/06/23 18:12:15 by abdelhamid       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int	setup_io(t_execution_info *info)
{
	info->stdin_save = dup(STDIN_FILENO);
	info->stdout_save = dup(STDOUT_FILENO);
	if (info->stdin_save == -1 || info->stdout_save == -1)
		return (perror("minishell: dup"), 1);
	return (0);
}

static int	handle_redirects(t_command *cmd, int *file)
{
	if (cmd->redirects && (cmd->redirects->type == TOKEN_REDIR_OUT \
		|| cmd->redirects->type == TOKEN_REDIR_APPEND) \
		&& dup2(*file, STDOUT_FILENO) == -1)
		return (perror("minishell: dup2"), 1);
	if (cmd->redirects && (cmd->redirects->type == TOKEN_REDIR_IN) \
	&& dup2(*file, STDIN_FILENO) == -1)
		return (perror("minishell: dup2"), 1);
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
	if (cmd->file != -1)
	{
		if (handle_redirects(cmd, &cmd->file) != 0)
			return (1);
	}
	env_path = get_my_env("PATH", *env);
	if (env_path == NULL)
		env_path = NULL;
	info.status = is_builtin(cmd->args[0], cmd->args, env_path, env);
	dup2(info.stdout_save, STDOUT_FILENO);
	dup2(info.stdin_save, STDIN_FILENO);
	close_fds(cmd, info.stdout_save, info.stdout_save);
	return (info.status);
}

static int first_proc(t_command *cmd, t_env **env, t_execution_info *info)
{
    char *env_path;
    int child;

    if (pipe(cmd->fd) == -1)
        return (perror("minishell: pipe"), -1);
    child = fork();
    if (child == -1)
        return (perror("minishell: fork"), close(cmd->fd[0]), close(cmd->fd[1]), -1);
    if (child == 0)
    {
        dup2(cmd->fd[0], STDIN_FILENO);
		dup2(cmd->fd[1], STDOUT_FILENO);
        if (handle_redir(cmd) != 0)
			exit(1);
        env_path = get_my_env("PATH", *env);
        if (!env_path)
            exit(1);
        info->status = is_builtin(cmd->args[0], cmd->args, env_path, env);
        exit(info->status);
    }
    close(cmd->fd[1]);
    return (cmd->fd[0]);
}

static int mid_proc(int fd_save, t_command *cmd, t_env **env, t_execution_info *info)
{
    int child;
	char *env_path;

    if (pipe(cmd->fd) == -1)
        return (perror("minishell: pipe"), -1);
    child = fork();
    if (child == -1)
        return (perror("minishell: fork"), close(cmd->fd[0]), close(cmd->fd[1]), -1);
    if (child == 0)
    {
        close(cmd->fd[0]);
        dup2(fd_save, STDIN_FILENO);
        close(fd_save);
        dup2(cmd->fd[1], STDOUT_FILENO);
        close(cmd->fd[1]);
        if (handle_redir(cmd) != 0)
            exit(1);
        env_path = get_my_env("PATH", *env);
        if (!env_path)
            exit(1);
        info->status = is_builtin(cmd->args[0], cmd->args, env_path, env);
        exit(info->status);
    }
    close(cmd->fd[1]);
    close(fd_save);
    return (cmd->fd[0]);
}

static int last_proc(int fd_save, t_command *cmd, t_env **env, t_execution_info *info)
{
    int child;
	char *env_path;

    child = fork();
    if (child == -1)
        return (perror("minishell: fork"), close(fd_save), 1);
    if (child == 0)
    {
		handle_signals();
        dup2(fd_save, STDIN_FILENO);
        close(fd_save);
        if (handle_redir(cmd) != 0)
            exit(1);
        env_path = get_my_env("PATH", *env);
        if (!env_path)
            exit(1);
        info->status = is_builtin(cmd->args[0], cmd->args, env_path, env);
        exit(info->status);
    }
	waitpid(child, &info->status, 0);
    close(fd_save); 
	return (WEXITSTATUS(info->status));
}

int execute_multiple_commands(t_command **cmds, t_env **env, int cmd_count, t_execution_info *info)
{
    int fd_save;
    int i;

    if (cmd_count == 0)
        return (0);
    fd_save = first_proc(cmds[0], env, info);
    if (fd_save == -1)
        return (1);
    if (cmd_count >= 3)
    {
        i = 0;
        while (i < cmd_count - 2)
        {
            fd_save = mid_proc(fd_save, cmds[i + 1], env, info);
            if (fd_save == -1)
            {
                while (wait(NULL) != -1);
                return (1);
            }
            i++;
        }
    }
    info->status = last_proc(fd_save, cmds[cmd_count - 1], env, info);
    while (wait(NULL) != -1);
    return (info->status);
}
