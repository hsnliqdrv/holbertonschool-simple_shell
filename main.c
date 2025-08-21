#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <limits.h>
#include "shell.h"
/**
 * copy_path - returns a dynamically allocated copy of PATH
 * Return: pointer to copy, or NULL if PATH not found
 */
char *copy_path()
{
	char *path_env = NULL;
	int i;

	for (i = 0; environ[i]; i++)
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
		{
			path_env = environ[i] + 5;
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
		if (cmd[0] != '/')
		{
			char *path, *token, *full = NULL;

			if (strchr(cmd, '/'))
			{
				path = malloc(PATH_MAX);
				getcwd(path, PATH_MAX);

			}
			else
				path = copy_path();
			token = strtok(path, ":");
			while (token)
			{
				full = malloc(strlen(token) + strlen(cmd) + 2);
				sprintf(full, "%s/%s", token, cmd);
				if (access(full, X_OK) == 0)
					break;
				free(full);
				full = NULL;
				token = strtok(NULL, ":");
			}
			free(path);
			return (full);
		}
		return (access(cmd, X_OK) == 0 ? strdup(cmd) : NULL);
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
	int is_interactive = isatty(STDIN_FILENO), status = 0, line = 1;
	pid_t pid;

	signal(SIGINT, SIG_IGN);

	while (argc)
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
		if (strncmp(_argv[0], "exit", 4) == 0)
		{
			free(cmd);
			free(_argv);
			exit(status);
		}
		else if (strncmp(_argv[0], "env", 3) == 0)
		{
			char **str;
			for (str = environ; *str != NULL; str++)
				printf("%s\n", *str);
		}
		else
		{
			res = resolve(_argv[0]);
			if (res)
			{
				pid = fork();
				if (pid == 0)
				{
					if (execve(res, _argv, environ) == -1)
					{
						if (errno == ENOENT)
							fprintf(stderr, "%s: %d: %s: %s\n",
							argv[0], line, _argv[0], "not found");
						_exit(errno == ENOENT ? 127 : 1);
					}
				}
				else if (pid > 0)
				{
					waitpid(pid, &status, 0);
					if (WIFEXITED(status))
						status = WEXITSTATUS(status);
					else
						status = 1;
				}
			}
			else if (_argv[0])
			{
				fprintf(stderr, "%s: %d: %s: %s\n", argv[0], line,
					_argv[0], "not found");
				status = 127;
			}
			free(res);
			free(_argv);
			free(cmd);
		}
		line++;
	}
	return (status);
}
