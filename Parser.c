#include "Parser.h"
#include "Token.h"

#pragma warning(disable:4996)

#define NULL 0

extern char *TokenName[];
SymbolTable* currentTable;


int CheckTypeByID(char* lexeme)
{
	SymbolTableEntry* entry = symboltable_find(currentTable, lexeme);
	if (entry == NULL)
	{
		return TYPE_UNDEFINED;
	}
	else
	{
		return entry->type;
	}
}


bool match(eTOKENS t, int size_follow, eTOKENS followArr[], int size_first, eTOKENS firstArr[])
{
	Token *current_token;
	current_token = next_token();

	if (current_token->kind != t)
	{
		errorHandler(followArr, firstArr, size_follow, size_first, current_token);
		return false;
	}
	

	return true;
}


void Parser()
{
	printf("Parser\n");
	Parse_PROGRAM();
}

void Parse_PROGRAM()
{
	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	firstArr[0] = TOKEN_BLOCK;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	followArr[0] = TOKEN_END_OF_FILE;

	Token *current_token = restartData();

	if (!current_token)
	{
		return;
	}
	switch (current_token->kind)
	{
		case TOKEN_BLOCK:
			//fprintf(yyout, "{PROGRAM --> BLOCK}\n");
			//printf("{PROGRAM --> BLOCK}\n");
			Parse_BLOCK();
			break;
		default:
			errorHandler(followArr, firstArr, 1, 1, current_token);
			break;
	}
}

void Parse_BLOCK()
{
	
	SymbolTable* innerTable;
	SymbolTable* newScopeTable;
	
	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	firstArr[0] = TOKEN_BLOCK;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 2);
	followArr[0] = TOKEN_SEMICOLON;
	followArr[1] = TOKEN_END_OF_FILE;

	Token *current_token = next_token();
	current_token = back_token();

	if (!current_token){
		return;
	}
	switch (current_token->kind)
	{
		case TOKEN_BLOCK:
			newScopeTable = (SymbolTable*)malloc(sizeof(SymbolTable));
			symboltable_init(newScopeTable, currentTable);
			currentTable = newScopeTable;

			
			
			//fprintf(yyout, "{Start - Sematic Analysis!}\n");
			//fprintf(yyout, "{BLOCK --> block DEFINITIONS; begin COMMANDS; end}\n");
			fprintf(yyout, "{Start - Sematic Analysis!}\n");
			//printf("{BLOCK --> block DEFINITIONS; begin COMMANDS; end}\n");
			Parse_DEFINITIONS();
			match(TOKEN_SEMICOLON, 2, followArr, 1, firstArr);
			match(TOKEN_BEGIN, 2, followArr, 1, firstArr);
			Parse_COMMANDS();
			match(TOKEN_SEMICOLON, 2, followArr, 1, firstArr);
			match(TOKEN_END, 2, followArr, 1, firstArr);
			//fprintf(yyout, "{End - Sematic Analysis!}\n");
			fprintf(yyout, "{End - Sematic Analysis!}\n");

			//What
			innerTable = currentTable;
			currentTable = innerTable->father;
			symboltable_destruct(innerTable);
			free(innerTable);

			break;
		default:
			errorHandler(followArr, firstArr, 2, 1, current_token);
			break;
	}
}

void Parse_DEFINITIONS()
{
	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 2);
	firstArr[0] = TOKEN_ID;
	firstArr[1] = TOKEN_TYPE;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	followArr[0] = TOKEN_SEMICOLON;

	Token *current_token = next_token();

	if (!current_token)
		return;
	switch (current_token->kind)
	{
		case TOKEN_TYPE:
		case TOKEN_ID:

			current_token = back_token();
			Parse_DEFINITION();
			Parse_DEFINITIONS_TAG();
			break;

		default:

			errorHandler(followArr, firstArr, 1, 2, current_token);
			break;
		}
}

