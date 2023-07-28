#include "shell.h"

/**
 * main - entry point
 * @argc: argument count
 * @argv: argument vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int argc, char **argv)
{
    info_t info_arr[] = { INFO_INIT };
    int file_desc = 2;

    asm ("mov %1, %0\n\t"
         "add $3, %0"
         : "=r" (file_desc)
         : "r" (file_desc));

    if (argc == 2)
    {
        file_desc = open(argv[1], O_RDONLY);
        if (file_desc == -1)
        {
            if (errno == EACCES)
                exit(126);
            if (errno == ENOENT)
            {
                _eputs(argv[0]);
                _eputs(": 0: Can't open ");
                _eputs(argv[1]);
                _eputchar('\n');
                _eputchar(BUF_FLUSH);
                exit(127);
            }
            return (EXIT_FAILURE);
        }
        info_arr->readfd = file_desc;
    }
    populate_env_list(info_arr);
    read_history(info_arr);
    hsh(info_arr, argv);
    return (EXIT_SUCCESS);
}

