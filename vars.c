#include "shell.h"

/**
 * is_chain_delimiter - test if the current char in buffer is a chain delimiter
 * @shell_info: the parameter struct
 * @buffer: the char buffer
 * @position: address of the current position in the buffer
 *
 * Return: 1 if chain delimiter, 0 otherwise
 */
int is_chain_delimiter(shell_info_t *shell_info, char *buffer, size_t *position)
{
    size_t j = *position;

    if (buffer[j] == '|' && buffer[j + 1] == '|')
    {
        buffer[j] = 0;
        j++;
        shell_info->command_buffer_type = CMD_OR;
    }
    else if (buffer[j] == '&' && buffer[j + 1] == '&')
    {
        buffer[j] = 0;
        j++;
        shell_info->command_buffer_type = CMD_AND;
    }
    else if (buffer[j] == ';') /* found end of this command */
    {
        buffer[j] = 0; /* replace semicolon with null */
        shell_info->command_buffer_type = CMD_CHAIN;
    }
    else
        return 0;
    
    *position = j;
    return 1;
}

/**
 * check_chain_status - checks if we should continue chaining based on the last status
 * @shell_info: the parameter struct
 * @buffer: the char buffer
 * @position: address of the current position in the buffer
 * @start_pos: starting position in buffer
 * @length: length of buffer
 *
 * Return: Void
 */
void check_chain_status(shell_info_t *shell_info, char *buffer, size_t *position, size_t start_pos, size_t length)
{
    size_t j = *position;

    if (shell_info->command_buffer_type == CMD_AND)
    {
        if (shell_info->status)
        {
            buffer[start_pos] = 0;
            j = length;
        }
    }
    if (shell_info->command_buffer_type == CMD_OR)
    {
        if (!shell_info->status)
        {
            buffer[start_pos] = 0;
            j = length;
        }
    }

    *position = j;
}

/**
 * replace_alias - replaces an alias in the tokenized string
 * @shell_info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_alias(shell_info_t *shell_info)
{
    int i;
    list_t *node;
    char *p;

    for (i = 0; i < 10; i++)
    {
        node = node_starts_with(shell_info->alias_list, shell_info->arguments[0], '=');
        if (!node)
            return 0;
        free(shell_info->arguments[0]);
        p = _strchr(node->str, '=');
        if (!p)
            return 0;
        p = _strdup(p + 1);
        if (!p)
            return 0;
        shell_info->arguments[0] = p;
    }
    return 1;
}

/**
 * replace_vars - replaces variables in the tokenized string
 * @shell_info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_vars(shell_info_t *shell_info)
{
    int i = 0;
    list_t *node;

    for (i = 0; shell_info->arguments[i]; i++)
    {
        if (shell_info->arguments[i][0] != '$' || !shell_info->arguments[i][1])
            continue;

        if (!_strcmp(shell_info->arguments[i], "$?"))
        {
            replace_string(&(shell_info->arguments[i]),
                    _strdup(convert_number(shell_info->status, 10, 0)));
            continue;
        }
        if (!_strcmp(shell_info->arguments[i], "$$"))
        {
            replace_string(&(shell_info->arguments[i]),
                    _strdup(convert_number(getpid(), 10, 0)));
            continue;
        }
        node = node_starts_with(shell_info->env_list, &shell_info->arguments[i][1], '=');
        if (node)
        {
            replace_string(&(shell_info->arguments[i]),
                    _strdup(_strchr(node->str, '=') + 1));
            continue;
        }
        replace_string(&shell_info->arguments[i], _strdup(""));
    }
    return 0;
}

/**
 * replace_string - replaces a string
 * @old_str: address of the old string
 * @new_str: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_string(char **old_str, char *new_str)
{
    free(*old_str);
    *old_str = new_str;
    return 1;
}

