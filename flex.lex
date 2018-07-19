%{
#include "Token.h"
#include "Parser.h"
int lineNumber=1;
#pragma warning(disable:4996)


%}

%name lex

LETTER [a-z|A-Z]
ID ({LETTER}+"_")*{LETTER}+
DIGIT [0-9]
INT [0|1-9]{DIGIT}*
REAL {INT}+"."{INT}+
COMMENT "--".*
%%

\n {lineNumber++;}

{COMMENT}
"block" {create_and_store_token(TOKEN_BLOCK,yytext,lineNumber);}
"begin" {create_and_store_token(TOKEN_BEGIN,yytext,lineNumber);}
"end" {create_and_store_token(TOKEN_END,yytext,lineNumber);}
"type" {create_and_store_token(TOKEN_TYPE,yytext,lineNumber);}
"is" {create_and_store_token(TOKEN_IS,yytext,lineNumber);}
"integer" {create_and_store_token(TOKEN_INTEGER,yytext,lineNumber);}
"real" {create_and_store_token(TOKEN_REAL,yytext,lineNumber);}
"array" {create_and_store_token(TOKEN_ARRAY,yytext,lineNumber);}
"of" {create_and_store_token(TOKEN_OF,yytext,lineNumber);}
"when" {create_and_store_token(TOKEN_WHEN,yytext,lineNumber);}
"do" {create_and_store_token(TOKEN_DO,yytext,lineNumber);}
"default" {create_and_store_token(TOKEN_DEFAULT,yytext,lineNumber);}
"end_when" {create_and_store_token(TOKEN_END_WHEN,yytext,lineNumber);}
"for" {create_and_store_token(TOKEN_FOR,yytext,lineNumber);}
"end_for" {create_and_store_token(TOKEN_END_FOR,yytext,lineNumber);}
"malloc" {create_and_store_token(TOKEN_MALLOC,yytext,lineNumber);}
"size_of" {create_and_store_token(TOKEN_SIZE_OF,yytext,lineNumber);}
"free" {create_and_store_token(TOKEN_FREE,yytext,lineNumber);}
"++" {create_and_store_token(TOKEN_UNARY,yytext,lineNumber);}
"&" {create_and_store_token(TOKEN_MEMORY,yytext,lineNumber);}
"=" {create_and_store_token(TOKEN_ASSIGNEMENT,yytext,lineNumber);}
"^" {create_and_store_token(TOKEN_POINTER,yytext,lineNumber);}
"=="|"!="|">"|"<"|"<="|">=" {create_and_store_token(TOKEN_REL_OP,yytext,lineNumber);}
";" {create_and_store_token(TOKEN_SEMICOLON,yytext,lineNumber);}
":" {create_and_store_token(TOKEN_COLON,yytext,lineNumber);}
"[" {create_and_store_token(TOKEN_OBRACKET,yytext,lineNumber);}
"]" {create_and_store_token(TOKEN_CBRACKET,yytext,lineNumber);}
"(" {create_and_store_token(TOKEN_OPARENTHESIS,yytext,lineNumber);}
")" {create_and_store_token(TOKEN_CPARENTHESIS,yytext,lineNumber);}
","|"{"|"}"|"." {create_and_store_token(TOKEN_SEP_SIGNS,yytext,lineNumber);}
"+" {create_and_store_token(TOKEN_PLUS,yytext,lineNumber);}
"*" {create_and_store_token(TOKEN_MUL,yytext,lineNumber);}
"/" {create_and_store_token(TOKEN_DIVIDE,yytext,lineNumber);}
"-" {create_and_store_token(TOKEN_SUBSTRUCT,yytext,lineNumber);}
"**" {create_and_store_token(TOKEN_POW,yytext,lineNumber);}
{ID} {create_and_store_token(TOKEN_ID,yytext,lineNumber);}
{INT} {create_and_store_token(TOKEN_INT,yytext,lineNumber);}
{REAL} {create_and_store_token(TOKEN_REAL,yytext,lineNumber);}
<<EOF>>	{return 1;}
[ \t\r]+
. fprintf(yyout,"The character '%s' at line: {%d} does not begin with legal token\n",yytext,lineNumber);


%%

int idan=10;


void main(int argc, char* argv[])
{	
	
	yyin=fopen("C:\\temp\\test1.txt","r");
	yyout=fopen("C:\\temp\\test1_300509494_206127128_semantic.txt","w");
	if(yyin==0)
	{
		fprintf(yyout,"Can't open file!");
	}
	else
	{
		yylex();
		
		Parser();

		//printTokens();
		fclose(yyin);
		fclose(yyout);
	}

	yyrestart();

	lineNumber=1;
	

	yyin=fopen("C:\\temp\\test2.txt","r");
	yyout=fopen("C:\\temp\\test2_300509494_206127128_semantic.txt","w");
	if(yyin==0)
	{
	  fprintf(yyout,"Can't not open the file!");
	}
	else
	{
	  yylex();

	  Parser();
	  
	  fclose(yyin);
	}
	
	fclose(yyout);
	
}
