/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 13:53:13 by mzoheir           #+#    #+#             */
/*   Updated: 2022/10/19 20:14:41 by mzoheir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*t;
	unsigned char	k;

	t = (unsigned char *)s;
	k = (unsigned char)c;
	if (!n)
		return (0);
	while (*t != k && --n)
		t++;
	if (*t == k)
		return (t);
	return (NULL);
}
// int main()
// {
//     char s[] = "hi how are you ";
//     char k = 'a';
//     printf("%s",ft_memchr(s, k, 7));
// }