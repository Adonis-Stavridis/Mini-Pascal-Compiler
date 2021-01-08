#include "symbolTable.h"
#include "../errors/errors.h"

size_t hash(char *name, size_t capacity)
{
	size_t i = 0;
	for (int j = 0; name[j]; j++)
	{
		i += name[j];
	}
	return i % capacity;
}

SymbolTable *initSymbolTable(void)
{
	SymbolTable *table = (SymbolTable *)malloc(sizeof(SymbolTable));
	if (!table)
	{
		compilerError(_FILE, _FUNCTION, _LINE);
	}
	table->data = (SymbolStack **)calloc(CAPACITY, sizeof(SymbolStack));
	if (!table->data)
	{
		compilerError(_FILE, _FUNCTION, _LINE);
	}

	table->size = 0;
	table->capacity = CAPACITY;

	return table;
}

void addSymbol(SymbolTable *table, Symbol *symbol)
{
	size_t index = hash(symbol->name, table->capacity);

	SymbolStack **stack = &table->data[index];
	addSymbolNode(stack, symbol);

	table->size++;
}

Symbol *lookupSymbol(SymbolTable *table, char *name)
{
	size_t index = hash(name, table->capacity);

	SymbolStack **stack = &table->data[index];

	Symbol *symbol = lookupSymbolNode(stack, name);
	if (!symbol)
	{
		symbolNotFoundError(name);
	}

	return symbol;
}

void rmSymbol(SymbolTable *table, Symbol *symbol)
{
	size_t index = hash(symbol->name, table->capacity);

	SymbolStack **stack = &table->data[index];
	rmSymbolNode(stack, symbol);

	table->size--;
}

void freeSymbolTable(SymbolTable *table)
{
	for (size_t i = 0; i < table->capacity; i++)
	{
		SymbolStack **stack = &table->data[i];
		if (stack)
		{
			freeSymbolStack(stack);
		}
	}
	free(table->data);
	free(table);
}

void printSymbolTable(SymbolTable *table)
{
	fprintf(stdout, "Symbol Table\n");

	if (table->size == 0)
	{
		fprintf(stdout, "! empty\n");
		return;
	}

	for (size_t i = 0; i < table->capacity; i++)
	{
		SymbolStack **stack = &table->data[i];
		if (stack)
		{
			printSymbolStack(stack);
		}
	}
	fprintf(stdout, "\n");
}
