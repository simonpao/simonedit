/*************************************************************/
/*                                                           */
/*               Programmer: Simon Paonessa                  */
/*                                                           */
/*               Program: Line Editor                        */
/*                                                           */
/*               Date: 2006-02-27                            */
/*                                                           */
/*               Updated: 2020-03-02                         */
/*                                                           */
/*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "user.h"

/**
 * Handles printing of errors, Indices of specific error message defined in globals.h
 *
 * E_IO       1
 * E_SPACE    2
 * E_LINES    3
 * E_BADCMD   4
 * E_DELETE   5
 * E_MOVE     6
 * E_FILE     7
 * E_OVERFLOW 8
 */
void printerror(int errnum)
{
  static char *errmsg[] = {
    "An error occured while attempting to write to the file.",
    "The computer is too low on resources to continue.",
    "Invalid line number specified. Identify lines using 0-9, '^', '$', or '.'.",
    "Invalid command. Valid commands are P, G, C, D, I, M, W, and Q.",
    "An error occured while attempting to delete the specified lines.",
    "An error occured while attempting to move the specified lines.",
	  "Requested file does not exist.",
    "Specified line number is greater than the maximum line number in the file."
  };

  if (errnum < 1 || errnum > MAXERROR)
    {
      printf("System Error. Invalid error number: %d\n", errnum);
      return;
    }
  printf("%s\n", errmsg[errnum-1]);
}

void trimInput(char * input) {
  int i = 0;
  while(input[i] != '\0')
    if(input[i] == ' ' || input[i] == '\n')
      memmove(&input[i], &input[i + 1], strlen(input) - i);
    else i++;
}

int main(int argc, char *argv[])
{
  char filename[BUFSIZ];
  char buffer[BUFSIZ];
  doubleList linelist, currentline;
  bool fileEdited, exitFlag;
  int rc, i;

  initDoubleList(&linelist);

  // If filename supplied at command line, use that
  //    otherwise, prompt user for filename
  if( argc == 1 ) {
    printf("Enter the name of the file to edit, or 'new': ");
    scanf("%s", filename);
  } else {
    strcpy( filename, argv[1] ) ;
  }

  if(strcmp("new", filename)) {
    // Determine if ".simon" should be added to end of filename
    char * testLoc = strrchr(filename, '.') ;
    if( testLoc ) {
      if(strcmp(".simon", testLoc) != 0) {
        strcat(filename, ".simon");
      }
    } else {
      strcat(filename, ".simon");
    }

    // Read the file into a doubly linked list
    if((rc = readfile(filename, &linelist)) != 0)
      {
        // If the file does not exist, exit
        printerror(rc);
        exit(1);
      }

    printf("%d lines read.\n", doubleLength(linelist));
  }

  currentline = nthDoubleNode(linelist, 1);
  fileEdited = FALSE;
  exitFlag = FALSE;
  while (exitFlag == FALSE)
    {
      printf("%d, cmd: ", emptyDoubleList( linelist ) ? 0 : doubleNodeNumber( currentline ));
      fgets(buffer, BUFSIZ, stdin);
      trimInput(buffer);
      switch(toupper(buffer[0]))
        {
        case '\n':
          break;
        case 'C':
          if( buffer[1] == '\0' ) strcat( buffer, "^,$" ) ; // Default for command is to count all lines
          rc = countLines(&buffer[1], &linelist, &currentline);
          if(rc) printerror(rc);
          break;
        case 'G':
          rc = gotoLine(&buffer[1], &linelist, &currentline);
          if(rc) printerror(rc);
          break;
		    case 'P':
          if( buffer[1] == '\0' ) strcat( buffer, "^,$" ) ; // Default for command is to print all lines
          rc = printlines(&buffer[1], &linelist, &currentline);
          if(rc) printerror(rc);
          break;
        case 'D':
          rc = deletelines(&buffer[1], &linelist, &currentline);
          if(rc) printerror(rc);
          else fileEdited = TRUE;
          break;
        case 'I':
          rc = insertlines(&buffer[1], &linelist, &currentline);
          if(rc) printerror(rc);
          else fileEdited = TRUE;
          break;
        case 'M':
          rc = movelines(&buffer[1], &linelist, &currentline);
          if(rc) printerror(rc);
          else fileEdited = TRUE;
          break;
        case 'W':
		      if(buffer[1] != '\0') { 
            strcpy(filename, &buffer[1]); 
            // Determine if ".simon" should be added to end of filename
            char * testLoc = strrchr(filename, '.') ;
            if( testLoc ) {
              if(strcmp(".simon", testLoc) != 0) {
                strcat(filename, ".simon");
              }
            } else {
              strcat(filename, ".simon");
            }
          }
          rc = writefile(filename, &linelist);
          if(rc != 0) printerror(rc);
          else {
            printf("%d lines written\n", doubleLength(linelist));
            fileEdited = FALSE;
          }
          break;
        case 'Q':
          if(fileEdited == TRUE)
            {
              printf("File modified. Enter W to save, Q to discard.\n");
              fileEdited = FALSE;
            }
          else exitFlag = TRUE;
          break;
        default:
          printerror(E_BADCMD);
          break;
        }
    }
}
