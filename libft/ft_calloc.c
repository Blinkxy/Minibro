/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 13:53:05 by mzoheir           #+#    #+#             */
/*   Updated: 2023/08/08 23:22:10 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(int count, int size)
{
	char	*str;
	int	t;

	t = count * size;
	str = (char *)malloc(t);
	if (!str)
		return (NULL);
	ft_bzero(str, t);
	return (str);
}