void Parse_DEFINITIONS_TAG()
{

	eTOKENS *firstArr_inside;
	eTOKENS *followArr_inside;
	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 2);
	firstArr[0] = TOKEN_SEMICOLON;
	firstArr[1] = TOKEN_TYPE;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	followArr[0] = TOKEN_SEMICOLON;


	Token *current_token = next_token();
	if (!current_token)
	{
		return;
	}
	//No need for active token here
	//printf("Line number: %d\n", current_token->lineNumber);
	switch (current_token->kind)
	{

		case TOKEN_SEMICOLON:

		firstArr_inside = (eTOKENS *)malloc(sizeof(eTOKENS) * 3);
		firstArr_inside[0] = TOKEN_BEGIN;
		firstArr_inside[1] = TOKEN_ID;
		firstArr_inside[2] = TOKEN_TYPE;

		followArr_inside = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
		followArr[0] = TOKEN_SEMICOLON;

		current_token = next_token();
		switch (current_token->kind)
		{

			case TOKEN_BEGIN:

				//fprintf(yyout, "{DEFINITION_TAG --> NULL}\n");
				//printf( "{DEFINITION_TAG --> NULL}\n");
				current_token = back_token();
				current_token = back_token();
				break;

			case TOKEN_ID:
			case TOKEN_TYPE:

				//fprintf(yyout, "{DEFINITION_TAG -->; DEFINITION  DEFINITION_TAG}\n");
				//printf("{DEFINITION_TAG -->; DEFINITION  DEFINITION_TAG}\n");
				current_token = back_token();
				Parse_DEFINITION();
				Parse_DEFINITIONS_TAG();
				break;

			default:
				errorHandler(followArr_inside, firstArr_inside, 1, 2, current_token);
				break;
		}

		break;
	default:
		errorHandler(followArr, firstArr, 1, 2, current_token);
		break;
	}
}

void Parse_DEFINITION()
{
	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 2);
	firstArr[0] = TOKEN_ID;
	firstArr[1] = TOKEN_TYPE;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	followArr[0] = TOKEN_SEMICOLON;

	Token *current_token = next_token();

	if (!current_token)
		return;
	printf("Line number is %d and token is %s\n", current_token->lineNumber, current_token->lexeme);
	switch (current_token->kind)
	{

		case TOKEN_ID:

			//fprintf(yyout, "{DEFINITION-->VAR_DEFINITION}\n");
			//printf("{DEFINITION-->VAR_DEFINITION}\n");
			current_token = back_token();
			Parse_VAR_DEFINITION();
			break;

		case TOKEN_TYPE:

			//fprintf(yyout, "{DEFINITION-->TYPE_DEFINITION}\n");
			//printf("{DEFINITION-->TYPE_DEFINITION}\n");
			current_token = back_token();
			Parse_TYPE_DEFENITION();
			break;

		default:

			errorHandler(followArr, firstArr, 1, 2, current_token);
			break;
	}
}

void Parse_VAR_DEFINITION()
{
	int type = 0;
	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	firstArr[0] = TOKEN_ID;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	followArr[0] = TOKEN_SEMICOLON;

	Token *current_token = next_token();
	if (!current_token)
	{
		return;
	}
	
	SymbolTableEntry* entry;

	switch (current_token->kind)
	{
		case TOKEN_ID:
			//fprintf(yyout, "{VAR_DEFINITION-->id : VAR_DEFINITION_TAG}\n");
			//printf("{VAR_DEFINITION-->id : VAR_DEFINITION_TAG}\n");
			 entry = (SymbolTableEntry*)malloc(sizeof(SymbolTableEntry));
			if (symboltable_lookup(currentTable, current_token->lexeme) == NULL)
			{
				
				entry->type = type;
				symboltable_insert(currentTable, current_token->lexeme, entry);
			}
			else
			{
				fprintf(yyout,"--Error: line %d duplicated declaration of %s \n",current_token->lineNumber,current_token->lexeme);
			}

			match(TOKEN_COLON, 1, followArr, 1, firstArr);

			entry->type=Parse_VAR_DEFINITION_TAG(entry);
			if (entry->type == 0) {
				fprintf(yyout,"--Error: line %d type %s is not defined\n",current_token->lineNumber ,current_token->lexeme);
			}

			break;
		default:
			errorHandler(followArr, firstArr, 1, 1, current_token);
			break;
	}
}

