#include "shell.h"

/**
 * _stringToInt - converts a string to an integer
 * @str: the string to be converted
 * Return: 0 if no numbers in string, converted number otherwise
 *       -1 on error
 */
int _stringToInt(char *str)
{
	int i = 0;
	unsigned long int result = 0;

	if (*str == '+')
		str++; /* TODO: why does this make main return 255? */

	for (i = 0;  str[i] != '\0'; i++)
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			result *= 10;
			result += (str[i] - '0');
			if (result > INT_MAX)
				return (-1);
		}
		else
			return (-1);
	}
	return (result);
}

/**
 * printErrorMessage - prints an error message
 * @info: the parameter & return info struct
 * @errorString: string containing specified error type
 */
void printErrorMessage(info_t *info, char *errorString)
{
	_print(info->fname);
	_print(": ");
	printInteger(info->line_count, STDERR_FILENO);
	_print(": ");
	_print(info->argv[0]);
	_print(": ");
	_print(errorString);
}

/**
 * printInteger - function prints a decimal (integer) number (base 10)
 * @input: the input
 * @fd: the file descriptor to write to
 *
 * Return: number of characters printed
 */
int printInteger(int input, int fd)
{
	int (*__putchar)(char) = _putchar;
	int i, count = 0;
	unsigned int absoluteValue, current;

	if (fd == STDERR_FILENO)
		__putchar = _eputchar;

	if (input < 0)
	{
		absoluteValue = -input;
		__putchar('-');
		count++;
	}
	else
		absoluteValue = input;

	current = absoluteValue;
	for (i = 1000000000; i > 1; i /= 10)
	{
		if (absoluteValue / i)
		{
			__putchar('0' + current / i);
			count++;
		}
		current %= i;
	}
	__putchar('0' + current);
	count++;

	return (count);
}

/**
 * convertToBase - converter function, a clone of itoa
 * @num: number
 * @base: base
 * @flags: argument flags
 *
 * Return: string
 */
char *convertToBase(long int num, int base, int flags)
{
	static char *characterArray;
	static char buffer[50];
	char sign = 0;
	char *ptr;
	unsigned long n = num;

	if (!(flags & CONVERT_UNSIGNED) && num < 0)
	{
		n = -num;
		sign = '-';
	}

	characterArray = flags & CONVERT_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
	ptr = &buffer[49];
	*ptr = '\0';

	do	{
		*--ptr = characterArray[n % base];
		n /= base;
	} while (n != 0);

	if (sign)
		*--ptr = sign;
	return (ptr);
}

/**
 * removeComment - function replaces the first instance of '#' with '\0'
 * @str: address of the string to modify
 */
void removeComment(char *str)
{
	int i;

	for (i = 0; str[i] != '\0'; i++)
		if (str[i] == '#' && (!i || str[i - 1] == ' '))
		{
			str[i] = '\0';
			break;
		}
}

