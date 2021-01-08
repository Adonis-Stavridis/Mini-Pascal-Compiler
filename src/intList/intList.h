#ifndef __INTLIST_H__
#define __INTLIST_H__

#include <stdlib.h>

#include "../errors/errors.h"

/**
 * @brief IntListNode type
 * 
 */
typedef struct IntListNode
{
	int data;									/*!< data of ListNode */
	struct IntListNode *next; /*!< next ListNode */
} IntListNode;

/**
 * @brief Initialize IntListNode
 * 
 * @return IntListNode* 
 */
IntListNode *initIntList(void);

/**
 * @brief Add new IntListNode
 * 
 * @param list 
 * @param data 
 * @return IntListNode* 
 */
IntListNode *addIntListNode(IntListNode *list, int data);

/**
 * @brief Get Id Data from IntListNode
 * 
 * @param node 
 * @return int 
 */
int getIntData(IntListNode *node);

/**
 * @brief Free IntListNode
 * 
 * @param list 
 */
void freeIntList(IntListNode *list);

/**
 * @brief Print IntListNode
 * 
 * @param list 
 */
void printIntList(IntListNode *list);

#endif /* __INTLIST_H__ */
