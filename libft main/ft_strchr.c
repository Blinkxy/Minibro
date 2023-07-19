/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 15:15:06 by mzoheir           #+#    #+#             */
/*   Updated: 2022/10/05 20:05:48 by mzoheir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	*q;
	char	b;
	int		i;
	int		len;

	q = (char *)s;
	b = (char)c;
	i = 0;
	len = ft_strlen(s);
	while (i <= len)
	{
		if (q[i] == b)
			return (&q[i]);
		i++;
	}
	return (0);
}

/*int main()
{
	char	*str;

    str = "bo\0njour 1337";
    printf("%s", strchr(str,'n'));
    return(0);
}
*/
