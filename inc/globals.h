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
/*               File: globals.h                             */
/*                                                           */
/*************************************************************/

#ifndef _globals
#define _globals

#define E_IO     1
#define E_SPACE  2
#define E_LINES  3
#define E_BADCMD 4
#define E_DELETE 5
#define E_MOVE   6
#define E_FILE   7
#define MAXERROR 8

#define DATA( L ) (( L ) -> datapointer )
#define NEXT( L ) (( L ) -> next )
#define PREV( L ) (( L ) -> prev )

typedef enum { OK, ERROR } status;
typedef enum { FALSE=0, TRUE=1 } bool;
typedef void *genericPtr;

#endif

