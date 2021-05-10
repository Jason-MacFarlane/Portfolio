/* File Name: scanner.c
*  Compiler: MS Visual Studio 2019
*  Author: Jason MacFarlane, 040-890-266
*  Course: CST8152 - Compilers, Lab Section: 012
*  Assignment: 03
*  Date: December 2nd, 2019
*  Professor: Sv. Ranev
*  Purpose: Scans and tokenizes a PLATYPUS text file.
*  Function list: scanner_init(), malar_next_token(), get_next_state(), char_class(),
*				  aa_func02(), aa_func03(), aa_func05(), aa_func08(), aa_func10(), aa_func11(),
*				  iskeyword(), addStringToAttribute()
*/

/* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
 * to suppress the warnings about using "unsafe" functions like fopen()
 * and standard sting library functions defined in string.h.
 * The define does not have any effect in Borland compiler projects.
 */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

#define NDEBUG       /* to suppress assert() call */

#include <assert.h>  /* assert() prototype */

 /* project header files */
#include "buffer.h"
#include "token.h"
#include "table.h"


#define DEBUG  /* for conditional processing */
#undef  DEBUG

/* Global objects - variables */
/* This buffer is used as a repository for string literals.
   It is defined in platy_st.c */
extern pBuffer str_LTBL; /*String literal table */
int line; /* current line number of the source code */
extern int scerrnum;     /* defined in platy_st.c - run-time error number */

/* Local(file) global objects - variables */
static pBuffer lex_buf;/*pointer to temporary lexeme buffer*/
static pBuffer sc_buf; /*pointer to input source buffer*/
/* No other global variable declarations/definitiond are allowed */

/* scanner.c static(local) function  prototypes */
static int char_class(char c); /* character class function */
static int get_next_state(int, char); /* state machine function */
static int iskeyword(char* kw_lexeme); /* keywords lookup functuion */
static void addStringToAttribute(char* lexeme, char* tAttrib, int len, int isSVID); /* Add a string to a Token Attribute and truncates if needed */


/* Fuction Name: scanner_init
*  Purpose: Initializes the scanner by preparing its buffer
*  Author: Svillen Ranev
*  History/Versions: 1.0
*  Called Functions: b_isempty(), b_rewind(), b_clear()
*  Parameters: psc_buf: [Type: Buffer Pointer], [Range: N/A], [Description: main buffer to use for scanner]
*  Return value: [Type: int] EXIT_FAILURE if there was an error, or EXIT_SUCCESS upon success
*  Algorithm: Ensures the buffer is empty then prepares the buffer for the scanner.
*/
int scanner_init(pBuffer psc_buf) {

	if (b_isempty(psc_buf)) return EXIT_FAILURE;/*1*/
	/* in case the buffer has been read previously  */

	b_rewind(psc_buf);
	b_clear(str_LTBL);
	line = 1;
	sc_buf = psc_buf;

	return EXIT_SUCCESS;/*0*/
/*   scerrnum = 0;  *//*no need - global ANSI C */

}

