#include "idList.h"

IdListNode *initIdList(void)
{
	return NULL;
}

IdListNode *addIdListNode(IdListNode *list, char *data)
{
	IdListNode *node = (IdListNode *)malloc(sizeof(IdListNode));
	if (!node)
	{
		compilerError(_FILE, _FUNCTION, _LINE);
	}

	node->data = data;
	node->next = list;

	return node;
}

char *getIdData(IdListNode *node)
{
	return node->data;
}

void freeIdList(IdListNode *list)
{
	while (list)
	{
		IdListNode *nextNode = list->next;
		free(list);
		list = nextNode;
	}
}

void printList(IdListNode *list)
{
	fprintf(stdout, "List\n");
	while (list)
	{
		fprintf(stdout, "| %s\n", list->data);
		list = list->next;
	}
}
