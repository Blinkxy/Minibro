#include "Minishell.h"

static int count_strings(char *s, char c)
{
    int i;
    int count;

    count = 0;
    i = 0;
    while (s[i])
    {
        while (s[i] && s[i] == c)
            i++;
        if (s[i])
            count++;
        while (s[i] && s[i] != c)
            i++;
    }
    return count;
}

static char **allocation(char *s, int i, char c, char **str)
{
    int len;
    int j;

    i = 0;
    j = 0;
    while (s[i] != '\0')
    {
        len = 0;
        while (s[i] && s[i] == c)
        {
            str[j] = ft_strdup("");
            j++;
            i++;
        }
        if (s[i] != '\0')
        {
            while (s[i + len] != '\0' && s[i + len] != c)
                len++;
            str[j] = ft_substr(s, i, len);
            j++;
        }
        while (s[i] && s[i] != c)
        {
            str[j] = ft_strdup("");
            j++;
            i++;
        }
    }
    str[j] = 0;
    return str;
}

char **sosplit(char *s, char c)
{
    int i;
    char **str;
    int str_count;

    i = 0;
    if (!s)
        return 0;
    str_count = count_strings(s, c);
    str = (char **)malloc(sizeof(char *) * (str_count + 1));
    if (!str)
        return 0;
    allocation(s, i, c, str);
    // free(s);
    return str;
}

int main()
{
    char str[20] = "      '  HELLO'";
 
    Expand_quotes(str);
    printf("%s\n", str);
    // int i = 0;
    // while(split[i])
    // {
    //     printf("%s\n", split[i]);
    //     i++;
    // }
    return(0);
}