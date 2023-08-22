/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 15:25:14 by mzoheir           #+#    #+#             */
/*   Updated: 2023/02/24 15:02:35 by mzoheir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
char *ft_strjoin(char *s1, char *s2)
{
    if (!s1 || !s2)
        return NULL;

    int len1 = ft_strlen(s1);
    int len2 = ft_strlen(s2);

    // Allocate memory for the concatenated string
    char *str = ft_calloc(len1 + len2 + 1, 1);
    if (!str)
        return NULL;

    // Copy the contents of s1 to str using a while loop
    int i = 0;
    while (s1[i] != '\0') {
        str[i] = s1[i];
        i++;
    }

    // Copy the contents of s2 to str using a while loop
    int j = 0;
    while (s2[j] != '\0') {
        str[i + j] = s2[j];
        j++;
    }

    // Free the original s1 since it's no longer needed
    // free(s1);

    // Return the concatenated string
    return str;
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
