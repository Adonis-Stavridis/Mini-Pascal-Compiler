#include <stdlib.h>

#include "../arguments/arguments.h"
#include "../gencode/gencode.h"
#include "../symbolTable/symbolTable.h"
#include "../quad/quad.h"

SymbolTable *table = NULL;
QuadList *quadList = NULL;

extern FILE *yyin;
extern FILE *yyout;

extern int yylex_destroy(void);
extern int yyparse(void);

int main(int argc, char **argv)
{
	Arguments *args = handleArguments(argc, argv);
	if (!args)
	{
		return EXIT_SUCCESS;
	}

	yyin = fopen(args->infile, "r");
	if (!yyin)
	{
		inputFileError(args->infile);
	}

	table = initSymbolTable();
	quadList = initQuadList();

	int parse = yyparse();
	if (parse != 0)
	{
		parseError(parse);
	}

	if (fclose(yyin) == EOF)
	{
		compilerError(_FILE, _FUNCTION, _LINE);
	}

	if (args->tos)
	{
		printSymbolTable(table);
	}

	createOutputDirectories(args->outfile);

	yyout = fopen(args->outfile, "w+");
	if (!yyout)
	{
		compilerError(_FILE, _FUNCTION, _LINE);
	}

	gencode(quadList, table);

	if (fclose(yyout) == EOF)
	{
		compilerError(_FILE, _FUNCTION, _LINE);
	}

	freeSymbolTable(table);
	freeQuadList(quadList);

	yylex_destroy();

	freeArguments(args);

	return EXIT_SUCCESS;
}