int Parse_VAR_DEFINITION_TAG(SymbolTableEntry* passedEntry)
{

	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 3);
	firstArr[0] = TOKEN_ID;
	firstArr[1] = TOKEN_INTEGER;
	firstArr[2] = TOKEN_REAL;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	followArr[0] = TOKEN_SEMICOLON;

	Token *current_token = next_token();
	if (!current_token)
	{
		return 0;
	}
	switch (current_token->kind)
	{
		case TOKEN_ID:
			return CheckTypeByID(current_token->lexeme);
			break;

		case TOKEN_INTEGER:
		case TOKEN_REAL:

			//fprintf(yyout, "{VAR_DEFINITION_TAG--> BASIC_TYPE}\n");
			//printf("{VAR_DEFINITION_TAG--> BASIC_TYPE}\n");


			current_token = back_token();
			return Parse_BASIC_TYPE(passedEntry);

		default:
			errorHandler(followArr, firstArr, 1, 3, current_token);
			return TYPE_UNDEFINED;
			break;
	}
}

void Parse_TYPE_DEFENITION()
{
	int typeHolderBeforeEnteringSymbolTable = 0;
	SymbolTableEntry* entry;
	Token* curr;
	
	//Token* active;
	
	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	firstArr[0] = TOKEN_TYPE;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	followArr[0] = TOKEN_SEMICOLON;

	Token *current_token = next_token();
	if (!current_token)
		return;
	switch (current_token->kind)
	{
		case TOKEN_TYPE:
			match(TOKEN_ID, 1, followArr, 1, firstArr);
			
			current_token = next_token();
			current_token = back_token();
			
			entry = (SymbolTableEntry*)malloc(sizeof(SymbolTableEntry));

			if (symboltable_find(currentTable, current_token->lexeme) != NULL) {
				fprintf(yyout, " -------- Semantic Error, line %d: ID %s, Already defined in current scope!\n", current_token->lineNumber, current_token->lexeme);
			}
			else
			{
				
				entry->type = typeHolderBeforeEnteringSymbolTable;

				symboltable_insert(currentTable, current_token->lexeme, entry);
			}
			
			match(TOKEN_IS, 1, followArr,1, firstArr);
			
			current_token = next_token();
			current_token = back_token();

			typeHolderBeforeEnteringSymbolTable = Parse_TYPE_INDICATOR();
			if (typeHolderBeforeEnteringSymbolTable == TYPE_UNDEFINED) {
				fprintf(yyout,"At line %d Parser Error - There is not a type like that",current_token->lineNumber);
			}
			else {
				//symboltable_find(currentTable, current_token->lexeme)->type = typeHolderBeforeEnteringSymbolTable;
				entry->type = typeHolderBeforeEnteringSymbolTable;
				entry->userDefined = 1;
			}

	
		


			//current_token = activeToken();
			//current_token = next_token();
			
			//Already defined
			
			break;

		default:

			errorHandler(followArr, firstArr, 1, 1, current_token);
			break;
	}
}

int Parse_TYPE_INDICATOR()
{

	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 4);
	firstArr[0] = TOKEN_INTEGER;
	firstArr[1] = TOKEN_REAL;
	firstArr[2] = TOKEN_ARRAY;
	firstArr[3] = TOKEN_POINTER;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	followArr[0] = TOKEN_SEMICOLON;

	Token *current_token = next_token();
	if (!current_token){
		return 0;
	}
	switch (current_token->kind)
	{
		case TOKEN_INTEGER:
		case TOKEN_REAL:
			current_token = back_token();
			return Parse_BASIC_TYPE(0);
		case TOKEN_ARRAY:

			current_token = back_token();
			return Parse_ARRAY_TYPE();
		case TOKEN_POINTER:

			current_token = back_token();
			return Parse_POINTER_TYPE();
		default:
			
			errorHandler(followArr, firstArr, 1, 4, current_token);
			return TYPE_UNDEFINED;
	}
}

