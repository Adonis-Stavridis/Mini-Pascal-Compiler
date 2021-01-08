#ifndef __SYMBOLSTACK_H__
#define __SYMBOLSTACK_H__

#include <string.h>

#include "../symbol/symbol.h"

/**
 * @brief SymbolNode type
 * 
 */
typedef struct SymbolNode
{
	Symbol *symbol;					 /*!< Symbol */
	struct SymbolNode *prev; /*!< Previous symbol */
	struct SymbolNode *next; /*!< Next symbol */
} SymbolNode;

/**
 * @brief SymbolStack type
 * 
 */
typedef SymbolNode SymbolStack;

/**
 * @brief Initialize SymbolNode
 * 
 * @return SymbolNode* 
 */
SymbolNode *initSymbolNode(void);

/**
 * @brief Free SymbolNode
 * 
 * @param node 
 */
void freeSymbolNode(SymbolNode *node);

/**
 * @brief Add Symbol to SymbolStack by adding SymbolNode
 * 
 * @param stack 
 * @param symbol 
 */
void addSymbolNode(SymbolStack **stack, Symbol *symbol);

/**
 * @brief Lookup SymbolNode for Symbol name
 * 
 * @param stack 
 * @param name 
 * @return Symbol* 
 */
Symbol *lookupSymbolNode(SymbolStack **stack, char *name);

/**
 * @brief Remove Symbol by removing SymbolNode
 * 
 * @param stack 
 * @param symbol 
 */
void rmSymbolNode(SymbolStack **stack, Symbol *symbol);

/**
 * @brief Free SymbolStack
 * 
 * @param stack 
 */
void freeSymbolStack(SymbolStack **stack);

/**
 * @brief Print SymbolStack
 * 
 * @param stack 
 */
void printSymbolStack(SymbolStack **stack);

#endif /* __SYMBOLSTACK_H__ */
