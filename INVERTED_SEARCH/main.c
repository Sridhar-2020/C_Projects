/***********************************************************************************************************************************************
Name 		: Sridhar
Date 		: 10/07/24	
Description 	: INVERTED SEARCH
********************************************************************************************************************************************/

#include "inv_srch.h"

hash_t arr[28];

// Fucntion to create a hashtable
void create_hashTable(hash_t arr[])
{
       for (int i = 0; i < 28; i++)
       {
	      arr[i].index = i;
	      arr[i].link = NULL;
       }
}

Node *head = NULL;
int main(int argc, char *argv[])
{
       // Validating whether the user passed argumnets in correct order or not
       if (read_validate(argc, argv, &head) == SUCCESS)
       {

	      printf("Validation successful...\n");
       }
       else
       {
	      printf("Validation failed!!\n");
	      return FAILURE;
       }
       print_ll(head);

       printf("1. Create database\n2. Display database\n3. Search database\n4. Save database\n5. Update database\n6. Exit\n\n");
       printf("Please choose a valid option from the above : ");
       char ch = 'Y';
       create_hashTable(arr);
       while (ch == 'Y' || ch == 'y')
       {
	      int option;
	      scanf("%d", &option);
	      switch(option)
	      {
		     case 1 :
			   // Creating a database 
			    if (create_database(argc, head, arr) == SUCCESS)
			    {
				   printf("Database created successfully\n\n");
			    }
			    break;

		     case 2:
			    // Displaying the database
			    if (disp_database(arr) != SUCCESS)
			    {
				   printf("Database not created!!\n\n");
			    }
			    break;
		     case 3:
			    // Searching the database
			    if (search_database(arr) != SUCCESS)
			    {
				   printf("String not found!!!\n\n");
			    }
			    break;
		     case 4:
			    // Saving the database
			    if (save_database(arr) == SUCCESS)
			    {
				   printf("Database saved successfully\n\n");
			    }
			    else
			    {
				   printf("Operation failed in saving the database!!\n\n");
			    }
			    break;
		     case 5:
			    // Updating the database
			    if (update_database(arr) == SUCCESS)
			    {
				   printf("Updated database successfully\n\n");
			    }
			    else
			    {
				   printf("Database is not updated\n\n");
			    }
			    break;
		  
		     case 6 : 
			    exit(0);
			    break;
	      }
label:
	      printf("\nDo you want to continue ?\n");
	      printf("Enter y/Y to continue and n/N to discontinue : ");
	      scanf(" %c", &ch);
	      if (ch != 'y' && ch != 'Y' && ch != 'n' && ch != 'N')
	      {
		     printf("\nPlease enter valid option\n");
		     printf("#########################################################################\n");
		     goto label;
	      }
       }
}
