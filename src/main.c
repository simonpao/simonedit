/*************************************************************/
/*                                                           */
/*               Programmer: Simon Paonessa                  */
/*                                                           */
/*               Program: Line Editor                        */
/*                                                           */
/*               Date: 02/27/06                              */
/*                                                           */
/*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"

/**
 * Handles printing of errors, Indices of specific error message defined in globals.h
 *
 * E_IO     1
 * E_SPACE  2
 * E_LINES  3
 *_BADCMD   4
 * E_DELETE 5
 * E_MOVE   6
 * E_FILE   7
 */
void printerror(int errnum)
{
  static char *errmsg[] = {
    "io error",
    "out of memory space",
    "invalid line specification",
    "invalid command",
    "error deleting lines",
    "error moving lines",
	"file does not exist"
  };

  if (errnum < 0 || errnum >= MAXERROR)
    {
      printf("System Error. Invalid error number: %d\n", errnum);
      return;
    }
  printf("%s\n", errmsg[errnum-1]);
}


int main(int argc, char *argv[])
{
  char filename[BUFSIZ];
  char buffer[BUFSIZ];
  doubleList linelist, currentline;
  bool fileEdited, exitFlag;
  int rc, i;

  initDoubleList(&linelist);

  printf("Enter the name of the file to edit: ");
  scanf("%s", filename);
  strcat(filename, ".simon");

  if((rc = readfile(filename, &linelist)) != 0)
    {
      printerror(rc);
      exit(1);
    }

  printf("%d lines read.\n", doubleLength(linelist));
  currentline = nthDoubleNode(linelist, -1);
  fileEdited = FALSE;
  exitFlag = FALSE;
  while (exitFlag == FALSE)
    {
      printf("%d, cmd: ", doubleNodeNumber( currentline ));
      scanf("%s", buffer);
      switch(toupper(buffer[0]))
        {
        case '\n':
          break;
		case 'P':
          rc = printlines(&buffer[1], &linelist, &currentline);
          if(rc) printerror(rc);
          break;
        case 'D':
          fileEdited = TRUE;
          rc = deletelines(&buffer[1], &linelist, &currentline);
          if(rc) printerror(rc);
          break;
        case 'I':
          fileEdited = TRUE;
          rc = insertlines(&buffer[1], &linelist, &currentline);
          if(rc) printerror(rc);
          break;
        case 'M':
          fileEdited = TRUE;
          rc = movelines(&buffer[1], &linelist, &currentline);
          if(rc) printerror(rc);
          break;
        case 'W':
		  if(buffer[1] != '\0') { strcpy(filename, &buffer[1]); strcat(filename, ".simon");}
          rc = writefile(filename, &linelist);
          if(rc != 0) printerror(rc);
          else printf("%d lines written\n", doubleLength(linelist));
          fileEdited = FALSE;
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
