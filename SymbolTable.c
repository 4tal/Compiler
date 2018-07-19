#include "SymbolTable.h"
#include <stddef.h>

SymbolTable* symboltable_init(SymbolTable* table, SymbolTable* father)
{
	table->father = father;
	hashtable_init(table->hashTable);

	return table;
}

void symboltable_destruct(SymbolTable* table)
{
	table->father = NULL;
	hashtable_destruct(table->hashTable);
}

int symboltable_insert(SymbolTable* table, char* key, SymbolTableEntry* entry)
{
	return hashtable_insert(table->hashTable, key, entry);
}

SymbolTableEntry* symboltable_lookup(SymbolTable* table, char* key)
{
	return (SymbolTableEntry*)hashtable_lookup(table->hashTable, key);
}

SymbolTableEntry* symboltable_find(SymbolTable* table, char* key)
{
	SymbolTable* currentTable = table;
	while (currentTable != NULL)
	{
		SymbolTableEntry* entry = (SymbolTableEntry*)hashtable_lookup(currentTable->hashTable, key);
		if (entry == NULL)
		{
			currentTable = currentTable->father;
		}
		else
		{
			return entry;
		}
	}

	return NULL;
}
