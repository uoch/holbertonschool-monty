#include "monty.h"

/**
 * push - pushes a node to a stack
 * @stack: head of the stack
 * @n: line number
 */
void push(stack_t **stack, unsigned int n)
{
	stack_t *node = NULL;
	(void)n;

	node = malloc(sizeof(stack_t));
	if (!node)
	{
		fprintf(stderr, "Error: malloc failed\n");
		free_stack(stack);
		exit(EXIT_FAILURE);
	}

	node->prev = node->next = NULL;

	if (!(*stack))
		(*stack) = node;
	else
	{
		(*stack)->prev = node;
		node->next = *stack;
		*stack = node;
	}
}
/**
 * pall - prints a stack
 * @stack: pointer to a stack_t
 * @n: line number
 */
void pall(stack_t **stack, unsigned int n)
{
	stack_t *node, *head;
	(void)n;

	if (!stack || !(*stack))
		return;
	node = head = *stack;

	while (node)
	{
		fprintf(stdout, "%d\n", node->n);
		node = node->next;
	}
}
/**
 * parser - function taht split the input in an array of string
 * @buff: the input
 * @limit: the delimenter
 * Return: array of string
 */
char **parser(char *buff, char *limit)
{
	char *ptr = NULL;
	char **cmd = NULL;
	size_t idx = 0;

	ptr = strtok(buff, limit);
	if (!ptr)
	{
		free(buff);
		exit(EXIT_FAILURE);
	}
	while (ptr)
	{
		cmd = realloc(cmd, ((idx + 1) * sizeof(char *)));
		cmd[idx] = strdup(ptr);
		ptr = strtok(NULL, limit);
		++idx;
	}
	cmd = realloc(cmd, ((idx + 1) * sizeof(char *)));
	cmd[idx] = NULL;
	return (cmd);
}
/**
 * free_stack - free the stack
 * @stack: ptr to stack
 * Return: Nothing
 */
void free_stack(stack_t **stack)
{
	stack_t *head = *stack;

	while (head)
	{
		if (!head->next)
		{
			free(head);
			break;
		}
		head = head->next;
		free(head->prev);
	}
}
/**
 * call - call appropriate function
 * @tokens: pointer to array of tokens
 * @stack: pointer to a stack
 *
 * Return: nothing
 */
void get_op_func(char **tokens, stack_t **stack)
{
	instruction_t ops[] = {
		{"push", push},
		{"pall", pall},
		{"pint", pint},
		{"pop", pop},
		{"swap", swap},
		{"add", add},
		{"nop", nop},
		{NULL, NULL}};
	int idx = 0;

	while (ops[idx].opcode)
	{
		if (tokens[0][0] == '#')
			return;
		if (strcmp(tokens[0], ops[idx].opcode) == 0)
		{
			if (ops[idx].f)
				ops[idx].f(stack, line_number);
			break;
		}
		idx++;
	}

	if (strcmp(tokens[0], "push") == 0)
	{
		is_valid(tokens, stack);
		(*stack)->n = atoi(tokens[1]);
	}
	else if (!(ops[idx].opcode))
	{
		fprintf(stderr, "L%u: unknown instruction %s\n",
				line_number,
				tokens[0]);
		exit(EXIT_FAILURE);
	}
}
/**
 * is_valid - check if second spot of a token array is valid
 * @token: tokens value to check
 * @stack: pointer to a stack
 */
void is_valid(char **token, stack_t **stack)
{
	int idx = 0;

	if (!token[1])
	{
		fprintf(stderr, "L%u: usage: push integer\n", line_number);
		if (*stack)
			free_stack(stack);
		free(token);
		exit(EXIT_FAILURE);
	}

	while (token[1][idx])
	{
		if (token[1][idx] == '-' && idx == 0)
			idx++;
		if (isdigit(token[1][idx]) == 0)
		{
			fprintf(stderr, "L%u: usage: push integer\n", line_number);
			if (*stack)
				free_stack(stack);
			free(token);
			exit(EXIT_FAILURE);
		}
		idx++;
	}
}
/**
 * pint - prints the top for the stack
 * @stack: pointered to a stack
 * @n: line number that opcode is call at.
 */
void pint(stack_t **stack, unsigned int n)
{
	if (!stack || !(*stack))
	{
		fprintf(stderr, "L%u: can't pint, stack empty\n", n);
		exit(EXIT_FAILURE);
	}

	fprintf(stdout, "%d\n", (*stack)->n);
}
/**
 * pop - remove the top elem of a stack
 * @stack: pointer to top of a stack
 * @n: line number
 */
void pop(stack_t **stack, unsigned int n)
{
	stack_t *node;

	(void)n;

	if (!stack || !(*stack))
	{
		fprintf(stderr, "L%u: can't pop an empty stack\n", n);
		exit(EXIT_FAILURE);
	}

	node = *stack;

	if ((*stack)->next)
	{
		*stack = (*stack)->next;
		(*stack)->prev = NULL;
	}
	else
		*stack = NULL;

	free(node);
}
/**
 * swap - swaps the data in two nodes
 * @stack: pointer to a stack_t
 * @line_number: line number
 */
void swap(stack_t **stack, unsigned int line_number)
{
	int store = 0;

	if ((stack == NULL) || (*stack == NULL) || ((*stack)->next) == NULL)
	{
		fprintf(stderr, "L%u: can't swap, stack too short\n", line_number);
		if (*stack)
			free_stack(stack);
		exit(EXIT_FAILURE);
	}

	(void)line_number;

	store = (*stack)->next->n;
	(*stack)->next->n = (*stack)->n;
	(*stack)->n = store;
}
/**
 * add - swaps the data in two nodes
 * @stack: pointer to a stack_t
 * @line_number: line number
 */
void add(stack_t **stack, unsigned int line_number)
{
	if (stack == NULL || (*stack == NULL) || ((*stack)->next == NULL))
	{
		fprintf(stderr, "L%u: can't add, stack too short\n", line_number);
		if (*stack)
			free_stack(stack);
		exit(EXIT_FAILURE);
	}
	(*stack)->next->n = ((*stack)->next->n) + ((*stack)->n);
	pop(stack, line_number);
}
/**
 * nop - doesn’t do anything
 * @stack: pointer to a stack_t
 * @line_number: line number
 */
void nop(stack_t **stack, unsigned int line_number)
{
	if (stack == NULL || (*stack == NULL) || ((*stack)->next == NULL))
	{
		fprintf(stderr, "L%u: can't add, stack too short\n", line_number);
		if (*stack)
			free_stack(stack);
		exit(EXIT_FAILURE);
	}
}
