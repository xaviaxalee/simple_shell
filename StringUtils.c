#include "shell.h"

/**
 **copy_string - copies a string
 *@destination: the destination string to be copied to
 *@source: the source string
 *@num_chars: the number of characters to be copied
 *Return: the concatenated string
 */
char *copy_string(char *destination, char *source, int num_chars)
{
    int i, j;
    char *result = destination;

    i = 0;
    while (source[i] != '\0' && i < num_chars - 1)
    {
        destination[i] = source[i];
        i++;
    }
    if (i < num_chars)
    {
        j = i;
        while (j < num_chars)
        {
            destination[j] = '\0';
            j++;
        }
    }
    return (result);
}

/**
 **concatenate_strings - concatenates two strings
 *@destination: the first string
 *@source: the second string
 *@num_bytes: the number of bytes to be maximally used
 *Return: the concatenated string
 */
char *concatenate_strings(char *destination, char *source, int num_bytes)
{
    int i, j;
    char *result = destination;

    i = 0;
    j = 0;
    while (destination[i] != '\0')
        i++;
    while (source[j] != '\0' && j < num_bytes)
    {
        destination[i] = source[j];
        i++;
        j++;
    }
    if (j < num_bytes)
        destination[i] = '\0';
    return (result);
}

/**
 **find_character - locates a character in a string
 *@str: the string to be parsed
 *@ch: the character to look for
 *Return: (str) a pointer to the memory area str
 */
char *find_character(char *str, char ch)
{
    do
    {
        if (*str == ch)
            return (str);
    } while (*str++ != '\0');

    return (NULL);
}

