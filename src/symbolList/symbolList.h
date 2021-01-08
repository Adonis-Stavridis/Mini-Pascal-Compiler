#ifndef __SYMBOLLIST_H__
#define __SYMBOLLIST_H__

#include <stdlib.h>

#include "../errors/errors.h"
#include "../symbol/symbol.h"

/**
 * @brief SymbolListNode type
 * 
 */
typedef struct SymbolListNode
{
	Symbol *data;								 /*!< symbol of node */
	struct SymbolListNode *next; /*!< next SymbolListNode */
} SymbolListNode;

/**
 * @brief Initialize SymbolListNode
 * 
 * @return SymbolListNode* 
 */
SymbolListNode *initSymbolList(void);

/**
 * @brief Add symbol to SymbolListNode
 * 
 * @param list 
 * @param data 
 * @return SymbolListNode* 
 */
SymbolListNode *addSymbolListNode(SymbolListNode *list, Symbol *data);

/**
 * @brief Get Symbol Data from SymbolListNode
 * 
 * @param node 
 * @return Symbol* 
 */
Symbol *getSymbolData(SymbolListNode *node);

/**
 * @brief Free SymbolListNode
 * 
 * @param list 
 */
void freeSymbolList(SymbolListNode *list);

/**
 * @brief Verify SymbolListNode and Symbol array have same dims
 * 
 * @param list 
 * @param symbol 
 */
void verifySymbolListLength(SymbolListNode *list, Symbol *symbol);

#endif /* __SYMBOLLIST_H__ */
