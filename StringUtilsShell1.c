#include "shell.h"

/**
 * copy_string - copies a string from source to destination
 * @dest: the destination buffer
 * @src: the source string
 *
 * Return: pointer to the destination buffer
 */
char *copy_string(char *dest, char *src)
{
    int i = 0;

    if (dest == src || src == NULL)
        return dest;

    while (src[i])
    {
        dest[i] = src[i];
        i++;
    }

    dest[i] = '\0';
    return dest;
}

/**
 * duplicate_string - duplicates a given string
 * @str: the string to duplicate
 *
 * Return: pointer to the duplicated string
 */
char *duplicate_string(const char *str)
{
    int length = 0;
    char *result;

    if (str == NULL)
        return NULL;

    while (*str++)
        length++;

    result = malloc(sizeof(char) * (length + 1));
    if (!result)
        return NULL;

    for (length++; length--;)
        result[length] = *--str;

    return result;
}

/**
 * print_string - prints a given string
 * @str: the string to be printed
 *
 * Return: Nothing
 */
void print_string(char *str)
{
    int i = 0;

    if (!str)
        return;

    while (str[i] != '\0')
    {
        write_character(str[i]);
        i++;
    }
}

/**
 * write_character - writes the character c to stdout
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int write_character(char c)
{
    static int i;
    static char buffer[WRITE_BUFFER_SIZE];

    if (c == BUFFER_FLUSH || i >= WRITE_BUFFER_SIZE)
    {
        write(1, buffer, i);
        i = 0;
    }
    if (c != BUFFER_FLUSH)
        buffer[i++] = c;
    return 1;
}

