#include "lexer.h"

extern FILE *fp;

static const char* keywords[MAX_KEYWORDS] = {
       "int", "float", "return", "if", "else", "while", "for", "do", "break", "continue",
       "char", "double", "void", "switch", "case", "default", "const", "static", "sizeof", "struct"
};

static const char* operators = "+-*/%=!<>|&";
static const char* specialCharacters = ",;{}()[]";

/************************************************************************************************************************************************
OPERATION :  Function to determine whether the argument passed is a valid file or not
PARAMETERS : Filename(String)
RETURN VALUE : SUCCESS/FAILURE
 ***********************************************************************************************************************************************/
int initializeLexer(const char* filename)
{
       fp = fopen(filename, "r");
       if (fp == NULL)
       {
	      printf("ERROR : Unable to open the file\n");
	      printf("USAGE : .exefile filename.c\n");
	      return FAILURE;
       }
       return SUCCESS;
}

/************************************************************************************************************************************************
OPERATION : Generate the token and categorise them
PARAMETERS : Nothing
RETURN VALUE : SUCCESS/FAILURE
 ***********************************************************************************************************************************************/
void initiate_lexer()
{
       int count = 0, k = 0;
       char ch, str[50], str_lit[30],str_num[20], str_op[10], str_ch[5];
       while ((ch = fgetc(fp)) != EOF)
       {
	      // STEP 1 : Check whether the character is alphabet or not
	      // STEP 2 : Store in a string and check whether it is keyword or identifier
	      if (isalpha(ch))
	      {
		     str[count++] = ch;
	      }
	      else if (isdigit(ch) == 0 && count > 0)
	      {
		     str[count] = '\0';
		     count = 0;
		     if(isKeyword(str))
		     {
			    printf("KEYWORD : %s\n", str);
		     }
		     else
		     {
			    printf("IDENTIFIER : %s\n", str);
		     }
	      }
	      // STEP 3 :  Check whether the character is digit or not
	      // STEP 4 :  Check if the character exactly after the present character is digit or not
	      // STEP 5 : If yes store it in a string and print the constant 
	      if (isdigit(ch))
	      {
		     str_num[0] = ch;
		     if (ch == '0')
		     {
			    char ch_check = fgetc(fp);

			    // Moving the offset to the previous position if the character is a number
			    if (isdigit(ch_check))
				   fseek(fp, -1, SEEK_CUR);

			    int i = 0, check, flag = 1;
			    char buffer[50];

			    if (ch_check == ';')
			    {
				   flag = 0;
				   buffer[0] = '0';
				   buffer[1] = '\0';
			    }

			    if (flag)
			    {
				   while ((ch = fgetc(fp)) != ';')
				   {
					  buffer[i++] = ch;
				   }
				   buffer[i] = '\0';
			    }

			    i = 0;
			    check = 1;

			    /* Error checking for hexadecimal number*/
			    if (ch_check == 'x')
			    {
				   while (buffer[i] != '\0')
				   {
					  if (!isxdigit(buffer[i]))
					  {
						 check = 0; 
						 break;
					  }
					  i++;
				   }
			    }

			    /* Error checking for binary number*/
			    else if (ch_check == 'b')
			    {
				   while (buffer[i] != '\0')
				   {
					  if (buffer[i] != '0' && buffer[i] != '1')
					  {
						 check = 0;
						 break;
					  }
					  i++;
				   }
			    }

			    /* Error checking for octal number*/
			    else
			    {
				   while (buffer[i] != '\0')
				   {
					  if (!flag) break;

					  if (buffer[i] < '0' || buffer[i] > '7')
					  {
						 check = 0;
						 break;
					  }
					  i++;
				   }
			    }

			    if (check)
			    {
				   printf("CONSTANT : %s\n", buffer);
			    }

			    else
			    {
				   printf("ERROR : The entered number is illegal\n");
				   return;
			    }
		     }

		     else
		     {
			    int index = 1, count = 0;
			    while ((ch = (fgetc(fp))) != ';' && ch != '\n')
			    {
				   str_num[index++] = ch;
				   if (!isdigit(ch) && ch != '.')
				   {
					  printf("ERROR : The entered syntax is not valid\n");
					  return;
				   }
				   if (ch == '.')
				   {
					  count++;
				   }
			    }
			    str_num[index] = '\0';

			    if (count > 1)
			    {
				   printf("ERROR :  The entered syntax is not valid\nOnly one dot is allowed to mention decimal values\n");
				   return;
			    }

			    if (index > 0)
			    {
				   printf("NUMERIC CONSTANT : %s\n", str_num);
			    }

			    else
			    {
				   printf("CONSTANT : %c\n", str_num[0]);
			    }
			    count = 0;
		     }

	      }
	      else if (ch == '\'')
	      {
		     int i = 0;
		     while ((ch = fgetc(fp)) != '\'' && ch != '\n')
		     {
			    str_ch[i++] = ch;
			    if (isalnum(ch) && i > 1)
			    {
				   printf("ERROR : Multi character constant is not allowed\n");
				   return;
			    }
		     }
		     str_ch[i] = '\0';
		     printf("Character constant : %s\n", str_ch);
	      }
	      // STEP 7 : Check a character is " or not 
	      // STEP 8 : Store the characters in a string and print the string
	      else if (ch == '"')
	      {
		     while ((ch = fgetc(fp)) != '"')
		     {
			    if (ch == '\n')
			    {
				   printf("Double quotes are not closed properly\n");
				   return;
			    }
			    str_lit[k++] = ch;
		     }
		     k = 0;
		     printf("LITERAL : %s\n", str_lit);
	      }
	      // STEP 9 : Check whether the character is an operator or not
	      // STEP 10 : Check the very next character if it is a operator store it in a string and print the string  
	      if (isOperator(ch))
	      {
		     str_op[0] = ch;
		     if (isOperator(ch = fgetc(fp)))
		     {
			    str_op[1] = ch;
			    str_op[2] = '\0';
			    if (is_double_opt(str_op))
			    {
				   printf("OPERATOR : %s\n", str_op);
			    }
		     }
		     else 
		     {
			    fseek(fp, -2, SEEK_CUR);
			    ch = fgetc(fp);
			    printf("OPERATOR : %c\n", ch);
		     }		    
	      }
	      // STEP 11 : Check whether the character is symbol or not 
	      if(isSpecialCharacter(ch))
	      {
		     printf("SYMBOL : %c\n", ch);
	      }
       }
}

