#ifndef __RULES_H__
#define __RULES_H__

#include "../intList/intList.h"
#include "../symbolList/symbolList.h"
#include "../types/types.h"

/**
 * @brief RangeRule type
 * 
 */
typedef struct RangeRule
{
	IntListNode *minList; /*!< list of mins of array */
	IntListNode *maxList; /*!< list of maxs of array */
	int dims;							/*!< dims of array */
} RangeRule;

/**
 * @brief ArrayRule type
 * 
 */
typedef struct ArrayRule
{
	RangeRule range;			/*!< range limits */
	ScalpaType arrayType; /*!< type of array */
} ArrayRule;

/**
 * @brief TypeRule
 * 
 */
typedef struct TypeRule
{
	ScalpaType type; /*!< type of symbol */
	ArrayRule array; /*!< array data */
} TypeRule;

/**
 * @brief SymbolRule
 * 
 */
typedef struct SymbolRule
{
	Symbol *symbol;							/*!< symbol pointer */
	SymbolListNode *symbolList; /*!< list of symbols data */
} SymbolRule;

#endif /* __RULES_H__*/
