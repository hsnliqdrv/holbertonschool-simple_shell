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
	char *str = malloc(256);
	size_t i = 0;

	if (!str)
		return (NULL);
	if (!fgets(str, 256, stdin))
	{
		free(str);
		return (NULL);
	}
	while (str[i] && str[i] != '\n')
		i++;
	if (str[i] == '\n')
		str[i] = '\0';
	return (str);
}
/**
 * main - the entry function
 *
 * Return: returns 0 in success
 * any other code indicates an error
 */
int main(int argc, char **argv)
{
	extern char **environ;
	const char *prompt = "#cisfun$ ";
	int print_prompt = isatty(STDIN_FILENO);

	if (argc < 1)
		return (1);
	while (1)
	{
		char *cmd;
		char *_argv[] = {NULL, NULL};
		pid_t pid;

		if (print_prompt)
			printf("%s", prompt);
		cmd = readline();
		if (!cmd)
			break;
		_argv[0] = cmd;
		pid = fork();
		if (pid < 0)
		{
		}
		else if (pid == 0)
		{
		}
		else
		{
		}
		if (execve(_argv[0], _argv, environ) == -1)
			perror(argv[0]);
		printf("\n");
		free(cmd);
	}
	printf("end");
	return (0);
}
