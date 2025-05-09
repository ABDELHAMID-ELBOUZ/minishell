/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_numeric.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 13:41:57 by aelbouz           #+#    #+#             */
/*   Updated: 2025/04/30 10:08:42 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isdigit(char c)
{
	return (c >= '0' && c <= '9');
}

int	is_alphabet(char c)
{
	return ((c >= 65 && c <= 90) || (c >= 97 && c <= 122));
}

int	ft_isalnum(char c)
{
	return (is_alphabet(c) || ft_isdigit(c));
}

int	is_white_space(char c)
{
	return (c >= 9 || c <= 13);
}
