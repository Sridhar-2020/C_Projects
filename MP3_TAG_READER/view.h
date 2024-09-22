#ifndef MP3_X
#define MP3_X

// Pre defined standard library
#include "types.h"

/*File that has all the function prototypes required */

/*Function to read and validate operation */
Status read_validate_operation(char *argv[]);

/*Function to open the mp3 file*/
Status open_mp3_file(char *argv[], TagReader *reader);

/* Function to skip the 10 bytes of header */
Status skip_header(TagReader *reader);

/*Function used to do the viewing */
Status view_option(char *argv[], TagReader *reader);

/* Function to read a tag */
Status read_tag(TagReader *reader, char string[]);

/*Function to fetch the size */
Status fetch_size(TagReader *reader, int *ptr);

/* Function to convert big endian to little endian */
Status convert_big_to_little(void *arr, int size);

/* Function to read the flag data from the mp3 file */
Status read_flag(TagReader *reader);

/* Function to extract content from the mp3 file*/
Status extract_content(TagReader *reader, int size, char string[]);

/*Function used to display the options to the user*/
Status display_help();

#endif