/* Fuction Name: malar_next_token
*  Purpose: Scans for the next token in a file
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [November 9th, 2019]
*  Called Functions: b_getc(), b_retract(), b_mark(), b_reset(), get_next_state(), b_getcoffset(), b_allocate()
					 b_addc(), b_compact(), b_rewind(), b_location(), b_free(), aa_func02(), aa_func03(),
					 aa_func05(), aa_func08(), aa_func10(), aa_func11()
*  Parameters: N/A
*  Return value: [Type: Token] The next token found in the file
*  Algorithm: Analyzes the file char by char until the valid token is found or an error.
*/
Token malar_next_token(void) {

	Token t = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	unsigned char c; /* input symbol */
	int state = 0; /* initial state of the FSM */
	short lexstart;  /*start offset of a lexeme in the input char buffer (array) */
	short lexend;    /*end   offset of a lexeme in the input char buffer (array)*/
	char test; /* a temp char variable */
	char* rtError = "RUN TIME ERROR :"; /* attribute value for run time errors */

	while (TRUE) { /* endless loop broken by token returns it will generate a warning */

		c = b_getc(sc_buf); /* input symbol to be analyzed */

		switch (c) {

			/* ignore white space */
		case ' ':
		case '\t':

			break;

		case '=':

			if (b_getc(sc_buf) == '=') { /* look ahead to see if the next symbol is also = */

				t.code = REL_OP_T; /* if so it is a relational operator, sets token accordingly */
				t.attribute.rel_op = EQ;

			}
			else {

				b_retract(sc_buf); /* return the test character */
				t.code = ASS_OP_T; /* it is an assignment operator */

			}

			return t; /* return token */

		case '+':

			t.code = ART_OP_T; /* it is an Arithmetic operator, sets and returns token accordingly */
			t.attribute.arr_op = PLUS;
			return t;

		case '-':

			t.code = ART_OP_T; /* it is an Arithmetic operator, sets and returns token accordingly */
			t.attribute.arr_op = MINUS;
			return t;

		case '*':

			t.code = ART_OP_T; /* it is an Arithmetic operator, sets and returns token accordingly */
			t.attribute.arr_op = MULT;
			return t;

		case '/':

			t.code = ART_OP_T; /* it is an Arithmetic operator, sets and returns token accordingly */
			t.attribute.arr_op = DIV;
			return t;

		case '<':

			test = b_getc(sc_buf); /* look ahead to the next character */

			if (test == '>') {

				t.code = REL_OP_T; /* if next char is > it is a relational operator, sets and returns token accordingly */
				t.attribute.rel_op = NE;
				return t;

			}
			else if (test == '<') {

				t.code = SCC_OP_T; /* if next char is < it is a string concatenation operator, sets and returns token accordingly */
				return t;

			}
			else {

				b_retract(sc_buf); /* if look ahead is other retract the buffer */
				t.code = REL_OP_T; /* it is a relational operator, sets and returns token accordingly */
				t.attribute.rel_op = LT;
				return t;

			}

		case '>':

			t.code = REL_OP_T; /* it is a relational operator, sets and returns token accordingly */
			t.attribute.rel_op = GT;
			return t;

		case '.':

			b_mark(sc_buf, b_getcoffset(sc_buf)); /* mark the buffer so we can look forward more than one character */

			if (b_getc(sc_buf) == 'A') { /* try to find .AND. if at any point is it fails we reset the buffer and create an error token */

				if (b_getc(sc_buf) == 'N') {

					if (b_getc(sc_buf) == 'D') {

						if (b_getc(sc_buf) == '.') {

							t.code = LOG_OP_T; /* if successful it is a logical operator, set and return token accordingly */
							t.attribute.log_op = AND;
							return t;

						}
						else {

							b_reset(sc_buf);

						}

					}
					else {

						b_reset(sc_buf);

					}

				}
				else {

					b_reset(sc_buf);

				}

			}
			else {

				b_reset(sc_buf);

			}

			if (b_getc(sc_buf) == 'O') { /* try to find .OR. if at any point is it fails we reset the buffer and create an error token */

				if (b_getc(sc_buf) == 'R') {

					if (b_getc(sc_buf) == '.') {

						t.code = LOG_OP_T; /* if successful it is a logical operator, set and return token accordingly */
						t.attribute.log_op = OR;
						return t;

					}
					else {

						b_reset(sc_buf);

					}

				}
				else {

					b_reset(sc_buf);

				}

			}
			else {

				b_reset(sc_buf);

			}


			t.code = ERR_T; /* if failed it is an error token, sets and returns Token accordingly */
			t.attribute.err_lex[0] = c;
			t.attribute.err_lex[1] = '\0';
			return t;

		case ')':

			t.code = RPR_T; /* it is a right parenthesis, sets and returns token accordingly */
			return t;

		case '(':

			t.code = LPR_T; /* it is a left parenthesis, sets and returns token accordingly */
			return t;

		case '}':

			t.code = RBR_T; /* it is a right brace, sets and returns token accordingly */
			return t;

		case '{':

			t.code = LBR_T; /* it is a left brace, sets and returns token accordingly */
			return t;

		case ',':

			t.code = COM_T; /* it is a comma, sets and returns token accordingly */
			return t;

		case ';':

			t.code = EOS_T; /* it is an end of statment, sets and returns token accordingly */
			return t;

		case '!':

			test = b_getc(sc_buf); /* looks forward a char */

			if (test == '!') { /* if another ! is found it is a comment */

				while (test != '\n' && test != SEOF && test != SEOF_1) { /* loop and ignore everything after the comment !! */

					test = b_getc(sc_buf);

				}

				if (test == SEOF || test == SEOF_1) { /* if char is SEOF retract buffer */

					b_retract(sc_buf);
					break;

				}

				line++; /* increment line */
				break;

			}
			else {

				t.code = ERR_T; /* if the second char isn't a ! it is an error, set token accordingly */
				t.attribute.err_lex[0] = c;
				t.attribute.err_lex[1] = test;
				t.attribute.err_lex[2] = '\0';

				test = b_getc(sc_buf);

				while (test != '\n' && test != SEOF && test != SEOF_1) { /* still ignore the rest of the line */

					test = b_getc(sc_buf);

				}

				if (test == SEOF || test == SEOF_1) {

					b_retract(sc_buf); /* SEOF is found retract */

				}

				return t; /* return Token */

			}

		case '\n':

			line++; /* if new line found increment line */
			break;

		case SEOF_1:

			t.code = SEOF_T; /* is it EOF, sets and returns token accordingly */
			t.attribute.seof = SEOF_EOF;
			return t;

		case SEOF:

			t.code = SEOF_T; /* is it SEOF, sets and returns token accordingly */
			t.attribute.seof = SEOF_0;
			return t;

		default: /* if any other character use finite state machine to process */

			lexstart = b_mark(sc_buf, b_getcoffset(sc_buf) - 1); /* save the start of the lexeme */
			state = 0; /* start at state 0 */

			while (TRUE) {

				state = get_next_state(state, c); /* get next state */

				if (as_table[state] == NOAS) {

					c = b_getc(sc_buf); /* if it is not an accepting state get next char and process again */
					continue;

				}
				else { /* else it is an accepting state, process accordingly */

					if (as_table[state] == ASWR) {

						b_retract(sc_buf); /* if it is an accpeting state with retract, retract */

					}

					lexend = b_getcoffset(sc_buf); /* mark the end of the lexeme */

					b_reset(sc_buf); /* reset the source buffer */

					lex_buf = b_allocate(DEFAULT_INIT_CAPACITY, DEFAULT_INC_FACTOR, 'a'); /* initialize temp lexeme buffer */

					if (lex_buf == NULL) { /* if allocate failed run time error, set and return token accordingly */

						scerrnum = RT_ERROR;
						t.code = RTE_T;
						addStringToAttribute(rtError, t.attribute.err_lex, ERR_LEN, FALSE);

					}

					while (b_getcoffset(sc_buf) != lexend) {

						b_addc(lex_buf, b_getc(sc_buf)); /* copy to lexeme buffer char by char */

					}

					lex_buf = b_compact(lex_buf, '\0'); /* compact lex buffer and add null terminator */

					b_rewind(lex_buf); /* rewinf lex buf */

					t = aa_table[state](b_location(lex_buf)); /* call appropriate accepting function with pointers */

					b_free(lex_buf); /* release lex buffer memory */

					return t; /* return complete token */

				}

			}

		}

	}

}

