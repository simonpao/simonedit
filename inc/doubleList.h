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

#include "globals.h"

#ifndef _doublelist
#define _doublelist

typedef struct doubleNode doubleNode, *doubleList;

struct doubleNode { genericPtr datapointer; doubleList next; doubleList prev; };

extern status initDoubleList(doubleList *pL);
extern bool emptyDoubleList(doubleList L);
extern status doubleInsert(doubleList *pL, genericPtr data);
extern status allocateDoubleNode(doubleList *pL, genericPtr data);
extern void freeDoubleNode(doubleList *pL);
extern status doubleDelete(doubleList *pL, genericPtr *pData);
extern status doubleDeleteNode(doubleList *pL, doubleList node);
extern void cutList(doubleList *pL, doubleList *pStart, doubleList *pEnd);
extern void pasteList(doubleList *pTarget, doubleList *pSource);
extern status doubleAppend(doubleList *pL, genericPtr data);

extern int doubleLength(doubleList L);
extern status doubleTraverse(doubleList L, status (*pFuncF)());
extern void destroyDoubleList(doubleList *pL, void (*pFuncF)());
extern int doubleNodeNumber(doubleList L);
extern doubleList nthDoubleNode(doubleList l, int n);
extern doubleList nthRelativeDoubleNode(doubleList L, int n);

#endif
