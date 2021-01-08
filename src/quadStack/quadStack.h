#ifndef __QUADSTACK_H__
#define __QUADSTACK_H__

#include <stdlib.h>

#include "../errors/errors.h"

/**
 * @brief QuadStackNode type
 * 
 */
typedef struct QuadStackNode
{
	int cnt;										/*!< counter data */
	struct QuadStackNode *next; /*!< next QuadStackNode */
} QuadStack;

/**
 * @brief Initialize QuadStack
 * 
 * @return QuadStack* 
 */
QuadStack *initQuadStack(void);

/**
 * @brief Add cnt to QuadStack
 * 
 * @param stack 
 * @param cnt 
 * @return QuadStack* 
 */
QuadStack *addQuadStackNode(QuadStack *stack, int cnt);

/**
 * @brief Get top data of QuadStack
 * 
 * @param stack 
 * @return int 
 */
int topQuadStack(QuadStack *stack);

/**
 * @brief Pop QuadStack
 * 
 * @param stack 
 * @return QuadStack* 
 */
QuadStack *popQuadStack(QuadStack *stack);

/**
 * @brief Free QuadStack
 * 
 * @param stack 
 */
void freeQuadStack(QuadStack *stack);

#endif /* __QUADSTACK_H__ */
