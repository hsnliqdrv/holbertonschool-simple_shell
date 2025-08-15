#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "shell.h"

/**
 * _strlen - find length of string
 * @s: string
 *
 * Return: length
 */
size_t _strlen(char *s)
{
	size_t l = 0;

	if (!s)
		return (0);
	while (s[l])
		l++;
	return (l);
}
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
 * words - splits string into words
 * @s: string
 *
 * Return: array of words (null terminated)
 */
char **words(char *s)
{
	char **array;
	size_t c = 0, i = 0, sp = 1;

	while (s[i])
	{
		if (s[i] == ' ')
			sp = 1;
		else
		{
			if (sp)
				c++;
			sp = 0;
		}
		i++;
	}
	array = malloc(sizeof(char *) * (c + 1));
	if (!array)
		return (NULL);
	array[0] = strtok(s, " ");
	for (i = 1; i <= c; i++)
		array[i] = strtok(NULL, " ");
	return (array);
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
	char *prompt = "#cisfun$ ", *cmd, **_argv;
	int is_interactive = isatty(STDIN_FILENO), status;
	pid_t pid;

	signal(SIGINT, SIG_IGN);
	if (argc < 1)
		return (1);
	while (1)
	{
		if (is_interactive)
			printf("%s", prompt);
		cmd = readline();
		if (!cmd)
		{
			if (is_interactive)
				putchar('\n');
			break;
		}
		_argv = words(cmd);
		if (_argv[0])
		{
			pid = fork();
			if (pid == 0)
			{
				if (execve(_argv[0], _argv, environ) == -1)
					perror(argv[0]);
				free(_argv);
				free(cmd);
				return (1);
			}
			if (pid > 0)
				waitpid(pid, &status, 0);
		}
		free(_argv);
		free(cmd);
	}
	return (0);
}
