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
/*               File: user.c                                */
/*                                                           */
/*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "user.h"
#include "refer.h"

static FILE *outputfd;

/**
 * Read the input file into a doubly linked list
 */
extern int readfile(char *filename, doubleList *pL)
{
  char buffer[BUFSIZ];
  FILE *fd;

  if((fd = fopen(filename, "r")) == NULL) return E_FILE;

  while (fgets(buffer, BUFSIZ, fd) != NULL)
    {
      if(stringDoubleAppend(pL, buffer) == ERROR)
        return E_SPACE;
    }
  fclose(fd);
  return 0;
}

/**
 * Write the doubly linked list to a newline separated file
 */
extern int writefile(char *filename, doubleList *pL)
{
  status rc;

  if ((outputfd = fopen(filename, "w")) == NULL)
    return E_IO;

  rc = doubleTraverse(*pL, writeline);
  fclose(outputfd);
  return(rc == ERROR) ? E_IO : 0;
}

extern status writeline(char *s)
{
  if(fputs(s, outputfd) == EOF)
    return ERROR;

  return OK;
}

/**
 * Insert line(s)
 *    I1 - Insert before line 1
 *    I  - Insert before active line
 */
extern int insertlines(char *linespec, doubleList *pHead, doubleList *pCurrent)
{
  doubleList newdata, startnode, endnode, lastnode;
  status rc;
  int cmp = 1, parseerror, insertlinenumber;
  char buffer[BUFSIZ];

  if (emptyDoubleList(*pHead) == TRUE)
    {
      if(strlen(linespec) != 0) return E_LINES;
      startnode = endnode = NULL;
    }
  else
    {
      parseerror = parseLinespec(linespec, *pHead, *pCurrent, &startnode, &endnode);
      if(parseerror && parseerror != E_OVERFLOW) return parseerror;
      if(startnode != endnode && parseerror != E_OVERFLOW) return E_LINES;
    }
  
  // If line number specified is less than the maximum line
  //    create a new list which will be inserted prior to the 
  //    specified line
  if(parseerror != E_OVERFLOW) 
    {
      initDoubleList(&newdata);
      insertlinenumber = doubleNodeNumber(startnode);
      if(insertlinenumber < 1) insertlinenumber = 1;
    }
  // If line number specified is greater than the maximum line
  //    then new data will be appended to the end of the list
  else insertlinenumber = doubleNodeNumber(nthDoubleNode(*pHead, -1)) + 1 ;

  fgets(buffer, BUFSIZ, stdin); // Read the initial newline and discard
  while (cmp != 0)
    {
      printf("%s> ", formatLineNumber(insertlinenumber++));
      fgets(buffer, BUFSIZ, stdin);
      cmp = strcmp(buffer, "\n");
      if(cmp != 0 && strlen(buffer) > 1)
        {
          // append to new list
          if(parseerror != E_OVERFLOW) rc = stringDoubleAppend(&newdata, buffer);
          // append to main list
          else rc = stringDoubleAppend(pHead, buffer);
          if(rc == ERROR) return E_SPACE;
        }
    }
  
  if(parseerror == E_OVERFLOW)
    *pCurrent = nthDoubleNode(*pHead, -1);
  
  // If no data was entered into a new list, 
  //    or if data was appended to the main list
  //    return here because nothing else needs to be done
  if(emptyDoubleList(newdata) == TRUE || parseerror == E_OVERFLOW) return 0;

  // Insert the new list into the main list
  if(startnode == NULL)
    {
      *pHead = newdata;
      *pCurrent = nthDoubleNode(newdata, -1);
    }
  else if(PREV(startnode) == NULL)
    {
      lastnode = nthDoubleNode(newdata, -1);
      pasteList(&lastnode, pHead);
      *pHead = newdata;
      *pCurrent = startnode;
    }
  else
    {
      pasteList(&PREV(startnode), &newdata);
      *pCurrent = startnode;
    }
  return 0;
}

/**
 * Delete line(s)
 *    D1   - Delete line 1
 *    D1,5 - Delete lines 1 through 5
 *    D^,5 - Delete first through fifth lines
 *    D5,$ - Delete fifth through last lines
 *    D.   - Delete active line
 *    D.+1 - Delete line after active line
 *    D^,$ - Delete all lines
 */
extern int deletelines(char *linespec, doubleList *pHead, doubleList *pCurrent)
{
  doubleList startnode, endnode, tmplist;
  doubleList newCurrent;
  int startnumber, endnumber;
  int rc;

  if( emptyDoubleList(*pHead) ) {
    printf("File is empty.\n") ;
    return 0;
  }

  rc = parseLinespec(linespec, *pHead, *pCurrent, &startnode, &endnode);
  if(rc) return rc;

  startnumber = doubleNodeNumber(startnode);
  endnumber   = doubleNodeNumber(endnode);
  if(startnumber > endnumber)
    {
      tmplist = startnode;
      startnode = endnode;
      endnode = tmplist;
    }
  newCurrent = nthRelativeDoubleNode(endnode, 1);
  if(newCurrent == NULL)
    newCurrent = nthRelativeDoubleNode(startnode, -1);

  cutList(pHead, &startnode, &endnode);
  *pCurrent = newCurrent;
  destroyDoubleList(&startnode, free);
  return 0;
}

