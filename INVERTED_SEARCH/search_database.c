#include "inv_srch.h"

/***********************************************************************************************************************************************
  	* Function to search from database
	* RETURN     : SUCCESS / FAILURE
	* PARAMETERS : Hashtable
***********************************************************************************************************************************************/
int found = 1;
int search_database(hash_t arr[])
{
       char str[100];
       printf("Enter the word you want to search : ");
       scanf("%s", str);

       for (int index = 0; index < 28; index++)
       {
	      if (arr[index].link != NULL)
	      {
		     main_n *temp = arr[index].link;
		     while (temp)
		     {
			    if (!strcmp(temp->word, str))
			    {
				   found = 0;
				   sub_n *read = temp->sub_link;
				   while (read)
				   {
					  printf("In file : %s\t%d time(s)\n", read->file_name, read->word_count);
					  read = read->sub_link;
				   }
			    }
			    temp = temp->main_link;
		     }
	      }
       }
       return (found) ? FAILURE : SUCCESS;
}
