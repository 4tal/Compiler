#ifndef HASHTABLE
#define HASHTABLE

#pragma warning(disable:4996)

#define HASH_SIZE 201

typedef struct HashTableEntry{
	void* value;
	char key[100];
	struct HashTableEntry* next;
}HashTableEntry;

typedef struct HashTable{
	HashTableEntry* table[HASH_SIZE];
}HashTable;

HashTable* hashtable_init(HashTable* table);
void hashtable_destruct(HashTable* table);
// 0 if OK, -1 if exists
int hashtable_insert(HashTable* table, char* key, void* value);
void* hashtable_lookup(HashTable* table, char* key);
int hashtable_hash_func(char* key);

#endif