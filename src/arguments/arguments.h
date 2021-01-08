#ifndef __ARGUMENTS_H__
#define __ARGUMENTS_H__

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>
#include <errno.h>

#include "../errors/errors.h"

#define ARG_VERSION "-version"
#define ARG_TOS "-tos"
#define ARG_OUTPUT "-o"

#define INFILE_EXT_P ".p"
#define INFILE_EXT_SCALPA ".scalpa"
#define OUTFILE_EXT_S ".s"

#define DEFAULT_OUTFILE "scalpaProgram.s"

/**
 * @brief Arguments data structure
 * 
 */
typedef struct Arguments
{
	char *infile;  /*!< Input file path */
	char *outfile; /*!< Output file path */
	bool tos;      /*!< Display SymbolTable boolean */
} Arguments;

/**
 * @brief Print version information
 * 
 */
void printVersion(void);

/**
 * @brief Initialize Arguments data structure
 * 
 * @return Arguments* 
 */
Arguments *initArguments(void);

/**
 * @brief Set input file in Arguments data structure
 * 
 * @param args 
 * @param path 
 */
void setArgumentsInfile(Arguments *args, char *path);

/**
 * @brief Set output file in Arguments data structure
 * 
 * @param args 
 * @param path 
 */
void setArgumentsOutfile(Arguments *args, char *path);

/**
 * @brief Set display Symbol table boolean in Arguments data structure
 * 
 * @param args 
 */
void setArgumentsTos(Arguments *args);

/**
 * @brief Verify input file extension
 * 
 * @param path 
 */
void verifyInfileExtension(char *path);

/**
 * @brief Verify ouput file extension
 * 
 * @param path 
 */
void verifyOutfileExtension(char *path);

/**
 * @brief Check if input file is empty
 * 
 * @param args 
 * @return true 
 * @return false 
 */
bool emptyArgumentsInfile(Arguments *args);

/**
 * @brief Handle empty output file
 * 
 * @param args 
 */
void handleEmptyArgumentsOutfile(Arguments *args);

/**
 * @brief Create output directories
 * 
 * @param path 
 */
void createOutputDirectories(char *path);

/**
 * @brief Print Arguments data structure
 * 
 * @param args 
 */
void printArguments(Arguments *args);

/**
 * @brief Free Arguments data structure
 * 
 * @param args 
 */
void freeArguments(Arguments *args);

/**
 * @brief Handle arguments passed into program
 * 
 * @param argc 
 * @param argv 
 * @return Arguments* 
 */
Arguments *handleArguments(int argc, char **argv);

#endif /* __ARGUMENTS_H__ */
