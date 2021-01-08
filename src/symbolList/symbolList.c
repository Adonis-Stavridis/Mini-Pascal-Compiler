#include "symbolList.h"

SymbolListNode *initSymbolList(void)
{
	return NULL;
}

SymbolListNode *addSymbolListNode(SymbolListNode *list, Symbol *data)
{
	SymbolListNode *node = (SymbolListNode *)malloc(sizeof(SymbolListNode));
	if (!node)
	{
		compilerError(_FILE, _FUNCTION, _LINE);
	}

	node->data = data;
	node->next = list;

	return node;
}

Symbol *getSymbolData(SymbolListNode *node)
{
	return node->data;
}

void freeSymbolList(SymbolListNode *list)
{
	while (list)
	{
		SymbolListNode *nextNode = list->next;
		free(list);
		list = nextNode;
	}
}

void verifySymbolListLength(SymbolListNode *list, Symbol *symbol)
{
	if (symbol->dataType != ARR_TYPE)
	{
		return;
	}

	int length = 0;
	while (list)
	{
		length++;
		list = list->next;
	}

	if (symbol->data.arrVal->dims != length)
	{
		arrayDimsError(length, symbol->data.arrVal->dims);
	}
}