/* Fuction Name: get_next_state
*  Purpose: using the current char and state, looks up the next state in the st_table
*  Author: Svillen Ranev
*  History/Versions: 1.0
*  Called Functions: char_class()
*  Parameters: state: [Type: int], [Range: int range], [Description: current state]
*			   c: [Type: char], [Range: char range ], [Description: current char passed with the state ]
*  Return value: [Type: int] next state found from the table
*  Algorithm: finds the the column of the char and uses that and the state to look up the next state in st_table
*			  and returns that number;
*/
int get_next_state(int state, char c) {

	int col;
	int next;
	col = char_class(c);
	next = st_table[state][col];

#ifdef DEBUG

	printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);

#endif

	assert(next != IS);

#ifdef DEBUG

	if (next == IS) {

		printf("Scanner Error: Illegal state:\n");
		printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
		exit(1);

	}

#endif

	return next;

}

/* Fuction Name: char_class
*  Purpose: Find what column in the table the current char falls into
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [November 9th, 2019]
*  Called Functions: N/A
*  Parameters: c: [Type: char], [Range: char range ], [Description: current char]
*  Return value: [Type: int] the column index value
*  Algorithm: uses and switch statemnt to find the column index
*/
int char_class(char c) {

	switch (c) {

	case '0':

		return COL2;

	case '.':

		return COL4;

	case '@':

		return COL5;

	case '"':

		return COL6;

	case SEOF_1:
	case SEOF:

		return COL7;

	default:

		if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z') { /* uses the char numeric values to find [a-zA-Z] */

			return COL1;

		}
		else if (c >= '1' && c <= '9') { /* uses the char numeric values to find [1-9] */

			return COL3;

		}
		else {

			return COL8; /* if no match is found processed as other */

		}

	}

}