/**
 * Move line(s) to before active line
 *    M1   - Move line 1 to before active line 
 *    M1,5 - Move lines 1 through 5 to before active line
 *    M^,5 - Move first through fifth line to before active line
 *    M5,$ - Move fifth through last line to before active line
 */
extern int movelines(char *linespec, doubleList *pHead, doubleList *pCurrent)
{
  doubleList startnode, endnode;
  doubleList tmpnode;
  int startnumber, endnumber;
  int rc, currentnumber;
  int tmp;

  if( emptyDoubleList(*pHead) ) {
    printf("File is empty.\n") ;
    return 0;
  }

  rc = parseLinespec(linespec, *pHead, *pCurrent, &startnode, &endnode);
  if(rc) return rc;

  startnumber = doubleNodeNumber(startnode);
  endnumber = doubleNodeNumber(endnode);
  currentnumber = doubleNodeNumber(*pCurrent);

  if(startnumber > endnumber)
    {
      tmp = startnumber;
      startnumber = endnumber;
      endnumber = tmp;
      tmpnode = startnode;
      startnode = endnode;
      endnode = tmpnode;
    }

  if(currentnumber >= startnumber && currentnumber <= endnumber)
    return E_LINES;

  cutList(pHead, &startnode, &endnode);
  pasteList(&PREV(*pCurrent), &startnode);
  return 0;
}

/**
 * Count the total number of lines:
 *    C^,$ - Count number of lines in the file
 *    C.,$ - Count number of lines from active line to last line
 *    C.,^ - Count number of lines from active line to first line
 *    C1   - Count number of characters on line 1
 */
extern int countLines(char *linespec, doubleList *pHead, doubleList *pCurrent)
{
  doubleList startnode, endnode;
  int count, rc, spaces;
  char tmp ;
  char * str ;

  if( emptyDoubleList(*pHead) ) {
    printf("0 lines.\n") ;
    return 0 ;
  }

  rc = parseLinespec(linespec, *pHead, *pCurrent, &startnode, &endnode);
  if(rc) return rc;

  count  = doubleNodeNumber( endnode   );
  count -= doubleNodeNumber( startnode );

  count = abs(count) + 1 ; 

  if(count == 1)
    {
      count = -1, spaces = 0 ;
      str = (char *) DATA(startnode) ;
      do { 
        tmp = str[++count] ;
        if(tmp == ' ') spaces++;
      } while(tmp != '\0') ;
      printf("%d characters (%d %s).\n", count-1, spaces, spaces == 1 ? "is a space" : "are spaces") ;
    }
  else
    printf("%d lines.\n", count);

  return 0;
}

/**
 * Change the active line (goto):
 *    G1 - Goto line 1
 *    G$ - Goto last line
 *    G^ - Goto first line
 */
extern int gotoLine(char *linespec, doubleList *pHead, doubleList *pCurrent)
{
  doubleList startnode, endnode;
  int startnumber, rc;

  if( emptyDoubleList(*pHead) ) {
    printf("File is empty.\n") ;
    return 0;
  }

  rc = parseLinespec(linespec, *pHead, *pCurrent, &startnode, &endnode);
  if(rc) return rc;

  startnumber = doubleNodeNumber(startnode);
  printf("%s:  %s", formatLineNumber(startnumber), (char *) DATA(startnode));
  *pCurrent = startnode;
  return 0;
}

/**
 * Print file lines:
 *    P     - print the active line
 *    P1    - print line 1
 *    P1,10 - print lines 1 through 10
 *    P1,$  - print lines 1 through last
 *    P.,10 - print current line through line 10
 *    P^,$  - print first line through last
 */
extern int printlines(char *linespec, doubleList *pHead, doubleList *pCurrent)
{
  doubleList startnode, endnode;
  int startnumber, endnumber, count, direction;
  int rc;

  if( emptyDoubleList(*pHead) ) {
    printf("File is empty.\n") ;
    return 0;
  }

  rc = parseLinespec(linespec, *pHead, *pCurrent, &startnode, &endnode);
  if(rc) return rc;

  startnumber = doubleNodeNumber(startnode);
  endnumber = doubleNodeNumber(endnode);
  direction = (startnumber < endnumber) ? 1 : -1;
  count = (endnumber - startnumber) *direction + 1;
  while(count-- > 0)
    {
      printf("%s:  %s", formatLineNumber(startnumber), (char *) DATA(startnode));
      startnumber += direction;
      startnode = nthRelativeDoubleNode(startnode, direction);
    }
  *pCurrent = endnode;
  return 0;
}

/**
 * Format the line numbers to three digits
 * 1   = 001
 * 10  = 010
 * 100 = 100
 */
char * formatLineNumber(int number)
{
    char *buf = malloc(5);

    if( number < 10 ) {
        snprintf(buf, 5, "00%d", number);
        return buf ;
    }
    if( number < 100 ) {
        snprintf(buf, 5, "0%d", number);
        return buf ;
    }
    snprintf(buf, 5, "%d", number);
    return buf ;
}