%{
	#include <stdio.h>

	#include "gencode/gencode.h"
	#include "idList/idList.h"
	#include "intList/intList.h"
	#include "quad/quad.h"
	#include "rules/rules.h"
	#include "symbol/symbol.h"
	#include "symbolList/symbolList.h"
	#include "symbolTable/symbolTable.h"
	#include "types/types.h"
	
	#define YYDEBUG 1

	extern int yylineno;
	extern char *yytext;

	extern int yylex();
	extern int yyparse();

	void yyerror(char const *text);
	int power(int a, int b);
	
	extern SymbolTable *table;
	extern QuadList *quadList;

	extern FILE *yyin;
	extern FILE *yyout;
%}
%define parse.error verbose

// Types
%union
{
	int intVal;
	bool boolVal;
	char *strVal;

	char *idVal;

	IdListNode *idList;
	
	ScalpaType scalpaType;

	Symbol *symbol;

	SymbolRule symbolRule;

	TypeRule typeRule;

	ArrayRule arrayRule;

	RangeRule rangeRule;

	SymbolListNode *symbolList;
}

// Tokens
%token PROG
%token VAR
%token FUN
%token OF

%token RTRN
%token BEGN
%token END

%token UNIT
%token INT
%token BOOL
%token ARR

%token IF
%token THEN
%token ELSE

%token WHLE
%token DO

%token AND
%token OR
%token NOT
%token XOR

%token READ
%token WRTE

%token ASS

%token DIFF
%token LEQ
%token GEQ

%token RANG

%token <intVal> INTG
%token <boolVal> BLN
%token <strVal> STR

%token <idVal> ID

// Priority
%left XOR OR
%left AND
%left '=' '<' '>' LEQ GEQ DIFF
%left '+' '-'
%left '*' '/'
%precedence UNRY
%right '^'
%right THEN ELSE

%type <idList> identlist
%type <typeRule> typename
%type <scalpaType> atomictype
%type <arrayRule> arraytype
%type <rangeRule> rangelist
%type <intVal> rangeintgr
%type <symbolRule> lvalue
%type <symbolList> exprlist
%type <symbolRule> strexpr
%type <symbolRule> expr
%type <symbol> cte

%start program

%%
program
	: PROG ID varsdecllist fundecllist instr
		{
			free($2);
		}
	;

varsdecllist
	: %empty
	| varsdecl
	| varsdecl ';' varsdecllist
	;

varsdecl
	: VAR identlist ':' typename
		{
			IdListNode *idList = $2;
			while(idList)
			{
				Symbol *symbol = NULL;
				if ($4.type == ARR_TYPE)
				{
					symbol = newArr(getIdData(idList), NO_SYMBOL);
					ArrayData *arrayData = initArrayData($4.array.arrayType, $4.array.range.minList, $4.array.range.maxList, $4.array.range.dims);
					setSymbolDataArray(symbol, arrayData);
				}
				else
				{
					symbol = newVar(getIdData(idList), $4.type, NO_SYMBOL);
				}
				addSymbol(table,symbol);
				idList = idList->next;
			}
			if ($4.type == ARR_TYPE)
			{
				freeIntList($4.array.range.minList);
				freeIntList($4.array.range.maxList);
			}
			freeIdList($2);
		}
	;

identlist
	: ID
		{
			$$ = initIdList();
			$$ = addIdListNode($$, $1);
		}
	| identlist ',' ID
		{
			$$ = addIdListNode($1, $3);
		}
	;

typename
	: atomictype
		{
			$$.type = $1;
		}
	| arraytype
		{
			$$.type = ARR_TYPE;
			$$.array = $1;
		}
	;

atomictype
	: UNIT
		{ $$ = UNIT_TYPE; }
	| BOOL
		{ $$ = BOOL_TYPE; }
	| INT
		{ $$ = INT_TYPE; }
	;

arraytype
	: ARR '[' rangelist ']' OF atomictype
		{
			$$.range = $3;
			$$.arrayType = $6;
		}
	;

rangelist
	: rangeintgr RANG rangeintgr
		{
			if ($1 >= $3)
			{
				rangeError();
			}
			$$.minList = initIntList();
			$$.maxList = initIntList();
			$$.minList = addIntListNode($$.minList, $1);
			$$.maxList = addIntListNode($$.maxList, $3);
			$$.dims = 1;
		}
	| rangelist ',' rangeintgr RANG rangeintgr
		{
			if ($3 >= $5)
			{
				rangeError();
			}
			$$.minList = addIntListNode($$.minList, $3);
			$$.maxList = addIntListNode($$.maxList, $5);
			$$.dims = $1.dims + 1;
		}
	;

