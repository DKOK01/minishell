/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 14:24:51 by aysadeq           #+#    #+#             */
/*   Updated: 2024/11/14 15:20:58 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	num_length(long n)
{
	int	length;

	length = 0;
	if (n <= 0)
		length++;
	while (n != 0)
	{
		n = n / 10;
		length++;
	}
	return (length);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		length;
	long	num;

	num = n;
	length = num_length(num);
	str = (char *)malloc(sizeof(char) * (length + 1));
	if (str != NULL)
	{
		str[length] = '\0';
		if (num == 0)
			str[0] = '0';
		if (num < 0)
		{
			str[0] = '-';
			num = -num;
		}
		while (num > 0)
		{
			str[--length] = (num % 10) + '0';
			num = num / 10;
		}
	}
	return (str);
}
