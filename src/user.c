/*************************************************************/
/*                                                           */
/*               Programmer: Simon Paonessa                  */
/*                                                           */
/*               Program: Line Editor                        */
/*                                                           */
/*               Date: 03/01/06                              */
/*                                                           */
/*               File: user.c                                */
/*                                                           */
/*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "refer.h"

static FILE *outputfd;

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

extern int insertlines(char *linespec, doubleList *pHead, doubleList *pCurrent)
{
  doubleList newdata, startnode, endnode, lastnode;
  status rc;
  int cmp, parseerror;
  char buffer[BUFSIZ];

  if (emptyDoubleList(*pHead) == TRUE)
    {
      if(strlen(linespec) != 0) return E_LINES;
      startnode = endnode = NULL;
    }
  else
    {
      parseerror = parseLinespec(linespec, *pHead, *pCurrent, &startnode, &endnode);

      if(parseerror) return parseerror;
      if(startnode != endnode) return E_LINES;
    }
  initDoubleList(&newdata);
  do
    {
      printf("I>");
      fgets(buffer, BUFSIZ, stdin);
      cmp = strcmp(buffer, ".\n");
      if(cmp != 0 && strlen(buffer) > 1)
        {
          rc = stringDoubleAppend(&newdata, buffer);
          if(rc == ERROR) return E_SPACE;
        }
    } while (cmp != 0);
  if(emptyDoubleList(newdata) == TRUE) return 0;

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

extern int deletelines(char *linespec, doubleList *pHead, doubleList *pCurrent)
{
  doubleList startnode, endnode, tmplist;
  doubleList newCurrent;
  int startnumber, endnumber;
  int rc;

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

extern int movelines(char *linespec, doubleList *pHead, doubleList *pCurrent)
{
  doubleList startnode, endnode;
  doubleList tmpnode;
  int startnumber, endnumber;
  int rc, currentnumber;
  int tmp;

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

extern int printlines(char *linespec, doubleList *pHead, doubleList *pCurrent)
{
  doubleList startnode, endnode;
  int startnumber, endnumber, count, direction;
  int rc;

  rc = parseLinespec(linespec, *pHead, *pCurrent, &startnode, &endnode);
  if(rc) return rc;

  startnumber = doubleNodeNumber(startnode);
  endnumber = doubleNodeNumber(endnode);
  direction = (startnumber < endnumber) ? 1 : -1;
  count = (endnumber - startnumber) *direction + 1;
  while(count-- > 0)
    {
      printf("%d  %s", startnumber, DATA(startnode));
      startnumber += direction;
      startnode = nthRelativeDoubleNode(startnode, direction);
    }
  *pCurrent = endnode;
  return 0;
}