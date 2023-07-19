/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 13:53:25 by mzoheir           #+#    #+#             */
/*   Updated: 2023/02/07 18:59:34 by mzoheir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	ldst;
	size_t	lsrc;

	if (dstsize == 0 && dst == 0)
		return (0);
	ldst = ft_strlen(dst);
	lsrc = ft_strlen(src);
	if (ldst >= dstsize)
		return (lsrc + dstsize);
	i = 0;
	while (src[i] && (ldst + i) < dstsize - 1)
	{
		dst[ldst + i] = src[i];
		i++;
	}
	dst[ldst + i] = '\0';
	return (lsrc + ldst);
}
