#include <stdio.h>
#include<string.h>
#include<unistd.h>
#include "decode.h"
#include "types.h"
#include "common.h"

int flag = 1, flag1 = 1;							// Global variables declared
Status dec_open_files(DecodeInfo *decInfo)
{
       // STEP1 : Open the output.bmp in read mode
       decInfo->fptr_stegged_image = fopen(decInfo->stegged_image_fname, "r");
       if (decInfo->fptr_stegged_image == NULL)
       {
	      fprintf(stderr, "Error in opening the file\n");
	      return e_failure;
       }
       return e_success;
}

Status decode_magic_string(FILE *fptr_stegged_image)
{
       // STEP 1 : Move the file pointer to 54th byte
       // STEP 2 : Find the lenght of MAGIC_STRING
       // STEP 3 : Check whether the decoded MAGIC_STRING is matched or not
       // STEP 4 : If mathced continue else return e_failure
       fseek(fptr_stegged_image, 54, SEEK_SET);
       int len = strlen(MAGIC_STRING);
       char temp_arr[len + 1];
       int get_bit;
       for (int i = 0 ; i < len; i++)
       {
	      char arr[8], ch = 0;
	      fread(arr, sizeof(arr), 1, fptr_stegged_image);
	      for (int i = 0; i < 8; i++)
	      {
		     get_bit = arr[i] & 1;
		     get_bit = get_bit << i;
		     ch = ch | get_bit;
	      }
	      temp_arr[i] = ch;
       }
       temp_arr[len] = '\0';
       if (strcmp(temp_arr, MAGIC_STRING) == 0) return e_success;
       else return e_failure;
}

Status decode_data_from_image(int size, FILE *fptr_stegged_image, DecodeInfo *decInfo)
{
       //STEP 1 : Read 8 bytes of data from the output_img_file
       // STEP 2: Decode part  -> decode_byte_from_lsb()
       // STEP 3: Write the decoded data into decoded_file
       // STEP 4: Repeat for the length of the data times
       char temp_arr[size];						// An array to store the decoded data
       for (int i = 0; i < size; i++)
       {
	      char arr[8], ch = 0;					// A character array of size 8 bytes to read data from output.bmp
	      fread (arr, sizeof(arr), 1, fptr_stegged_image);		// Reading data.....
	      decode_byte_from_lsb(&ch, arr);				
	      temp_arr[i] = ch;						// Decoded data is collected in temp_arr
       }
       temp_arr[size] = '\0';
       if (flag1)							// Flag1 global variable declared
       {
	      strcat(decInfo->decoded_file_name, temp_arr);
	      flag1 = 0;
       }
       else
       {
	      fwrite(temp_arr, sizeof(temp_arr), 1, decInfo->fptr_decoded);
       }
       return e_success;
}

Status decode_byte_from_lsb(char* ch, char arr[])
{
       // STEP 1 : Declare get_bit
       // STEP 2 : Get a bit from LSB of first byte
       // STEP 3 : Decode a byte from 8 bytes
       // STEP 4 : Run for 8 times
       int get_bit;
       for (int i = 0; i < 8; i++)
       {
	      get_bit = arr[i] & 1;					// Getting LSB bit from 8 bytes
	      get_bit = get_bit << i;					// Left shift by i times
	      *ch = *ch | get_bit;					// Decoded a bit
       }
       return e_success;
}

Status decode_secret_file_extn_size(FILE *fptr_stegged_image, DecodeInfo *decInfo)
{
       // STEP 1 : Read 32 bytes of data from the source file (output.bmp) and store it in arr
       // STEP 2 : Call decode_size_to_lsb()
       char arr[32];
       fread(arr, sizeof(arr), 1, fptr_stegged_image); 
       decode_size_from_lsb(arr,decInfo);
       return e_success;
}

