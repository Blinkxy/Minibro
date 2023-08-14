#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Minishell.h"

int isQuote(char c) {
    return (c == '\'' || c == '\"');
}

char** splitString(char* str) {
    int count = 0, inQuotes = 0, wordStart = 0, i = 0;
    while (str[i]) {
        if (str[i] == '\'')
            inQuotes = !inQuotes;
        else if (str[i] == ' ' && !inQuotes) {
            if (wordStart == 1) {
                count++;
                wordStart = 0;
            }
        } else
            wordStart = 1;
        i++;
    }
    if (wordStart == 1)
        count++;

    char** split = (char**)malloc((count + 1) * sizeof(char*));
    if (!split) {
        return NULL;
    }

    i = 0;
    int splitIndex = 0, start = 0;
    wordStart = 0;

    while (str[i]) {
        if (str[i] == '\'')
            inQuotes = !inQuotes;
        else if (str[i] == ' ' && !inQuotes) {
            if (wordStart == 1) {
                int len = i - start;
                if (inQuotes && str[start] == '\'') {
                    start++;
                    len--;
                }
                split[splitIndex] = (char*)malloc((len + 1) * sizeof(char));
                if (!split[splitIndex]) {
                    return NULL;
                }
                ft_strlcpy(split[splitIndex], &str[start], len + 1);
                wordStart = 0;
                splitIndex++;
            }
        } else {
            if (wordStart == 0) {
                start = i;
                wordStart = 1;
            }
        }
        i++;
    }
    if (wordStart == 1) {
        int len = i - start;
        if (inQuotes && str[start] == '\'') {
            start++;
            len--;
        }
        split[splitIndex] = (char*)malloc((len + 1) * sizeof(char));
        if (!split[splitIndex]) {
            return NULL;
        }
        ft_strlcpy(split[splitIndex], &str[start], len + 1);
        splitIndex++;
    }
    split[splitIndex] = NULL;

    free(str);
    return split;
}

void freeSplit(char** split) {
    if (!split)
        return;

    int i = 0;
    while (split[i] != NULL) {
        free(split[i]);
        i++;
    }
    free(split);
}

int main() {
    char* input = strdup("This is a 'test string' with spaces   and single 'quotes'.");
    char** split = splitString(input);
    
    if (split == NULL) {
        perror("Memory allocation error");
        return 1;
    }

    int i = 0;
    while (split[i] != NULL) {
        printf("Token %d: %s\n", i, split[i]);
        i++;
    }

    freeSplit(split);
    return 0;
}
