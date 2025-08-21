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
 * copy_path - returns a dynamically allocated copy of PATH
 * @envp: environment variables array
 *
 * Return: pointer to copy, or NULL if PATH not found
 */
char *copy_path(char *envp[])
{
	char *path_env = NULL;
	int i;

	for (i = 0; envp[i]; i++)
	{
		if (strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_env = envp[i] + 5;
			break;
		}
	}
	if (!path_env)
		return (NULL);
	return (strdup(path_env));
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
 * resolve - resolves path to a command
 * @cmd: command
 *
 * Return: full path if exists
 * else NULL
 */
char *resolve(char *cmd)
{
	if (cmd)
	{
		char *path, *token, *full = NULL;

		if (cmd[0] == '/')
			return (access(cmd, F_OK) == 0 ? strdup(cmd) : NULL);
		path = copy_path(environ);
		token = strtok(path, ":");
		while (token)
		{
			full = malloc(strlen(token) + strlen(cmd) + 2);
			sprintf(full, "%s/%s", token, cmd);
			if (access(full, F_OK) == 0)
				break;
			free(full);
			full = NULL;
			token = strtok(NULL, ":");
		}
		free(path);
		return (full);
	}
	return (NULL);
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
	char *prompt = "#cisfun$ ", *cmd, **_argv, *res;
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
		res = resolve(_argv[0]);
		if (res)
		{
			pid = fork();
			if (pid == 0)
			{
				if (execve(res, _argv, environ) == -1)
					perror(argv[0]);
				free(res);
				free(_argv);
				free(cmd);
				return (1);
			}
			if (pid > 0)
				waitpid(pid, &status, 0);
		}
		free(res);
		free(_argv);
		free(cmd);
	}
	return (0);
}
