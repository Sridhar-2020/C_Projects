#include "inv_srch.h"

/***********************************************************************************************************************************************
  	* Function to display database
	* RETURN     : SUCCESS / FAILURE
	* PARAMETERS : Hashtable
***********************************************************************************************************************************************/
int disp_database(hash_t arr[])
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

       printf("Index\t\t\tWord\t\t\tFilecount\t\tFilename\t\tWordcount\n");
       printf("-----------------------------------------------------------------------------------------------------\n");
       for (int index = 0; index < 28; index++)
       {
	      if (arr[index].link != NULL)
	      {
		     main_n *temp = arr[index].link;
		     while (temp)
		     {
			    int count = 0;
			    printf("[%d]\t\t\t[%s]\t\t\t[%d]", index, temp->word, temp->file_count);
			    sub_n *read = temp->sub_link;
			    while (read)
			    {
				   count++;
				   if (count > 1)
				   {
					  printf("\t\t\t\t\t\t\t\t\t[%s]\t\t\t[%d]\n", read->file_name, read->word_count);
				   }
				   else
					  printf("\t\t\t[%s]\t\t\t[%d]\n", read->file_name, read->word_count);

				   read = read->sub_link;
			    }
			    temp = temp->main_link;
		     }
	      }
       }
       return SUCCESS;
}
