/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 13:27:23 by aelbouz           #+#    #+#             */
/*   Updated: 2025/04/15 13:28:47 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_pwd(void)
{
    char *path;

	path = getcwd(NULL, 0);
    if (!path)
    {
        perror("pwd");
        return (1);
    }
    write(1, path, ft_strlen(path));
    write(1, "\n", 1);
    free(path);
    return (0);
}
