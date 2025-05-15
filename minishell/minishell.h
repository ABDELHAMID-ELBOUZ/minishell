/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 13:21:20 by aelbouz           #+#    #+#             */
/*   Updated: 2025/05/15 11:47:49 by aelbouz          ###   ########.fr       */
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

typedef enum e_redir_type
{
    REDIR_OUT,
    REDIR_IN,
    REDIR_APPEND,
    REDIR_HEREDOC,
    REDIR_PIPE
} t_redir_type;

typedef struct s_redir {
	int				fd[2];
	char			*outfile;
	char			*infile;
	t_redir_type	redir_type;
}	t_redir;

typedef struct s_command {
	char	**args;
	t_redir	*redir_info;
}	t_command;

int			ft_lstsize(t_env *lst);
char		*ft_strchr(const char *s, int c);
int			ft_strcmp(const char *s1, const char *s2);
char		**ft_split(char const *s, char c);
char		*ft_substr(char const *s, unsigned int start, size_t len);
size_t		ft_strlen(const char *s);
char		*ft_strdup(const char *s1);
int			ft_isdigit(char c);
int			ft_atoi(const char *str);
int			is_builtin(char *cmd, char **args, char *env_path, t_env **env);
int			ft_echo(char **av);
int			ft_pwd(void);
int			ft_exit(char **args);
char		*find_executable(char *cmd, char *path_env);
char		*ft_strjoin(char const *s1, char const *s2);
size_t		ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t		ft_strlcat(char *dst, const char *src, size_t dstsize);
void		free_arr(char **arr);
void		ft_putstr_fd(char *s, int fd);
int			is_not_builtin(char **args, char *env_path);
char		**env_to_array(t_env *env);
void		init_env(char **envp);
void		free_env(t_env *env);
t_env		*get_env(t_env *new_env);
int			ft_cd(char **args, t_env **env);
int			ft_isalnum(char c);
int			is_alphabet(char c);
int			ft_export(char **args, t_env **env);
void		updat_env(t_env **env, char *key, char *value);
void		ft_lstadd_back(t_env **lst, t_env *new);
char		*ft_strtrim(char const *s1, char const *set);
int			find_and_update(t_env **env, char *key, char *value);
t_env		*env_sorted(t_env *env);
int			handl_export_args(char *arg, t_env **env);
t_env		*copie_env(t_env	*head);
int			is_valide_args(char *key);
int			ft_unset(char **args, t_env **env);
int			count_args(char **arr);
t_command	*parse_command(char **args);
int 		execute_command(t_command *cmd, t_env **env);
void		free_cmd(t_command *cmd);
int			parse_rediraction(char **args, int i, t_redir *redire_info);

#endif