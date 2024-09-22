#include "inv_srch.h"

int check_equal_word, check_equal_fname ;

/***********************************************************************************************************************************************
  	* Function to create a database and store the words relative to the indexes
	* RETURN     : SUCCESS / FAILURE
	* PARAMETERS : Integer, Structure pointer, Hashtable
***********************************************************************************************************************************************/
int create_database(int argc, Node *head, hash_t arr[])
{
       // STEP 1  : Find the index
       // STEP 2  : Check if link is NULL or not
       // STEP 3  : If NULL create mainnode & subnode else traverse the nodes
       // STEP 4  : If words are same check file_name if same increment word_count else increment the file_count and create sub_node      
       // STEP 5  : If words are not same create mainnode and subnode and connect them with previous nodes

       int count = 0, index;
       FILE *fp;
       char ch, str[100];
       Node *Slist = head;

       while (Slist)
       {
	      fp = fopen(Slist->data, "r");
	      if (!fp) return FAILURE;		// Meomory validation

	      while (fscanf(fp, "%s", str) == 1)
	      {
		     check_equal_word = 1, check_equal_fname = 1;
		     if (isalpha(str[0]))
		     {
			    str[0] = tolower(str[0]);
			    index = str[0] - 'a';
		     }
		     else
		     {
			    if (isdigit(str[0])) index = 26;
			    else index = 27;
		     }

		     if (arr[index].link == NULL)
		     {
			    main_n *new = malloc(sizeof(main_n));
			    if (!new) return FAILURE; 

			    new->file_count = 1;
			    strcpy(new->word, str);
			    new->main_link = NULL;

			    sub_n *new1 = malloc(sizeof(sub_n));
			    if (!new1) return FAILURE;

			    new1->word_count = 1;
			    strcpy(new1->file_name,Slist->data);
			    new1->sub_link = NULL; 

			    // Connect the main_node and sub_node
			    new->sub_link = new1;

			    // Connect the hashtable and main_node
			    arr[index].link = new;
		     }

		     else
		     {
			    main_n *temp = arr[index].link;

			    while (temp)
			    {
				   if (strcmp(temp->word, str) == 0)
				   {
					  check_equal_word = 0;
					  sub_n *move = temp->sub_link;
					  while (move)
					  {
						 if (strcmp(move->file_name, Slist->data) == 0)
						 {
							move->word_count++;
							check_equal_fname = 0;
						 }
						 move = move->sub_link;
					  }
				   }
				   temp = temp->main_link;			// Traversing through the main node until NULL
			    }

			    if(check_equal_fname && !check_equal_word) 		// Words are equal and filenames are not equal 
			    {
				   temp = arr[index].link;
				   while(temp->main_link)
				   {
					  if (!strcmp(temp->word, str))	// Move temp until temp reaches the matched word
						 break;
					  temp = temp->main_link;
				   }

				   temp->file_count++;
				   sub_n *next = malloc(sizeof(sub_n));
				   if (!next) return FAILURE;

				   next->word_count = 1;
				   strcpy(next->file_name, Slist->data);
				   next->sub_link = NULL;

				   // Connect the new sub_node with previous subnodes link
				   sub_n *trav = temp->sub_link;
				   while (trav->sub_link)
				   {
					  trav = trav->sub_link;	// Traversing through the sub node until NULL
				   }
				   trav->sub_link = next; 
			    }

			    if (check_equal_word)				// Words are not equal
			    {
				   main_n *traverse = arr[index].link;
				   while (traverse->main_link)
				   {
					  traverse = traverse->main_link;	// Traversing the main node until NULL
				   }

				   main_n *first = malloc(sizeof(main_n));
				   if (!first) return FAILURE;

				   first->file_count = 1;
				   strcpy(first->word, str);
				   first->main_link = NULL;

				   // Connect the new main_node with old one
				   traverse->main_link = first;

				   sub_n *second = malloc(sizeof(sub_n));
				   if (!second) return FAILURE;

				   second->word_count = 1;
				   strcpy(second->file_name, Slist->data);
				   second->sub_link = NULL;

				   // Connect the main_node and sub_node
				   first->sub_link = second;
			    }
		     }
	      }
	      Slist = Slist->link;
       }
       return SUCCESS;
}
