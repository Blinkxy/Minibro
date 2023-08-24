#include "Minishell.h"

// int     final_size(t_list *cmds)

void    final_cmd(t_list *cmds)
{
    int i;
    int j;
    t_list *tmp;

    tmp = cmds;
    // printf("SIZE FINAL%d")
    while(tmp)
    {
        tmp->final_cmd = (char **)malloc(sizeof(char*) * (tmp->size_cmd - (tmp->red_nb*2)) + 1);
        int size = (tmp->size_cmd - (tmp->red_nb*2)) + 1;
        printf("size:%d\n", size);
        if(!tmp->final_cmd)
            return;
        i = 0;
        j = 0;
        while(i < tmp->size_cmd)
        {
            if (tmp->define[i].state == WORD)
            {
                tmp->final_cmd[j] = ft_strdup(tmp->define[i].content);
                j++;
            }
            i++;
        }
        // printf("j = %d\n",j);
        // printf("i = %d\n",i);
        tmp->final_cmd[j] = '\0';
        tmp = tmp->next;
    }
}

// int main() {
//     char input[] = "This 'is a test' string with 'multiple words' in single quotes.";
//     int wordCount;
//     char** words = splitWords(input, &wordCount);

//     if (words == NULL) {
//         return 1;  // Error exit code
//     }

//     printf("Total words: %d\n", wordCount);
//     for (int i = 0; i < wordCount; i++) {
//         printf("Word %d: %s\n", i + 1, words[i]);
//         free(words[i]);
//     }

//     free(words);

//     return 0;
// }
