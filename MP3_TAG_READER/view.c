#include "view.h"
#include "types.h"

/************************************************************************************************************************************************
  	* Function to display the options to the user
	* RETURN VALUE : VOID
	* PARAMETERS   : VOID
**********************************************************************************************************************************************/
Status display_help()
{
       printf("USAGE : \n");
       printf("-t Modifies a Title tag\n");
       printf("-T Modifies a Track tag\n");
       printf("-a Modifies a Artist tag\n");
       printf("-A Modifies a Album tag\n");
       printf("-y Modifies a Year tag\n");
       printf("-c Modifies a Comment tag\n");
       printf("-v Prints version info\n");
       printf("-h Displays this help info\n");
}

/************************************************************************************************************************************************
  	* Function to check whether user passed the arguments in a corret manner
	* RETURN VALUE : Status(int)
	* PARAMETERS   : 2D array (Array of strings)
**********************************************************************************************************************************************/
Status read_validate_operation(char *argv[])
{
       // STEP 1 : Check whether passed file is mp3 file or not
       // STEP 2 : If the passed file is a mp3 file then check whether it is present or not
       if (strstr(argv[2], ".mp3") != NULL)
       {
	      //if (!strcmp(argv[2], "sample.mp3"))
		     return success;
       }
       return failure;
}

/************************************************************************************************************************************************
  	* Function to open the MP3 file
	* RETURN VALUE : Status(int)
	* PARAMETERS   : 2d array, Struncture pointer
**********************************************************************************************************************************************/
Status open_mp3_file(char *argv[], TagReader *reader)
{
       // STEP 1 : Open the file in the read mode
       // STEP 2 : If the FILE pointer value is NULL return failure else return success
       reader->fptr_mp3 = fopen(argv[2], "r");
       if (reader->fptr_mp3 == NULL)
       {
      printf("Unable to open the file\n");
	      return failure;
       }
       return success;
}

/************************************************************************************************************************************************
  	* Function to skip the header of the MP3 file
	* RETURN VALUE : Status(int)
	* PARAMETERS   : Structure pointer
**********************************************************************************************************************************************/
Status skip_header(TagReader *reader)
{
       // STEP 1 : Initialise a variable with 10 bytes of size
       // STEP 2 : Read 10 bytes of data from the mp3 file and skip the header
       char arr[10];
       fread(arr, sizeof(arr), 1, reader->fptr_mp3);
       return success;
}

/************************************************************************************************************************************************
  	* Function to read the tag 
	* RETURN VALUE : Status(int)
	* PARAMETERS   : Structure pointer, 1d array
**********************************************************************************************************************************************/
Status read_tag(TagReader *reader, char string[])
{
       // STEP 1 : Read 4 bytes of data from the file 
       // STEP 2 : Copy the 4 bytes of data to the string[]
       char arr[5];
       fread(arr, sizeof(arr) - (unsigned)1, 1, reader->fptr_mp3);
       arr[4] = '\0';
       return success;
}

/************************************************************************************************************************************************
  	* Function to fetch the size of the file
	* RETURN VALUE : Status(int)
	* PARAMETERS   : Structure pointer, Integer pointer
**********************************************************************************************************************************************/
Status fetch_size(TagReader *reader, int *ptr)
{
       // STEP 1 : Read 4 bytes of data from file
       // STEP 2 : Convert the data to little endian
       // STEP 3 : Store the converted data in a integer pointer
       int var;
       fread(&var, sizeof(var), 1, reader->fptr_mp3);
       convert_big_to_little(&var, sizeof(var));
       *ptr = var - 1;

       return success;
}

/************************************************************************************************************************************************
  	* Function to convert big endian to little endian
	* RETURN VALUE : Status(int)
	* PARAMETERS   : 1d array, integer
**********************************************************************************************************************************************/
Status convert_big_to_little(void *arr, int size)
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
  	* Function to read the flag of the file
	* RETURN VALUE : Status(int)
	* PARAMETERS   : Structure pointer
**********************************************************************************************************************************************/
Status read_flag(TagReader *reader)
{
       // STEP 1 : Intialise 3 bytes of array
       // STEP 2 : Read the flag data from the mp3 file
       char arr[3];
       fread(arr, sizeof(arr), 1, reader->fptr_mp3);
       return success;
}

