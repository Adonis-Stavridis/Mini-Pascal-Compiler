%{
	#include <string.h>
	#include <stdbool.h>
	
	#include "gencode/gencode.h"
	#include "idList/idList.h"
	#include "intList/intList.h"
	#include "quad/quad.h"
	#include "rules/rules.h"
	#include "symbol/symbol.h"
	#include "symbolList/symbolList.h"
	#include "symbolTable/symbolTable.h"
	#include "types/types.h"
	
	#include "scalpa.tab.h"
%}
%option nounput noinput yylineno

%%
program										{ return PROG; }
var												{ return VAR; }
function									{ return FUN; }
of												{ return OF; }

return									  { return RTRN; }
begin											{ return BEGN; }
end												{ return END; }

unit											{ return UNIT; }
int												{ return INT; }
bool											{ return BOOL; }
array											{ return ARR; }

if												{ return IF; }
then											{ return THEN; }
else											{ return ELSE; }

while											{ return WHLE; }
do												{ return DO; }

and												{ return AND; }
or												{ return OR; }
not												{ return NOT; }
xor												{ return XOR; }

read											{ return READ; }
write											{ return WRTE; }

":="											{ return ASS; }

"<="											{ return LEQ; }
">="											{ return GEQ; }
"<>"											{ return DIFF; }

".."											{ return RANG; }

[;,:\[\].()=+\-*/^<>]			{ return yytext[0]; }
[\n\t ]										;

0|[1-9][0-9]*							{
														yylval.intVal = atoi(yytext);
														return INTG;
													}

true											{
														yylval.boolVal = true;
														return BLN;
													}

false											{
														yylval.boolVal = false;
														return BLN;
													}

\"(\\.|[^"\\])*\"					{
														yylval.strVal = strdup(yytext);
														return STR;
													}

[a-zA-Z][0-9a-zA-Z'_]*		{
														yylval.idVal = strdup(yytext);
														return ID;
													}

.													{ syntaxError(); }
