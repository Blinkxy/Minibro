#include "Minishell.h"

int checkQuoteIndex(char* str, char* index) 
{
    int singleQuotes = 0;
    int doubleQuotes = 0;
    int i = 0;

    while (str[i] != '\0')
    {
        if (str[i] == '\'' && (i == 0 || str[i - 1] != '\\') && doubleQuotes % 2 == 0) 
        {
            singleQuotes++;
            if (singleQuotes % 2 == 1 && &str[i] <= index)
                return 1;  // Index is between single quotes
        } 
        else if (str[i] == '"' && (i == 0 || str[i - 1] != '\\') && singleQuotes % 2 == 0)
        {
            doubleQuotes++;
            if (doubleQuotes % 2 == 1 && &str[i] <= index)
                return 2;  // Index is between double quotes
        }
        i++;
    }
    return 0;  // Index is not within quotes
}
