#include "../Minishell.h"

int ft_size(char **str)
{
    int i;

    i = 0;
    while(str[i])
        i++;
    return(i);
}

int ft_strcmp(char *s1, char *s2)
{
    int i;

    i = 0;
    while(s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return(s1[i] - s2[i]);
}

void free_tab(char **tab)
{
    int i;

    i = 0;
    while(tab[i])
        free(tab[i++]);
    free(tab);
}