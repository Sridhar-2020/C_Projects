#include "lexer.h"

FILE *fp;
int main(int argc, char *argv[]) 
{
    if (initializeLexer(argv[1]) == FAILURE)
    {
	   return FAILURE;
    }

    /*
       Function that is responsible to tokenize all the words in a C program and gives a detailed
       information about each token in such a way that in which field the respective token belongs to
     */
    initiate_lexer();
}
