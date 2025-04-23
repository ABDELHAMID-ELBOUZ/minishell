#include "minishell.h"

t_env *get_env(t_env *new_env)
{
    static t_env *env = NULL;

    if (new_env)
        env = new_env;
    return (env);
}

void free_env(t_env *env)
{
    t_env *tmp;

    while (env)
    {
        tmp = env;
        env = env->next;
        free(tmp->key);
        free(tmp->value);
        free(tmp);
    }
}

char *join_env_var(t_env *env)
{
    char *env_var;

    if (!env || !env->key || !env->value)
        return (NULL);
    env_var = ft_strjoin(env->key, env->value);
    if (!env_var)
        ft_putstr_fd("minishell: join_env_var: allocation failed\n", 2);
    return (env_var);
}

char **env_list_to_array(t_env *env)
{
    t_env *temp;
    int i;
    char **env_arr;

    if (!env)
        return (NULL);
    i = 0;
    temp = env;
    while (temp)
    {
        i++;
        temp = temp->next;
    }
    env_arr = malloc(sizeof(char *) * (i + 1));
    if (!env_arr)
        return (ft_putstr_fd("minishell: env_list_to_array: malloc failed\n", 2), NULL);
    i = 0;
    while (env)
    {
        env_arr[i] = join_env_var(env);
        if (!env_arr[i])
            return (free_arr(env_arr), NULL);
        env = env->next;
        i++;
    }
    env_arr[i] = NULL;
    return (env_arr);
}

t_env *create_env_node(char *key, char *value)
{
    t_env *node;

    if (!key || !value)
        return (NULL);
    node = malloc(sizeof(t_env));
    if (!node)
        return (ft_putstr_fd("minishell: create_env_node: malloc failed\n", 2), NULL);
    node->key = ft_strdup(key);
    node->value = ft_strdup(value);
    if (!node->key || !node->value)
    {
        free(node->key);
        free(node->value);
        free(node);
        return (NULL);
    }
    node->next = NULL;
    return (node);
}

t_env	*init_env_list(char **envp)
{
	t_env	*head;
	t_env	**curr;
	char	*key;
	char	*value;
	char	*eq;
	int		i;

	head = NULL;
	curr = &head;
	i = -1;
	if (!envp || !envp[0])
		return (NULL);
	while (envp[++i])
	{
		eq = ft_strchr(envp[i], '=');
		if (!eq)
			continue;
		key = ft_substr(envp[i], 0, eq - envp[i] + 1);
		value = ft_substr(eq + 1, 0, ft_strlen(eq + 1));
		if (!key || !value)
			return (free(key), free(value), free_env(head), NULL);
		*curr = create_env_node(key, value);
		free(key);
		free(value);
		if (!*curr)
			return (free_env(head), NULL);
		curr = &(*curr)->next;
	}
	return (head);
}

char *get_env_value(char *key, t_env *env)
{
    char *key_with_eq = ft_strjoin(key, "=");

    if (!key_with_eq)
        return (NULL);
    while (env)
    {
        if (env->key && ft_strcmp(env->key, key_with_eq) == 0)
        {
            free(key_with_eq);
            return (env->value);
        }
        env = env->next;
    }
    free(key_with_eq);
    return (NULL);
}

void update_env(t_env **env, char *key, char *value)
{
    t_env *tmp = *env;

    while (tmp)
    {
        if (tmp->key && ft_strcmp(tmp->key, key) == 0)
        {
            free(tmp->value);
            tmp->value = ft_strdup(value);
            if (!tmp->value)
                ft_putstr_fd("minishell: update_env: strdup failed\n", 2);
            return;
        }
        tmp = tmp->next;
    }
    tmp = malloc(sizeof(t_env));
    if (!tmp)
        return (ft_putstr_fd("minishell: update_env: malloc failed\n", 2));
    tmp->key = ft_strdup(key);
    tmp->value = ft_strdup(value);
    if (!tmp->key || !tmp->value)
    {
        free(tmp->key);
        free(tmp->value);
        free(tmp);
        return;
    }
    tmp->next = *env;
    *env = tmp;
}

char *find_executable(char *cmd, char *env_path)
{
    char **dirs;
    char *full_path;
    char *tmp;
    int i = -1;

    if (!env_path || !cmd || !*env_path)
        return (NULL);
    dirs = ft_split(env_path, ':');
    if (!dirs)
        return (NULL);
    while (dirs[++i])
    {
        tmp = ft_strjoin(dirs[i], "/");
        if (!tmp)
            return (free_arr(dirs), NULL);
        full_path = ft_strjoin(tmp, cmd);
        free(tmp);
        if (!full_path)
            return (free_arr(dirs), NULL);
        if (access(full_path, X_OK) == 0)
            return (free_arr(dirs), full_path);
        free(full_path);
    }
    free_arr(dirs);
    return (NULL);
}

int check_executable(char *cmd, char *env_path, char **full_path)
{
    if (!cmd)
        return (127);
    if (cmd[0] == '/' || !ft_strcmp(cmd, "../") || !ft_strcmp(cmd, "./"))
    {
        if (access(cmd, F_OK) != 0)
            return (127);
        if (access(cmd, X_OK) == 0)
        {
            *full_path = ft_strdup(cmd);
            if (!*full_path)
                return (1);
            return (0);
        }
        return (126);
    }
    *full_path = find_executable(cmd, env_path);
    if (!*full_path)
        return (127);
    return (0);
}

