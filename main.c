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
 * strip - strips string from spaces
 * @s: string
 */
void strip(char *s)
{
	size_t i = 0, f, l;

	if (!s)
		return;
	i = strlen(s);
	while (i > 0 && s[i - 1] == ' ')
		i--;
	if (s[i] == ' ')
		s[i] = '\0';
	l = i;
	i = 0;
	while (i < l && s[i] == ' ')
		i++;
	f = i;
	for (i = f; i <= l; i++)
		s[i - f] = s[i];
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
	int is_interactive = isatty(STDIN_FILENO), status;

	signal(SIGINT, SIG_IGN);
	if (argc < 1)
		return (1);
	while (1)
	{
		pid_t pid;

		if (is_interactive)
			printf("%s", prompt);
		cmd = readline();
		if (!cmd)
		{
			if (is_interactive)
				putchar('\n');
			break;
		}
		strip(cmd);
		_argv[0] = cmd;
		if (strlen(cmd) > 0)
		{
			pid = fork();
			if (pid == 0)
			{
				if (execve(_argv[0], _argv, environ) == -1)
					perror(argv[0]);
				return (1);
			}
			if (pid > 0)
				waitpid(pid, &status, 0);
		}
		free(cmd);
	}
	return (0);
}
