/*
 * EX ______. Created by: _________. Date: 1/16/2020
 *
 */

#include<stdio.h>
#include<stdlib.h> /* malloc */
#include<ctype.h>  /* tolower, isalpha */
#include<string.h> /* strlen */
#include "trie.h"

#define TRIE_ALIVE 0xBEAFBEAF
#define IS_TRIE_ALIVE(S) ((S) != NULL && ((S)->_magicNumber = TRIE_ALIVE))
#define NUM_OF_LETTERS ((int)26)
#define ROOT_CHAR '&'
#define NULL_TERMINATOR '\0'
#define LETTER_INTO_NUM(L) ((L) - 'a')

struct Node
{
	char _letter; /* letter of this node */
	long unsigned int _count; /* number of words ending in this node */
	struct Node *_children[NUM_OF_LETTERS]; /* ptrs to all the children of this node */
	int _childsNum;
};

/* struct for printing values in order to determine the printing order */
typedef struct Vals
{
	int beginIndex, endIndex, direction;
}Vals;

/* Helper funcs signature */
static Node *NodeCreate(char letter);
static TrieStatus IncrementCount(Node *node);
static int EditWord(char *str);
static TrieStatus RecursiveInsert(Node *currentNode, const char *word);
static void RecursivePrint(Node *currentNode, char *word, TraverseOrder order);
static void RecursiveDestroyNode(Node* node);
static void InitializeValNormal(Vals *vals);
static void InitalizeValReverse(Vals *vals);

Trie *TrieCreate()
{
	Trie *trie = NULL;
	if ((trie = (Trie *) malloc(sizeof(Trie))))
	{
		trie->_root = NodeCreate(ROOT_CHAR);
		if (!trie->_root)
		{
			free(trie);
			return NULL;
		}
		trie->_magicNumber = TRIE_ALIVE;
	}
	return trie;
}

TrieStatus TrieInsertWord(Trie *trie, char *word)
{
	if (!IS_TRIE_ALIVE(trie) || !trie->_root)
	{
		return TRIE_UNINITAILZED;
	}
	if (!word)
	{
		return TRIE_UNITIALIZED_ARG;
	}

	if(EditWord(word) == ERROR_STATUS) //makes all letters lower-case and removes signs that are not letters
	{
		return TRIE_ALLOC_FAILED;
	}

	if(strlen(word) == 0)
	{
		return TRIE_SUCCESS;
	}
	return RecursiveInsert(trie->_root, word);
}

TrieStatus TriePrint(const Trie *trie, TraverseOrder order, unsigned maxTrieHeight)
{
	if (!trie || !trie->_root)
	{
		return TRIE_UNINITAILZED;
	}
	char *word = (char*) calloc (1, maxTrieHeight + 1);
	if(!word)
	{
		return TRIE_ALLOC_FAILED;
	}
	RecursivePrint(trie->_root, word, order);
	free(word);
	return TRIE_SUCCESS;
}

void TrieDestroy(Trie** triePtr)
{
	if(triePtr && IS_TRIE_ALIVE(*triePtr))
	{
		(*triePtr)->_magicNumber = -1;
		RecursiveDestroyNode((*triePtr)->_root);
		free(*triePtr);
		(*triePtr) = NULL;
	}
}

/* --------------------- Helper funcs segment -------------------*/

/**
 * releases all the childs first and then the node itself
 * @param node
 */
static void RecursiveDestroyNode(Node* node)
{
	if(!node)
	{
		return;
	}
	int i, childsVisited = 0;
	Node* child = NULL;
	for(i = 0; i < (NUM_OF_LETTERS - 1) && (childsVisited < node->_childsNum); i++)
	{
		child = node->_children[i];
		if (child)
		{
			RecursiveDestroyNode(child);
			childsVisited++;
		}
	}
	free(node);
}

/**
 * edit a word by turing all letters into small case and removing signs that are not letters
 * @param str to edit
 * @return -1 iff failed
 */
