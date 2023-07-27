#include "shell.h"
/**
 * _myexit - exits the shell
 * @shell: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: exits with a given exit status
 * (0) if shell->args[0] != "exit"
 */
int _myexit(shell_t *shell)
{
    int exitcheck;

    if (shell->args[1]) /* If there is an exit argument */
    {
        exitcheck = _erratoi(shell->args[1]);
        if (exitcheck == -1)
        {
            shell->status = 2;
            print_error(shell, "Illegal number: ");
            _eputs(shell->args[1]);
            _eputchar('\n');
            return 1;
        }
        shell->err_num = _erratoi(shell->args[1]);
        return -2;
    }
    shell->err_num = -1;
    return -2;
}
/**
 * _mycd - changes the current directory of the process
 * @shell: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0
 */
int _mycd(shell_t *shell)
{
    char *current_dir, *dir, buffer[1024];
    int chdir_ret;

    current_dir = getcwd(buffer, 1024);
    if (!current_dir)
        _puts("TODO: >>getcwd failure emsg here<<\n");

    if (!shell->args[1])
    {
        dir = _getenv(shell, "HOME=");
        if (!dir)
            chdir_ret = /* TODO: what should this be? */
                chdir((dir = _getenv(shell, "PWD=")) ? dir : "/");
        else
            chdir_ret = chdir(dir);
    }
    else if (_strcmp(shell->args[1], "-") == 0)
    {
        if (!_getenv(shell, "OLDPWD="))
        {
            _puts(current_dir);
            _putchar('\n');
            return 1;
        }
        _puts(_getenv(shell, "OLDPWD=")), _putchar('\n');
        chdir_ret = /* TODO: what should this be? */
            chdir((dir = _getenv(shell, "OLDPWD=")) ? dir : "/");
    }
    else
        chdir_ret = chdir(shell->args[1]);
    
    if (chdir_ret == -1)
    {
        print_error(shell, "can't cd to ");
        _eputs(shell->args[1]), _eputchar('\n');
    }
    else
    {
        _setenv(shell, "OLDPWD", _getenv(shell, "PWD="));
        _setenv(shell, "PWD", getcwd(buffer, 1024));
    }
    
    return 0;
}
/**
 * _myhelp - displays help message for the shell
 * @shell: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0
 */
int _myhelp(shell_t *shell)
{
    char **arg_array;

    arg_array = shell->args;
    _puts("help call works. Function not yet implemented \n");
    if (0)
        _puts(*arg_array); /* temp att_unused workaround */
    return 0;
}
int main(int argc, char **argv, char **envp)
{
    shell_t shell;

    shell_init(&shell, argc, argv, envp);

    do {
        print_prompt(&shell);
        read_command(&shell);
        parse_command(&shell);
        execute_command(&shell);
    } while (shell.status == 1);

    shell_cleanup(&shell);
    return shell.exit_code;
}
