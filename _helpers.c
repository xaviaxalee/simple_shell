#include "shell.h"

/**
 * is_shell_interactive - returns true if the shell is in interactive mode
 * @info: pointer to the struct containing shell information
 *
 * Return: 1 if in interactive mode, 0 otherwise
 */
int is_shell_interactive(info_t *shell_info)
{
	return (isatty(STDIN_FILENO) && shell_info->read_fd <= 2);
}

/**
 * is_delimiter - checks if a character is a delimiter
 * @ch: the character to check
 * @delim_str: the delimiter string
 * Return: 1 if true, 0 if false
 */
int is_delimiter(char ch, char *delim_str)
{
	while (*delim_str)
	{
		if (*delim_str++ == ch)
			return (1);
	}
	return (0);
}

/**
 * is_alpha_character - checks for an alphabetic character
 * @ch: The character to check
 * Return: 1 if ch is alphabetic, 0 otherwise
 */
int is_alpha_character(int ch)
{
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
		return (1);
	else
		return (0);
}

/**
 * convert_string_to_integer - converts a string to an integer
 * @str: the string to be converted
 * Return: 0 if no numbers in string, converted number otherwise
 */
int convert_string_to_integer(char *str)
{
	int i, sign = 1, flag = 0, output;
	unsigned int result = 0;

	for (i = 0; str[i] != '\0' && flag != 2; i++)
	{
		if (str[i] == '-')
			sign *= -1;

		if (str[i] >= '0' && str[i] <= '9')
		{
			flag = 1;
			result *= 10;
			result += (str[i] - '0');
		}
		else if (flag == 1)
			flag = 2;
	}

	if (sign == -1)
		output = -result;
	else
		output = result;

	return (output);
}