Status decode_size_from_lsb(char arr[], DecodeInfo *decInfo)
{
       // STEP 1 : Declare get_bit integer
       // STEP 2 : Get a bit from LSB of first byte
       // STEP 3 : Decode a byte from 8 bytes
       // STEP 4 : Run for 32 times
       int get_bit, size = 0;
       for(int i = 0 ; i < 32; i++)
       {
	      get_bit = arr[i] & 1;
	      get_bit = get_bit << i;
	      size = size | get_bit;
       }
       if (flag)								// Flag varaiable declared globally
       {
	      decInfo->decoded_file_extn_size = size; 
	      flag = 0;
       }
       else
       {
	      decInfo->secret_file_size = size;
       }
       return e_success;

}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
       // STEP 1 : call decode_data_from_image()
       decode_data_from_image(decInfo->decoded_file_extn_size, decInfo->fptr_stegged_image, decInfo);
       return e_success;
}

Status open_decoded_file(DecodeInfo *decInfo)
{
       // STEP 1 : Open the decoded_file in write mode
       decInfo->fptr_decoded = fopen(decInfo->decoded_file_name, "a");
       return e_success;
}


Status decode_secret_file_size(DecodeInfo *decInfo)
{
       // STEP 1 : Declare 32 bytes of array
       // STEP 2 : Read 32 bytes of data from the output.bmp file
       // STEP 3 : Call decode_size_from_lsb()
       char arr[32];
       fread(arr, sizeof(arr), 1, decInfo->fptr_stegged_image);
       decode_size_from_lsb(arr, decInfo);
       return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
       // STEP 1 : Call decode_data_from_image()
       // STEP 2 : Close the decoded file file pointer
       decode_data_from_image(decInfo->secret_file_size, decInfo->fptr_stegged_image, decInfo);
       fclose(decInfo->fptr_decoded);				// Closing the decoded_file file pointer
       return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
       // STEP 1 : Call dec_open_files()
       if (dec_open_files(decInfo) == e_success)
       {
	      printf("INFO : File opened successfully...\n\n");
       }
       else
       {
	      printf("ERROR : File not opened!!!\n\n");
	      return e_failure;
       }
       sleep(1);

       // STEP 2 : Call decode_magic_string()
       if (decode_magic_string(decInfo->fptr_stegged_image) == e_success)
       {
	      printf("INFO : Magic string matched successfully...\n\n");
       }
       else
       {
	      printf("ERROR : Magic string not matched!!!\n\n");
	      return e_failure;
       }
       sleep(1);

       // STEP 3 : Call decode_secret_file_extn_size()
       if (decode_secret_file_extn_size(decInfo->fptr_stegged_image,decInfo) == e_success)
       {
	      printf("INFO : Extension size found successfully...\n\n");
       }
       else
       {
	      printf("ERROR : Extension size not found!!!\n\n");
	      return e_failure;
       }
       sleep(1);

       // STEP 4 : Call decode_secret_file_extn()
       if (decode_secret_file_extn(decInfo) ==  e_success)
       {
	      printf("INFO : Extension type found successfully...\n\n");
       }
       else
       {
	      printf("ERROR : Extension type not found!!!\n\n");
	      return e_failure;
       }
       sleep(1);

       // STEP 5 : Call open_decoded_file()
       if (open_decoded_file(decInfo) ==  e_success)
       {
	      printf("INFO : Decoded file opened successfully...\n\n");
       }
       else
       {
	      printf("ERROR : File not found!!!\n\n");
	      return e_failure;
       }
       sleep(1);

       // STEP 6 : Call decode_secret_file_size()
       if (decode_secret_file_size(decInfo) == e_success)
       {
	      printf("INFO : Successfully decoded secret file size...\n\n");
       }
       else
       {
	      printf("ERROR : Decoding secret file size operation failed\n\n");
	      return e_failure;
       }
       sleep(1);

       // STEP 7 : Call decode_secret_file_data()
       if (decode_secret_file_data(decInfo) == e_success)
       {
	      printf("INFO : Successfully decoded secret file data...\n\n");
       }
       else
       {
	      printf("ERROR : Decoding secret file data operation failed\n\n");
	      return e_failure;
       }
       sleep(1);
       printf("\t\t\t:::DECODED SUCCESSFULLY:::\n");
       return e_success;
}
