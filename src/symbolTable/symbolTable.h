#ifndef __SYMBOLTABLE_H__
#define __SYMBOLTABLE_H__

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../symbolStack/symbolStack.h"
#include "../errors/errors.h"

#define CAPACITY 512

/**
 * @brief SymbolTable type
 * 
 */
typedef struct SymbolTable
{
	SymbolStack **data; /*!< Array of SymbolStack */
	size_t size;				/*!< Size of SymbolStack */
	size_t capacity;		/*!< Size of data array of SymbolStack */
} SymbolTable;

/**
 * @brief String hash function
 * 
 * @param name 
 * @param capacity 
 * @return size_t 
 */
size_t hash(char *name, size_t capacity);

/**
 * @brief Initialize SymbolTable
 * 
 * @return SymbolTable* 
 */
SymbolTable *initSymbolTable(void);

/**
 * @brief Add a symbol to the SymbolTable
 * 
 * @param table 
 * @param symbol 
 */
void addSymbol(SymbolTable *table, Symbol *symbol);

/**
 * @brief Lookup symbol in SymbolTable by name
 * 
 * @param table 
 * @param name 
 * @return Symbol* 
 */
Symbol *lookupSymbol(SymbolTable *table, char *name);

/**
 * @brief Remove symbol from the SymbolTable
 * 
 * @param table 
 * @param symbol 
 */
void rmSymbol(SymbolTable *table, Symbol *symbol);

/**
 * @brief Free the SymbolTable
 * 
 * @param table 
 */
void freeSymbolTable(SymbolTable *table);

/**
 * @brief Print SymbolTable
 * 
 * @param table 
 */
void printSymbolTable(SymbolTable *table);

#endif /* __SYMBOLTABLE_H__ */
