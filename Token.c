#include "Token.h"
#pragma warning(disable:4996)


int currentIndex = 0;
Node* currentNode = NULL;
Node* head = NULL;
Node* tail = NULL;

#define TOKEN_ARRAY_SIZE 100

char arr[50][22] = { "BLOCK","BEGIN","END","TYPE","IS","INTEGER","REAL","ARRAY","OF","WHEN","DO","DEFAULT","END_WHEN","FOR","END_FOR","MALLOC","SIZE_OF","FREE","UNARY","MEMORY","ASSIGNEMENT","POINTER","REL_OP","SEMICOLON","TOKEN_COLON","TOKEN_OBRACKET","TOKEN_CBRACKET","OPARENTHESIS","CPARENTHESIS","SEP_SIGNS","PLUS","MUL","DIVIDE","SUBSTRUCT","POW","VARIABLE_TYPE","ID","INT","END_OF_FILE" };


void errorHandler(eTOKENS *followArr, eTOKENS *firstArr, int size_follow, int size_first, Token *current_token)
{
	current_token = next_token();
	int flag = 1;
	int index = 0;
	int i;
	//fprintf(yyout, "----Semantic Error at line: %d\n", current_token->lineNumber);
	for (index = 0; index < size_follow; index++)
	{
		while (current_token && flag)
		{
			if (current_token->kind != followArr[index] && current_token->kind != TOKEN_END_OF_FILE)
			{
				current_token = next_token();
			}
			else
			{
				flag = 0;
			}
		}

		//index = 0;
	}
	if (current_token->kind == TOKEN_END_OF_FILE)
	{
		exit(0);
	}
	else
	{
		current_token = back_token();
	}
}

void create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine)
{
	// case 1: there is still no tokens in the storage.
	if (currentNode == NULL)
	{

		currentNode = (Node*)malloc(sizeof(Node));

		head = currentNode;

		if (currentNode == NULL)
		{
			fprintf(yyout, "\nUnable to allocate memory! \n");
			exit(0);
		}
		currentNode->tokensArray = (Token*)malloc(sizeof(Token)*TOKEN_ARRAY_SIZE);
		if (currentNode->tokensArray == NULL)
		{
			fprintf(yyout, "\nUnable to allocate memory! \n");
			exit(0);
		}
		currentNode->prev = NULL;
		currentNode->next = NULL;
	}

	// case 2: at least one token exsits in the storage.
	else
	{
		// the array (the current node) is full, need to allocate a new node
		if (currentIndex == TOKEN_ARRAY_SIZE - 1)
		{
			currentIndex = 0;
			currentNode->next = (Node*)malloc(sizeof(Node));

			if (currentNode == NULL)
			{
				fprintf(yyout, "\nUnable to allocate memory! \n");
				exit(0);
			}
			currentNode->next->prev = currentNode;
			currentNode = currentNode->next;
			currentNode->tokensArray = (Token*)malloc(sizeof(Token)*TOKEN_ARRAY_SIZE);

			if (currentNode->tokensArray == NULL)
			{
				fprintf(yyout, "\nUnable to allocate memory! \n");
				exit(0);
			}
			currentNode->next = NULL;
		}
		else
		{
			currentIndex++;
		}
		if (kind == TOKEN_END_OF_FILE)
		{
			tail = currentNode;
		}
	}
	currentNode->tokensArray[currentIndex].kind = kind;
	currentNode->tokensArray[currentIndex].lexeme = (char*)malloc(sizeof(char)*(strlen(lexeme) + 1));
	strcpy(currentNode->tokensArray[currentIndex].lexeme, lexeme);
	currentNode->tokensArray[currentIndex].lineNumber = numOfLine;
	//fprintf(yyout, "Token of type %s was found at line: %d, lexeme: %s.\n", arr[kind], numOfLine, lexeme);
}

Token *back_token()
{
	if (!currentNode)
	{
		return NULL;
	}
	if (currentIndex == 0 && currentNode == head)
	{
		return NULL;
	}

	if (currentIndex == 0)
	{
		currentIndex = 99;
		currentNode = currentNode->prev;
		if (!currentNode)
		{
			return NULL;
		}
		else
		{
			return &(currentNode->tokensArray[currentIndex]);
		}
	}
	return &(currentNode->tokensArray[--currentIndex]);
}


Token *next_token()
{
	if (!currentNode)
	{
		return NULL;
	}

	if (currentNode == tail && currentIndex == 99)
	{
		return NULL;
	}

	if (currentIndex == 99)
	{
		currentIndex = 0;
		currentNode = currentNode->next;
		if (!currentNode)
		{
			return NULL;
		}
		else
		{
			return &(currentNode->tokensArray[currentIndex]);
		}
	}
	else
	{
		return &(currentNode->tokensArray[++currentIndex]);
	}

}

void printTokens()
{
	currentNode = head;
	currentIndex = 0;
	Token* temp = &(head->tokensArray[currentIndex]);

	int i = 0;

	while (temp)
	{
		if (temp->lineNumber >= 0)
		{
			printf("Curr Node: ");
			printf(currentNode);
			printf(" Lexeme: ");
			printf(temp->lexeme);
			printf("\n");
			temp = next_token();
			i++;
		}
		else
		{
			temp = NULL;
		}
	}
}

Token* restartData()
{
	currentNode = head;
	currentIndex = 0;
	return &(currentNode->tokensArray[currentIndex]);
}


Token *activeToken()
{
	if (!currentNode)
	{
		return NULL;
	}

	return &(currentNode->tokensArray[currentIndex]);
}
