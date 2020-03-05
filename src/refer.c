/*************************************************************/
/*                                                           */
/*               Programmer: Simon Paonessa                  */
/*                                                           */
/*               Program: Line Editor                        */
/*                                                           */
/*               Date: 2006-03-01                            */
/*                                                           */
/*               Updated: 2020-03-02                         */
/*                                                           */
/*               File: refer.c                               */
/*                                                           */
/*************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "refer.h"
#include "globals.h"

extern int parseLinespec(char *linespec, doubleList head, doubleList current, doubleList *pStart, doubleList *pEnd)
{
  int rc;
  char *nextnumber;

  if(*linespec == '\0')
    *pStart = current;

  else
    {
      rc = parseNumber(linespec, head, current, pStart);
      if(rc) return rc;
    }
  nextnumber = strchr(linespec, ',');
  if(nextnumber == NULL)
    *pEnd = *pStart;

  else
    {
      rc = parseNumber(nextnumber + 1, head, current, pEnd);
      if(rc) return rc;
    }
  if(*pStart == NULL || *pEnd == NULL)
    return E_LINES;
  return 0;
}

extern int parseNumber(char *numberspec, doubleList head, doubleList current, doubleList *pNode)
{
  char numberbuffer[BUFSIZ], *pNum;
  int nodenumber;
  int direction;

  // Determine which line number is being referenced in the command
  if(*numberspec == '.') // the active line
    {
      *pNode = current;
      numberspec++;
    }
  else if (*numberspec == '^') // the first line
    {
      *pNode = nthDoubleNode(head, 0);
      if(*pNode == NULL) return E_LINES;
      numberspec++;
    }
  else if (*numberspec == '$') // the last line
    {
      *pNode = nthDoubleNode(head, -1);
      if(*pNode == NULL) return E_LINES;
      numberspec++;
    }
  else if(isdigit(*numberspec)) // a specific line number
    {
      pNum = numberbuffer;
      while(isdigit(*numberspec))
        *pNum++ = *numberspec++;
      *pNum = '\0';
      nodenumber = atoi(numberbuffer);
      *pNode = nthDoubleNode(head, nodenumber);
      if(*pNode == NULL) return E_OVERFLOW;
    }
  else return E_LINES;

  // Determine if line is relative (+/-) to the specified line
  if(*numberspec == '+') // line(s) after specified line
    {
      direction = 1;
      numberspec++;
    }
  else if(*numberspec == '-') // line(s) before specified line
    {
      direction = -1;
      numberspec++;
    }
  else direction = 0; // not relative

  // If relative, need to determine the number to which the requested
  //    line is relative to the specified line
  if(isdigit(*numberspec) && direction != 0)
    {
      pNum = numberbuffer;
      while(isdigit(*numberspec))
        *pNum++ = *numberspec++;
      *pNum = '\0';
      nodenumber = atoi(numberbuffer) * direction;
      *pNode = nthRelativeDoubleNode(*pNode, nodenumber);
      if(pNode == NULL) return E_LINES;
      direction = 0;
    }
  // If not relative, then no other input is allowed
  if(direction == 0 && (*numberspec == '\0' || *numberspec == ','))
    return 0;
  else return E_LINES;
}

extern status stringDoubleAppend(doubleList *pL, char *buffer)
{
  char *string = (char *) malloc ((strlen(buffer)+1) * sizeof(char));
  int i;

  if (string == NULL) return ERROR;

  for(i=0; i<=(strlen(buffer)+1); i++) string[i] = buffer[i];

  if(doubleAppend(pL, (genericPtr ) string) == ERROR)
    {
      free(string);
      return ERROR;
    }
  return OK;
}
