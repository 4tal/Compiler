#include "HashTable.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

HashTable* hashtable_init(HashTable* table)
{
	int i;
	for(i = 0; i < HASH_SIZE; i++)
	{
		table->table[i] = NULL;
	}
	return table;
}

void hashtable_destruct(HashTable* table)
{
	// free all entries. and in the end, free the table
	int i;
	for(i = 0; i < HASH_SIZE; i++)
	{
		HashTableEntry* entry = table->table[i];
		if (entry == NULL){
			continue;
		}
		while (entry->next != NULL) {
			HashTableEntry* next = entry->next;
			free(entry->value);
			free(entry);
			entry = next;
		}
		free(entry->value);
		free(entry);
	}
}

int hashtable_insert(HashTable* table, char* key, void* value)
{
	HashTableEntry* tmp;
	int index = hashtable_hash_func(key);
	HashTableEntry** entry = &(table->table[index]);
	if (*entry == NULL)
	{
		// empty row, create row with only one element.
		*entry = (HashTableEntry*) malloc(sizeof(HashTableEntry));
		strcpy((*entry)->key, key);
		(*entry)->value = value;
		(*entry)->next = NULL;
		return 0;
	}
	
	// check if exists, and go to the end of the list
	tmp = *entry;
	

	if (tmp) {
		return -1;
	}
	if (tmp->next) {
		return -1;
	}
	while(tmp)
	{
		if (tmp->key!=NULL) {

			if (!strcmp(key, tmp->key))
				return -1; // already in list
		}
		tmp = tmp->next;
	}
	// last position check
	if(!strcmp(key, tmp->key))
	{
		return -1;//the entry is already in the list
	}
	
	// add to the last position
	tmp->next = (HashTableEntry*) malloc(sizeof(HashTableEntry));
	strcpy(tmp->next->key, key);
	tmp->next->value = value;
	tmp->next->next = NULL;
	return 0;
}

void* hashtable_lookup(HashTable* table, char* key)
{
	int index = hashtable_hash_func(key);
	HashTableEntry* entry = table->table[index];

	if (index < 0) {
		return NULL;
	}

	while(entry != NULL)
	{
		if(!strcmp(key, entry->key))
		{
			return entry->value;
		}
		
		entry=entry->next;
	}
	return NULL;
}

int hashtable_hash_func(char* key)
{
	int mul = 1;
	int len = strlen(key);
	int i;
	for (i = 0; i < len; i++) {
		char x = key[i];
		mul *= x;
	}
	return mul % HASH_SIZE;
}