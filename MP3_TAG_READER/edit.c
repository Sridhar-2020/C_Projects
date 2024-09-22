#include "edit.h"
#include "types.h"

/************************************************************************************************************************************************
  	* Function to validate whether used arguments in a proper order
	* RETURN VALUE : Status(int)
	* PARAMETERS   : 2d array (Array of strings), Structure pointer
**********************************************************************************************************************************************/
Status read_validate_arguments(char *argv[], TagEditor *editor)
{
       if (!strcmp(argv[2], "-t") || !strcmp(argv[2], "-a") || !strcmp(argv[2], "-A") || !strcmp(argv[2], "-Y")
		     || !strcmp(argv[2], "-C") || !strcmp(argv[2], "-M")) 
       {
	      if (strstr(argv[4], ".mp3") != NULL)
	      {
		     if (!strcmp(argv[4], "sample.mp3"))
			    return success;
	      }
       }
       return failure;
}

/************************************************************************************************************************************************
  	* Function to open the MP3 file
	* RETURN VALUE : Status(int)
	* PARAMETERS   : 2d array (Array of strings), Structure pointer
**********************************************************************************************************************************************/
Status open_files(char *argv[], TagEditor *editor)
{
       editor->fptr_temp = fopen("temp.mp3", "w");
       if ((editor->fptr_mp3  = fopen(argv[4], "r")) != NULL)
       {
	      return success;
       }
       return failure;
}

/************************************************************************************************************************************************
  	* Function to copy the header from the original file
	* RETURN VALUE : Status(int)
	* PARAMETERS   : Structure pointer
**********************************************************************************************************************************************/
Status copy_header(TagEditor *editor)
{
       // STEP 1 : Initialise an array of 10 bytes 
       // STEP 2 : Read 10 bytes of data from mp3 file 
       // STEP 3 : Write the data in the temporary file
       char arr[10];
       fread(arr, sizeof(arr), 1, editor->fptr_mp3);	
       fwrite(arr, sizeof(arr), 1, editor->fptr_temp);
       return success;
}

/************************************************************************************************************************************************
  	* Function to copy the tag from the original file
	* RETURN VALUE : Status(int)
	* PARAMETERS   : Structure pointer
**********************************************************************************************************************************************/
Status copy_tag(TagEditor *editor)
{
       // STEP 1 : Intialise an array of size 4 bytes
       // STEP 2 : Read 4 bytes of data from mp3 file 
       // STEP 3 : Write the data in the temporary file
       char arr[4];
       fread(arr, sizeof(arr), 1, editor->fptr_mp3);	
       fwrite(arr, sizeof(arr), 1, editor->fptr_temp);
       return success;
}

/************************************************************************************************************************************************
  	* Function to copy the size from the original file
	* RETURN VALUE : Status(int)
	* PARAMETERS   : Structure pointer, integer pointer
**********************************************************************************************************************************************/
Status copy_size(TagEditor *editor, int *ptr)
{
       // STEP 1 : Initialise an array of 4 bytes
       // STEP 2 : Read 3 bytes of flag from mp3 file
       // STEP 3 : Copy the 3 bytes of flag to temporary file
       int copy;
       fread(&copy, sizeof(copy), 1, editor->fptr_mp3);
       convert_big_to_little1(&copy,sizeof(copy));
       *ptr = copy - 1;
       convert_little_to_big(&copy, sizeof(copy));
       fwrite(&copy, sizeof(copy), 1, editor->fptr_temp);
       return success;
}

/************************************************************************************************************************************************
  	* Function to modify the size of the file
	* RETURN VALUE : Status(int)
	* PARAMETERS   : 2d array (Array of strings), Structure pointer
**********************************************************************************************************************************************/
Status modify_size(char *argv[], TagEditor *editor)
{
       // STEP 1 : Intialse an pointer variable ptr
       // STEP 2 : Find the length of string passed through command line argument
       // STEP 3 : Call the convert_little_to_big()
       // STEP 4 : Write the *ptr no of bytes to temporary file 
       int len = strlen(argv[3]) + 1 ;
       convert_little_to_big(&len, sizeof(len));
       fwrite(&len, sizeof(len), 1, editor->fptr_temp);
       return success;
}

/************************************************************************************************************************************************
  	* Function to fetch the size from the original file
	* RETURN VALUE : Status(int)
	* PARAMETERS   : Structure pointer, integer pointer
**********************************************************************************************************************************************/
Status fetch_size1(TagEditor *editor, int *ptr)
{
       // STEP 1 : Read 4 bytes of data from file
       // STEP 2 : Convert the data to little endian
       // STEP 3 : Store the converted data in a integer pointer
       int var;
       fread(&var, sizeof(var), 1, editor->fptr_mp3);
       convert_big_to_little1(&var, sizeof(var));
       *ptr = var - 1;

       return success;
}

