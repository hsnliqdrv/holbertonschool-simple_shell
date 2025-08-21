#include "shell.h"
#include <stdlib.h>
#include <string.h>
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
