#include "shell.h"

/**
 * **split_str_by_delim - splits a string into words. Repeat delimiters are ignored
 * @str: the input string
 * @delim: the delimiter string
 * Return: a pointer to an array of strings, or NULL on failure
 */

char **split_str_by_delim(char *str, char *delim)
{
	int i, j, k, m, num_words = 0;
	char **result;

	if (str == NULL || str[0] == 0)
		return (NULL);
	if (!delim)
		delim = " ";
	for (i = 0; str[i] != '\0'; i++)
		if (!is_delim(str[i], delim) && (is_delim(str[i + 1], delim) || !str[i + 1]))
			num_words++;

	if (num_words == 0)
		return (NULL);
	result = malloc((1 + num_words) * sizeof(char *));
	if (!result)
		return (NULL);
	for (i = 0, j = 0; j < num_words; j++)
	{
		while (is_delim(str[i], delim))
			i++;
		k = 0;
		while (!is_delim(str[i + k], delim) && str[i + k])
			k++;
		result[j] = malloc((k + 1) * sizeof(char));
		if (!result[j])
		{
			for (k = 0; k < j; k++)
				free(result[k]);
			free(result);
			return (NULL);
		}
		for (m = 0; m < k; m++)
			result[j][m] = str[i++];
		result[j][m] = 0;
	}
	result[j] = NULL;
	return (result);
}

/**
 * **split_str_by_delim2 - splits a string into words
 * @str: the input string
 * @delim: the delimiter
 * Return: a pointer to an array of strings, or NULL on failure
 */
char **split_str_by_delim2(char *str, char delim)
{
	int i, j, k, m, num_words = 0;
	char **result;

	if (str == NULL || str[0] == 0)
		return (NULL);
	for (i = 0; str[i] != '\0'; i++)
		if ((str[i] != delim && str[i + 1] == delim) ||
				    (str[i] != delim && !str[i + 1]) || str[i + 1] == delim)
			num_words++;
	if (num_words == 0)
		return (NULL);
	result = malloc((1 + num_words) * sizeof(char *));
	if (!result)
		return (NULL);
	for (i = 0, j = 0; j < num_words; j++)
	{
		while (str[i] == delim && str[i] != delim)
			i++;
		k = 0;
		while (str[i + k] != delim && str[i + k] && str[i + k] != delim)
			k++;
		result[j] = malloc((k + 1) * sizeof(char));
		if (!result[j])
		{
			for (k = 0; k < j; k++)
				free(result[k]);
			free(result);
			return (NULL);
		}
		for (m = 0; m < k; m++)
			result[j][m] = str[i++];
		result[j][m] = 0;
	}
	result[j] = NULL;
	return (result);
}

