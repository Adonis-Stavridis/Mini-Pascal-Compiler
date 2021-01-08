#include "symbol.h"

Symbol *initSymbol(char *name, ScalpaType dataType, SymbolType symbolType)
{
	Symbol *symbol = (Symbol *)malloc(sizeof(Symbol));
	if (!symbol)
	{
		compilerError(_FILE, _FUNCTION, _LINE);
	}

	symbol->name = name;
	symbol->dataType = dataType;
	symbol->symbolType = symbolType;
	symbol->initialized = false;

	switch (symbol->dataType)
	{
	case INT_TYPE:
		symbol->data.intVal = 0;
		break;

	case BOOL_TYPE:
		symbol->data.boolVal = false;
		break;

	case STR_TYPE:
		symbol->data.strVal = NULL;
		break;

	case ARR_TYPE:
		symbol->data.arrVal = NULL;
		break;

	default:
		break;
	}

	return symbol;
}

void freeSymbol(Symbol *symbol)
{
	free(symbol->name);
	switch (symbol->dataType)
	{
	case STR_TYPE:
		free(symbol->data.strVal);
		break;

	case ARR_TYPE:
		freeArrayData(symbol->data.arrVal);
		break;

	default:
		break;
	}
	free(symbol);
}

Symbol *newVar(char *name, ScalpaType dataType, SymbolType symbolType)
{
	return initSymbol(name, dataType, symbolType);
}

Symbol *newTemp(ScalpaType dataType)
{
	static int counter = 0;
	char buffer[TMP_NAME_SIZE];
	snprintf(buffer, TMP_NAME_SIZE, "_t_%d", counter++);

	char *name = strdup(buffer);
	if (!name)
	{
		compilerError(_FILE, _FUNCTION, _LINE);
	}

	return initSymbol(name, dataType, TMP_SYMBOL);
}

Symbol *newArr(char *name, SymbolType symbolType)
{
	return initSymbol(name, ARR_TYPE, symbolType);
}

void setSymbolDataInt(Symbol *symbol, int intVal)
{
	symbol->initialized = true;
	symbol->data.intVal = intVal;
}

void setSymbolDataBool(Symbol *symbol, bool boolVal)
{
	symbol->initialized = true;
	symbol->data.boolVal = boolVal;
}

void setSymbolDataString(Symbol *symbol, char *strVal)
{
	symbol->initialized = true;
	symbol->data.strVal = strVal;
}

ArrayData *initArrayData(ScalpaType arrayType, IntListNode *minList, IntListNode *maxList, int dims)
{
	if (!(arrayType == INT_TYPE || arrayType == BOOL_TYPE))
	{
		typeError(arrayType, INT_TYPE);
	}

	ArrayData *arrayData = (ArrayData *)malloc(sizeof(ArrayData));
	if (!arrayData)
	{
		compilerError(_FILE, _FUNCTION, _LINE);
	}

	arrayData->arrayType = arrayType;
	arrayData->dims = dims;

	arrayData->min = (int *)calloc(dims, sizeof(int));
	if (!arrayData->min)
	{
		compilerError(_FILE, _FUNCTION, _LINE);
	}

	arrayData->max = (int *)calloc(dims, sizeof(int));
	if (!arrayData->max)
	{
		compilerError(_FILE, _FUNCTION, _LINE);
	}

	for (int i = dims - 1; i >= 0; i--)
	{
		arrayData->min[i] = minList->data;
		arrayData->max[i] = maxList->data;
		minList = minList->next;
		maxList = maxList->next;
	}

	return arrayData;
}

void freeArrayData(ArrayData *data)
{
	free(data->min);
	free(data->max);
	free(data);
}

void setSymbolDataArray(Symbol *symbol, ArrayData *arrVal)
{
	symbol->initialized = true;
	symbol->data.arrVal = arrVal;
}

void verifySymbolType(Symbol *symbol, ScalpaType expectedType)
{
	if (symbol->dataType != expectedType)
	{
		typeError(symbol->dataType, expectedType);
	}
}

void verifySymbolType2(Symbol *symbol, ScalpaType expectedType1, ScalpaType expectedType2)
{
	if (symbol->dataType == expectedType1 || symbol->dataType == expectedType2)
	{
		return;
	}

	typeError(symbol->dataType, expectedType1);
}

void setInitialized(Symbol *symbol)
{
	symbol->initialized = true;
}

void verifyInitialized(Symbol *symbol)
{
	if (!symbol->initialized)
	{
		uninitializedError();
	}
}

void printSymbol(Symbol *symbol)
{
	fprintf(stdout, "| ");

	if (!symbol)
	{
		fprintf(stdout, "NULL symbol\n");
		return;
	}

	switch (symbol->symbolType)
	{
	case REF_SYMBOL:
		fprintf(stdout, "(ref) ");
		break;

	case TMP_SYMBOL:
		fprintf(stdout, "(temp) ");
		break;

	default:
		break;
	}

	fprintf(stdout, "%s", symbol->name);

	switch (symbol->dataType)
	{
	case UNIT_TYPE:
		fprintf(stdout, ": unit");
		break;

	case INT_TYPE:
		fprintf(stdout, ": int");
		break;

	case BOOL_TYPE:
		fprintf(stdout, ": bool");
		break;

	case STR_TYPE:
		fprintf(stdout, ": string");
		break;

	case ARR_TYPE:
		fprintf(stdout, ": array");
		break;

	default:
		break;
	}

	if (!symbol->initialized)
	{
		fprintf(stdout, "\n");
		return;
	}

	switch (symbol->dataType)
	{
	case INT_TYPE:
		fprintf(stdout, " = %d", symbol->data.intVal);
		break;

	case BOOL_TYPE:
		fprintf(stdout, " = %d", symbol->data.boolVal);
		break;

	case STR_TYPE:
		fprintf(stdout, " = %s", symbol->data.strVal);
		break;

	case ARR_TYPE:
	{
		int dims = symbol->data.arrVal->dims;
		for (int i = 0; i < dims; i++)
		{
			fprintf(stdout, "[%d..%d]", symbol->data.arrVal->min[i], symbol->data.arrVal->max[i]);
		}
		switch (symbol->data.arrVal->arrayType)
		{
		case INT_TYPE:
			fprintf(stdout, " of int");
			break;

		case BOOL_TYPE:
			fprintf(stdout, " of bool");
			break;

		default:
			break;
		}
		break;
	}

	default:
		break;
	}

	fprintf(stdout, "\n");
}
