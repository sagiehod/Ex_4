/*
 * EX ______. Created by: __________. Date: 1/16/2020
 *
 */

#ifndef TRIE_H
#define TRIE_H

#define ERROR_STATUS -1
#define OKAY_STATUS 0
typedef enum
{
	NORMAL_ORDER,
	REVERSE_ORDER
}TraverseOrder;

typedef enum {
	TRIE_SUCCESS = 0,
	TRIE_UNINITAILZED,
	TRIE_ALLOC_FAILED,
	TRIE_UNITIALIZED_ARG
} TrieStatus;

typedef struct Node Node;

typedef struct Trie
{
	int _magicNumber; /* an arbitrary number in order to prevent double free */
	Node* _root;
}Trie;

/**
 * creates a new empty trie ptr and returns it to the user
 * @return a new trie ptr of null if failed
 */
Trie* TrieCreate();

/**
 * destroyes the trie and release all allocated memory
 * @param trie a ptr to a triePtr
 */
void TrieDestroy(Trie** triePtr);

/**
 * @param trie to insert into
 * @param word to be insert into the trie
 * @return TRIE_UNITINALIZED if trie is unitialized
 *         TRIE_UNITIALIZED_ARG if word is not valid string ptr
 *         TRIE_NODE_ALLOC_FAILED if needed to allocate new nodes and failed
 *         TRIE_SUCCESS upon success
 * Note: big letters in a word will be convereted into small words
 * Note: any sign that is not abc letter will be ignored and ommited
 */
TrieStatus TrieInsertWord(Trie * trie, char* word);

/**
 * prints the trie by a given order
 * @param trie trie to print
 * @param order normal order or reversed order
 * @return TRIE_UNITINALIZED if trie is unitialized
 * 		   TRIE_SUCCESS upon success
 */
TrieStatus TriePrint(const Trie * trie, TraverseOrder order, unsigned maxTrieHeight);

#endif