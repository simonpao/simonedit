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
#include "globals.h"
#include "doubleList.h"

#ifndef _user
#define _user

extern int readfile(char *filename, doubleList *pL);
extern int writefile(char *filename, doubleList *pL);
extern status writeline(char *s);
extern int insertlines(char *linespec, doubleList *pHead, doubleList *pCurrent);
extern int deletelines(char *linespec, doubleList *pHead, doubleList *pCurrent);
extern int movelines(char *linespec, doubleList *pHead, doubleList *pCurrent);
extern int printlines(char *linespec, doubleList *pHead, doubleList *pCurrent);

#endif
