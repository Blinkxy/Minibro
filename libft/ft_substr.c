/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 15:01:48 by mzoheir           #+#    #+#             */
/*   Updated: 2023/02/24 15:05:45 by mzoheir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char *s, int start, int len)
{
	int		i;
	char	*str;
	int		j;

	j = 0;
	if (start >= ft_strlen(s) || !s)
		return (0);
	if (start + len <= ft_strlen(s))
		str = malloc(len + 1);
	else
		str = malloc(ft_strlen(s) - start + 1);
	if (!str)
		return (0);
	i = start;
	while (i < (start + len) && s[i])
		str[j++] = s[i++];
	str[j] = '\0';
	return (str);
}
