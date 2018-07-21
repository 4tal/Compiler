#ifndef SYMBOLTABLE
#define SYMBOLTABLE

#pragma warning(disable:4996)
#include "HashTable.h"

#define TYPE_UNDEFINED			0
#define TYPE_REAL				1
#define TYPE_INT				2
#define TYPE_ARRAY				4
#define TYPE_ALLOCATION         7
#define TYPE_POINTER			8
#define TYPE_ID					9


typedef struct SymbolTableEntry {
	int type; // TYPE_REAL / TYPE_INT
	int userDefined;
} SymbolTableEntry;

typedef struct SymbolTable {
	struct SymbolTable* father;
	HashTable hashTable[HASH_SIZE];
} SymbolTable;

SymbolTable* symboltable_init(SymbolTable* table, SymbolTable* father);
// will free the values as well.
void symboltable_destruct(SymbolTable* table);
// 0 if OK, -1 if exists. pay attention, that the entry will be freed at destruct.
int symboltable_insert(SymbolTable* table, char* key, SymbolTableEntry* entry);
SymbolTableEntry* symboltable_lookup(SymbolTable* table, char* key);
SymbolTableEntry* symboltable_find(SymbolTable* table, char* key);

#endif