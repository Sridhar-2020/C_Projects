#include <stdio.h>
#include<string.h>
#include<unistd.h>
#include "encode.h"
#include "types.h"
#include "common.h"

int extn_size;
/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
       uint width, height;
       // Seek to 18th byte
       fseek(fptr_image, 18, SEEK_SET);

       // Read the width (an int)
       fread(&width, sizeof(int), 1, fptr_image);
       //printf("width = %u\n", width);

       // Read the height (an int)
       fread(&height, sizeof(int), 1, fptr_image);
       //printf("height = %u\n", height);

       // Return image capacity
       return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
       // Src Image file
       encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
       // Do Error handling
       if (encInfo->fptr_src_image == NULL)
       {
	      perror("fopen");
	      fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

	      return e_failure;
       }

       // Secret file
       encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
       // Do Error handling
       if (encInfo->fptr_secret == NULL)
       {
	      perror("fopen");
	      fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

	      return e_failure;
       }

       // Stego Image file
       encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
       // Do Error handling
       if (encInfo->fptr_stego_image == NULL)
       {
	      perror("fopen");
	      fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

	      return e_failure;
       }

       // No failure return e_success
       return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
       // Size of src_image, sizeof extension, sizeof secretfile
       int size_src_img = get_image_size_for_bmp(encInfo->fptr_src_image);
       fseek(encInfo->fptr_secret, 0 ,SEEK_END);
       int size_secret_file = ftell(encInfo->fptr_secret);
       if (strstr(encInfo->secret_fname, ".txt") != NULL)
       {
	      extn_size = 4;
	      encInfo->extn_secret_file = ".txt";  
       }
       else if (strstr(encInfo->secret_fname, ".c") != NULL)
       {
	      extn_size = 2;
	      encInfo->extn_secret_file = ".c";
       }
       else if (strstr(encInfo->secret_fname, ".sh") != NULL)
       {
	      extn_size = 3;
	      encInfo->extn_secret_file = ".sh";
       }
       else if (strstr(encInfo->secret_fname, ".docx") != NULL)
       {
	      extn_size = 5;
	      encInfo->extn_secret_file = ".docx";
       }

       if (size_src_img >= 54 + 16 + 32 +(extn_size * 8) + 32 + (size_secret_file*8))
       {
	      return e_success;
       }
       else
       {
	      return e_failure;
       }
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_stego_image)
{
       char arr[54];
       rewind(fptr_src_image);
       fread(arr, sizeof(arr), 1, fptr_src_image);
       if (fwrite(arr, 1, 54, fptr_stego_image) == sizeof(arr))
       {
	      return e_success;
       }
       else
	      return e_failure;
}

Status encode_magic_string(char *magic_string, FILE *fptr_src_image, FILE *fptr_stego_image)
{
       // STEP 1: Call encode_data_to_image(magic_string, fptr_src_image, fptr_stego_image)
       encode_data_to_image(magic_string,fptr_src_image, fptr_stego_image);
       return e_success;
}

Status encode_data_to_image(char *data, FILE *fptr_src_image, FILE *fptr_stego_image)
{
       //STEP 1 : Read 8 bytes of data from the source_img_file
       // STEP 2: Fetch one byte of data from data pointer
       // STEP 3: Encode part  -> encode_byte_to_lsb()
       // STEP 4: Write the encoded data into stego image
       // STEP 5: Repeat for the length of the data times
       char image_buffer[8];
       int len = strlen(data);
       for (int i = 0; i < len; i++)
       {
	      fread (image_buffer, sizeof(image_buffer), 1, fptr_src_image);
	      char ch = data[i];
	      encode_byte_to_lsb(ch, image_buffer);
	      fwrite(image_buffer, sizeof(image_buffer), 1, fptr_stego_image);
       }
       return e_success;
}

Status encode_byte_to_lsb(char ch, char image_buffer[])
{
       // STEP 1: Clear the LSB bit of image_buffer[0]
       // Step 2 : Get LSB bit of ch
       // Step 3 : Or with the image_buffer[0]
       // Step 4 : Run For loop for 8 times
       unsigned int get_bit;
       for (int i = 0; i < 8; i++)
       {
	      image_buffer[i] = image_buffer[i] & (~1); 		// Clearing the LSB bit of image_buffer
	      get_bit = ch & (1 << i);					// Get bit of ch
	      get_bit = (get_bit) >> i;					// Right shift by i times
	      image_buffer[i] = image_buffer[i] | get_bit;		// Encoding the data in image_buffer
       }
       return e_success;
}

Status encode_secret_file_extn_size(FILE *fptr_src_image, FILE *fptr_stego_image)
{
       // STEP 1 : Read 32 bytes of data from the source file and store it in temp_arr
       // STEP 2 : Call encode_size_to_lsb()
       // STEP 3: Write the encoded data into steggo image
      char temp_arr[32];
      fread(temp_arr, sizeof(temp_arr), 1, fptr_src_image);
      encode_size_to_lsb(extn_size, temp_arr);				// Global variable declared extn_size
      fwrite(temp_arr, sizeof(temp_arr), 1, fptr_stego_image);
      return e_success;
}

