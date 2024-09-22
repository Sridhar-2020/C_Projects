#include "types.h"
#ifndef EDITOR
#define EDITOR
/* Header file to write all the function prototypes that are required for editing */

/*Function to read and validate */
Status read_validate_arguments(char *argv[], TagEditor *editor);

/*Function to open files */
Status open_files(char *argv[], TagEditor *editor);

/*Function to copy the header from the source mp3 file to temp file */
Status copy_header(TagEditor *editor);

/*Function to copy the tag from the source mp3 file to temp file */
Status copy_tag(TagEditor *editor);

/* Function to copy the size from the mp3 file to temp file*/
Status copy_size(TagEditor *editor, int *ptr);

/*Function to fetch the size from the cla and copy to temp file */
Status modify_size(char *argv[], TagEditor *editor);

/* Function to fetch the size from the mp3 file */
Status fetch_size1(TagEditor *editor, int *ptr);

/* Function to convert big endian to little endian*/
Status convert_big_to_little1(void *arr, int size);

/* Function to convert little endian to big endian*/
Status convert_little_to_big(void *ptr, int size);

/* Function to copy the flag from the source mp3 file to temp file */
Status copy_flag(TagEditor *editor);

/* Function to modify the data from the command line argument to temp file*/
Status modify_data(char *argv[], TagEditor *editor, int size);

/* Function to copy the data from mp3 to temp file*/
Status copy_data(TagEditor *editor, int size);

/* Function to copy the remaining data to temp file*/
Status copy_remaining_data(TagEditor *editor);

/*Function used to do the editing */
Status edit_option(char *argv[], TagEditor *editor);

#endif
