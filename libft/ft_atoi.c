/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thrychka <thrychka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 19:19:18 by thrychka          #+#    #+#             */
/*   Updated: 2024/07/08 14:29:56 by thrychka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	res;
	int	num;

	i = 0;
	res = 1;
	num = 0;
	while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r')
	{
		i++;
	}
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			res *= -1;
		i++;
	}
	while (str[i] >= 48 && str[i] <= 57)
	{
		num = num * 10 + (str[i] - 48);
		i++;
	}
	return (num * res);
}

// int main () {
//    int val;
//    char str[20];
//    strcpy(str, "  +98993489");
//    val = ft_atoi(str);
//    printf("String value = %s, Int value = %d\n", str, val);

//    strcpy(str, "tutorialspoint.com");
//    val = ft_atoi(str);
//    printf("String value = %s, Int value = %d\n", str, val);

//    return(0);
// }