/************************************************************************************************************************************************
  	* Function to convert the big endian value to little endian
	* RETURN VALUE : Status(int)
	* PARAMETERS   : 1d array, integer 
**********************************************************************************************************************************************/
Status convert_big_to_little1(void *arr, int size)
{
       // STEP 1 : Store the starting byte address in start 
       // STEP 2 : Store the ending byte address in end
       // STEP 3 : Increment the start and decrement the end
       // STEP 4 : Repeat this until start < end
       char *start = (char*)arr, *end = (char*)arr + size - 1 ;
       while (start < end)
       {
	      char temp = *start;
	      *start = *end;
	      *end = temp;
	      start++, end--;
       }
       return success;

}


/************************************************************************************************************************************************
  	* Function to convert the little endian value to big endian
	* RETURN VALUE : Status(int)
	* PARAMETERS   : 1D array, integer pointer
**********************************************************************************************************************************************/
Status convert_little_to_big(void *ptr, int size)
{
       char *start = (char*)ptr, *end = (char*)ptr + (size - 1); 
       while (start < end)
       {
	      char temp = *start;
	      *start = *end;
	      *end = temp;
	      start++, end--;
       }
       return success;
}

/************************************************************************************************************************************************
  	* Function to copy the flag from the original file
	* RETURN VALUE : Status(int)
	* PARAMETERS   : Structure pointer
**********************************************************************************************************************************************/
Status copy_flag(TagEditor *editor)
{
       // STEP 1 : Intialise an array of size 3 bytes
       // STEP 2 : Read 3 bytes of flag from mp3 file
       // STEP 3 : Copy the 3 bytes of flag to temporary file
       char arr[3];
       fread(arr, sizeof(arr), 1, editor->fptr_mp3);
       fwrite(arr, sizeof(arr), 1, editor->fptr_temp);
       return success;
}

/************************************************************************************************************************************************
  	* Function to modify the data in the file
	* RETURN VALUE : Status(int)
	* PARAMETERS   : 2d array (Array of strings), Structure pointer, integer
**********************************************************************************************************************************************/
Status modify_data(char *argv[], TagEditor *editor, int size)
{
       // STEP 1 : Copy the command line argument in a string
       // STEP 2 : Intialise an array of size equal to length of the string
       // STEP 3 : Write the string into the temp file
       // STEP 4 : Move the file pointer of mp3 to next tag using fseek
       char string[70];
       strcpy(string, argv[3]);
       int len = strlen(string) + 1;
       fwrite(string, len - 1, 1, editor->fptr_temp);		// Len - 1 is to write the data through command line argument to temp file
       fseek(editor->fptr_mp3, size, SEEK_CUR);			/* Pushpha-> len = 6 + 1, store len - 1 bytes of string in temp file */ 
       return success;
}

/************************************************************************************************************************************************
  	* Function to copy the data from the source file to destination file
	* RETURN VALUE : Status(int)
	* PARAMETERS   : Structure pointer, integer
**********************************************************************************************************************************************/
Status copy_data(TagEditor *editor, int size)
{
       // STEP 1 : Intialise an array of size bytes
       // STEP 2 : Read size no of bytes from the mp3 file and write to temp file
       char arr[size];
       fread(arr, sizeof(arr), 1, editor->fptr_mp3);
       fwrite(arr, sizeof(arr), 1, editor->fptr_temp);
       return success;
}

/************************************************************************************************************************************************
  	* Function to copy the remaining from the source file to the destination file
	* RETURN VALUE : Status(int)
	* PARAMETERS   : Structure pointer
**********************************************************************************************************************************************/
Status copy_remaining_data(TagEditor *editor)
{
       // STEP 1 : Intialise a character varaible
       // STEP 2 : Read the data from the mp3 file  
       // STEP 3 : Write the data to temp file
       char ch;
       while (fread(&ch, 1, 1, editor->fptr_mp3))
       {
	      fwrite(&ch, 1, 1, editor->fptr_temp);
       }
       return success;
}

