#include "shell.h"

/**
 * get_environ - returns the string array copy of our environ
 * @environment_data: Structure containing potential arguments. Used to maintain
 *                    constant function prototype.
 * Return: Always 0
 */
char **get_environ(info_t *environment_data)
{
    if (!environment_data->environ || environment_data->env_changed)
    {
        environment_data->environ = list_to_strings(environment_data->env);
        environment_data->env_changed = 0;
    }

    return (environment_data->environ);
}

/**
 * _unsetenv - Remove an environment variable
 * @environment_data: Structure containing potential arguments. Used to maintain
 *                    constant function prototype.
 * Return: 1 on delete, 0 otherwise
 * @var: the string env var property
 */
int _unsetenv(info_t *environment_data, char *var)
{
    list_t *node = environment_data->env;
    size_t i = 0;
    char *p;

    if (!node || !var)
        return (0);

    while (node)
    {
        p = starts_with(node->str, var);
        if (p && *p == '=')
        {
            environment_data->env_changed = delete_node_at_index(&(environment_data->env), i);
            i = 0;
            node = environment_data->env;
            continue;
        }
        node = node->next;
        i++;
    }
    return (environment_data->env_changed);
}

/**
 * _setenv - Initialize a new environment variable,
 *           or modify an existing one
 * @environment_data: Structure containing potential arguments. Used to maintain
 *                    constant function prototype.
 * @var: the string env var property
 * @value: the string env var value
 * Return: Always 0
 */
int _setenv(info_t *environment_data, char *var, char *value)
{
    char *buf = NULL;
    list_t *node;
    char *p;

    if (!var || !value)
        return (0);

    buf = malloc(_strlen(var) + _strlen(value) + 2);
    if (!buf)
        return (1);
    _strcpy(buf, var);
    _strcat(buf, "=");
    _strcat(buf, value);
    node = environment_data->env;
    while (node)
    {
        p = starts_with(node->str, var);
        if (p && *p == '=')
        {
            free(node->str);
            node->str = buf;
            environment_data->env_changed = 1;
            return (0);
        }
        node = node->next;
    }
    add_node_end(&(environment_data->env), buf, 0);
    free(buf);
    environment_data->env_changed = 1;
    return (0);
}