int Parse_BASIC_TYPE(SymbolTableEntry* passedEntry)
{
	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 2);
	firstArr[0] = TOKEN_INTEGER;
	firstArr[1] = TOKEN_REAL;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	followArr[0] = TOKEN_SEMICOLON;

	Token *current_token = next_token();
	
	if (!current_token){
		return 0;
	}
	switch (current_token->kind)
	{
		case TOKEN_INTEGER:
			if (passedEntry) {
				passedEntry->userDefined = 1;
			}
			return TYPE_INT;
		case TOKEN_REAL:
			if (passedEntry) {
				passedEntry->userDefined = 1;
			}
			return TYPE_REAL;
		default:
			errorHandler(followArr, firstArr, 1, 2, current_token);
			return TYPE_UNDEFINED;
	}
}

int Parse_ARRAY_TYPE()
{
	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	firstArr[0] = TOKEN_ARRAY;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	followArr[0] = TOKEN_SEMICOLON;

	Token *current_token = next_token();
	if (!current_token){
		return 0;
	}
	switch (current_token->kind)
	{
		case TOKEN_ARRAY:
		//	fprintf(yyout, "{ARRAY_TYPE --> array [SIZE] of BASIC_TYPE}\n");
			//printf("{ARRAY_TYPE --> array [SIZE] of BASIC_TYPE}\n");
			match(TOKEN_OBRACKET, 1, followArr, 1, firstArr);
			Parse_SIZE();
			match(TOKEN_CBRACKET, 1, followArr, 1, firstArr);
			match(TOKEN_OF, 1, followArr, 1, firstArr);
			return TYPE_ARRAY + Parse_BASIC_TYPE(0);
		default:
			errorHandler(followArr, firstArr, 1, 1, current_token);
			return TYPE_UNDEFINED;
	}
}

int Parse_POINTER_TYPE()
{

	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	firstArr[0] = TOKEN_POINTER;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	followArr[0] = TOKEN_SEMICOLON;

	Token *current_token = next_token();
	if (!current_token)
		return 0;
	switch (current_token->kind)
	{

	case TOKEN_POINTER:

		//fprintf(yyout, "{POINTER_TYPE --> ^ VAR_DEFINITION_TAG}\n");
		//printf("{POINTER_TYPE --> ^ VAR_DEFINITION_TAG}\n");
		return TYPE_POINTER + Parse_VAR_DEFINITION_TAG(0);
		break;

	default:

		errorHandler(followArr, firstArr, 1, 1, current_token);
		return TYPE_UNDEFINED;
		break;
	}
}

void Parse_SIZE()
{

	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	firstArr[0] = TOKEN_INT;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	followArr[0] = TOKEN_CBRACKET;

	Token *current_token = next_token();
	if (!current_token)
		return;
	switch (current_token->kind)
	{

	case TOKEN_INT:

		break;

	default:

		errorHandler(followArr, firstArr, 1, 1, current_token);
		break;
	}
}

void Parse_COMMANDS()
{

	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 5);
	firstArr[0] = TOKEN_ID;
	firstArr[1] = TOKEN_WHEN;
	firstArr[2] = TOKEN_FOR;
	firstArr[3] = TOKEN_FREE;
	firstArr[4] = TOKEN_BLOCK;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 2);
	followArr[0] = TOKEN_SEMICOLON;

	Token *current_token = next_token();
	if (!current_token)
		return;
	switch (current_token->kind)
	{

	case TOKEN_ID:
	case TOKEN_WHEN:
	case TOKEN_FOR:
	case TOKEN_FREE:
	case TOKEN_BLOCK:

		//fprintf(yyout, "{COMMANDS --> COMMAND COMMANDS_TAG}\n");
		//printf("{COMMANDS --> COMMAND COMMANDS_TAG}\n");

		current_token = back_token();
		Parse_COMMAND();
		Parse_COMMANDS_TAG();
		break;

	default:

		errorHandler(followArr, firstArr, 1, 5, current_token);
		break;
	}
}

