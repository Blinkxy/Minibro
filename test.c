#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int inQuote(char c) 
{
    return (c == '\'' || c == '\"');
}

int countTokens(char* str, char c) 
{
    int len = strlen(str);
    int count = 1;
    int inQuotes = 0;
    int i = 0;

    while (i < len) 
    {
        if (inQuote(str[i]))
            inQuotes = !inQuotes;
        else if (str[i] == c && !inQuotes)
            count++;
        i++;
    }
    return count;
}

void copyToken(char* src, char* dest, int len) 
{
    if (len <= 0)
        return;
    ft_strlcpy(dest, src, len + 1);
}

char** splitString(char* str, char c)
{
    int len = strlen(str);
    int tokenCount = countTokens(str, c);
    int resultSize = 0;
    int i = 0;
    int start = 0;
    int end = 0;
    int inQuotes;
    char** result = (char**)malloc(tokenCount * sizeof(char*));

    if (!result)
        return(NULL);
    while (i < len && resultSize < tokenCount) 
    {
        start = i;
        inQuotes = 0;
        while (i < len) 
        {
            if (isQuote(str[i]))
                inQuotes = !inQuotes;
            else if (str[i] == c && !inQuotes)
                break;
            i++;
        }

        int end = i, tokenLen = end - start;
        result[resultSize] = (char*)malloc((tokenLen + 1) * sizeof(char));
        if (!result[resultSize])
            return(NULL);
        copyToken(&str[start], result[resultSize], tokenLen);
        resultSize++;
        if (i == len)
            break;
        i++; // Skip the separator
    }

    result[resultSize] = NULL;
    return result;
}

void freeTokens(char** tokens) {
    for (int i = 0; tokens[i] != NULL; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

int main() {
    char* input = "apple,banana,'cherry,grape',orange,'pear'";
    char** tokens = splitString(input, ',');

    int i = 0;
    while (tokens[i] != NULL) {
        printf("Token %d: %s\n", i, tokens[i]);
        i++;
    }

    freeTokens(tokens);
    return 0;
}
