#include "Token.h"
#include "SymbolTable.h"

#ifndef PARSER_H
#define PARSER_H



bool match(eTOKENS t, int size_follow, eTOKENS follow[], int size_first, eTOKENS first[]);


int CheckTypeByID(char* lexeme);
void Parser();
void Parse_PROGRAM();
void Parse_BLOCK();
void Parse_DEFINITIONS();
void Parse_DEFINITION();
void Parse_DEFINITIONS_TAG();
void Parse_VAR_DEFINITION();
int Parse_VAR_DEFINITION_TAG();
void Parse_TYPE_DEFENITION();
int Parse_TYPE_INDICATOR();
int Parse_BASIC_TYPE();
int Parse_ARRAY_TYPE();
int Parse_POINTER_TYPE();
void Parse_SIZE();
void Parse_COMMANDS();
void Parse_COMMANDS_TAG();
void Parse_COMMAND();
void Parse_COMMAND_TAG();
int Parse_RECEIVER();
void Parse_EXPRESSION();
void Parse_EXPRESSION_TAG();


#endif