void Parse_COMMANDS_TAG()
{
	eTOKENS *firstArr_inside;
	eTOKENS *followArr_inside;
	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	firstArr[0] = TOKEN_SEMICOLON;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	followArr[0] = TOKEN_SEMICOLON;

	Token *current_token = next_token();
	if (!current_token)
		return;
	switch (current_token->kind)
	{
		case TOKEN_SEMICOLON:

			firstArr_inside = (eTOKENS *)malloc(sizeof(eTOKENS) * 9);
			firstArr_inside[0] = TOKEN_END;
			firstArr_inside[1] = TOKEN_DEFAULT;
			firstArr_inside[2] = TOKEN_END_WHEN;
			firstArr_inside[3] = TOKEN_END_FOR;
			firstArr_inside[4] = TOKEN_ID;
			firstArr_inside[5] = TOKEN_WHEN;
			firstArr_inside[6] = TOKEN_FOR;
			firstArr_inside[7] = TOKEN_FREE;
			firstArr_inside[8] = TOKEN_BLOCK;

			followArr_inside = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
			followArr_inside[0] = TOKEN_SEMICOLON;

			current_token = next_token();
			switch (current_token->kind)
			{
				case TOKEN_END:
				case TOKEN_DEFAULT:
				case TOKEN_END_WHEN:
				case TOKEN_END_FOR:

					current_token = back_token();
					current_token = back_token();
					break;

				case TOKEN_ID:
				case TOKEN_WHEN:
				case TOKEN_FOR:
				case TOKEN_FREE:
				case TOKEN_BLOCK:



					current_token = back_token();
					Parse_COMMAND();
					Parse_COMMANDS_TAG();
					break;

				default:

					errorHandler(followArr_inside, firstArr_inside, 1, 9, current_token);
					break;
			}
			break;
		default:
			errorHandler(followArr, firstArr, 1, 1, current_token);
			break;
	}
}

