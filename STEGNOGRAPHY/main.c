/* Name : Sridhar
Date : 23/05/24
Description: STEGNOGRAPHY PROJECT
Sample input:
Sample output:
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main()
{
	int option;
	printf("\t\t\tMENU:::::\n");
	printf("Please select an option from the below\n");
	printf("1.Encode\n2.Decode\n3.Exit\n");
	scanf("%d", &option);

	EncodeInfo encInfo;
	DecodeInfo decInfo;
	switch(option)
	{
	       case 1:
		      encInfo.src_image_fname = "beautiful.bmp";
		      encInfo.secret_fname = "secret.txt";
		      encInfo.stego_image_fname = "output.bmp";
		      do_encoding(&encInfo);
		      break;
	       case 2:
		      strcpy(decInfo.decoded_file_name,"decoded_file");
		      decInfo.stegged_image_fname = "output.bmp";
		      do_decoding(&decInfo);
		      break;
	       case 3:
		      printf("\t\t\tThank you have a nice day...\n");
		      exit(0);
		      break;
	       default:
		      printf("Please select a valid option!!!\n");
		      break;
	}
}