int check_status(char **args, char *env_path, char **full_path)
{
    int status;

    if (!args || !args[0])
        return (1);
    status = check_executable(args[0], env_path, full_path);
    if (status == 127)
    {
        ft_putstr_fd(args[0], 2);
        ft_putstr_fd(": command not found\n", 2);
    }
    else if (status == 126)
        ft_putstr_fd("permission denied\n", 2);
    return (status);
}

int execute_command(char **args, char *env_path)
{
    char *full_path = NULL;
    int status;
    int pid;
    char **envp;

    status = check_status(args, env_path, &full_path);
    if (status != 0)
        return (free(full_path), status);
    envp = env_list_to_array(get_env(NULL));
    if (!envp)
        return (free(full_path), ft_putstr_fd("minishell: env_list_to_array failed\n", 2), 1);
    pid = fork();
    if (pid == -1)
        return (free(full_path), free_arr(envp), perror("minishell: fork"), 1);
    if (pid == 0)
    {
        execve(full_path, args, envp);
        perror("minishell: execve");
        free(full_path);
        free_arr(envp);
        exit(1);
    }
    waitpid(pid, &status, 0);
    free(full_path);
    free_arr(envp);
    return (WEXITSTATUS(status));
}

void free_arr(char **arr)
{
    int j = 0;

    if (!arr)
        return;
    while (arr[j])
        free(arr[j++]);
    free(arr);
}

int ft_echo(char **args)
{
    int no_new_line = 1;
    int i = 1;

    if (!args)
        return (1);
    while (args[i] && ft_strcmp(args[i], "-n") == 0)
    {
        no_new_line = 0;
        i++;
    }
    while (args[i])
    {
        ft_putstr_fd(args[i], 1);
        if (args[i + 1])
            ft_putstr_fd(" ", 1);
        i++;
    }
    if (no_new_line)
        ft_putstr_fd("\n", 1);
    return (0);
}

int ft_cd(char **args, t_env **env)
{
    char *path = args[1];
    char *oldpwd = getcwd(NULL, 0);
    char *newpwd;

    if (!oldpwd)
        return (ft_putstr_fd("minishell: cd: getcwd failed\n", 2), 1);
    if (!path || ft_strcmp(path, "~") == 0)
        path = get_env_value("HOME", *env);
    if (!path)
        return (free(oldpwd), ft_putstr_fd("minishell: cd: HOME not set\n", 2), 1);
    if (chdir(path) == -1)
    {
        ft_putstr_fd("minishell: cd: ", 2);
        ft_putstr_fd(path, 2);
        ft_putstr_fd(": No such file or directory\n", 2);
        free(oldpwd);
        return (1);
    }
    newpwd = getcwd(NULL, 0);
    if (!newpwd)
        return (free(oldpwd), ft_putstr_fd("minishell: cd: getcwd failed\n", 2), 1);
    update_env(env, "OLDPWD=", oldpwd);
    update_env(env, "PWD=", newpwd);
    free(oldpwd);
    free(newpwd);
    return (0);
}

int ft_pwd(void)
{
    char *path = getcwd(NULL, 0);

    if (!path)
    {
        ft_putstr_fd("minishell: pwd: getcwd failed\n", 2);
        return (1);
    }
    ft_putstr_fd(path, 1);
    ft_putstr_fd("\n", 1);
    free(path);
    return (0);
}

int ft_env(t_env *env)
{
    char **envp;
    int i = 0;

    envp = env_list_to_array(env);
    if (!envp)
        return (ft_putstr_fd("minishell: env: failed to allocate envp\n", 2), 1);
    while (envp[i])
    {
        ft_putstr_fd(envp[i], 1);
        ft_putstr_fd("\n", 1);
        i++;
    }
    free_arr(envp);
    return (0);
}

int ft_exit(char **args)
{
    ft_putstr_fd("exit\n", 1);
    if (!args || !args[1])
        exit(0);
    if (!is_numeric(args[1]))
    {
        ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
        exit(2);
    }
    if (args[2])
    {
        ft_putstr_fd("minishell: exit: too many arguments\n", 2);
        return (1);
    }
    exit(ft_atoi(args[1]) % 256);
}

int is_builtin(char *cmd, char **args, char *env_path, t_env **env)
{
    if (!cmd)
        return (0);
    if (ft_strcmp(cmd, "echo") == 0)
        return (ft_echo(args));
    if (ft_strcmp(cmd, "pwd") == 0)
        return (ft_pwd());
    if (ft_strcmp(cmd, "exit") == 0)
        return (ft_exit(args));
    if (ft_strcmp(cmd, "cd") == 0)
        return (ft_cd(args, env));
    if (ft_strcmp(cmd, "env") == 0)
        return (ft_env(*env));
    return (execute_command(args, env_path));
}

int main(int ac, char **av, char **envp)
{
    char *input;
    char **args;
    char *env_path;
    t_env *env;

    (void)ac;
    (void)av;
    env = init_env_list(envp);
    if (!env)
        return (ft_putstr_fd("minishell: init_env_list failed\n", 2), 1);
    get_env(env);
    env_path = getenv("PATH");
    if (!env_path)
        env_path = "";
    while (1)
    {
        input = readline("mini_shell$ ");
        if (!input)
        {
            ft_putstr_fd("exit\n", 1);
            break;
        }
        if (!*input)
        {
            free(input);
            continue;
        }
        add_history(input);
        args = ft_split(input, ' ');
        if (!args)
        {
            ft_putstr_fd("minishell: split failed\n", 2);
            free(input);
            continue;
        }
        if (args[0])
            is_builtin(args[0], args, env_path, &env);
        free_arr(args);
        free(input);
    }
    free_env(env);
    rl_clear_history();
    return (0);
}