/************************************************************************************************************************************************
OPERATION : Checks whether the string passed is Keyword or not
PARAMETERS : Token(String)
RETURN VALUE : SUCCESS/FAILURE
 ***********************************************************************************************************************************************/
int isKeyword(const char* str)
{
       for (int i = 0; i < 20; i++)
       {
	      if (!strcmp(keywords[i], str))
		     return SUCCESS;
       }
       return FAILURE;
}

/************************************************************************************************************************************************
OPERATION : Checks whether the string passed is Operator or not
PARAMETERS : Token(String)
RETURN VALUE : SUCCESS/FAILURE
 ***********************************************************************************************************************************************/
int isOperator(char ch)
{
       for (int i = 0; operators[i] != '\0'; i++)
       {
	      if (operators[i] == ch) return SUCCESS;
       }
       return FAILURE;
}

/************************************************************************************************************************************************
OPERATION : Checks whether the string passed is Special character or not
PARAMETERS : Token(String)
RETURN VALUE : SUCCESS/FAILURE
 ***********************************************************************************************************************************************/
int isSpecialCharacter(char ch)
{
       for (int i = 0; specialCharacters[i] != '\0'; i++)
       {
	      if (specialCharacters[i] == ch) return SUCCESS;
       }
       return FAILURE;
}


/************************************************************************************************************************************************
OPERATION : Checks whether the string passed is double or not
PARAMETERS : Token(String)
RETURN VALUE : SUCCESS/FAILURE
 ***********************************************************************************************************************************************/
int is_double_opt(char str[])
{
       char opt[] = {'=', '=' , '<', '=', '>', '=', '+', '+', '-', '-' ,'(', ')', '<', '<', '>', '>', '&', '&', '|', '|', '?', ':', '[', ']', 
	      '+', '=', '-', '=', '*', '=', '/', '=', '%', '=', '\0'};
       if ((strstr(opt, str) != NULL)) return SUCCESS;
       return FAILURE;
}