Status encode_size_to_lsb(int size, char temp_arr[])
{
       // STEP 1 : Clear a bit from arr[0]
       // Step 2 : Get a bit from size
       // Step 3 : Replace -> STEP1 | STEP2
       // Step 4 : Repeat for 32 times.
       unsigned get_bit;
       for (int i = 0; i < 32; i++)
       {
	      temp_arr[i] = temp_arr[i] & (~1);			// Clearing the LSB bit
	      get_bit = (size) & (1 << i);			// Getting a bit from size
	      get_bit = get_bit >> i;				// Right shift by i 
	      temp_arr[i] = temp_arr[i] | get_bit;		// Encoding the data in temp_arr
       }
       return e_success;
}

Status encode_secret_file_extn(char *extn, FILE *fptr_src_image, FILE *fptr_stego_image)
{
       // STEP 1 : Call encode_data_to_image()
       encode_data_to_image(extn, fptr_src_image, fptr_stego_image);
       return e_success;
}

Status encode_secret_file_size(FILE *fptr_src_image, FILE * fptr_secret, FILE *fptr_stego_image)
{
       char arr[32];
       int size_secret_file = ftell(fptr_secret);
       fread(arr, sizeof(arr), 1, fptr_src_image);
       encode_size_to_lsb(size_secret_file, arr);
       fwrite(arr, sizeof(arr), 1, fptr_stego_image);
       return e_success;
}

Status encode_secret_file_data(FILE *fptr_src_image, FILE *fptr_secret, FILE *fptr_stego_image)
{
       // STEP 1 : REwind the secret file
       // STEP 2 : Read the entire data from secret file and store into temp_arr
       // STEP 3 : Call encode_data_to_image()
       int size_secret_file = ftell(fptr_secret);
       rewind(fptr_secret);
       char arr[size_secret_file];
       fread(arr, sizeof(arr), 1, fptr_secret);
       encode_data_to_image(arr, fptr_src_image, fptr_stego_image);
       return e_success;
}


Status copy_remaining_img_data(FILE *fptr_src_image, FILE *fptr_stego_image)
{
       char ch;
       while (fread(&ch, 1, 1, fptr_src_image))
	      fwrite(&ch, 1, 1, fptr_stego_image);
       return e_success;
}


Status do_encoding(EncodeInfo *encInfo)
{
       // STEP 1: Call open_files(encInfo)
       if (open_files(encInfo) == e_success)
       {
	      printf("INFO : All the required files are successfully opened...\n\n");
       }
       else
       {
	      printf("ERROR : Files are not opened succesfully!!!!\n\n");
	      return e_failure;
       }
       sleep(1);
       // STEP 2: Call check_capacity(encInfo)
       if (check_capacity(encInfo) == e_success)
       {
	      printf("INFO : Secret file can be encoded in our image file...\n\n");
       }
       else
       {
	      printf("ERROR : Secret file cannot be encoded in image file!!!\n\n");
	      return e_failure;
       }
       sleep(1);
       // STEP 3: Call copy_bmp_header(encInfo)
       if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
       {
	      printf("INFO : BMP header copied to stego file successfully...\n\n");
       }
       else 
       {
	      printf("ERROR : BMP header not copied to stego file..\n\n");
	      return e_failure;
       }
       sleep(1);
       // STEP 4 : Call encode_magic_string(magic_string,encInfo->fptr_src_image, encInfo->fptr_stego_image)
       if (encode_magic_string(MAGIC_STRING,encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
       {
	      printf("INFO : Successfully encoded Magic String...\n\n");
       }
       else
       {
	      printf("ERROR : Encoding magic string operation failed\n\n");
	      return e_failure;
       }
       sleep(1);

       // STEP 5 : Call encode_secret_file_extn_size()  Check success or not
       if (encode_secret_file_extn_size(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
       {
	      printf("INFO : Successfully encoded secret file extension size...\n\n");
       }
       else
       {
	      printf("ERROR : Encoding secret file extension size operation failed\n\n");
	      return e_failure;
       }
       sleep(1);

       // STEP 6 : Call encode_secret_file_extn()
       if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
       {
	      printf("INFO : Successfully encoded secret file extension...\n\n");
       }
       else
       {
	      printf("ERROR : Encoding secret file extension operation failed\n\n");
	      return e_failure;
       }
       sleep(1);

       // STEP 7 : Call enocde_secret_file_size()
       if (encode_secret_file_size(encInfo->fptr_src_image, encInfo->fptr_secret, encInfo->fptr_stego_image) == e_success)
       {
	      printf("INFO : Successfully encoded secret file size...\n\n");
       }
       else
       {
	      printf("ERROR : Encoding secret file size operation failed\n\n");
	      return e_failure;
       }
       sleep(1);

       // STEP 8 : call encode_secret_file_data()
       if (encode_secret_file_data(encInfo->fptr_src_image, encInfo->fptr_secret, encInfo->fptr_stego_image) == e_success)
       {
	      printf("INFO : Successfully encoded secret file data...\n\n");
       }
       else
       {
	      printf("ERROR : Encoding secret file data operation failed\n\n");
	      return e_failure;
       }
       sleep(1);

       // STEP 9 : Call copy_remaining_img_data()
       if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
       {
	      printf("INFO : Successfully copied remaining image data...\n\n");
       }
       else
       {
	      printf("ERROR : Copying image data operation failed\n\n");
	      return e_failure;
       }
       sleep(1);
       printf("\t\t\t::::ENCODED SUCCESSFULLY::::\n");
}
