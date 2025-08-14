#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shell.h"

/**
 * readline - reads a line from stdin
 * Return: dynamically allocated string
 */
char *readline()
{
	size_t size = 0;
	char *str = NULL;
	size_t i = 0;

	if (getline(&str, &size, stdin) == -1)
		return (NULL);
	while (str[i] && str[i] != '\n')
		i++;
	if (str[i] == '\n')
		str[i] = '\0';
	return (str);
}
/**
 * main - the entry function
 * @argc: argument count
 * @argv: argument list
 *
 * Return: returns 0 in success
 * any other code indicates an error
 */
int main(int argc, char **argv)
{
	char *prompt = "#cisfun$ ", *cmd, *_argv[] = {NULL, NULL};
	int print_prompt = isatty(STDIN_FILENO), status;

	if (argc < 1)
		return (1);
	while (1)
	{
		pid_t pid;

		if (print_prompt)
			printf("%s", prompt);
		cmd = readline();
		if (!cmd)
			break;
		_argv[0] = cmd;
		pid = fork();
		if (pid == 0)
		{
			if (execve(_argv[0], _argv, environ) == -1)
				perror(argv[0]);
			return (1);
		}
		if (pid > 0)
			waitpid(pid, &status, 0);
		free(cmd);
	}
	return (0);
}