void Parse_COMMAND()
{

	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 5);
	firstArr[0] = TOKEN_ID;
	firstArr[1] = TOKEN_WHEN;
	firstArr[2] = TOKEN_FOR;
	firstArr[3] = TOKEN_FREE;
	firstArr[4] = TOKEN_BLOCK;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	followArr[0] = TOKEN_SEMICOLON;

	Token *current_token = next_token();
	if (!current_token)
		return;
	printf("Command lineNumber: %d and the token is %s \n", current_token->lineNumber, current_token->lexeme);
	switch (current_token->kind)
	{
		case TOKEN_ID:
		
			Parse_COMMAND_TAG();
			break;

		case TOKEN_WHEN:

	//		fprintf(yyout, "{COMMAND --> when (EXPRESSION rel_op EXPRESSION) do COMMANDS; default COMMANDS; end_when}\n");
			//printf("{COMMAND --> when (EXPRESSION rel_op EXPRESSION) do COMMANDS; default COMMANDS; end_when}\n");

			match(TOKEN_OPARENTHESIS, 1, followArr, 5, firstArr);
			Parse_EXPRESSION(0);
			match(TOKEN_REL_OP, 1, followArr, 5, firstArr);
			Parse_EXPRESSION(0);
			match(TOKEN_CPARENTHESIS, 1, followArr, 5, firstArr);
			match(TOKEN_DO, 1, followArr, 5, firstArr);
			Parse_COMMANDS();
			match(TOKEN_SEMICOLON, 1, followArr, 5, firstArr);
			match(TOKEN_DEFAULT, 1, followArr, 5, firstArr);
			Parse_COMMANDS();
			match(TOKEN_SEMICOLON, 1, followArr, 5, firstArr);
			match(TOKEN_END_WHEN, 1, followArr, 5, firstArr);
			break;

		case TOKEN_FOR:

	//		fprintf(yyout, "{COMMAND-->for (id = EXPRESSION; id rel_op EXPRESSION; id++)COMMANDS; end_for}\n");
			//printf("{COMMAND-->for (id = EXPRESSION; id rel_op EXPRESSION; id++)COMMANDS; end_for}\n");

			match(TOKEN_OPARENTHESIS, 1, followArr, 5, firstArr);
			match(TOKEN_ID, 1, followArr, 5, firstArr);
			match(TOKEN_ASSIGNEMENT, 1, followArr, 5, firstArr);
			Parse_EXPRESSION(0);
			match(TOKEN_SEMICOLON, 1, followArr, 5, firstArr);
			match(TOKEN_ID, 1, followArr, 5, firstArr);
			match(TOKEN_REL_OP, 1, followArr, 5, firstArr);
			Parse_EXPRESSION(0);
			match(TOKEN_SEMICOLON, 1, followArr, 5, firstArr);
			match(TOKEN_ID, 1, followArr, 5, firstArr);
			match(TOKEN_PLUS, 1, followArr, 5, firstArr);
			match(TOKEN_CPARENTHESIS, 1, followArr, 5, firstArr);
			Parse_COMMANDS();
			match(TOKEN_SEMICOLON, 1, followArr, 5, firstArr);
			match(TOKEN_END_FOR, 1, followArr, 5, firstArr);
			break;

		case TOKEN_FREE:


			match(TOKEN_OPARENTHESIS, 1, followArr, 5, firstArr);
			match(TOKEN_ID, 1, followArr, 5, firstArr);
			match(TOKEN_CPARENTHESIS, 1, followArr, 5, firstArr);
			break;

		case TOKEN_BLOCK:


			Parse_BLOCK();
			break;

		default:

			errorHandler(followArr, firstArr, 1, 5, current_token);
			break;
	}
}

void Parse_COMMAND_TAG()
{
	int leftOperand;
	int rightOperand;
	int holder;
	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 3);
	firstArr[0] = TOKEN_OBRACKET;
	firstArr[1] = TOKEN_POINTER;
	firstArr[2] = TOKEN_ASSIGNEMENT;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	followArr[0] = TOKEN_SEMICOLON;

	Token *current_token = next_token();
	if (!current_token)
		return;
	switch (current_token->kind)
	{

		case TOKEN_OBRACKET:

	//		fprintf(yyout, "{COMMAND_TAG --> [EXPRESSION]  = EXPRESSION}\n");
			//printf("{COMMAND_TAG --> [EXPRESSION]  = EXPRESSION}\n");

			Parse_EXPRESSION(0);
			match(TOKEN_CBRACKET, 1, followArr, 3, firstArr);
			match(TOKEN_ASSIGNEMENT, 1, followArr, 3, firstArr);
			Parse_EXPRESSION(0);
			break;

		case TOKEN_POINTER:

			match(TOKEN_ASSIGNEMENT, 1, followArr, 3, firstArr);
			return Parse_EXPRESSION(0);
			break;

		case TOKEN_ASSIGNEMENT:
			
			current_token = back_token();
			if (symboltable_find(currentTable, current_token->lexeme)!=NULL) {
				leftOperand = symboltable_find(currentTable, current_token->lexeme)->type;
				current_token = next_token();
				rightOperand = Parse_RECEIVER(leftOperand);
				if (leftOperand != TYPE_UNDEFINED && rightOperand != TYPE_UNDEFINED){
					if (rightOperand != leftOperand) {
						fprintf(yyout,"--Error:  in line %d mismatch between types of the left and the right sides of the assignment\n",current_token->lineNumber);
					}
					else if (rightOperand > 8) {
						current_token = back_token();
						current_token = next_token();
						if (symboltable_find(currentTable, current_token->lexeme)->userDefined != 1) {
							fprintf(yyout,"--Error: in line %d mismatch between types of the left and the right sides of the assignment\n",current_token->lineNumber);
						}
					}
					
				}
			}
			else {
				fprintf(yyout,"--Error: in line %d variable %s is not declared\n",current_token->lineNumber ,current_token->lexeme);
				next_token();
				next_token();
			}
			
			break;
			
		
		default:

			errorHandler(followArr, firstArr, 1, 3, current_token);
			break;
	}
}

