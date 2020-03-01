/*************************************************************/
/*                                                           */
/*               Programmer: Simon Paonessa                  */
/*                                                           */
/*               Program: Line Editor                        */
/*                                                           */
/*               Date: 03/01/06                              */
/*                                                           */
/*               File: refer.c                               */
/*                                                           */
/*************************************************************/

#include "doubleList.h"

extern int parseLinespec(char *linespec, doubleList head, doubleList current, doubleList *pStart, doubleList *pEnd);
extern int parseNumber(char *numberspec, doubleList head, doubleList current, doubleList *pNode);
extern status stringDoubleAppend(doubleList *pL, char *buffer);
