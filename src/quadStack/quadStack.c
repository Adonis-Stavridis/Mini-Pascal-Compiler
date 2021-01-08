#include "quadStack.h"

QuadStack *initQuadStack(void)
{
	return NULL;
}

QuadStack *addQuadStackNode(QuadStack *stack, int cnt)
{
	QuadStack *node = (QuadStack *)malloc(sizeof(QuadStack));
	if (!node)
	{
		compilerError(_FILE, _FUNCTION, _LINE);
	}

	node->cnt = cnt;
	node->next = stack;

	return node;
}

int topQuadStack(QuadStack *stack)
{
	if (!stack)
	{
		defaultError("quadStack is empty");
	}
	return stack->cnt;
}

QuadStack *popQuadStack(QuadStack *stack)
{
	if (!stack)
	{
		defaultError("quadStack is empty");
	}

	QuadStack *next = stack->next;
	free(stack);
	return next;
}

void freeQuadStack(QuadStack *stack)
{
	while (stack)
	{
		QuadStack *next = stack->next;
		free(stack);
		stack = next;
	}
}
