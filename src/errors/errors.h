#ifndef __ERRORS_H__
#define __ERRORS_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../types/types.h"

#define ERROR_MESSAGE_SIZE 512

#define _FILE __FILE__
#define _FUNCTION __func__
#define _LINE __LINE__

extern int yylineno;
extern char *yytext;

/**
 * @brief Output usage error
 * 
 */
void usageError(void);

/**
 * @brief Ouput input file error
 * 
 */
void infileError(void);

/**
 * @brief Ouput output file error
 * 
 */
void outfileError(void);

/**
 * @brief Output error message with perror
 * 
 * @param text
 */
void defaultError(const char *text);

/**
 * @brief Output syntax error during lexical and syntaxical analysis
 * 
 */
void syntaxError(void);

/**
 * @brief Output parse error
 * 
 * @param val 
 */
void parseError(int val);

/**
 * @brief Output symbol not found error
 * 
 * @param name 
 */
void symbolNotFoundError(char *name);

/**
 * @brief Ouput uninitialized error
 * 
 */
void uninitializedError(void);

/**
 * @brief Output type error with actual and expected type
 * 
 * @param type 
 * @param expected 
 */
void typeError(ScalpaType type, ScalpaType expected);

/**
 * @brief Output array dimension error
 * 
 * @param dims 
 * @param expected 
 */
void arrayDimsError(int dims, int expected);

/**
 * @brief Output range error
 * 
 */
void rangeError(void);

/**
 * @brief Output not yet implemented error
 * 
 * @param text
 */
void notYetImplementedError(char *text);

/**
 * @brief Output input file error
 * 
 * @param infile 
 */
void inputFileError(char *infile);

/**
 * @brief Output compiler / runtime error
 * 
 * @param file 
 * @param function 
 * @param line 
 */
void compilerError(const char *file, const char *function, const int line);

#endif /* __ERRORS_H__ */