rangeintgr
	: INTG
		{ $$ = $1; }
	| '-' INTG %prec UNRY
		{ $$ = -$2;	}
	;

fundecllist
	: %empty
	| FUN ID '('
		{
			free($2);
			notYetImplementedError("function");
		}
	;

instr
	: IF ifmarker THEN instr
		{
			Quad *quad = initQuad(IF_END_OP, NULL, NULL, NULL);
			addQuad(quadList, quad);
		}
	| IF ifmarker THEN instr elsemarker instr
		{
			Quad *quad = initQuad(IF_END_OP, NULL, NULL, NULL);
			addQuad(quadList, quad);
		}
	| WHLE exprmarker whilemarker DO instr
		{
			Quad *quad = initQuad(WHLE_END_OP, NULL, NULL, NULL);
			addQuad(quadList, quad);
		}
	| lvalue ASS expr
		{
			if ($1.symbol->dataType != ARR_TYPE)
			{
				setInitialized($1.symbol);
				verifySymbolType($3.symbol, $1.symbol->dataType);
				Quad *quad = initQuad(ASS_OP, $3.symbol, NULL, $1.symbol);
				addQuad(quadList, quad);
			}
			else
			{
				verifySymbolType($3.symbol, $1.symbol->data.arrVal->arrayType);
				Quad *quad = initQuad(ARR_ASS_OP, $3.symbol, NULL, $1.symbol);
				setQuadSymbolList(quad, $1.symbolList);
				addQuad(quadList, quad);
			}
		}
	| RTRN expr
		{	
			notYetImplementedError("return in function");
		}
	| RTRN
		{
			notYetImplementedError("return in function");
		}
	| ID '(' exprlist ')'
		{
			free($1);
			notYetImplementedError("function");
		}
	| ID '(' ')'
		{
			free($1);
			notYetImplementedError("function");
		}
	| BEGN sequence END
		{ }
	| BEGN END
		{ }
	| READ lvalue
		{
			if ($2.symbol->dataType != ARR_TYPE)
			{
				setInitialized($2.symbol);
				Quad *quad = initQuad(READ_OP, NULL, NULL, $2.symbol);
				addQuad(quadList, quad);
			}
			else
			{
				Quad *quad = initQuad(ARR_R_OP, NULL, NULL, $2.symbol);
				setQuadSymbolList(quad, $2.symbolList);
				addQuad(quadList, quad);
			}
		}
	| WRTE strexpr
		{
			verifyInitialized($2.symbol);
			Quad *quad = initQuad(WRTE_OP, $2.symbol, NULL, NULL);
			addQuad(quadList, quad);
		}
	;

ifmarker
	:	expr
		{
			verifySymbolType($1.symbol, BOOL_TYPE);
			verifyInitialized($1.symbol);
			Quad *quad = initQuad(IF_OP, $1.symbol, NULL, NULL);
			addQuad(quadList, quad);
		}
	;

elsemarker
	: ELSE
		{
			Quad *quad = initQuad(ELSE_OP, NULL, NULL, NULL);
			addQuad(quadList, quad);
		}
	;

exprmarker
	: %empty
		{
			Quad *quad = initQuad(EXPR_M_OP, NULL, NULL, NULL);
			addQuad(quadList, quad);
		}
	;

whilemarker
	: expr
		{
			verifySymbolType($1.symbol, BOOL_TYPE);
			verifyInitialized($1.symbol);
			Quad *quad = initQuad(WHLE_OP, $1.symbol, NULL, NULL);
			addQuad(quadList, quad);
		}
	;

sequence
	: instr ';' sequence
	| instr ';'
	| instr
	;

lvalue
	: ID
		{
			Symbol *symbol = lookupSymbol(table, $1);
			verifySymbolType2(symbol, INT_TYPE, BOOL_TYPE);
			$$.symbol = symbol;
			free($1);
		}
	| ID '[' exprlist ']'
		{
			Symbol *symbol = lookupSymbol(table, $1);
			verifySymbolType(symbol, ARR_TYPE);
			verifySymbolListLength($3, symbol);
			$$.symbol = symbol;
			$$.symbolList = $3;
			free($1);
		}
	;

exprlist
	: expr
		{
			verifySymbolType($1.symbol, INT_TYPE);
			$$ = initSymbolList();
			$$ = addSymbolListNode($$, $1.symbol);
		}
	| exprlist ',' expr
		{
			$$ = addSymbolListNode($1, $3.symbol);
		}
	;

strexpr
	: expr
		{ $$ = $1; }
	| STR
		{
			Symbol *symbol = newTemp(STR_TYPE);
			addSymbol(table,symbol);
			setSymbolDataString(symbol, $1);
			$$.symbol = symbol;
		}
	;

