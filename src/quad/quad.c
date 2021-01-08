#include "quad.h"

Quad *initQuad(Operator op, Symbol *arg1, Symbol *arg2, Symbol *result)
{
	Quad *quad = (Quad *)malloc(sizeof(Quad));
	if (!quad)
	{
		compilerError(_FILE, _FUNCTION, _LINE);
	}

	quad->op = op;
	quad->arg1 = arg1;
	quad->arg2 = arg2;
	quad->result = result;
	quad->list = NULL;

	quad->next = NULL;

	return quad;
}

void freeQuad(Quad *quad)
{
	free(quad);
}

QuadList *initQuadList(void)
{
	QuadList *quadList = (QuadList *)malloc(sizeof(QuadList));
	if (!quadList)
	{
		compilerError(_FILE, _FUNCTION, _LINE);
	}

	quadList->start = NULL;
	quadList->end = NULL;

	return quadList;
}

void addQuad(QuadList *quadList, Quad *quad)
{
	if (!quad)
	{
		return;
	}

	if (!quadList->start)
	{
		quadList->start = quad;
		quadList->end = quad;
	}
	else
	{
		quadList->end->next = quad;
		quadList->end = quad;
	}
}

void freeQuadList(QuadList *quadList)
{
	Quad *quad = quadList->start;
	while (quad)
	{
		Quad *nextQuad = quad->next;
		freeQuad(quad);
		quad = nextQuad;
	}
	free(quadList);
}

void setQuadSymbolList(Quad *quad, SymbolListNode *list)
{
	quad->list = list;
}
