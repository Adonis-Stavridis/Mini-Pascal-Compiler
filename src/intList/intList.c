#include "intList.h"

IntListNode *initIntList(void)
{
	return NULL;
}

IntListNode *addIntListNode(IntListNode *list, int data)
{
	IntListNode *node = (IntListNode *)malloc(sizeof(IntListNode));
	if (!node)
	{
		compilerError(_FILE, _FUNCTION, _LINE);
	}

	node->data = data;
	node->next = list;

	return node;
}

int getIntData(IntListNode *node)
{
	return node->data;
}

void freeIntList(IntListNode *list)
{
	while (list)
	{
		IntListNode *nextNode = list->next;
		free(list);
		list = nextNode;
	}
}

void printIntList(IntListNode *list)
{
	fprintf(stdout, "List\n");
	while (list)
	{
		fprintf(stdout, "| %d\n", list->data);
		list = list->next;
	}
}
