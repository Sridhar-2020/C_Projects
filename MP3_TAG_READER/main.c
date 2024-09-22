#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"view.h"
#include"edit.h"
#include"types.h"

int main(int argc, char *argv[])
{
       TagReader reader;
       TagEditor editor;

       // Error handling
       if (argc == 1)
       {
	      printf("ERROR : ./a.out : INVALID ARGUENTS\n");
	      printf("USAGE : \nTo view please pass like : ./a.out -v filename\n");
	      printf("To edit please pass like : ./a.out -e -t/-a/-A/-m/-y/-c mp3filename\n");
       }

       // Display the options
       else if (!strcmp(argv[1], "--help"))
       {
	      display_help();
       }

       // Enter view mode
       else if (!strcmp(argv[1], "-v"))
       {
	      if (argc == 3)
	      {
		     view_option(argv, &reader);
	      }

	      else
	      {
		     printf("Please pass arguments in a proper manner\n");
		     printf("USAGE : \nTo view please pass like : ./a.out -v filename\n");
		     printf("To seek help please enter :  ./a.out --help\n");
	      }
       }

       // Enter edit mode
       else if (!strcmp(argv[1], "-e"))
       {
	      if (argc == 5) 
	      {
		     edit_option(argv, &editor);
	      }
	      else
	      {
		     printf("Please pass arguments in a proper manner\n");
		     printf("To edit please pass like  : ./a.out -e -t/-a/-A/-m/-y/-c mp3filename\n");
		     printf("To seek help please enter : ./a.out --help\n");
	      }
       }
       else
       {
	      printf("Please enter arguments in a proper manner\n");
	      printf("To seek help press ./a.out --help\n");
	      printf("\t\t\t:::THANK YOU HAVE A NICE DAY:::\n");
	      exit(0);
       }
}

