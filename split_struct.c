#include "Minishell.h"

char** splitWords(char* str, int* wordCount) 
{
    int totalWords = countWords(str);
    char** words = (char**)malloc(totalWords * sizeof(char*));

    if (words == NULL)
        return NULL;

    int inQuotes = 0;
    int wordStart = 0;
    int wordIndex = 0;
    char* word = NULL;
    int i = 0;

    while (str[i]) 
	{
        if (str[i] == '\'') 
		{
            inQuotes = !inQuotes;
            if (wordStart == 0) 
			{
                wordStart = 1;
                word = &str[i];
            }
        } 
		else if (str[i] == ' ' && !inQuotes) 
		{
            if (wordStart == 1) 
			{
                wordStart = 0;
                str[i] = '\0';
                words[wordIndex] = ft_strdup(word);
                if (words[wordIndex] == NULL) 
				{
                    fprintf(stderr, "Memory allocation failed.\n");
                    while (wordIndex > 0) 
					{
                        wordIndex--;
                        free(words[wordIndex]);
                    }
                    free(words);
                    return NULL;
                }
                wordIndex++;
            }
        } else {
            if (wordStart == 0) 
			{
                wordStart = 1;
                word = &str[i];
            }
        }
        i++;
    }
    if (wordStart == 1) 
	{
        words[wordIndex] = ft_strdup(word);
        if (words[wordIndex] == NULL)
		{
            fprintf(stderr, "Memory allocation failed.\n");
            while (wordIndex > 0) 
			{
                wordIndex--;
                free(words[wordIndex]);
            }
            free(words);
            return NULL;
        }
        wordIndex++;
    }
    *wordCount = wordIndex;
    free(str);
    return words;
}

void    final_cmd(t_list *cmds)
{
    int i;
    int j;
    t_list *tmp;
    tmp = cmds;

    
    while(tmp)
    {
        tmp->final_cmd = (char **)malloc(sizeof(char*)  * tmp->size_cmd + 1);
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
        tmp->final_cmd[j] = '\0';
        i = 0;
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
