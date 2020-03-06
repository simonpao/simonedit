/*************************************************************/
/*                                                           */
/*               Programmer: Simon Paonessa                  */
/*                                                           */
/*               Program: Doubly-Linked List                 */
/*                                                           */
/*               Date: 2006-02-27                            */
/*                                                           */
/*               Updated: 2020-03-02                         */
/*                                                           */
/*************************************************************/

#include <stdlib.h>
#include "globals.h"
#include "doubleList.h"

extern status initDoubleList(doubleList *pL)
{
  *pL = NULL;
  return OK;
}

extern bool emptyDoubleList(doubleList L)
{
  return (L == NULL) ? TRUE : FALSE;
}

extern status doubleInsert(doubleList *pL, genericPtr data)
{
  doubleList L;

  if(allocateDoubleNode(&L, data) == ERROR) return ERROR;

  if(emptyDoubleList(*pL) == TRUE)
    PREV(L) = NEXT(L) = NULL;
  else
    {
      NEXT(L) = *pL;
      PREV(L) = PREV(*pL);
      PREV(*pL) = L;
      if(PREV(L) != NULL) NEXT(PREV(L)) = L;
    }
  *pL = L;
  return OK;
}

extern status allocateDoubleNode(doubleList *pL, genericPtr data)
{
  doubleList L = (doubleList) malloc (sizeof(doubleNode));

  if(L == NULL) return ERROR;

  *pL = L;
  DATA(L) = data;
  NEXT(L) = NULL;
  PREV(L) = NULL;
  return OK;
}

extern void freeDoubleNode(doubleList *pL)
{
  free(*pL);
  *pL = NULL;
}

extern status doubleDelete(doubleList *pL, genericPtr *pData)
{
  if(emptyDoubleList(*pL) == TRUE) return ERROR;

  *pData = DATA(*pL);
  return doubleDeleteNode(pL, *pL);
}

extern status doubleDeleteNode(doubleList *pL, doubleList node)
{
  doubleList prev, next;

  if(emptyDoubleList(*pL) == TRUE) return ERROR;

  prev = PREV(node);
  next = NEXT(node);

  if(prev != NULL) NEXT(prev) = next;
  if(next != NULL) PREV(next) = prev;

  if(node == *pL)
    {
      if(next != NULL) *pL = next;
      else *pL = prev;
    }

  freeDoubleNode(pL);
  return OK;
}

extern void cutList(doubleList *pL, doubleList *pStart, doubleList *pEnd)
{
  doubleList start, end;

  start = *pStart;
  end = *pEnd;

  if(PREV(start)) NEXT(PREV(start)) = NEXT(end);
  if(NEXT(end)) PREV(NEXT(end)) = PREV(start);
  if(*pL == start) *pL = NEXT(end);
  PREV(start) = NEXT(end) = NULL;
}

extern void pasteList(doubleList *pTarget, doubleList *pSource)
{
  doubleList target, source, lastnode;

  if(emptyDoubleList(*pSource) == TRUE) return;

  if(emptyDoubleList(*pTarget) == TRUE)
    *pTarget = *pSource;
  else
    {
      source = *pSource;
      target = *pTarget;
      lastnode = nthDoubleNode(source, -1);
      NEXT(lastnode) = NEXT(target);
      if(NEXT(target) != NULL)
        PREV(NEXT(target)) = lastnode;
      PREV(source) = target;
      NEXT(target) = source;
    }
  *pSource = NULL;
}

extern status doubleAppend(doubleList *pL, genericPtr data)
{
  doubleList L, tmpList;

  if(allocateDoubleNode(&L, data) == ERROR) return ERROR;

  if (emptyDoubleList(*pL) == TRUE) *pL = L;
  else
    {
      for (tmpList = *pL; NEXT(tmpList) != NULL; tmpList=NEXT(tmpList));
      NEXT(tmpList) = L;
      PREV(L) = tmpList;
    }
  return OK;
}

extern int doubleLength(doubleList L)
{
  doubleList tmplist;
  int length = 1;
  
  if( emptyDoubleList(L) ) return 0 ;
  
  for(tmplist = L; NEXT(tmplist) != NULL; tmplist = NEXT(tmplist))
    length++;

  return length;
}

extern status doubleTraverse(doubleList L, status (*pFuncF)())
{
  if(emptyDoubleList(L)) return OK;

  if ((*pFuncF)(DATA(L)) == ERROR) return ERROR;
  else return doubleTraverse(NEXT(L), pFuncF);
}

extern void destroyDoubleList(doubleList *pL, void (*pFuncF)())
{
  if (emptyDoubleList(*pL) == FALSE)
    {
      destroyDoubleList(&NEXT(*pL), pFuncF);
      if (pFuncF != NULL)
        (*pFuncF)(DATA(*pL));
      freeDoubleNode(pL);
    }
}

extern int doubleNodeNumber(doubleList L)
{
  int count = 1;

  if(emptyDoubleList(L) == TRUE) return 0;

  for( ; PREV(L) != NULL; L = PREV(L)) count++;

  return count;
}

extern doubleList nthDoubleNode(doubleList L, int n)
{
  int count=2;

  if(n > doubleLength(L)) return NULL;

  if(n == -1)
    {
      for( ; NEXT(L) != NULL; L = NEXT(L));
      return L;
    }

  for( ; NEXT(L) != NULL && count <= n; L = NEXT(L))
    count++;

  return L;
}

extern doubleList nthRelativeDoubleNode(doubleList L, int n)
{
  int i;

  if(n == 0) return L;

  if(n > 0)
    {
      for(i=n; NEXT(L) != NULL && i>0; L = NEXT(L)) i--;
      return L;
    }
  if(n < 0)
    {
      for(i=n; PREV(L) != NULL && i>0; L = PREV(L)) i--;
      return L;
    }
}
