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
/*               File: refer.h                               */
/*                                                           */
/*************************************************************/

#include "doubleList.h"

extern int parseLinespec(char *linespec, doubleList head, doubleList current, doubleList *pStart, doubleList *pEnd);
extern int parseNumber(char *numberspec, doubleList head, doubleList current, doubleList *pNode);
extern status stringDoubleAppend(doubleList *pL, char *buffer);
