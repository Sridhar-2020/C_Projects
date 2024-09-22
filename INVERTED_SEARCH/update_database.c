#include "inv_srch.h"

/***********************************************************************************************************************************************
  	* Function to update database by taking the contents from the external file
	* RETURN     : SUCCESS / FAILURE
	* PARAMETERS : Integer, Structure pointer, Hashtable
***********************************************************************************************************************************************/
int update_database(hash_t arr[])
{
       int flag = 1;
       for (int index = 0; index < 28; index++)
       {
	      if (arr[index].link != NULL)
	      {
		     flag = 0;
		     break;
	      }
       }
       if (flag == 0) return FAILURE;
       // STEP 1 : Validate whether the file passed through the command line argument is valid or not
       // STEP 2 : Extract the contents from the file and create a new database

       char str[100];
       printf("Enter the backup file name : ");
       scanf(" %s", str);
       FILE *fptr;
       if (strstr(str, ".txt") != NULL)
       {
	      // Checking the file is existing or not
	      fptr = fopen(str, "r");
	      if (!fptr)
	      {
		     printf("[%s] file not exists\n\n", str);
		     return FAILURE;
	      }

	      // Checking the file is empty or not
	      fseek(fptr, 0, SEEK_END);
	      if (ftell(fptr) == 1 || ftell(fptr) == 0)
	      {
		     printf("[%s] is an empty file!!\n\n", str);
		     return FAILURE;
	      }
	      rewind(fptr);

	      // Checking the file is the backup file or not
	      char ch;
	      if ((ch = fgetc(fptr)) != '#')
	      {
		     printf("[%s] is not an backup file!!\n\n", str);
		     return FAILURE;
	      }
	      rewind(fptr);
       }
       else
       {
	      printf("Please a file with .txt extenion via command line\n\n");
	      return FAILURE;
       }

       // Store the contents of the backup file in array of strings
       char str1[100];
       fptr = fopen(str, "r");
       char *token;

       while (fscanf(fptr, "%s", str1))
       {
	      if (feof(fptr)) break;

	      // Extracting the index
	      token = strtok(str1, "#;");
	      int index = atoi(token);

	      // Extracting the word
	      token = strtok(NULL, ";");
	      char *word = token;

	      // Extracting the filecount
	      token = strtok(NULL, ";");
	      int filecount = atoi(token);

	      main_n *new = malloc(sizeof (main_n));
	      if (!new) return FAILURE;
	      strcpy(new->word, word);
	      new->file_count = filecount;
	      new->main_link = NULL;
	      new->sub_link = NULL;

	      main_n *travs = arr[index].link;
	      if (travs == NULL) 	 
	      {
		     arr[index].link = new;
	      }     
	      else
	      {
		     while (travs->main_link) 			// Traversing till the last main node
		     {
			    travs = travs->main_link;
		     }
		     travs->main_link = new;			// Linking the last main node with its previous node
	      }

	      for (int i = 0; i < filecount; i++)
	      {
		     // Extracting the filename
		     token = strtok(NULL, ";");
		     char *filename = token;

		     // Extracting the wordcount
		     token = strtok(NULL, ";");
		     int wordcount = atoi(token);

		     sub_n *new1 = malloc(sizeof (sub_n));
		     if (!new1) return FAILURE;
		     strcpy(new1->file_name, filename);
		     new1->word_count = wordcount;
		     new1->sub_link = NULL;

		     if (new->sub_link == NULL)
		     {
			    new->sub_link = new1;
		     }
		     else
		     {
			    sub_n *temp = new->sub_link;
			    while (temp->sub_link)
			    {
				   temp = temp->sub_link; 	// Traverse till the last
			    }
			    temp->sub_link = new1;
		     }
	      }
       }
       disp_database(arr);
       return SUCCESS;
}
