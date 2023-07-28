#include "shell.h"

/**
 * count_list_nodes - determines the number of nodes in the linked list
 * @head: pointer to the first node
 *
 * Return: number of nodes in the list
 */
size_t count_list_nodes(const list_t *head)
{
    size_t count = 0;

    while (head)
    {
        head = head->next;
        count++;
    }
    return count;
}

/**
 * list_to_strings - returns an array of strings from the list->str
 * @head: pointer to the first node
 *
 * Return: array of strings
 */
char **list_to_strings(list_t *head)
{
    list_t *node = head;
    size_t node_count = count_list_nodes(head);
    size_t i;
    char **strings_array;
    char *str;

    if (!head || !node_count)
        return NULL;

    strings_array = malloc(sizeof(char *) * (node_count + 1));
    if (!strings_array)
        return NULL;

    for (i = 0; node; node = node->next, i++)
    {
        str = malloc(_strlen(node->str) + 1);
        if (!str)
        {
            for (size_t j = 0; j < i; j++)
                free(strings_array[j]);
            free(strings_array);
            return NULL;
        }

        str = _strcpy(str, node->str);
        strings_array[i] = str;
    }
    strings_array[i] = NULL;
    return strings_array;
}

/**
 * print_list - prints all elements of a list_t linked list
 * @head: pointer to the first node
 *
 * Return: size of list
 */
size_t print_list(const list_t *head)
{
    size_t count = 0;

    while (head)
    {
        _puts(convert_number(head->num, 10, 0));
        _putchar(':');
        _putchar(' ');
        _puts(head->str ? head->str : "(nil)");
        _puts("\n");
        head = head->next;
        count++;
    }
    return count;
}

/**
 * find_node_starts_with - returns the node whose string starts with prefix
 * @head: pointer to the list head
 * @prefix: string to match
 * @next_char: the next character after prefix to match
 *
 * Return: matching node or NULL
 */
list_t *find_node_starts_with(list_t *head, char *prefix, char next_char)
{
    char *p = NULL;

    while (head)
    {
        p = starts_with(head->str, prefix);
        if (p && ((next_char == -1) || (*p == next_char)))
            return head;
        head = head->next;
    }
    return NULL;
}

/**
 * get_node_index - gets the index of a node
 * @head: pointer to list head
 * @target_node: pointer to the node
 *
 * Return: index of node or -1
 */
ssize_t get_node_index(list_t *head, list_t *target_node)
{
    size_t index = 0;

    while (head)
    {
        if (head == target_node)
            return index;
        head = head->next;
        index++;
    }
    return -1;
}