/* Fuction Name: aa_func02
*  Purpose: accepting function for AVID
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [November 9th, 2019]
*  Called Functions: iskeyword(), strncpy()
*  Parameters: lexeme: [Type: char[]], [Range: N/A], [Description: C-type string ]
*  Return value: [Type: Token] completed token
*  Algorithm: Find out is the lexeme is a keyword or not, sets and returns Token accordingly
*/
Token aa_func02(char lexeme[]) {

	Token t = { 0 }; /* initialize token */

	if (iskeyword(lexeme)) {

		t.code = KW_T; /* it is a keyword, set and return token accordingly */
		t.attribute.kwt_idx = iskeyword(lexeme) - 1;
		return t;

	}

	t.code = AVID_T; /* it is a AVID, set and return token accordingly */
	strncpy(t.attribute.vid_lex, lexeme, VID_LEN); /* copy first 8 characters to attribute*/
	t.attribute.vid_lex[8] = '\0'; //add null terminator

	return t; /* return Token */

}

/* Fuction Name: aa_func03
*  Purpose: accepting function for SVID
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [November 9th, 2019]
*  Called Functions: addStringToAttribute()
*  Parameters: lexeme: [Type: char[]], [Range: N/A], [Description: C-type string ]
*  Return value: [Type: Token] completed token
*  Algorithm: sets and returns token accordingly
*/
Token aa_func03(char lexeme[]) {

	Token t = { 0 }; /* initialize token */

	t.code = SVID_T; /* is a SVID, set and return token accordingly */
	addStringToAttribute(lexeme, t.attribute.vid_lex, VID_LEN, TRUE);

	return t;

}

/* Fuction Name: aa_func08
*  Purpose: accepting function for FPL
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [November 9th, 2019]
*  Called Functions: atof(), addStringToAttribute()
*  Parameters: lexeme: [Type: char[]], [Range: N/A], [Description: C-type string ]
*  Return value: [Type: Token] completed token
*  Algorithm: validates float and sets and returns token accordingly
*/
Token aa_func08(char lexeme[]) {

	Token t = { 0 }; /* initialize token */
	double number = atof(lexeme); /* convert to double to check for overflow */

	if (number <= FLT_MAX && number >= FLT_MIN || number == 0.0) { /* make sure number is in valid float range */

		t.attribute.flt_value = (float)number; /* set and return token accordingly */
		t.code = FPL_T;

	}
	else {

		t.code = ERR_T; /* if error set and return token accordingly */
		addStringToAttribute(lexeme, t.attribute.err_lex, ERR_LEN, FALSE);

	}

	return t;

}

/* Fuction Name: aa_func05
*  Purpose: accepting function for IL/DIL
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [November 9th, 2019]
*  Called Functions: atol(), addStringToAttribute()
*  Parameters: lexeme: [Type: char[] ], [Range: N/A], [Description: C-type string ]
*  Return value: [Type: Token] completed token
*  Algorithm: validates int, sets and returns token accordingly
*/
Token aa_func05(char lexeme[]) {

	Token t = { 0 }; /* initialize token */
	long number = atol(lexeme); /* convert to long to check for overflow */

	if (number <= SHRT_MAX && number >= SHRT_MIN) { /* make sure number is in valid short range */

		t.attribute.int_value = (short)number; /* set and return token accordingly */
		t.code = INL_T;

	}
	else {

		t.code = ERR_T; /* if error, set and return token accordingly */
		addStringToAttribute(lexeme, t.attribute.err_lex, ERR_LEN, FALSE);

	}

	return t;

}