expr
	: cte
		{ $$.symbol = $1; }
	| '(' expr ')'
		{ $$.symbol = $2.symbol; }
	| expr '+' expr
		{
			verifySymbolType($1.symbol, INT_TYPE);
			verifySymbolType($3.symbol, INT_TYPE);
			Symbol *symbol = newTemp(INT_TYPE);
			setInitialized(symbol);
			addSymbol(table,symbol);
			Quad *quad = initQuad(ADD_OP, $1.symbol, $3.symbol, symbol);
			addQuad(quadList, quad);
			$$.symbol = symbol;
		}
	| expr '-' expr
		{
			verifySymbolType($1.symbol, INT_TYPE);
			verifySymbolType($3.symbol, INT_TYPE);
			Symbol *symbol = newTemp(INT_TYPE);
			setInitialized(symbol);
			addSymbol(table,symbol);
			Quad *quad = initQuad(SUB_OP, $1.symbol, $3.symbol, symbol);
			addQuad(quadList, quad);
			$$.symbol = symbol;
		}
	| '-' expr %prec UNRY
		{
			Symbol *symbol = newTemp(INT_TYPE);
			setInitialized(symbol);
			addSymbol(table,symbol);
			Quad *quad = initQuad(SUB_OP, $2.symbol, NULL, symbol);
			addQuad(quadList, quad);
			$$.symbol = symbol;
		}
	| expr '*' expr
		{
			verifySymbolType($1.symbol, INT_TYPE);
			verifySymbolType($3.symbol, INT_TYPE);
			Symbol *symbol = newTemp(INT_TYPE);
			setInitialized(symbol);
			addSymbol(table,symbol);
			Quad *quad = initQuad(MUL_OP, $1.symbol, $3.symbol, symbol);
			addQuad(quadList, quad);
			$$.symbol = symbol;
		}
	| expr '/' expr
		{
			verifySymbolType($1.symbol, INT_TYPE);
			verifySymbolType($3.symbol, INT_TYPE);
			Symbol *symbol = newTemp(INT_TYPE);
			setInitialized(symbol);
			addSymbol(table,symbol);
			Quad *quad = initQuad(DIV_OP, $1.symbol, $3.symbol, symbol);
			addQuad(quadList, quad);
			$$.symbol = symbol;
		}
	| expr '^' expr
		{
			verifySymbolType($1.symbol, INT_TYPE);
			verifySymbolType($3.symbol, INT_TYPE);
			Symbol *symbol = newTemp(INT_TYPE);
			setInitialized(symbol);
			addSymbol(table,symbol);
			Quad *quad = initQuad(POW_OP, $1.symbol, $3.symbol, symbol);
			addQuad(quadList, quad);
			$$.symbol = symbol;
		}
	| expr '<' expr
		{
			verifySymbolType($1.symbol, INT_TYPE);
			verifySymbolType($3.symbol, INT_TYPE);
			Symbol *symbol = newTemp(BOOL_TYPE);
			setInitialized(symbol);
			addSymbol(table,symbol);
			Quad *quad = initQuad(LTH_OP, $1.symbol, $3.symbol, symbol);
			addQuad(quadList, quad);
			$$.symbol = symbol;
		}
	| expr LEQ expr
		{
			verifySymbolType($1.symbol, INT_TYPE);
			verifySymbolType($3.symbol, INT_TYPE);
			Symbol *symbol = newTemp(BOOL_TYPE);
			setInitialized(symbol);
			addSymbol(table,symbol);
			Quad *quad = initQuad(LEQ_OP, $1.symbol, $3.symbol, symbol);
			addQuad(quadList, quad);
			$$.symbol = symbol;
		}
	| expr '>' expr
		{
			verifySymbolType($1.symbol, INT_TYPE);
			verifySymbolType($3.symbol, INT_TYPE);
			Symbol *symbol = newTemp(BOOL_TYPE);
			setInitialized(symbol);
			addSymbol(table,symbol);
			Quad *quad = initQuad(GTH_OP, $1.symbol, $3.symbol, symbol);
			addQuad(quadList, quad);
			$$.symbol = symbol;
		}
	| expr GEQ expr
		{
			verifySymbolType($1.symbol, INT_TYPE);
			verifySymbolType($3.symbol, INT_TYPE);
			Symbol *symbol = newTemp(BOOL_TYPE);
			setInitialized(symbol);
			addSymbol(table,symbol);
			Quad *quad = initQuad(GEQ_OP, $1.symbol, $3.symbol, symbol);
			addQuad(quadList, quad);
			$$.symbol = symbol;
		}
	| expr '=' expr
		{
			verifySymbolType2($1.symbol, INT_TYPE, BOOL_TYPE);
			verifySymbolType2($3.symbol, INT_TYPE, BOOL_TYPE);
			verifySymbolType($1.symbol, $3.symbol->dataType);
			Symbol *symbol = newTemp(BOOL_TYPE);
			setInitialized(symbol);
			addSymbol(table,symbol);
			Quad *quad = initQuad(EQ_OP, $1.symbol, $3.symbol, symbol);
			addQuad(quadList, quad);
			$$.symbol = symbol;
		}
	| expr DIFF expr
		{
			verifySymbolType2($1.symbol, INT_TYPE, BOOL_TYPE);
			verifySymbolType2($3.symbol, INT_TYPE, BOOL_TYPE);
			verifySymbolType($1.symbol, $3.symbol->dataType);
			Symbol *symbol = newTemp(BOOL_TYPE);
			setInitialized(symbol);
			addSymbol(table,symbol);
			Quad *quad = initQuad(DIFF_OP, $1.symbol, $3.symbol, symbol);
			addQuad(quadList, quad);
			$$.symbol = symbol;
		}
	| expr AND expr
		{
			verifySymbolType($1.symbol, BOOL_TYPE);
			verifySymbolType($3.symbol, BOOL_TYPE);
			Symbol *symbol = newTemp(BOOL_TYPE);
			setInitialized(symbol);
			addSymbol(table,symbol);
			Quad *quad = initQuad(AND_OP, $1.symbol, $3.symbol, symbol);
			addQuad(quadList, quad);
			$$.symbol = symbol;
		}
	| expr OR expr
		{
			verifySymbolType($1.symbol, BOOL_TYPE);
			verifySymbolType($3.symbol, BOOL_TYPE);
			Symbol *symbol = newTemp(BOOL_TYPE);
			setInitialized(symbol);
			addSymbol(table,symbol);
			Quad *quad = initQuad(OR_OP, $1.symbol, $3.symbol, symbol);
			addQuad(quadList, quad);
			$$.symbol = symbol;
		}
	| expr XOR expr
		{
			verifySymbolType($1.symbol, BOOL_TYPE);
			verifySymbolType($3.symbol, BOOL_TYPE);
			Symbol *symbol = newTemp(BOOL_TYPE);
			setInitialized(symbol);
			addSymbol(table,symbol);
			Quad *quad = initQuad(XOR_OP, $1.symbol, $3.symbol, symbol);
			addQuad(quadList, quad);
			$$.symbol = symbol;
		}
	| NOT expr %prec UNRY
		{
			verifySymbolType($2.symbol, BOOL_TYPE);
			Symbol *symbol = newTemp(BOOL_TYPE);
			setInitialized(symbol);
			addSymbol(table,symbol);
			Quad *quad = initQuad(NOT_OP, $2.symbol, NULL, symbol);
			addQuad(quadList, quad);
			$$.symbol = symbol;
		}
	| ID '(' exprlist ')'
		{
			free($1);
			notYetImplementedError("function");
		}
	| ID '(' ')'
		{
			free($1);
			notYetImplementedError("function");
		}
	| ID '[' exprlist ']'
		{
			Symbol *symbol = lookupSymbol(table, $1);
			verifySymbolType(symbol, ARR_TYPE);
			verifySymbolListLength($3, symbol);
			Symbol *tempsymbol = newTemp(symbol->data.arrVal->arrayType);
			setInitialized(tempsymbol);
			addSymbol(table,tempsymbol);
			Quad *quad = initQuad(ARR_OP, symbol, NULL, tempsymbol);
			setQuadSymbolList(quad, $3);
			addQuad(quadList, quad);
			$$.symbol = tempsymbol;
			$$.symbolList = $3;
			free($1);
		}
	| ID
		{
			Symbol *symbol = lookupSymbol(table, $1);
			verifyInitialized(symbol);
			$$.symbol = symbol;
			free($1);
		}
	;

cte
	: INTG
		{
			Symbol *symbol = newTemp(INT_TYPE);
			addSymbol(table,symbol);
			setSymbolDataInt(symbol, $1);
			$$ = symbol;
		}
	| BLN
		{
			Symbol *symbol = newTemp(BOOL_TYPE);
			addSymbol(table,symbol);
			setSymbolDataBool(symbol, $1);
			$$ = symbol;
		}
	;

%%

void yyerror(const char *text)
{
	fprintf(stderr, "\033[1mscalpa: \033[31m%s:\033[0m\033[39m unexpected token \033[1m%s\033[0m at line %d\n", text, yytext, yylineno);
}
