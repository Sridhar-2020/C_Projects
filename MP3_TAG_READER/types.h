#ifndef MP3
#define MP3

/* Standard Libraries*/
#include <stdio.h>
#include <string.h>

/*Enum declaration for the purpose of the readability*/
typedef enum
{
       failure,
       success

}Status;

/* Structure to have the accessibility of members throughout the program */
typedef struct Tag_Reader_MP3
{
       FILE *fptr_mp3;
       int size_tag1;
       int size_tag2;
       int size_tag3;
       int size_tag4;
       int size_tag5;
       int size_tag6;
       char tag1[5];
       char song_name[70];
       char tag2[5];
       char artist_name[70];
       char tag3[5];
       char album_name[70];
       char tag4[5];
       char year[70];
       char tag5[5];
       char genre[70];
       char tag6[5];
       char comment[70];

}TagReader;

/* Structure to have the accessibility of members throughout the program */
typedef struct Tag_Editor_MP3
{
       FILE *fptr_temp;					// FILE POINTER FOR TEMP FILE
       FILE *fptr_mp3;					// FILE POINTER FOR THE MP3 file
       int size_tag1;
       int size_tag2;
       int size_tag3;
       int size_tag4;
       int size_tag5;
       int size_tag6;
       int reader_tag1_size;
       int reader_tag2_size;
       int reader_tag3_size;
       int reader_tag4_size;
       int reader_tag5_size;
       int reader_tag6_size;

}TagEditor;

#endif
