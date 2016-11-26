/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

//create nodes for linked list
typedef struct node
{
    char word[LENGTH+1];
    struct node* next;
}
node;

//create hashtable
node* hashtable[SIZETABLE] = {NULL};

//hash function
int hash (const char* word)
{
    int index = 0;
    
    //the first three characters of a word for hashing
    char arr[3] = {word[0], word[1], word[2]};

    for (int i = 0; i < 3; i++)
    {
        if (isalpha(arr[i]))
        {
            arr[i] -= 'a';
            if (i == 0)
                index += arr[i] * (SYMBOLS * SYMBOLS);
            else if (i == 1)
                index += arr[i] * (SYMBOLS);
            else
                index += arr[i];
        }
    }

    return index;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    char temp[LENGTH + 1] = {0};
    int length = strlen(word);

    for(int i = 0; i < length; i++)
        temp[i] = tolower(word[i]);

    int index = hash(temp);
    node* cursor = hashtable[index];
    
    while (cursor != NULL)
    {
        if (strcmp(cursor->word, temp) == 0)
            return true;

        cursor = cursor->next;
    }

    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    //open dictionary
    FILE* file = fopen(dictionary, "r");

    if(file == NULL)
        return false;

    //create an array for word
    char word[LENGTH+1];

    int index = 0;
    node* cursor = NULL;

    while (fscanf(file, "%s", word) != EOF)
    {
        index = hash(word);
        cursor = hashtable[index];

        if (cursor == NULL)
        {
            node* newNode = malloc(sizeof(node));
            strcpy(newNode->word, word);
            hashtable[index] = newNode;
        } 
        else 
        {
            //length of word in the text
            int inText = strlen(word);
            
            //length of word in the dictionary
            int inDictionary = strlen(cursor->word);
                
            while (inDictionary <= inText && cursor->next != NULL)
            {
                cursor = cursor->next;
                inDictionary = strlen(cursor->word);
            }

            node* newNode = malloc(sizeof(node));
            strcpy(newNode->word, word);
            newNode->next = cursor->next;
            cursor->next = newNode;
        }
    }
    
    fclose(file);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    unsigned int words = 0;
    node* cursor = NULL;

    for(int i = 0; i < SIZETABLE; i++)
    {
        cursor = hashtable[i];
        
        while (cursor != NULL)
        {
            words++;
            cursor = cursor->next;
        }
    }
    
    return words;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    node* cursor = NULL;
    node* temp = NULL;
    int i = 0;
    
    while(i < SIZETABLE)
    {
        cursor = hashtable[i];
        
        while (cursor != NULL)
        {
            temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
        
        i++;
    }
    
    return true;
}
