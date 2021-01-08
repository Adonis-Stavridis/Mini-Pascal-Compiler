#include "errors.h"

void usageError(void)
{
	fprintf(stderr, "\033[1mscalpa: \033[31musage:\033[0m\033[39m scalpa [-version] [-tos] [-o outfile] infile\n");
	exit(EXIT_FAILURE);
}

void infileError(void)
{
	fprintf(stderr, "\033[1mscalpa: \033[31musage:\033[0m\033[39m input should have .p or .scalpa file extension\n");
	exit(EXIT_FAILURE);
}

void outfileError(void)
{
	fprintf(stderr, "\033[1mscalpa: \033[31musage:\033[0m\033[39m output should have .s file extension\n");
	exit(EXIT_FAILURE);
}

void defaultError(const char *text)
{
	fprintf(stderr, "\033[1mscalpa: \033[31merror:\033[0m\033[39m %s\n", text);
	exit(EXIT_FAILURE);
}

void syntaxError(void)
{
	fprintf(stderr, "\033[1mscalpa: \033[31msyntax error:\033[0m\033[39m unexpected token \033[1m%s\033[0m at line %d\n", yytext, yylineno);
	exit(EXIT_FAILURE);
}

void parseError(int val)
{
	switch (val)
	{
	case 1:
		fprintf(stderr, "\033[1mscalpa: \033[31mparsing error:\033[0m\033[39m parsing failed due to invalid input\n");
		break;
	case 2:
		fprintf(stderr, "\033[1mscalpa: \033[31mparsing error:\033[0m\033[39m parsing failed due to memory exhaustion\n");
		break;

	default:
		break;
	}
	exit(EXIT_FAILURE);
}

void symbolNotFoundError(char *name)
{
	fprintf(stderr, "\033[1mscalpa: \033[31merror:\033[0m\033[39m symbol \033[1m%s\033[0m not found\n", name);
	exit(EXIT_FAILURE);
}

void uninitializedError(void)
{
	fprintf(stderr, "\033[1mscalpa: \033[31merror:\033[0m\033[39m trying to access uninitialized symbol at token \033[1m%s\033[0m at line %d\n", yytext, yylineno);
	exit(EXIT_FAILURE);
}

void typeError(ScalpaType type, ScalpaType expected)
{
	char *typeString = NULL;
	char *expectedString = NULL;

	switch (type)
	{
	case UNIT_TYPE:
		typeString = strdup("unit");
		break;

	case INT_TYPE:
		typeString = strdup("int");
		break;

	case BOOL_TYPE:
		typeString = strdup("bool");
		break;

	case STR_TYPE:
		typeString = strdup("string");
		break;

	default:
		typeString = strdup("unknown");
		break;
	}

	switch (expected)
	{
	case UNIT_TYPE:
		expectedString = strdup("unit");
		break;

	case INT_TYPE:
		expectedString = strdup("int");
		break;

	case BOOL_TYPE:
		expectedString = strdup("bool");
		break;

	case STR_TYPE:
		expectedString = strdup("string");
		break;

	default:
		typeString = strdup("unknown");
		break;
	}

	fprintf(stderr, "\033[1mscalpa: \033[31mtype error:\033[0m\033[39m unexpected type at token %s\033[0m at line %d: expected type \033[1m%s\033[0m but got \033[1m\033[31m%s\033[0m\033[39m\n", yytext, yylineno, expectedString, typeString);

	free(typeString);
	free(expectedString);
	exit(EXIT_FAILURE);
}

void arrayDimsError(int dims, int expected)
{
	fprintf(stderr, "\033[1mscalpa: \033[31merror:\033[0m\033[39m wrong dimensions at token \033[1m%s\033[0m at line %d: expected dimension of \033[1m%d\033[0m but got \033[1m\033[31m%d\033[0m\033[39m\n", yytext, yylineno, expected, dims);
	exit(EXIT_FAILURE);
}

void rangeError(void)
{
	fprintf(stderr, "\033[1mscalpa: \033[31merror:\033[0m\033[39m range at line %d should be ascending and not null\n", yylineno);
	exit(EXIT_FAILURE);
}

void notYetImplementedError(char *text)
{
	fprintf(stderr, "\033[1mscalpa: \033[31merror:\033[0m\033[39m %s feature at line %d not yet implemented\n", text, yylineno);
	exit(EXIT_FAILURE);
}

void inputFileError(char *infile)
{
	fprintf(stderr, "\033[1mscalpa: \033[31musage:\033[0m\033[39m cannot open input file %s\n", infile);
	exit(EXIT_FAILURE);
}

void compilerError(const char *file, const char *function, const int line)
{
	char message[ERROR_MESSAGE_SIZE];
	snprintf(message, ERROR_MESSAGE_SIZE, "\033[1mscalpa: \033[31merror:\033[0m\033[39m file \033[1m\033[33m%s\033[0m\033[39m - function \033[1m\033[33m%s\033[0m\033[39m - line \033[1m\033[33m%d\033[0m\033[39m", file, function, line);

	perror(message);
	exit(EXIT_FAILURE);
}