/* Fuction Name: aa_func10
*  Purpose: accepting function for string literals
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [November 9th, 2019]
*  Called Functions: b_limit(), strlen(), b_addc()
*  Parameters: lexeme: [Type: char[] ], [Range: N/A], [Description: C-type string ]
*  Return value: [Type: Token] completed token
*  Algorithm: adds string to SLT, and sets and returns token accordingly
*/
Token aa_func10(char lexeme[]) {

	Token t = { 0 }; /* initialize token */
	t.attribute.str_offset = b_limit(str_LTBL); /* set attribute */

	for (int i = 1; i < (int)strlen(lexeme) - 1; i++) { /* copy lexeme to SLT without the '"'*/

		if (lexeme[i] == '\n') {

			line++; /* if new line character if found increment line */

		}

		b_addc(str_LTBL, lexeme[i]);

	}

	b_addc(str_LTBL, '\0'); /* add null terminator*/

	t.code = STR_T; /* set and return token */

	return t;

}

/* Fuction Name: aa_func11
*  Purpose: accepting function for error tokens
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [November 9th, 2019]
*  Called Functions: addStringToAttribute()
*  Parameters: lexeme: [Type: char[] ], [Range: N/A], [Description: C-type string ]
*  Return value: [Type: Token] completed token
*  Algorithm: sets and returns token accordingly
*/
Token aa_func11(char lexeme[]) {

	Token t = { 0 }; /* initialize token */

	t.code = ERR_T; /* set token */
	addStringToAttribute(lexeme, t.attribute.err_lex, ERR_LEN, FALSE);

	for (int i = 0; i < (int)strlen(lexeme); i++) {

		if (lexeme[i] == '\n') { /* looks through lexeme for new line characters and increments line */

			line++;

		}

	}

	return t; /* return token */

}

/* Fuction Name: iskeyword
*  Purpose: finds out if the lexeme is a keyword
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [November 9th, 2019]
*  Called Functions: strcmp()
*  Parameters: lexeme: [Type: char[] ], [Range: N/A], [Description: C-type string ]
*  Return value: [Type: int] the index of the keyword in the keyword table +1 and 0 if not match
*  Algorithm: discovers if the lexeme is a keyword
*/
int iskeyword(char* kw_lexeme) {

	for (int i = 1; i < 11; i++) {

		if (!strcmp(kw_lexeme, kw_table[i - 1])) { /* if match return i */

			return i;

		}

	}

	return 0;

}

/* Fuction Name: addStringToAttribute
*  Purpose: adds lexeme to the token attribute and truncates the lexeme if needed
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [November 9th, 2019]
*  Called Functions: strlen(), strncpy()
*  Parameters: lexeme: [Type: char[] ], [Range: N/A], [Description: C-type string ]
*			   tAttrib: [Type: char[] ], [Range: N/A], [Description: C-type string ]
*			   len: [Type: int ], [Range: int range], [Description: the desired lenths to be copied to the attribute ]
*			   isSVID: [Type: int ], [Range: int range ], [Description: 0 if not a SVID otherwise is a SVID ]
*  Return value: N/A
*  Algorithm: adds lexeme to the token attribute and truncates the lexeme if needed
*/
void addStringToAttribute(char* lexeme, char* tAttrib, int len, int isSVID) {

	if ((int)strlen(lexeme) > len) {

		if (isSVID) { /* if longer then len, copy and truncate */

			strncpy(tAttrib, lexeme, len - 1);
			tAttrib[len - 1] = '@';
			tAttrib[len] = '\0';

		}
		else {

			strncpy(tAttrib, lexeme, len - 3); /* if longer then len, copy and truncate */
			tAttrib[len - 3] = '.';
			tAttrib[len - 2] = '.';
			tAttrib[len - 1] = '.';
			tAttrib[len] = '\0';

		}

	}
	else {

		strcpy(tAttrib, lexeme); /* else just copy the whole lexeme*/

	}

}