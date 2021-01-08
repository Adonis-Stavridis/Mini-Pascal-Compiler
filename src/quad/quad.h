#ifndef __QUAD_H__
#define __QUAD_H__

#include "../symbol/symbol.h"
#include "../symbolList/symbolList.h"
#include "../symbolTable/symbolTable.h"

/**
 * @brief Operator type
 * 
 */
typedef enum Operator
{
	ASS_OP = 0,				/*!< Assignment */
	ADD_OP = 1,				/*!< Addition */
	SUB_OP = 2,				/*!< Substraction */
	MUL_OP = 3,				/*!< Multiplication */
	DIV_OP = 4,				/*!< Division */
	POW_OP = 5,				/*!< Power */
	AND_OP = 6,				/*!< And Logic */
	OR_OP = 7,				/*!< Or Logic */
	XOR_OP = 8,				/*!< Xor Logic */
	NOT_OP = 9,				/*!< Not Logic */
	EQ_OP = 10,				/*!< Egal Logic */
	DIFF_OP = 11,			/*!< Diff Logic */
	GTH_OP = 12,			/*!< Greater than Logic */
	LTH_OP = 13,			/*!< Lower than Logic */
	GEQ_OP = 14,			/*!< Greater or Equal than Logic */
	LEQ_OP = 15,			/*!< Lower or Equal than Logic */
	WRTE_OP = 16,			/*!< Write */
	READ_OP = 17,			/*!< Read */
	IF_OP = 18,				/*!< If */
	ELSE_OP = 19,			/*!< Else */
	IF_END_OP = 20,		/*!< End of if */
	EXPR_M_OP = 21,		/*!< Expression marker */
	WHLE_OP = 22,			/*!< While */
	WHLE_END_OP = 23, /*!< End of while */
	ARR_ASS_OP = 24,	/*!< Array assign operator */
	ARR_R_OP = 25,		/*!< Array read operator */
	ARR_OP = 26,			/*!< Array operator */
} Operator;

/**
 * @brief Quad type
 * 
 */
typedef struct Quad
{
	Operator op;					/*!< Operator */
	Symbol *arg1;					/*!< First symbol */
	Symbol *arg2;					/*!< Second symbol */
	Symbol *result;				/*!< Result symbol */
	SymbolListNode *list; /*!< Symbol list args */
	struct Quad *next;		/*!< Next Quad */
} Quad;

/**
 * @brief QuadList type
 * 
 */
typedef struct QuadList
{
	Quad *start; /*!< Start of list */
	Quad *end;	 /*!< End of list */
} QuadList;

/**
 * @brief Initialize a Quad
 * 
 * @param op 
 * @param arg1 
 * @param arg2 
 * @param result 
 * @return Quad* 
 */
Quad *initQuad(Operator op, Symbol *arg1, Symbol *arg2, Symbol *result);

/**
 * @brief Free a Quad
 * 
 * @param quad 
 */
void freeQuad(Quad *quad);

/**
 * @brief Initialize a QuadList
 * 
 * @return Quad* 
 */
QuadList *initQuadList(void);

/**
 * @brief Add a Quad to a QuadList
 * 
 * @param quadList 
 * @param quad 
 * @return Quad* 
 */
void addQuad(QuadList *quadList, Quad *quad);

/**
 * @brief Free a QuadList
 * 
 * @param quadList 
 */
void freeQuadList(QuadList *quadList);

/**
 * @brief Set Quad SymbolListNode
 * 
 * @param quad 
 * @param list 
 */
void setQuadSymbolList(Quad *quad, SymbolListNode *list);

#endif /* __QUAD_H__ */
