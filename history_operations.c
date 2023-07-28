#include "shell.h"

/**
 * get_history_file_path - gets the history file path
 * @info: parameter struct
 *
 * Return: allocated string containing history file path
 */
char *get_history_file_path(info_t *info)
{
    char *buffer, *home_dir;

    home_dir = _getenv(info, "HOME=");
    if (!home_dir)
        return NULL;

    buffer = malloc(sizeof(char) * (_strlen(home_dir) + _strlen(HISTORY_FILE_NAME) + 2));
    if (!buffer)
    {
        free(home_dir);
        return NULL;
    }

    buffer[0] = '\0';
    _strcpy(buffer, home_dir);
    _strcat(buffer, "/");
    _strcat(buffer, HISTORY_FILE_NAME);

    free(home_dir);
    return buffer;
}

/**
 * write_history_to_file - creates a file, or appends to an existing file
 * @info: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int write_history_to_file(info_t *info)
{
    ssize_t file_descriptor;
    char *file_path = get_history_file_path(info);
    list_t *node = NULL;

    if (!file_path)
        return -1;

    file_descriptor = open(file_path, O_CREAT | O_TRUNC | O_RDWR, 0644);
    free(file_path);

    if (file_descriptor == -1)
        return -1;

    for (node = info->command_history; node; node = node->next)
    {
        _putsfd(node->str, file_descriptor);
        _putfd('\n', file_descriptor);
    }

    _putfd(BUFFER_FLUSH, file_descriptor);
    close(file_descriptor);

    return 1;
}

/**
 * read_history_from_file - reads history from file
 * @info: the parameter struct
 *
 * Return: history count on success, 0 otherwise
 */
int read_history_from_file(info_t *info)
{
    int i, last = 0, line_count = 0;
    ssize_t file_descriptor, read_length, file_size = 0;
    struct stat file_stat;
    char *buffer = NULL, *file_path = get_history_file_path(info);

    if (!file_path)
        return 0;

    file_descriptor = open(file_path, O_RDONLY);
    free(file_path);

    if (file_descriptor == -1)
        return 0;

    if (!fstat(file_descriptor, &file_stat))
        file_size = file_stat.st_size;

    if (file_size < 2)
        return 0;

    buffer = malloc(sizeof(char) * (file_size + 1));
    if (!buffer)
        return 0;

    read_length = read(file_descriptor, buffer, file_size);
    buffer[file_size] = '\0';

    if (read_length <= 0)
        return (free(buffer), 0);

    close(file_descriptor);

    for (i = 0; i < file_size; i++)
    {
        if (buffer[i] == '\n')
        {
            buffer[i] = '\0';
            build_history_list(info, buffer + last, line_count++);
            last = i + 1;
        }
    }

    if (last != i)
        build_history_list(info, buffer + last, line_count++);

    free(buffer);
    info->history_count = line_count;

    while (info->history_count-- >= HISTORY_MAX)
        delete_node_at_index(&(info->command_history), 0);

    renumber_history(info);
    return info->history_count;
}

/**
 * build_history_list - adds an entry to a history linked list
 * @info: Structure containing potential arguments. Used to maintain
 * @buffer: buffer
 * @line_count: the history line count
 *
 * Return: Always 0
 */
int build_history_list(info_t *info, char *buffer, int line_count)
{
    list_t *node = NULL;

    if (info->command_history)
        node = info->command_history;

    add_node_end(&node, buffer, line_count);

    if (!info->command_history)
        info->command_history = node;

    return 0;
}

/**
 * renumber_history - renumbers the history linked list after changes
 * @info: Structure containing potential arguments. Used to maintain
 *
 * Return: the new history count
 */
int renumber_history(info_t *info)
{
    list_t *node = info->command_history;
    int i = 0;

    while (node)
    {
        node->num = i++;
        node = node->next;
    }

    return (info->history_count = i);
}

