#include "shell.h"

/**
 * is_executable - determines if a file is an executable command
 * @info: the info struct
 * @file_path: path to the file
 *
 * Return: 1 if true, 0 otherwise
 */
int is_executable(info_t *info, char *file_path)
{
    struct stat file_stat;

    (void)info;
    if (!file_path || stat(file_path, &file_stat))
        return 0;

    if (file_stat.st_mode & S_IFREG)
    {
        return 1;
    }
    return 0;
}

/**
 * duplicate_characters - duplicates characters
 * @path_str: the PATH string
 * @start_index: starting index
 * @stop_index: stopping index
 *
 * Return: pointer to new buffer
 */
char *duplicate_characters(char *path_str, int start_index, int stop_index)
{
    static char buffer[1024];
    int i = 0, k = 0;

    for (k = 0, i = start_index; i < stop_index; i++)
        if (path_str[i] != ':')
            buffer[k++] = path_str[i];
    buffer[k] = 0;
    return buffer;
}

/**
 * find_executable_path - finds the executable cmd in the PATH string
 * @info: the info struct
 * @path_str: the PATH string
 * @cmd: the cmd to find
 *
 * Return: full path of cmd if found or NULL
 */
char *find_executable_path(info_t *info, char *path_str, char *cmd)
{
    int i = 0, curr_pos = 0;
    char *path;

    if (!path_str)
        return NULL;
    if ((strlen(cmd) > 2) && starts_with(cmd, "./"))
    {
        if (is_executable(info, cmd))
            return cmd;
    }
    while (1)
    {
        if (!path_str[i] || path_str[i] == ':')
        {
            path = duplicate_characters(path_str, curr_pos, i);
            if (!*path)
                strcat(path, cmd);
            else
            {
                strcat(path, "/");
                strcat(path, cmd);
            }
            if (is_executable(info, path))
                return path;
            if (!path_str[i])
                break;
            curr_pos = i;
        }
        i++;
    }
    return NULL;
}

