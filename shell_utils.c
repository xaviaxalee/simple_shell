#include "shell.h"

/**
 * print_string_to_stderr - prints an input string to standard error (stderr)
 * @str: the string to be printed
 *
 * Return: Nothing
 */
void print_string_to_stderr(char *message)
{
	int i = 0;

	if (!message)
		return;
	while (message[i] != '\0')
	{
		write_char_to_stderr(message[i]);
		i++;
	}
}

/**
 * write_char_to_stderr - writes the character c to standard error (stderr)
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int write_char_to_stderr(char ch)
{
	static int index;
	static char buffer[WRITE_BUF_SIZE];

	if (ch == BUF_FLUSH || index >= WRITE_BUF_SIZE)
	{
		write(2, buffer, index);
		index = 0;
	}
	if (ch != BUF_FLUSH)
		buffer[index++] = ch;
	return (1);
}

/**
 * write_char_to_fd - writes the character c to given file descriptor (fd)
 * @ch: The character to print
 * @fd: The file descriptor to write to
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int write_char_to_fd(char ch, int fd)
{
	static int index;
	static char buffer[WRITE_BUF_SIZE];

	if (ch == BUF_FLUSH || index >= WRITE_BUF_SIZE)
	{
		write(fd, buffer, index);
		index = 0;
	}
	if (ch != BUF_FLUSH)
		buffer[index++] = ch;
	return (1);
}

/**
 * print_string_to_fd - prints an input string to given file descriptor (fd)
 * @str: the string to be printed
 * @fd: the file descriptor to write to
 *
 * Return: the number of chars put
 */
int print_string_to_fd(char *message, int fd)
{
	int i = 0;

	if (!message)
		return (0);
	while (*message)
	{
		i += write_char_to_fd(*message++, fd);
	}
	return (i);
}

