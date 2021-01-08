#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../errors/errors.h"
#include "../intList/intList.h"
#include "../types/types.h"

#define TMP_NAME_SIZE 64

/**
 * @brief Array data
 * 
 */
typedef struct ArrayData
{
	ScalpaType arrayType; /*!< type of array */
	int *min;							/*!< list of mins of array */
	int *max;							/*!< list of maxs of array */
	int dims;							/*!< dims of array */
} ArrayData;

/**
 * @brief Symbol data
 * 
 */
typedef union
{
	int intVal;				 /*!< int value */
	bool boolVal;			 /*!< bool value */
	char *strVal;			 /*!< string value */
	ArrayData *arrVal; /*!< array value */
} Data;

/**
 * @brief Type of symbol
 * 
 */
typedef enum SymbolType
{
	NO_SYMBOL = 0,	/*!< no special symbol */
	REF_SYMBOL = 1, /*!< reference symbol */
	TMP_SYMBOL = 2	/*!< temporary symbol */
} SymbolType;

/**
 * @brief Symbol type
 * 
 */
typedef struct Symbol
{
	char *name;						 /*!< Name of symbol */
	ScalpaType dataType;	 /*!< ScalpaType of symbol */
	Data data;						 /*!< Data of symbol */
	SymbolType symbolType; /*!< Type of symbol */
	bool initialized;			 /*!< Initialized boolean */
} Symbol;

/**
 * @brief Initialize a Symbol
 * 
 * @param name 
 * @param dataType 
 * @param symbolType 
 * @return Symbol* 
 */
Symbol *initSymbol(char *name, ScalpaType dataType, SymbolType symbolType);

/**
 * @brief Free a Symbol
 * 
 * @param symbol 
 */
void freeSymbol(Symbol *symbol);

/**
 * @brief Create a new Variable Symbol
 * 
 * @param name 
 * @param dataType 
 * @param symbolType 
 * @return Symbol* 
 */
Symbol *newVar(char *name, ScalpaType dataType, SymbolType symbolType);

/**
 * @brief Create a new Temporary Variable Symbol
 * 
 * @param dataType 
 * @return Symbol* 
 */
Symbol *newTemp(ScalpaType dataType);

/**
 * @brief Create a new Array Symbol
 * 
 * @param name 
 * @param symbolType 
 * @return Symbol* 
 */
Symbol *newArr(char *name, SymbolType symbolType);

/**
 * @brief Set Symbol Data Int value
 * 
 * @param symbol 
 * @param intVal 
 */
void setSymbolDataInt(Symbol *symbol, int intVal);

/**
 * @brief Set Symbol Data Bool value
 * 
 * @param symbol 
 * @param boolVal 
 */
void setSymbolDataBool(Symbol *symbol, bool boolVal);

/**
 * @brief Set Symbol Data String value
 * 
 * @param symbol 
 * @param strVal 
 */
void setSymbolDataString(Symbol *symbol, char *strVal);

/**
 * @brief Initialize Array Data
 * 
 * @param arrayType
 * @param minList 
 * @param maxList 
 * @param dims 
 * @return ArrayData* 
 */
ArrayData *initArrayData(ScalpaType arrayType, IntListNode *minList, IntListNode *maxList, int dims);

/**
 * @brief Free Array Data
 * 
 * @param data 
 */
void freeArrayData(ArrayData *data);

/**
 * @brief Set Symbol Data Array value
 * 
 * @param symbol 
 * @param arrVal
 */
void setSymbolDataArray(Symbol *symbol, ArrayData *arrVal);

/**
 * @brief Verify Symbol type with expected type
 * 
 * @param symbol 
 * @param expectedType 
 */
void verifySymbolType(Symbol *symbol, ScalpaType expectedType);

/**
 * @brief Verify Symbol type with expected type1 or type2
 * 
 * @param symbol 
 * @param expectedType1 
 * @param expectedType2 
 */
void verifySymbolType2(Symbol *symbol, ScalpaType expectedType1, ScalpaType expectedType2);

/**
 * @brief Set Symbol to initialized
 * 
 * @param symbol 
 */
void setInitialized(Symbol *symbol);

/**
 * @brief Verify if Symbol is initialized
 * 
 * @param symbol 
 */
void verifyInitialized(Symbol *symbol);

/**
 * @brief Print a Symbol
 * 
 * @param symbol 
 */
void printSymbol(Symbol *symbol);

#endif /* __SYMBOL_H__ */
