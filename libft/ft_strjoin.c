/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 15:25:14 by mzoheir           #+#    #+#             */
/*   Updated: 2023/10/04 04:02:02 by mzoheir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char *s1, char *s2)
{
	int		i;
	int		j;
	int		len1;
	int		len2;
	char	*str;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	str = malloc(len1 + len2 + 1);
	if (!str)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		str[i + j] = s2[j];
		j++;
	}
	str[len1 + len2] = '\0';
	return (str);
}

// char	*ft_strjoin(char *s1, char *s2)
// {
// 	char	*str;
// 	int		i;
// 	int		j;

// 	i = -1;
// 	j = -1;
// 	if (!s1 || !s2)
// 		return (0);
// 	str = ft_calloc(ft_strlen(s1) + ft_strlen(s2) + 2, 1);
// 	if (!str)
// 		return (0);
// 	while (s1[++i] && i < ft_strlen(s1))
// 		str[i] = s1[i];
// 	while (s2[++j] && i < ft_strlen(s2) + ft_strlen(s1) && j < ft_strlen(s2))
// 		str[i + j] = s2[j];
// 	str[i + j] = ' ';
// 	free(s1);
// 	return (str);
// }

// #include<stdio.h>

// int		main(void)
// {
// 	char	s1[] = "lorem ipsum";
// 	char	s2[] = "\0olor sit amet";
// 	char	*strjoin;
// 	strjoin = ft_strjoin(s1, s2);
// 	printf("%s", strjoin);
// 	return (0);
// }

// while(str[i] && s1[i])
// 		str[i] = s1[i++];
// 	while(str[i] && s2[j])
// 		str[i+j] = s2[j++];
// 	str[i + j] = '\0';