/************************************************************************************************************************************************
  	* Function to extract the contents of a MP3 file
	* RETURN VALUE : Status(int)
	* PARAMETERS   : Structure pointer, integer, 1d array
**********************************************************************************************************************************************/
Status extract_content(TagReader *reader, int size, char string[])
{
       // STEP 1 : Intialise an array size bytes
       // STEP 2 : Read the size bytes of data from the mp3 file
       // STEP 3 : Store the data in the required structure member
       char arr[size + 1];
       fread(arr,size, 1, reader->fptr_mp3);
       // convert_big_to_little(arr, sizeof(arr)); 	DATA NEED NOT BE CONVERTED TO LITTLE ENDIAN IF IT IS A CHAR ARRAY
       arr[size] = '\0';
       strcpy(string, arr);
       return success;
}

/************************************************************************************************************************************************
  	* Function to display the contents of the file
	* RETURN VALUE : Status(int)
	* PARAMETERS   : 2d array (Array of strings), Structure pointer
**********************************************************************************************************************************************/
Status view_option(char *argv[], TagReader *reader)
{
       // STEP 1 : Read and validate operation
       if (read_validate_operation(argv) != success)
       {
	      printf("ERROR : Please pass the arguments in a correct manner!!!\n");
	      return failure;
       }

       // STEP 2 : open_mp3_file()
       if (open_mp3_file(argv, reader) != success)
       {
	      printf("ERROR : Error occurred in opening the mp3 file!!!\n");
	      return failure;
       }

       // STEP 3 : Skip the 10 bytes of header
       skip_header(reader);

       // STEP 4 : Call read_tag()
       read_tag(reader, reader->tag1);

       // STEP 5 : Call fetch_size()
       fetch_size(reader, &(reader->size_tag1));
       //  (reader->size_tag1)--;			// IF we are modifying the size here, if we call the fetch_size func again
       // then it will fetch 3 bytes of data from size and one byte from the content and three bytes from size and we convert big endian to 
       //little hence we will get very huge value of size if we try to read the size number of bytes in the mp3 file we get segmentation fault...

       // STEP 6 : Call read_flag()
       read_flag (reader); 

       // STEP 7 : Call extract_content()
       extract_content(reader, reader->size_tag1, reader->song_name);

       // STEP 8 : Call read_tag()
       read_tag(reader, reader->tag2);

       // STEP 9 : Call fetch_size()
       fetch_size(reader, &(reader->size_tag2));

       //STEP 10 : Call read flag()
       read_flag(reader);

       // STEP 11 : Call extract_content()
       extract_content(reader, reader->size_tag2, reader->artist_name);

       // STEP 12 : Call read_tag()
       read_tag(reader, reader->tag3);

       // STEP 13 : Call fetch_size()
       fetch_size(reader, &(reader->size_tag3));

       //STEP 14 : Call read flag()
       read_flag(reader);

       // STEP 15 : Call extract_content()
       extract_content(reader, reader->size_tag3, reader->album_name);

       // STEP 13 : Call read_tag()
       read_tag(reader, reader->tag4);

       // STEP 14 : Call fetch_size()
       fetch_size(reader, &(reader->size_tag4));

       //STEP 15 : Call read flag()
       read_flag(reader);

       // STEP 16 : Call extract_content()
       extract_content(reader, reader->size_tag4, reader->year);

       // STEP 17 : Call read_tag()
       read_tag(reader, reader->tag5);

       // STEP 18 : Call fetch_size()
       fetch_size(reader, &(reader->size_tag5));

       //STEP 19 : Call read flag()
       read_flag(reader);

       // STEP 20 : Call extract_content()
       extract_content(reader, reader->size_tag5, reader->genre);

       // STEP 21 : Call read_tag()
       read_tag(reader, reader->tag6);

       // STEP 22 : Call fetch_size()
       fetch_size(reader, &(reader->size_tag6));

       //STEP 23 : Call read flag()
       read_flag(reader);

       // STEP 24 : Call extract_content()
       extract_content(reader, reader->size_tag6, reader->comment);

       // STEP 25 : Close the fptr_mp3
       fclose(reader->fptr_mp3);

       printf("---------------------------------------SELECTED VIEW DETAILS------------------------------------------------------\n\n\n");
       printf("------------------------------------------------------------------------------------------------------------------\n");
       printf("\t\t\t\tMP3 TAG READER AND EDITOR FOR ID3v2\n");
       printf("------------------------------------------------------------------------------------------------------------------\n");
       printf("TITLE  : \t%s\n",reader->song_name);
       printf("ARTIST : \t%s\n",reader->artist_name);
       printf("ALBUM  : \t%s\n",reader->album_name);
       printf("YEAR   : \t%s\n",reader->year);
       printf("MUSIC  : \t%s\n",reader->genre);
       printf("COMMENT: \t%s\n",reader->comment);
       printf("------------------------------------------------------------------------------------------------------------------\n\n\n");
       printf("-------------------------------------DETAILS DISPLAYED SUCCESSFULLY----------------------------------------------------\n");
}
