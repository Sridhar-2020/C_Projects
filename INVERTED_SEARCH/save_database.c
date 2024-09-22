#include "inv_srch.h"

/***********************************************************************************************************************************************
  	* Function to save database in a external file
	* RETURN     : SUCCESS / FAILURE
	* PARAMETERS : Hashtable
***********************************************************************************************************************************************/
int save_database(hash_t arr[])
{
       int flag = 0;
       for (int index = 0; index < 28; index++)
       {
	      if (arr[index].link != NULL)
	      {
		     flag = 1;
		     break;
	      }
       }
       if (flag == 0) return FAILURE;

       char str[100];
       printf("Enter the filename to backup the existing database : ");
       scanf("%s", str);

       if (strstr(str, ".txt") == NULL) return FAILURE;			// Checking whether the file passed is a text file or not

       FILE *fptr = fopen(str, "w");
       if (!fptr) return FAILURE;

       for (int index = 0; index < 28; index++)
       {
	      if (arr[index].link != NULL)
	      {
		     main_n *temp = arr[index].link;
		     while (temp)
		     {
			    fprintf(fptr, "#;%d;%s;%d;", index, temp->word, temp->file_count);
			    sub_n *read = temp->sub_link;
			    while (read)
			    {
				   fprintf(fptr, "%s;%d;", read->file_name, read->word_count);
				   read = read->sub_link;
			    }
			    temp = temp->main_link;
			    fprintf(fptr, "%c" "%c",'#', '\n');
		     }
	      }
       }
       fclose(fptr);
       return SUCCESS;
}