/************************************************************************************************************************************************
  	* Function to edit the contents of the file
	* RETURN VALUE : Status(int)
	* PARAMETERS   : 2d array (Array of strings), Structure pointer
**********************************************************************************************************************************************/
Status edit_option(char *argv[], TagEditor *editor)
{
       // STEP 1 : Call read and validate arguments
       if(read_validate_arguments(argv, editor) != success)
       {
	      printf("ERROR : Validation failed!!!\n");
	      return failure;
       }
       // STEP 2 : Call open_files()
       if (open_files(argv, editor) != success)
       {
	      printf("ERROR : Operation failed in opening files!!!\n");
	      return failure;
       }
       // STEP 3 : Call copy 10 bytes of header
       copy_header(editor);

       // STEP 4 : Call copy tag
       copy_tag(editor);

       // STEP 5 : Call fetch_size
       if (!strcmp(argv[2], "-t"))
       {
	      modify_size(argv, editor);
	      fetch_size1(editor, &(editor->size_tag1));
       }
       else
       {
	      // STEP 6 : Call copy_size()
	      copy_size(editor, &(editor->reader_tag1_size));
       }

       // STEP 7 : Call copy flag
       copy_flag(editor);

       // STEP 8 : Call modify_data()
       if (!strcmp(argv[2], "-t"))
       {
	      modify_data(argv, editor, editor->size_tag1);
       }
       // STEP 9 : Call copy_data() 
       else
       {
	      copy_data(editor, (editor->reader_tag1_size));
       }

       // STEP 10 : Call copy tag
       copy_tag(editor);

       // STEP 11 : Call fetch_size
       if (!strcmp(argv[2], "-a"))
       {
	      modify_size(argv, editor);
	      fetch_size1(editor, &(editor->size_tag2));
       }
       else
       {
	      // STEP 12 : Call copy_size()
	      copy_size(editor, &(editor->reader_tag2_size));
       }
       // STEP 13 : Call copy flag
       copy_flag(editor);

       // STEP 14 : Call modify_data()
       if (!strcmp(argv[2], "-a"))
       {
	      modify_data(argv, editor, editor->size_tag2);
       }
       // STEP 15 : Call copy_data() 
       else
       {
	      copy_data(editor, (editor->reader_tag2_size));
       }

       // STEP 16 : Call copy tag
       copy_tag(editor);

       // STEP 17 : Call fetch_size
       if (!strcmp(argv[2], "-A"))
       {
	      modify_size(argv, editor);
	      fetch_size1(editor, &(editor->size_tag3));
       }
       else
       {
	      // STEP 18 : Call copy_size()
	      copy_size(editor, &(editor->reader_tag3_size));
       }

       // STEP 19 : Call copy flag
       copy_flag(editor);

       // STEP 20 : Call modify_data()
       if (!strcmp(argv[2], "-A"))
       {
	      modify_data(argv, editor, editor->size_tag3);
       }
       // STEP 21 : Call copy_data() 
       else
       {
	      copy_data(editor, editor->reader_tag3_size);
       }
       // STEP 22 : Call copy tag
       copy_tag(editor);

       // STEP 23 : Call fetch_size
       if (!strcmp(argv[2], "-Y"))
       {
	      modify_size(argv, editor);
	      fetch_size1(editor, &(editor->size_tag4));
       }
       else
       {
	      // STEP 24 : Call copy_size()
	      copy_size(editor, &(editor->reader_tag4_size));
       }
       // STEP 25 : Call copy flag
       copy_flag(editor);

       // STEP 26 : Call modify_data()
       if (!strcmp(argv[2], "-Y"))
       {
	      modify_data(argv, editor, editor->size_tag4);
       }
       // STEP 27 : Call copy_data() 
       else
       {
	      copy_data(editor, editor->reader_tag4_size);
       }
       // STEP 28 : Call copy tag
       copy_tag(editor);

       // STEP 29 : Call fetch_size
       if (!strcmp(argv[2], "-M"))
       {
	      modify_size(argv, editor);
	      fetch_size1(editor, &(editor->size_tag5));
       }
       else
       {
	      // STEP 30 : Call copy_size()
	      copy_size(editor, &(editor->reader_tag5_size));
       }
       // STEP 31 : Call copy flag
       copy_flag(editor);

       // STEP 32 : Call modify_data()
       if (!strcmp(argv[2], "-M"))
       {
	      modify_data(argv, editor,editor->size_tag5);
       }
       // STEP 33 : Call copy_data() 
       else
       {
	      copy_data(editor, editor->reader_tag5_size);
       }
       // STEP 34 : Call copy tag
       copy_tag(editor);

       // STEP 35 : Call fetch_size
       if (!strcmp(argv[2], "-C"))
       {
	      modify_size(argv, editor);
	      fetch_size1(editor, &(editor->size_tag6));
       }
       else
       {
	      // STEP 36 : Call copy_size()
	      copy_size(editor, &(editor->reader_tag6_size));
       }
       // STEP 37 : Call copy flag
       copy_flag(editor);

       // STEP 38 : Call modify_data()
       if (!strcmp(argv[2], "-C"))
       {
	      modify_data(argv, editor, editor->size_tag6);
       }
       // STEP 39 : Call copy_data() 
       else
       {
	      copy_data(editor, editor->reader_tag6_size);
       }

       // STEP 40 : Call copy_remaining_data() 
       copy_remaining_data(editor);

       // This character decides whether to store the contents of original or simply overwrite them
       char ch;
       printf("Do you wish to save the contents of the original file?\nIf yes press Y or y else n or N : ");
       scanf(" %c", &ch);
       if (ch != 'Y' && ch != 'y')
       {
	      remove("sample.mp3");
	      rename ("temp.mp3", "sample.mp3");
       }
       else
       {
	      rename ("temp.mp3", "edited.mp3");
       }
       fclose(editor->fptr_mp3);
       fclose(editor->fptr_temp);
}
