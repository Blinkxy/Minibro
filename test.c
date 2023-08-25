#include "Minishell.h"


void ft_putfd(char *str, int fd)
{
    int i = 0;

    while(str[i])
    {
        write(fd, &str[i], 1);
        i++;
    }
}
int open_fdheredoc(void)
{
    int fd;
    char file[] = "/tmp/.herdoc";
    if(access(file, F_OK) != -1)
        unlink(file);
    fd = open(file, O_CREAT | O_RDWR | O_TRUNC, 0644);
    if(fd == -1)
    {
        ft_putendl_fd("open error\n", 2);
        return(-1);
    }
    return(fd);
}

int main(void)
{
    int fd = open_fdheredoc();
    char str[] = "work\n";
    ft_putfd(str, fd);
}