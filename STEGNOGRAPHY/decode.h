#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * decoding output.bmp to another file
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
       /*Output File Info */
       int decoded_file_extn_size;
       char decoded_file_name[15];
       char *decoded_file_extn;
       FILE *fptr_decoded;


       /* Secret File Info */
       int secret_file_size;

       /* Stegged Image Info */
       char *stegged_image_fname;
       FILE *fptr_stegged_image;

} DecodeInfo;



/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status dec_open_files(DecodeInfo *decInfo);

/* Store Magic String */
Status decode_magic_string(FILE *fptr_stegged_image);

/* Decode secret file extension size */
Status decode_secret_file_extn_size(FILE *fptr_stego_image, DecodeInfo *decInfo);

/* Decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/*Open the decoded file */
Status open_decoded_file(DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode function, which does the real encoding */
Status decode_data_from_image(int size, FILE *fptr_stegged_image, DecodeInfo *decInfo);

/* Decode a byte into LSB of image data array */
Status decode_byte_from_lsb(char* ch, char arr[]);

/* Decode the size into LSB of image data array */
Status decode_size_from_lsb(char arr[], DecodeInfo *decInfo);

#endif
