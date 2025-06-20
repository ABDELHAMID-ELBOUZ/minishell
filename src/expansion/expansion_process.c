/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_process.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houabell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 20:48:52 by houabell          #+#    #+#             */
/*   Updated: 2025/06/05 16:46:42 by houabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	process_regular_text(char *str, t_expansion_state *state)
{
	int		start;
	char	*text_segment;

	start = state->i;
	skip_regular_text(str, state);
	if (state->i > start)
	{
		text_segment = ft_substr(str, start, state->i - start);
		if (text_segment)
		{
			append_segment(&state->buffer, text_segment);
			free(text_segment);
		}
	}
}

char	*extract_variable_name(char *str, t_expansion_state *state, \
		t_var_info **var_list)
{
	char	*var_name;

	var_name = ft_substr(str, state->i, (*var_list)->length);
	if (var_name)
		state->i += (*var_list)->length;
	return (var_name);
}

void	skip_regular_text(char *str, t_expansion_state *state)
{
	while (str[state->i] && !(str[state->i] == '$' \
			&& is_allowed(str[state->i + 1])))
		state->i++;
}

void	process_variable_expansion(char *str, t_expansion_state *state, \
		t_var_info **var_list, t_shell *shell)
{
	char	*var_name;
	char	*expanded_value;

	state->i++;
	var_name = extract_variable_name(str, state, var_list);
	if (!var_name)
		return (handle_extraction_failure(state, var_list));
	if ((*var_list)->quote_context == QUOTE_SINGLE)
	{
		expanded_value = NULL;
		apply_expansion_by_context(state, expanded_value, var_list);
	}
	else
	{
		expanded_value = get_env_value(var_name, shell);
		if (!expanded_value)
			return (handle_expansion_failure(state, var_name));
		apply_expansion_by_context(state, expanded_value, var_list);
	}
	cleanup_expansion_step(var_name, expanded_value, var_list);
}