int Parse_RECEIVER(int type)
{
	SymbolTableEntry* entry;
	Token* active;
	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 6);
	firstArr[0] = TOKEN_MALLOC;
	firstArr[1] = TOKEN_INT;
	firstArr[2] = TOKEN_REAL;
	firstArr[3] = TOKEN_MEMORY;
	firstArr[4] = TOKEN_SIZE_OF;
	firstArr[5] = TOKEN_ID;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	followArr[0] = TOKEN_SEMICOLON;

	Token *current_token = next_token();
	if (!current_token)
		return 0;
	switch (current_token->kind)
	{
		case TOKEN_MALLOC:

			match(TOKEN_OPARENTHESIS, 1, followArr, 6, firstArr);
			match(TOKEN_SIZE_OF, 1, followArr, 6, firstArr);
			match(TOKEN_OPARENTHESIS, 1, followArr, 6, firstArr);
			match(TOKEN_ID, 1, followArr, 6, firstArr);
			match(TOKEN_CPARENTHESIS, 1, followArr, 6, firstArr);
			match(TOKEN_CPARENTHESIS, 1, followArr, 6, firstArr);
			return TYPE_ALLOCATION;
			

		case TOKEN_INT:
			return TYPE_INT;
		case TOKEN_REAL:
			return TYPE_REAL;
		
		case TOKEN_SIZE_OF:
			current_token = next_token();
			
			entry=symboltable_find(currentTable, current_token->lexeme);
			current_token = back_token();
			current_token = back_token();
			if (entry) {
				return entry->type;
			}
			return 0;
		case TOKEN_MEMORY:
			

		case TOKEN_ID:


			current_token = back_token();
			return Parse_EXPRESSION(type);

		default:

			errorHandler(followArr, firstArr, 1, 6, current_token);
			return 0;
	}
}

int Parse_EXPRESSION(int type)
{
	Token* active;
	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 5);
	firstArr[0] = TOKEN_INT;
	firstArr[1] = TOKEN_REAL;
	firstArr[2] = TOKEN_MEMORY;
	firstArr[3] = TOKEN_SIZE_OF;
	firstArr[4] = TOKEN_ID;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 4);
	followArr[0] = TOKEN_SEMICOLON;
	followArr[1] = TOKEN_CBRACKET;
	followArr[2] = TOKEN_CPARENTHESIS;
	followArr[3] = TOKEN_REL_OP;

	Token *current_token = next_token();
	if (!current_token)
		return 0;
	switch (current_token->kind)
	{
		case TOKEN_ID:
			
			current_token = next_token();
			if (current_token->kind == TOKEN_SEMICOLON) {
				current_token = back_token();
				if(symboltable_find(currentTable, current_token->lexeme)){
					return symboltable_find(currentTable, current_token->lexeme)->type;
				}
				current_token = back_token();
			}
			else {
				current_token = back_token();
				return Parse_EXPRESSION_TAG(type);
			}
			
		case TOKEN_INT:
			if (type != TYPE_UNDEFINED && type != TYPE_INT) {
				//printf("--Error: mismatch between types of the left and the right sides of the assignment \n");
			}
			//current_token = back_token();
			return TYPE_INT;
		case TOKEN_REAL:
			if (type != TYPE_UNDEFINED && type != TYPE_REAL) {
				//printf("--Error: mismatch between types of the left and the right sides of the assignment \n");
			}
			//current_token = back_token();
			return TYPE_REAL;


		case TOKEN_MEMORY:

		//	fprintf(yyout, "{EXPRESSION --> &id}\n");
			//printf("{EXPRESSION --> &id}\n");

			match(TOKEN_ID, 4, followArr, 5, firstArr);
			current_token = back_token();
			current_token = next_token();
			return (8 + symboltable_lookup(currentTable, current_token->lexeme)->type);

		case TOKEN_SIZE_OF:

	//		fprintf(yyout, "{EXPRESSION --> size_of(id)}\n");
			//printf("{EXPRESSION --> size_of(id)}\n");

			match(TOKEN_OPARENTHESIS, 4, followArr, 5, firstArr);
			match(TOKEN_ID, 4, followArr, 5, firstArr);
			active = activeToken();
			if (symboltable_find(currentTable, active->lexeme) != TYPE_INT) {
				fprintf(yyout,"Semantic Error in line %d ---- Expect integer in Size of ----- \n",current_token->lineNumber);
			}
			match(TOKEN_CPARENTHESIS, 4, followArr, 5, firstArr);
			return 0;
		default:

			errorHandler(followArr, firstArr, 4, 5, current_token);
			return 0;
	}
}

