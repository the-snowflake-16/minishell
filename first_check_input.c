/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_check_input.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:24:49 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/03 10:27:09 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	contain_queter(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\'')
			return (1);
		else if (s[i] == '"')
			return (1);
		i++;
	}
	return (0);
}

int	incorect_quote(char *s)
{
	int		i;
	int		single_open;
	int		double_open;

	i = 0;
	single_open = 0;
	double_open = 0;
	while (s[i])
	{
		if (s[i] == '\'' && double_open == 0)
			single_open = !single_open;
		else if (s[i] == '"' && single_open == 0)
			double_open = !double_open;
		i++;
	}
	if (single_open || double_open)
	{
		printf("Incorrect input\n");
		return (1);
	}
	return (0);
}

int	incorect_input(char *s)
{
	if (contain_queter(s))
		return (incorect_quote(s));
	return (0);
}