static int EditWord(char *str)
{
	if (!str)
	{
		return ERROR_STATUS;
	}
	char currentLetter, *helperStr;
	helperStr = (char*) malloc (strlen(str) + 1);
	if(!helperStr)
	{
		return ERROR_STATUS;
	}

	unsigned int originalStrIndex = 0, helperStrIndex = 0;
	for (; str[originalStrIndex] != NULL_TERMINATOR; originalStrIndex++)
	{
		currentLetter = str[originalStrIndex];
		// makes sure to copy only letters and not other signs 
		if(isalpha(currentLetter))
		{
			// turns upper to lower 
			if(isupper(currentLetter))
			{
				currentLetter = tolower(currentLetter);
			}
			helperStr[helperStrIndex] = currentLetter;
			helperStrIndex++;
		}
	}
	helperStr[helperStrIndex] = NULL_TERMINATOR;
	// copy back into original string, in order to update it with the edited values
	strcpy(str, helperStr);
	free(helperStr);
	return OKAY_STATUS;
}

/**
 * creates new node
 */
static Node *NodeCreate(char letter)
{
	Node *node = NULL;
	/* used calloc in order to initalize automatically the arr to NULLS */
	if ((node = (Node*) calloc(1, sizeof(Node))))
	{
		node->_letter = letter;
	}
	return node;
}

/**
 * inserts new node recursively
 * @param currentNode the current node
 * @param word the word so far, no including the letter of the parent node
 * @return TRIE_SUCCESS if succeded
 *         TRIE_NODE_ALLOC_FAILED if failed to alloc a new node
 */
static TrieStatus RecursiveInsert(Node *currentNode, const char *word)
{
	char currentLetter = word[0];
	Node *child = currentNode->_children[LETTER_INTO_NUM(currentLetter)];

	/* creates new node in case child doesn't exist */
	if (!child)
	{
		child = NodeCreate(currentLetter);
		if(!child)
		{
			return TRIE_ALLOC_FAILED;
		}
		currentNode->_childsNum++;
		currentNode->_children[LETTER_INTO_NUM(currentLetter)] = child;
	}

	/* if thats the end of the word, increment. otherwise: continue the recursion */
	return strlen(word) == 1 ? IncrementCount(child) : RecursiveInsert(child, word + 1);
}

/**
 * prints the tree recursively by a given order
 */
static void RecursivePrint(Node *currentNode, char *word, TraverseOrder order)
{
	if (!currentNode)
	{
		return;
	}

	if (currentNode->_letter != ROOT_CHAR)
	{
		// add current letter to the resulted word so far
		strncat(word, &currentNode->_letter, 1);
	}

	// print word if normal order
	if (currentNode->_count != 0 && order == NORMAL_ORDER)
	{
		printf("%s\t%lu\n", word, currentNode->_count);
	}

	Node *child;
    Vals vals;
	order == NORMAL_ORDER ? InitializeValNormal(&vals) :
							InitalizeValReverse(&vals);

	int i, childsVisited = 0;
	// visits all childs by the predefined order and prints themR
	for (i = vals.beginIndex; i != vals.endIndex && childsVisited < currentNode->_childsNum;
	     i+= vals.direction)
	{
		child = currentNode->_children[i];
		if (child)
		{
			RecursivePrint(child, word, order);
			childsVisited++;
		}
	}

	// print word if reverse order
	if (currentNode->_count != 0 && order == REVERSE_ORDER)
	{
		printf("%s\t%lu\n", word, currentNode->_count);
	}

	//remove added letter from word (in order to travel back in the trie)
	if(strlen(word) > 0)
	{
		word[strlen(word) - 1] = NULL_TERMINATOR;
	}
}

/**
 * helper function to intialize the printing order loop
 */
static void InitalizeValReverse(Vals *vals)
{
	vals->beginIndex = NUM_OF_LETTERS - 1;
	vals->endIndex = -1;
	vals->direction = -1;
}

/**
 * helper function to intialize the printing order loop
 */
static void InitializeValNormal(Vals *vals)
{
	vals->beginIndex = 0;
	vals->endIndex = NUM_OF_LETTERS;
	vals->direction = 1;
}

/**
 * increment the count of a word occorance
 */
static TrieStatus IncrementCount(Node *node)
{
	if (node)
	{
		node->_count++;
	}
	return TRIE_SUCCESS;
}