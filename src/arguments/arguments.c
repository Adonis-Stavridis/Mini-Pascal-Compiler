#include "arguments.h"

void printVersion(void)
{
	fprintf(stdout, "\033[1m\033[33mScalpa Compiler\033[0m\033[39m - created by:\n- BOUAFIA Amir\n- FILLEUL Mika\n- SAKUMA Marion\n- STAVRIDIS Adonis\n");
}

Arguments *initArguments(void)
{
	Arguments *args = (Arguments *)malloc(sizeof(Arguments));

	args->infile = NULL;
	args->outfile = NULL;
	args->tos = false;

	return args;
}

void setArgumentsInfile(Arguments *args, char *path)
{
	args->infile = malloc(strlen(path) + 1);
	if (!args->infile)
	{
		compilerError(_FILE, _FUNCTION, _LINE);
	}
	strcpy(args->infile, path);
}

void setArgumentsOutfile(Arguments *args, char *path)
{
	args->outfile = malloc(strlen(path) + 1);
	if (!args->outfile)
	{
		compilerError(_FILE, _FUNCTION, _LINE);
	}
	strcpy(args->outfile, path);
}

void setArgumentsTos(Arguments *args)
{
	args->tos = true;
}

void verifyInfileExtension(char *path)
{
	char *ext = strrchr(path, '.');
	if (!ext || ext == path)
	{
		infileError();
	}
	if (!(strcmp(ext, INFILE_EXT_P) == 0 || strcmp(ext, INFILE_EXT_SCALPA) == 0))
	{
		infileError();
	}
}

void verifyOutfileExtension(char *path)
{
	char *ext = strrchr(path, '.');
	if (!ext || ext == path)
	{
		outfileError();
	}
	if (!(strcmp(ext, OUTFILE_EXT_S) == 0))
	{
		outfileError();
	}
}

bool emptyArgumentsInfile(Arguments *args)
{
	return args->infile ? false : true;
}

void handleEmptyArgumentsOutfile(Arguments *args)
{
	if (!args->outfile)
	{
		setArgumentsOutfile(args, DEFAULT_OUTFILE);
	}
}

void createOutputDirectories(char *path)
{
	if (!path || strcmp(path, ".") == 0)
	{
		return;
	}

	char *outPath = strdup(path);
	char *dirPath = dirname(outPath);

	createOutputDirectories(dirPath);

	if (mkdir(dirPath, 0777) == -1 && errno != EEXIST)
	{
		compilerError(_FILE, _FUNCTION, _LINE);
	}

	free(outPath);
}

void printArguments(Arguments *args)
{
	fprintf(stdout, "Arguments:\n- Input file: %s\n- Output file: %s\n- Display Symbol Table: %d\n", args->infile, args->outfile, args->tos);
}

void freeArguments(Arguments *args)
{
	free(args->infile);
	free(args->outfile);
	free(args);
}

Arguments *handleArguments(int argc, char **argv)
{
	if (argc == 1)
	{
		usageError();
	}

	Arguments *args = initArguments();
	bool versionFlag = true;

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], ARG_VERSION) == 0)
		{
			printVersion();
		}
		else if (strcmp(argv[i], ARG_TOS) == 0)
		{
			setArgumentsTos(args);
			versionFlag = false;
		}
		else if (strcmp(argv[i], ARG_OUTPUT) == 0)
		{
			i++;
			if (i < argc)
			{
				verifyOutfileExtension(argv[i]);
				setArgumentsOutfile(args, argv[i]);
			}
			else
			{
				usageError();
			}
			versionFlag = false;
		}
		else if (argv[i][0] == '-')
		{
			usageError();
		}
		else
		{
			verifyInfileExtension(argv[i]);
			setArgumentsInfile(args, argv[i]);
			versionFlag = false;
		}
	}

	if (!versionFlag)
	{
		if (emptyArgumentsInfile(args))
		{
			usageError();
		}
		else
		{
			handleEmptyArgumentsOutfile(args);
		}
	}
	else
	{
		freeArguments(args);
		return NULL;
	}

	return args;
}
