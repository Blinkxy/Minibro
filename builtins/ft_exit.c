#include "../Minishell.h"

long long ft_ex_atoi(char *str)
{
    long long res;
    int i;
    int sign;

    i = 0;
    sign = 1;
    res = 0;
    while(str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
        i++;
    if(str[i] == '-')
    {
        sign = -1;
        i++;
    }
    if(str[i] == '+')
        i++;
    while(str[i] && (str[i] >= '0' && str[i] <= '9'))
    {
        res = (res * 10) + str[i] - '0';
        i++;
    }
    return(res * sign);
}

int is_greater_than_llong(char *str)
{
    if (str[0] == '-' && strlen(str) >= 20 && strcmp(&str[1], "9223372036854775808") > 0)
        return(1);
    else if (strlen(str) >= 19 && strcmp(str, "9223372036854775807") > 0)
        return(1);
    return (0);
}

int is_digit(char *str, int index)
{
    int i;

    i = index;
    while(str[i] != '\0')
    {
        if(ft_isdigit(str[i]) == 0)
            return(0);
        i++;
    }
    return(1);
}

int check_exit(char *arg)
{
    int i;
    long long exit_digit;

    i = 0;
    exit_digit = 0;
    while (arg[i] == 32 || (arg[i] >= 9 && arg[i] <= 13))
		i++;
    if(arg[i] && (arg[i] == '+' || arg[i] == '-'))
        i++;
    if(is_digit(arg, i)== 0 || is_greater_than_llong(arg) == 1)
        return(1);
    exit_digit = ft_ex_atoi(arg);
    gb_ex_st = exit_digit % 256;
    return(0);
}

int ft_exit(char **cmd)
{
    if(!cmd[1])
        exit(gb_ex_st);
    if(check_exit(cmd[1]) == 1 )
    {
        ft_putstr_fd("minishell: exit: ", 2);
        ft_putstr_fd(cmd[1], 2);
        ft_putstr_fd(": numeric argument required\n", 2);
        exit(255);
    }
    else if(cmd[2])
    {
        ft_putstr_fd("minishell: exit: too many arguments\n", 2);
        return(1);
    }
    exit(gb_ex_st);
}





