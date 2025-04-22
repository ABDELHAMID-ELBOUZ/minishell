/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 13:21:20 by aelbouz           #+#    #+#             */
/*   Updated: 2025/04/22 11:29:52 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef struct s_env {
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

int		ft_lstsize(t_env *lst);
char	*ft_strchr(const char *s, int c);
int		ft_strcmp(const char *s1, const char *s2);
char	**ft_split(char const *s, char c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
size_t	ft_strlen(const char *s);
char	*ft_strdup(const char *s1);
int		is_numeric(char *str);
int		ft_atoi(const char *str);
int		is_builtin(char *cmd, char **args, char *env_path, t_env **env);
int		ft_echo(char **av);
int		ft_pwd(void);
int		ft_exit(char **args);
char	*find_executable(char *cmd, char *path_env);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);
void	free_arr(char **arr);
void	ft_putstr_fd(char *s, int fd);
int		execute_command(char **args, char *env_path);
char	**env_to_array(t_env *env);
void	init_env(char **envp);
void	free_env(t_env *env);
t_env	*get_env(t_env *new_env);
int		add_env_entry(t_env *tmp, char **envp, int i);
int		ft_cd(char **args, t_env **env);

#endif