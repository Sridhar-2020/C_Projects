
#include "inv_srch.h"

/***********************************************************************************************************************************************
  	* Function to check whether the arguments are passed in a correct manner or not
	* RETURN     : SUCCESS / FAILURE
	* PARAMETERS : Integer, Structure pointer, Hashtable
***********************************************************************************************************************************************/
int read_validate(int argc, char *argv[], Node **head)
{
       int flag;
       if (argc > 1)
       {
	      for (int i = 1; i < argc; i++)
	      {
		     flag = 1;
		     if (strstr(argv[i], ".txt") != NULL)
		     {
			    FILE *fp = fopen(argv[i], "r");
			    if (fp == NULL)
			    {
				   printf("File %s doesnot exists!!\n", argv[i]);
				   continue;
			    }

			    // Checking the file is empty or not
			    fseek(fp, 0, SEEK_END);
			    if (ftell(fp) == 1 || ftell(fp) == 0)
			    {
				   printf("%s is an empty file!!\n", argv[i]);
				   continue;
			    }

			    // Checking for duplicate file
			    if (*head)
			    {
				   Node *temp = *head;
				   while (temp)
				   {
					  if (strcmp(temp->data, argv[i]) == 0)
					  {
						 printf("The passed file %s is a duplicate file\n", argv[i]);
						 flag = 0;			// Flag value to check whether the file is duplicate or not
					  }
					  temp = temp->link;
				   }
			    }

			    // Inserting the file name to the list
			    if (flag)
				   insert_at_last(head, argv[i]);
		     }
		     else
		     {
			    printf("Please pass the file with .txt extension..\n");
		     }
	      }
	      print_ll(*head);
	      return SUCCESS;
       }
       else
       {
	      printf("Please pass the command line arguments...\n");
	      printf("Usage : <executable file> <input file name>\n");
	      return FAILURE;
       }
}

/***********************************************************************************************************************************************
  	* Function to insert the node at the end of the linked list
	* RETURN     : SUCCESS / FAILURE
	* PARAMETERS : Address of the head of linked list, String
***********************************************************************************************************************************************/
int insert_at_last(Node **head, char *data)
{
       Node *new = malloc(sizeof(Node));
       // Memory validation
       if (!new) return FAILURE;

       strcpy(new->data, data);
       new->link = NULL;

       // List empty
       if (*head == NULL)
       {
	      *head = new;
	      return SUCCESS;
       }
       // List non empty
       Node *temp = *head;
       while (temp->link)
       {
	      temp = temp->link;
       }
       temp->link = new;
       return SUCCESS;
}

/***********************************************************************************************************************************************
  	* Function to display the linked list
	* RETURN     : void
	* PARAMETERS : Head of the linked list
***********************************************************************************************************************************************/
void print_ll(Node *head)
{
       if (head == NULL)
       {
	      printf("List is empty\n");
       }
       else
       {
	      while(head)
	      {
		     printf("%s-> ", head->data);
		     head = head->link;
	      }
	      printf("NULL\n");
       }
}
