#ifndef __IDLIST_H__
#define __IDLIST_H__

#include <stdlib.h>

#include "../errors/errors.h"

/**
 * @brief IdListNode type
 * 
 */
typedef struct IdListNode
{
	char *data;							 /*!< data of ListNode */
	struct IdListNode *next; /*!< next ListNode */
} IdListNode;

/**
 * @brief Initialize IdListNode
 * 
 * @return IdListNode* 
 */
IdListNode *initIdList(void);

/**
 * @brief Add new IdListNode
 * 
 * @param list 
 * @param data 
 * @return IdListNode* 
 */
IdListNode *addIdListNode(IdListNode *list, char *data);

/**
 * @brief Get Id Data from IdListNode
 * 
 * @param node 
 * @return char* 
 */
char *getIdData(IdListNode *node);

/**
 * @brief Free IdListNode
 * 
 * @param list 
 */
void freeIdList(IdListNode *list);

/**
 * @brief Print IdListNode
 * 
 * @param list 
 */
void printIdList(IdListNode *list);

#endif /* __IDLIST_H__ */
