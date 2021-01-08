#ifndef __GENCODE_H__
#define __GENCODE_H__

#include <stdio.h>
#include "../quad/quad.h"
#include "../quadStack/quadStack.h"
#include "../symbolTable/symbolTable.h"

extern FILE *yyout;

/**
 * @brief Generate Mips Data code
 * 
 * @param table 
 */
void genData(SymbolTable *table);

/**
 * @brief Generate Quad code
 * 
 * @param quad 
 */
void genQuad(Quad *quad);

/**
 * @brief Generate Mips Text code
 * 
 * @param quadList 
 */
void genText(QuadList *quadList);

/**
 * @brief Generate entire code
 * 
 * @param quadList 
 * @param table 
 */
void gencode(QuadList *quadList, SymbolTable *table);

#endif /* __GENCODE_H__ */
