#include "symbolStack.h"

SymbolNode *initSymbolNode(void)
{
	SymbolNode *node = (SymbolNode *)malloc(sizeof(SymbolNode));
	if (!node)
	{
		compilerError(_FILE, _FUNCTION, _LINE);
	}

	return node;
}

void freeSymbolNode(SymbolNode *node)
{
	freeSymbol(node->symbol);
	free(node);
}

void addSymbolNode(SymbolStack **stack, Symbol *symbol)
{
	SymbolNode *node = initSymbolNode();

	node->symbol = symbol;
	node->next = (*stack);
	node->prev = NULL;

	if ((*stack))
	{
		(*stack)->prev = node;
	}

	(*stack) = node;
}

Symbol *lookupSymbolNode(SymbolStack **stack, char *name)
{
	SymbolNode *node = (*stack);

	while (node)
	{
		if (strcmp(node->symbol->name, name) == 0)
		{
			return node->symbol;
		}
	}
	return NULL;
}

void rmSymbolNode(SymbolStack **stack, Symbol *symbol)
{
	if (!(*stack) || !symbol)
	{
		return;
	}

	SymbolNode *node = (*stack);

	while (node)
	{
		if (node->symbol == symbol)
		{
			node->prev->next = node->next;
			node->next->prev = node->prev;
			freeSymbolNode(node);
			return;
		}
		node = node->next;
	}
}

void freeSymbolStack(SymbolStack **stack)
{
	SymbolNode *node = (*stack);
	while (node)
	{
		SymbolNode *nextNode = node->next;
		freeSymbolNode(node);
		node = nextNode;
	}
}

void printSymbolStack(SymbolStack **stack)
{
	SymbolNode *node = (*stack);
	while (node)
	{
		printSymbol(node->symbol);
		node = node->next;
	}
}
