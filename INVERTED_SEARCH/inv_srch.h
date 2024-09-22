#ifndef SEARCH_H
#define SEARCH_H

// Standard libraries
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

// Macros
#define SUCCESS 1
#define FAILURE 0

//UDT for list
typedef struct Listnode
{
       char data[100];
       struct Listnode *link;
}Node;

// For subnode
typedef struct subNode
{
       int word_count;
       char file_name[100];
       struct subNode *sub_link;
}sub_n;

// For mainnode
typedef struct mainNode
{
       int file_count;
       char word[100];
       struct mainNode *main_link;
       struct subNode *sub_link;
}main_n;

// For hastable
typedef struct hashTable
{
       int index;
       struct mainNode *link;
}hash_t;

// Function prototypes
/* Function to read and validate the info passed by the user*/
int read_validate(int argc, char *argv[], Node **head);

/* Function to insert the file to the list(sll)*/
int insert_at_last(Node **head, char *data);

/* Function to print the linked list*/
void print_ll(Node *head);

/* Function to create database*/
int create_database(int argc, Node *head, hash_t arr[]);

/* Function to create a hashtable*/
void create_hashTable(hash_t arr[]);

/* Function to display database*/
int disp_database(hash_t arr[]);

/* Function to search an element in the database*/
int search_database(hash_t arr[]);

/* Function to save the database that is already existing*/
int save_database(hash_t arr[]);

/* Function to update the database when there is no existing database*/
int update_database(hash_t arr[]);
#endif
