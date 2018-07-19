#ifndef TOKEN_H
#define TOKEN_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

extern FILE *yyin, *yyout;

typedef enum eTOKENS
{
	TOKEN_BLOCK,
	TOKEN_BEGIN,
	TOKEN_END,
	TOKEN_TYPE,
	TOKEN_IS,
	TOKEN_INTEGER,
	TOKEN_REAL,
	TOKEN_ARRAY,
	TOKEN_OF,
	TOKEN_WHEN,
	TOKEN_DO,
	TOKEN_DEFAULT,
	TOKEN_END_WHEN,
	TOKEN_FOR,
	TOKEN_END_FOR,
	TOKEN_MALLOC,
	TOKEN_SIZE_OF,
	TOKEN_FREE,
	TOKEN_UNARY,
	TOKEN_MEMORY,
	TOKEN_ASSIGNEMENT,
	TOKEN_POINTER,
	TOKEN_REL_OP,
	TOKEN_SEMICOLON,
	TOKEN_COLON,
	TOKEN_OBRACKET,
	TOKEN_CBRACKET,
	TOKEN_OPARENTHESIS,
	TOKEN_CPARENTHESIS,
	TOKEN_SEP_SIGNS,
	TOKEN_PLUS,
	TOKEN_MUL,
	TOKEN_DIVIDE,
	TOKEN_SUBSTRUCT,
	TOKEN_POW,
	TOKEN_VARIABLE_TYPE,
	TOKEN_ID,
	TOKEN_INT,
	TOKEN_END_OF_FILE,
}eTOKENS;

typedef struct Token
{
	eTOKENS kind;
	char *lexeme;
	int lineNumber;
}Token;

typedef struct Node
{
	Token *tokensArray;
	struct Node *prev;
	struct Node *next;
} Node;

void errorHandler(eTOKENS *followArr, eTOKENS *firstArr, int size_follow, int size_first, Token *current_token);
void create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine);

Token *next_token();

Token *back_token();

Token* restartData();

void printTokens();

Token *activeToken();

#endif
