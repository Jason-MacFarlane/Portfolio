/* File Name: table.h
*  Compiler: MS Visual Studio 2019
*  Author: Jason MacFarlane, 040-890-266
*  Course: CST8152 - Compilers, Lab Section: 012
*  Assignment: 03
*  Date: December 2nd, 2019
*  Professor: Sv. Ranev
*  Purpose: Header file for the transition table
*  Function list: aa_func02, aa_func03, aa_func05, aa_func08, aa_func10, aa_func11
*/

#ifndef  TABLE_H_
#define  TABLE_H_ 

#ifndef BUFFER_H_
#include "buffer.h"
#endif

#ifndef TOKEN_H_
#include "token.h"
#endif

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*  Special case tokens processed separately one by one
 *  in the token-driven part of the scanner
 *  '=' , ' ' , '(' , ')' , '{' , '}' , == , <> , '>' , '<' , ';',
 *  white space
 *  !!comment , ',' , ';' , '-' , '+' , '*' , '/', << ,
 *  .AND., .OR. , SEOF,
 */


#define SEOF '\0' /* Source end-of-file (SEOF) */
#define SEOF_1 255	  /* EOF */

#define COL1 0 /* index for column 1 of the T Table */
#define COL2 1 /* index for column 2 of the T Table */
#define COL3 2 /* index for column 3 of the T Table */
#define COL4 3 /* index for column 4 of the T Table */
#define COL5 4 /* index for column 5 of the T Table */
#define COL6 5 /* index for column 6 of the T Table */
#define COL7 6 /* index for column 7 of the T Table */
#define COL8 7 /* index for column 8 of the T Table */

#define RT_ERROR 1 /* Runtime error value for scerrnum */

#define ES  11 /* Error state  with no retract */
#define ER  12 /* Error state  with retract */
#define IS -1  /* Invalid state */

 /* State transition table definition */
#define TABLE_COLUMNS 8

/*transition table - type of states defined in separate table */
int  st_table[][TABLE_COLUMNS] = {
	/* State 0 */  {1, 6, 4, ES, ES, 9, ER, ES},
	/* State 1 */  {1, 1, 1, 2, 3, 2, 2, 2},
	/* State 2 */  {IS,	IS,	IS,	IS,	IS,	IS,	IS,	IS},
	/* State 3 */  {IS,	IS,	IS,	IS,	IS,	IS,	IS,	IS},
	/* State 4 */  {ES, 4, 4, 7, 5, 5, 5, 5},
	/* State 5 */  {IS,	IS,	IS,	IS,	IS,	IS,	IS,	IS},
	/* State 6 */  {ES, 6, ES, 7, 5, 5, 5, 5},
	/* State 7 */  {8, 7, 7, 8, 8, 8, 8, 8},
	/* State 8 */  {IS,	IS,	IS,	IS,	IS,	IS,	IS,	IS},
	/* State 9 */  {9, 9, 9, 9, 9, 10, ER, 9},
	/* State 10 */ {IS,	IS,	IS,	IS,	IS,	IS,	IS,	IS},
	/* State 11 */ {IS,	IS,	IS,	IS,	IS,	IS,	IS,	IS},
	/* State 12 */ {IS,	IS,	IS,	IS,	IS,	IS,	IS,	IS}
};

/* Accepting state table definition */
#define ASWR 0  /* accepting state with retract */
#define ASNR 1  /* accepting state with no retract */
#define NOAS 2  /* not accepting state */

int as_table[] = { NOAS, NOAS, ASWR, ASNR, NOAS, ASWR, NOAS, NOAS, ASWR, NOAS, ASNR, ASNR, ASWR };

/* Accepting action function declarations */
Token aa_func02(char lexeme[]); /* AVID/Keyword accepting function */
Token aa_func03(char lexeme[]); /* SVID accepting function */
Token aa_func05(char lexeme[]); /* DIL/IL accepting function */
Token aa_func08(char lexeme[]); /* FPL accepting function */
Token aa_func10(char lexeme[]); /* SL accepting function */
Token aa_func11(char lexeme[]); /* Error accepting function */

/* defining a new type: pointer to function (of one char * argument)
   returning Token
*/
typedef Token(*PTR_AAF)(char* lexeme);


/* Accepting function (action) callback table (array) definition */
/* If you do not want to use the typedef, the equvalent declaration is:
 * Token (*aa_table[])(char lexeme[]) = {
 */

PTR_AAF aa_table[] = { NULL, NULL, aa_func02, aa_func03, NULL, aa_func05, NULL, NULL, aa_func08, NULL, aa_func10, aa_func11, aa_func11 };

/* Keyword lookup table (.AND. and .OR. are not keywords) */
#define KWT_SIZE  10

char* kw_table[] =
{
"ELSE",
"FALSE",
"IF",
"PLATYPUS",
"READ",
"REPEAT",
"THEN",
"TRUE",
"WHILE",
"WRITE"
};



#endif
