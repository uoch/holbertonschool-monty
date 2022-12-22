#include "monty.h"
unsigned int line_number = 0;
/**
 * main - control program flow
 * @argc: argument count
 * @argv: argument list
 * Return: Nothing
 */
int main(int argc, char *argv[])
{
	char **tokens = NULL;
	size_t n = 1;
	FILE *fp;
	char *buffer = NULL;
	stack_t *head = NULL;

	if (argc != 2)
	{
		fprintf(stderr, "USAGE: monty file\n");
		exit(EXIT_FAILURE);
	}
	fp = fopen(argv[1], "r+");
	if (fp == NULL)
	{
		fprintf(stderr, "Error: Can't open file %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	while (getline(&buffer, &n, fp) > 0)
	{
		++line_number;
		if (strlen(buffer) == 1)
			continue;
		tokens = parser(buffer, DELIM); /* result is at top of list */
		if (tokens)
		{
			get_op_func(tokens, &head);
			freeArr(tokens);
		}
	}
	free(buffer);
	free_stack(&head);
	fclose(fp);
	return (0);
}