int Parse_EXPRESSION_TAG(int type)
{
	int leftOp,rightOp,holder;
	eTOKENS *firstArr_inside;

	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 7);
	firstArr[0] = TOKEN_OBRACKET;
	firstArr[1] = TOKEN_POINTER;
	firstArr[2] = TOKEN_MUL;
	firstArr[3] = TOKEN_PLUS;
	firstArr[4] = TOKEN_DIVIDE;
	firstArr[5] = TOKEN_SUBSTRUCT;
	firstArr[6] = TOKEN_POW;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 4);
	followArr[0] = TOKEN_SEMICOLON;
	followArr[1] = TOKEN_CBRACKET;
	followArr[2] = TOKEN_CPARENTHESIS;
	followArr[3] = TOKEN_REL_OP;

	Token *current_token = next_token();
	if (!current_token)
		return 0;
	switch (current_token->kind)
	{

		case TOKEN_OBRACKET:

		//	fprintf(yyout, "{EXPRESSION_TAG --> [EXPRESSION]}\n");
			//printf("{EXPRESSION_TAG --> [EXPRESSION]}\n");

			Parse_EXPRESSION(0);
			match(TOKEN_CBRACKET, 4, followArr, 7, firstArr);
			break;

		case TOKEN_POINTER:

	//		fprintf(yyout, "{EXPRESSION_TAG --> ^}\n");
			//printf("{EXPRESSION_TAG --> ^}\n");

			break;

		case TOKEN_SUBSTRUCT:
		case TOKEN_MUL:
		case TOKEN_PLUS:
		case TOKEN_DIVIDE:
		case TOKEN_POW:
			current_token = back_token();
			if (symboltable_find(currentTable, current_token->lexeme) != NULL) {
				leftOp = symboltable_find(currentTable, current_token->lexeme)->type;
			}
			current_token = next_token();
		
		 
			return Parse_EXPRESSION(leftOp);
		

		case TOKEN_REL_OP:
		case TOKEN_CPARENTHESIS:
		case TOKEN_CBRACKET:
		case TOKEN_SEMICOLON:

			//fprintf(yyout, "{EXPRESSION_TAG --> NULL}\n");
			//printf("{EXPRESSION_TAG --> NULL}\n");

			current_token = back_token();

			firstArr_inside = (eTOKENS *)malloc(sizeof(eTOKENS) * 4);
			firstArr_inside[0] = TOKEN_REL_OP;
			firstArr_inside[1] = TOKEN_CPARENTHESIS;
			firstArr_inside[2] = TOKEN_CBRACKET;
			firstArr_inside[3] = TOKEN_SEMICOLON;

			break;

		default:

			errorHandler(followArr, firstArr, 4, 7, current_token);
			break;

	}
}




