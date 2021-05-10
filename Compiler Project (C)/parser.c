/* File Name: parser.c
*  Compiler: MS Visual Studio 2019
*  Author: Jason MacFarlane, 040-890-266
*  Course: CST8152 - Compilers, Lab Section: 012
*  Assignment: 03
*  Date: December 2nd, 2019
*  Professor: Sv. Ranev
*  Purpose: Takes tokens produced by the scanner and parses the file
*  Function list: parser(), match(), syn_printe(),
*				  syn_eh(), gen_incode(), program(), opt_statements(),
*				  statements(), statement(), statments_dir(), assignment_statement(),
*				  assignment_expression(), selection_statement(), iteration_statement(),
*				  pre_condition(), input_statement(), variable_list(),
*				  variable_list_dir(), variable_identifier(), output_statment()
*				  output_list(), opt_variable_list(), arithmetic_expression(),
*				  unary_arithmetic_expression(), additive_arithmetic_expression(),
*				  additive_arithmetic_expression_dir(), mulitplicative_arithmetic_expression(),
*				  mulitplicative_arithmetic_expression_dir(), primary_arithmetic_expression(),
*				  string_expression(), string_expression_dir(), primary_string_expression(),
*				  conditional_expression(), logical_or_expression(), 
*				  logical_or_expression_dir(), logical_and_expression(),
*				  logical_and_expression_dir(), relational_expression(),
*			      primary_a_relational_expression(), primary_s_relational_expression(),
*				  relational_op()
*/

/* Include statements */
#include "token.h"
#include "parser.h"
#include "buffer.h"

#include <stdio.h>
#include <stdlib.h> /* Constants for calls to exit()*/

static Token lookahead; /* Global variable to hold current Token */
extern int line; /* External variable with the current line number */
extern pBuffer str_LTBL; /* External buffer holding the string literal table */
extern char* kw_table[]; /* External string array holding the keywords */
int synerrno; /* Global variable to hold the current number of syntax errors found*/


/* Fuction Name: parser
*  Purpose: Starts the parsing of a PLATYPUS program
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [November 26th, 2019]
*  Called Functions: malar_next_token(), program(), match(), gen_incode()
*  Parameters: N/A
*  Return value: N/A
*  Algorithm: Get the first token and starts parsing by calling program()
*/
void parser(void) {

	lookahead = malar_next_token(); /* Get first Token */
	program(); match(SEOF_T, NO_ATTR); /* Start parsing */
	gen_incode("PLATY: Source file parsed");

}

/* Fuction Name: match
*  Purpose: Matches a desired token with the token currently saved in the lookahead variable
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [November 26th, 2019]
*  Called Functions: malar_next_token(), syn_printe(), syn_eh()
*  Parameters: pr_token_code: [Type: int], [Range: Valid int range], [Description: Token code to match]
*			   pr_token_attribute: [Type: int], [Range: Valid int range], [Description: Token attribute to match]
*  Return value: N/A
*  Algorithm: Checks to see if the token is a match, if not calls error handling function syn_eh()
*/
void match(int pr_token_code, int pr_token_attribute) {

	int match = FALSE; /* boolean to hold the current match value */
	
	if (pr_token_code == lookahead.code) { /* Try to match Token codes */

		match = TRUE;

		switch (pr_token_code) { /* Contine to check attributes as well, if needed (depends on the Token code) */

		case KW_T:

			if (pr_token_attribute != lookahead.attribute.kwt_idx) {

				match = FALSE;

			}
			break;

		case LOG_OP_T:

			if (pr_token_attribute != lookahead.attribute.log_op) {

				match = FALSE;

			}
			break;

		case ART_OP_T:

			if (pr_token_attribute != lookahead.attribute.arr_op) {

				match = FALSE;

			}
			break;

		case REL_OP_T:

			if (pr_token_attribute != lookahead.attribute.rel_op) {

				match = FALSE;

			}
			break;

		}

	}

	if (match) {

		if (pr_token_code == SEOF_T) { /* Check for SEOF Token */

			return;

		}
		else {

			lookahead = malar_next_token(); /* Continue to next Token */
				
			if (lookahead.code == ERR_T) { /* Check for error Token */

				syn_printe(); /* Print error token */
				lookahead = malar_next_token(); /* Continue to next Token */
				++synerrno; /* Increase the variable holding the number of syntax errors */
				return;

			}

		}

	}
	else {

		syn_eh(pr_token_code); /* If no match was found call error handling function */
		return;

	}

}

/* Fuction Name: syn_eh
*  Purpose: Token error handling function
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [November 26th, 2019]
*  Called Functions: syn_printe(), exit(), malar_next_token
*  Parameters: pr_token_code: [Type: int], [Range: Valid int range], [Description: Token code that is needed to match]
*  Return value: N/A
*  Algorithm: Cycles through tokens until match is found.
*/
void syn_eh(int pr_token_code) {

	syn_printe(); /* Print the token causing the error */
	++synerrno;

	while (TRUE) {

		if (pr_token_code != SEOF_T && lookahead.code == SEOF_T) {

			exit(synerrno); /* If end of file exit */ 

		}

		lookahead = malar_next_token(); /* Continue to next Token*/

		if (pr_token_code == lookahead.code) { /* If match is found and token is not SEOF_T */

			if (pr_token_code != SEOF_T) {

				lookahead = malar_next_token(); /* Continue to next Token */
				return;

			}
			else {

				return;

			}

		}

	}

}

/* Fuction Name: syn_printe
*  Purpose: Print the token causing the error with attribute and line number
*  Author: Svillen Ranev
*  History/Versions: 1.0 
*  Called Functions: printf(), b_mark()
*  Parameters: N/A
*  Return value: N/A
*  Algorithm: Uses switch statement to find Token code and prints values
*/
void syn_printe(void) {

	Token t = lookahead;

	printf("PLATY: Syntax error:  Line:%3d\n", line);
	printf("*****  Token code:%3d Attribute: ", t.code);

	switch (t.code) {

	case  ERR_T: /* ERR_T     0   Error token */

		printf("%s\n", t.attribute.err_lex);
		break;

	case  SEOF_T: /*SEOF_T    1   Source end-of-file token */

		printf("SEOF_T\t\t%d\t\n", t.attribute.seof);
		break;

	case  AVID_T: /* AVID_T    2   Arithmetic Variable identifier token */
	case  SVID_T:/* SVID_T    3  String Variable identifier token */

		printf("%s\n", t.attribute.vid_lex);
		break;

	case  FPL_T: /* FPL_T     4  Floating point literal token */

		printf("%5.1f\n", t.attribute.flt_value);
		break;

	case INL_T: /* INL_T      5   Integer literal token */

		printf("%d\n", t.attribute.get_int);
		break;

	case STR_T:/* STR_T     6   String literal token */

		b_mark(str_LTBL, t.attribute.str_offset);
		printf("%s\n", b_location(str_LTBL));
		break;

	case SCC_OP_T: /* 7   String concatenation operator token */

		printf("NA\n");
		break;

	case  ASS_OP_T:/* ASS_OP_T  8   Assignment operator token */

		printf("NA\n");
		break;

	case  ART_OP_T:/* ART_OP_T  9   Arithmetic operator token */

		printf("%d\n", t.attribute.get_int);
		break;

	case  REL_OP_T: /*REL_OP_T  10   Relational operator token */

		printf("%d\n", t.attribute.get_int);
		break;

	case  LOG_OP_T:/*LOG_OP_T 11  Logical operator token */

		printf("%d\n", t.attribute.get_int);
		break;

	case  LPR_T: /*LPR_T    12  Left parenthesis token */

		printf("NA\n");
		break;

	case  RPR_T: /*RPR_T    13  Right parenthesis token */

		printf("NA\n");
		break;

	case LBR_T: /*    14   Left brace token */

		printf("NA\n");
		break;

	case RBR_T: /*    15  Right brace token */

		printf("NA\n");
		break;

	case KW_T: /*     16   Keyword token */

		printf("%s\n", kw_table[t.attribute.get_int]);
		break;

	case COM_T: /* 17   Comma token */

		printf("NA\n");
		break;

	case EOS_T: /*    18  End of statement *(semi - colon) */

		printf("NA\n");
		break;

	default:

		printf("PLATY: Scanner error: invalid token code: %d\n", t.code);

	}/*end switch*/

}/* end syn_printe()*/

/* Fuction Name: gen_incode
*  Purpose: Prints the string passed through the parameters and also prints new line
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [November 26th, 2019]
*  Called Functions: printf()
*  Parameters: string: [Type: char*], [Range: Valid pointer range], [Description: C-type string to be printed]
*  Return value: N/A
*  Algorithm: Prints specified C-type string with new line
*/
void gen_incode(char* string) {

	printf("%s\n", string);

}

/*  <program> ->
*  	PLATYPUS {<opt_statements>} 
*
*  FIRST(<program>) = {KW_T(PLATYPUS)}
*/
void program(void) {

	/* Tries to match "PLATYPUS{<opt_statements>}" */
	match(KW_T, PLATYPUS); match(LBR_T, NO_ATTR); opt_statements();
	match(RBR_T, NO_ATTR);
	gen_incode("PLATY: Program parsed");

}

/*<opt_statements> ->
*		<statements> | e
*
*  FIRST(<opt_statements>) = {AVID, SVID, KW_T(IF), KW_T(WHILE), KW_T(READ), KW_T(WRITE), e}
*/
void opt_statements(void) {

	/* Tries to match Token with member of <opt_statements> FIRST set */
	switch (lookahead.code) {

	case AVID_T:
	case SVID_T:

		statements();
		break;

	case KW_T:

		switch (lookahead.attribute.kwt_idx) {
			
		case IF:
		case WHILE:
		case READ:
		case WRITE:

			statements();
			break;

		default:
			gen_incode("PLATY: Opt_statements parsed");
			break;
		}

		break;


	default:

		gen_incode("PLATY: Opt_statements parsed");
		break;

	}


}

/* <statements> ->
*	<statement> <statements’>
*
*  FIRST(<statements>) = {AVID, SVID, KW_T(IF), KW_T(WHILE), KW_T(READ), KW_T(WRITE)}
*/
void statements(void) {

	statement(); statements_dir();

}

/* <statement> ->
*   <assignment statement>
* | <selection statement>
* | <iteration statement>
* | <input statement>
* | <output statement>
*
*  FIRST(<statement>) = {AVID, SVID, KW_T(IF), KW_T(WHILE), KW_T(READ), KW_T(WRITE)}
*/
void statement(void) {

	/* Tries to match Token with member of <statement> FIRST set */
	switch (lookahead.code) {

	case AVID_T:
	case SVID_T:
		assignment_statement();
		break;

	case KW_T:

		switch (lookahead.attribute.kwt_idx) {

		case IF:

			selection_statement();
			break;

		case WHILE:

			iteration_statement();
			break;

		case READ:

			input_statement();
			break;

		case WRITE:

			output_statement();
			break;

		}

		break;

	default: /* If no match is found, print the Token */

		syn_printe();
		break;

	}

}

/* <statements’> ->
*	<statement> <statements’> | e
*
*  FIRST(<statements’>) = {AVID, SVID, KW_T(IF), KW_T(WHILE), KW_T(READ), KW_T(WRITE), e}
*/
void statements_dir(void) {

	/* Tries to match Token with member of <statements'> FIRST set */
	switch (lookahead.code) {

	case AVID_T:
	case SVID_T:

		statement(); statements_dir();
		break;

	case KW_T:

		switch (lookahead.attribute.kwt_idx) {

		case IF:
		case WHILE:
		case READ:
		case WRITE:

			statement(); statements_dir();
			break;

		}

		break;

	}

}

/* <assignment statement> -> 
*	<assignment expression>;
*
*  FIRST(<assignment statement>) = {AVID, SVID}
*/
void assignment_statement(void) {

	assignment_expression(); match(EOS_T, NO_ATTR);

	gen_incode("PLATY: Assignment statement parsed");

}

/* < assignment expression> -> 
*   AVID = <arithmetic expression>
* | SVID = <string expression>
*
*  FIRST(<assignment expression>) = {AVID, SVID}
*/
void assignment_expression(void) { 

	/* tries to match Token with member of FIRST set */
	if (lookahead.code == AVID_T) {

		match(AVID_T, NO_ATTR); match(ASS_OP_T, NO_ATTR); arithmetic_expression();

		gen_incode("PLATY: Assignment expression (arithmetic) parsed");

	}
	else if(lookahead.code == SVID_T){

		match(SVID_T, NO_ATTR); match(ASS_OP_T, NO_ATTR); string_expression();

		gen_incode("PLATY: Assignment expression (string) parsed");

	}
	else {

		syn_printe();

		gen_incode("PLATY: Assignment expression parsed"); 

	}

}

/* <selection statement> ->
*  IF <pre-condition> (<conditional expression>) THEN {<opt_statements>}
*  ELSE {<opt_statements>};
*
*  FIRST(<selection statement>) = {KW_T(IF)}
*/
void selection_statement(void) {

	match(KW_T, IF); pre_condition(); match(LPR_T, NO_ATTR);
	conditional_expression(); match(RPR_T, NO_ATTR); match(KW_T, THEN);
	match(LBR_T, NO_ATTR); opt_statements(); match(RBR_T, NO_ATTR);
	match(KW_T, ELSE); match(LBR_T, NO_ATTR); opt_statements();
	match(RBR_T, NO_ATTR); match(EOS_T, NO_ATTR); 

	gen_incode("PLATY: Selection statement parsed");

}

/* <iteration statement> ->
*   WHILE <pre-condition> (<conditional expression>)
*   REPEAT { <statements>};
*
*  FIRST(<iteration statement>) = {KW_T(WHILE)} 
*/
void iteration_statement(void) {

	match(KW_T, WHILE); pre_condition(); match(LPR_T, NO_ATTR);
	conditional_expression(); match(RPR_T, NO_ATTR); match(KW_T, REPEAT);
	match(LBR_T, NO_ATTR); statements(); match(RBR_T, NO_ATTR);
	match(EOS_T, NO_ATTR); 

	gen_incode("PLATY: Iteration statement parsed");

}

/* <pre-condition> ->
*	TRUE | FALSE
*
*  FIRST(<pre-condition>) = {KW_T(TRUE), KW_T(FALSE)}
*/
void pre_condition(void) {

	if (lookahead.attribute.kwt_idx == KW_TRUE) {

		match(KW_T, KW_TRUE);

	}
	else if(lookahead.attribute.kwt_idx == KW_FALSE){

		match(KW_T, KW_FALSE);

	}
	else {

		syn_printe();

	}

}

/* <input statement> ->
* READ (<variable list>);
*
*  FIRST(<input statement>) = {KW_T(READ)}
*/
void input_statement(void) {

	match(KW_T, READ); match(LPR_T, NO_ATTR);
	variable_list(); match(RPR_T, NO_ATTR); match(EOS_T, NO_ATTR);

	gen_incode("PLATY: Input statement parsed");

}

/* <variable list> ->
*	<variable identifier> <variable list’>
*
*  FIRST(<variable list>) = {AVID_T, SVID_T} 
*/
void variable_list(void) {

	variable_identifier(); variable_list_dir();

	gen_incode("PLATY: Variable list parsed");

}

/* <variable list’> ->
*	,<variable identifier> <variable list’> | e
*
*  FIRST(<variable list’>) = {COM_T, e} 
*/
void variable_list_dir(void) {

	if (lookahead.code == COM_T) {

		match(COM_T, NO_ATTR); variable_identifier(); variable_list_dir();

	}

}

/* <variable identifier> ->
* 	 AVID_T
*	|SVID_T
*
* FIRST(<variable identifier>) = {AVID_T, SVID_T} 
*/
void variable_identifier(void) {

	if (lookahead.code == AVID_T) {

		match(AVID_T, NO_ATTR);

	}
	else if (lookahead.code == SVID_T){

		match(SVID_T, NO_ATTR);

	}
	else {

		syn_printe();

	}

}

/* <output statement> ->
*	WRITE (<output list>);
*
*  FIRST(<output statement>) = {KW_T(WRITE)} 
*/
void output_statement(void) {

	match(KW_T, WRITE); match(LPR_T, NO_ATTR);
	output_list(); match(RPR_T, NO_ATTR ); match(EOS_T, NO_ATTR);

	gen_incode("PLATY: Output statement parsed");

}

/* <output list> ->
*	<opt_variable list> | STR_T
*
*  FIRST(<output list>) = {AVID_T, SVID_T, e, STR_T} 
*/
void output_list(void) {

	if (lookahead.code == STR_T) {

		match(STR_T, NO_ATTR);

		gen_incode("PLATY: Output list (string literal) parsed");

	}
	else{

		opt_variable_list();

	}


}

/* <opt_variable list> ->
*	<variable list> | e
*
*  FIRST(<opt_variable list>) = {AVID_T, SVID_T, e} 
*/
void opt_variable_list(void) {

	if (lookahead.code == AVID_T || lookahead.code == SVID_T) {

		variable_list();

	}
	else {

		gen_incode("PLATY: Output list (empty) parsed");

	}

}

/* <arithmetic expression> - >
*   <unary arithmetic expression>  
* | <additive arithmetic expression>	
*
*  FIRST(<arithmetic expression>) = {ART_OP_T(MINUS), ART_OP_T(PLUS), AVID_T, FPL_T, INL_T, LPR_T}
*/
void arithmetic_expression(void) {

	if (lookahead.code == ART_OP_T) {

		if (lookahead.attribute.arr_op == PLUS || lookahead.attribute.arr_op == MINUS) {

			unary_arithmetic_expression();

		}
	}
	else if(lookahead.code == AVID_T || lookahead.code == FPL_T || lookahead.code == INL_T || lookahead.code == LPR_T){

		additive_arithmetic_expression();

	}
	else {

		syn_printe();

	}

	gen_incode("PLATY: Arithmetic expression parsed"); 

}

/* <unary arithmetic expression> ->
*   -  <primary arithmetic expression> 
* | + <primary arithmetic expression>
*
*  FIRST(<unary arithmetic expression>) = {ART_OP_T(MINUS), ART_OP_T(PLUS)}
*/
void unary_arithmetic_expression(void) {

	if (lookahead.attribute.arr_op == PLUS) {

		match(ART_OP_T, PLUS);
		primary_arithmetic_expression();

	}
	else if (lookahead.attribute.arr_op == MINUS) {

		match(ART_OP_T, MINUS);
		primary_arithmetic_expression();

	}
	else {

		syn_printe();

	}

	gen_incode("PLATY: Unary arithmetic expression parsed");

}
/* <additive arithmetic expression> ->
*	<multiplicative arithmetic expression> <additive arithmetic expression’>
*
*  FIRST(<additive arithmetic expression>) = {AVID_T, FPL_T, INL_T, LPR_T}
*/
void additive_arithmetic_expression(void) {

	mulitplicative_arithmetic_expression(); additive_arithmetic_expression_dir();

}

/* <additive arithmetic expression’> ->
*	+ <multiplicative arithmetic expression> <additive arithmetic expression’>
*	| - <multiplicative arithmetic expression> <additive arithmetic expression’>
*	| e
*
* FIRST(<additive arithmetic expression’>) = {ART_OP_T(PLUS), ART_OP_T(MINUS), e} 
*/
void additive_arithmetic_expression_dir(void) {

	if (lookahead.code == ART_OP_T) {

		if (lookahead.attribute.arr_op == PLUS) {

			match(ART_OP_T, PLUS); mulitplicative_arithmetic_expression();  additive_arithmetic_expression_dir();

			gen_incode("PLATY: Additive arithmetic expression parsed");

		}
		else if (lookahead.attribute.arr_op == MINUS) {

			match(ART_OP_T, MINUS); mulitplicative_arithmetic_expression(); additive_arithmetic_expression_dir();

			gen_incode("PLATY: Additive arithmetic expression parsed");

		}

	}

}

/* <multiplicative arithmetic expression> ->
* <primary arithmetic expression> <multiplicative arithmetic expression’>
*
*  FIRST(<multiplicative arithmetic expression>) = {AVID_T, FPL_T, INL_T, LPR_T} 
*/
void mulitplicative_arithmetic_expression(void) {

	primary_arithmetic_expression(); mulitplicative_arithmetic_expression_dir();

}

/* <multiplicative arithmetic expression’> ->
* <primary arithmetic expression> <multiplicative arithmetic expression’>
* | / <primary arithmetic expression> <multiplicative arithmetic expression’>
* | e
*
*  FIRST(<multiplicative arithmetic expression’>) = {ART_OP_T(MULT), ART_OP_T(DIV), e} 
*/
void mulitplicative_arithmetic_expression_dir(void) {

	if (lookahead.code == ART_OP_T) {

		if (lookahead.attribute.arr_op == MULT) {

			match(ART_OP_T, MULT); primary_arithmetic_expression(); mulitplicative_arithmetic_expression_dir();

			gen_incode("PLATY: Multiplicative arithmetic expression parsed");

		}
		else if (lookahead.attribute.arr_op == DIV) {

			match(ART_OP_T, DIV); primary_arithmetic_expression(); mulitplicative_arithmetic_expression_dir();

			gen_incode("PLATY: Multiplicative arithmetic expression parsed");

		}

	}

}

/* <primary arithmetic expression> ->
*  AVID_T
* | FPL_T
* | INL_T
* | (<arithmetic expression>)	
*
*  FIRST(<primary arithmetic expression>) = {AVID_T, FPL_T, INL_T, LPR_T} 
*/
void primary_arithmetic_expression(void) {

	switch (lookahead.code) {

	case AVID_T:

		match(AVID_T, NO_ATTR);
		break;

	case FPL_T:

		match(FPL_T, NO_ATTR);
		break;

	case INL_T:

		match(INL_T, NO_ATTR);
		break;

	case LPR_T:

		match(LPR_T, NO_ATTR);
		arithmetic_expression();
		match(RPR_T, NO_ATTR);
		break;

	default:

		syn_printe();
		break;

	}

	gen_incode("PLATY: Primary arithmetic expression parsed"); 

}

/* <string expression> ->
* <primary string expression> <string expression’>
*
*  FIRST(<string expression>) = {SVID_T, STR_T} 
*/
void string_expression(void) {

	primary_string_expression(); string_expression_dir();
	gen_incode("PLATY: String expression parsed");

}

/* <string expression’> ->
*	<< <primary string expression> <string expression’>
*	| e
*
*  FIRST(<string expression’>) = {SCC_T, e} 
*/
void string_expression_dir(void) {

	if (lookahead.code == SCC_OP_T) {

		match(SCC_OP_T, NO_ATTR); primary_string_expression(); string_expression_dir();

	}

}

/* <primary string expression> ->
*   SVID_T
* | STR_T
*
*  FIRST(<primary string expression>) = {SVID_T, STR_T}
*/
void primary_string_expression(void) {

	if (lookahead.code == SVID_T) {

		match(SVID_T, NO_ATTR);

	}
	else if (lookahead.code == STR_T) {

		match(STR_T, NO_ATTR);

	}
	else {

		syn_printe();

	}

	gen_incode("PLATY: Primary string expression parsed"); 

}

/* <conditional expression> ->
* <logical OR  expression>
*
*  FIRST(<conditional expression>) = {AVID_T, FPL_T, INL_T, SVID_T, STR_T} 
*/
void conditional_expression(void) {

	logical_or_expression();

	gen_incode("PLATY: Conditional expression parsed");

}

/* <logical  OR expression> ->
* <logical AND expression> <logical OR expression’>
*
*  FIRST(<logical OR expression>) = {AVID_T, FPL_T, INL_T, SVID_T, STR_T} 
*/
void logical_or_expression(void) {

	logical_and_expression(); logical_or_expression_dir();

}

/* <logical OR expression’>
*	.OR. <logical AND expression> < logical OR expression’>
*	| e
*
*  FIRST(<logical OR expression’>) = {LOG_OP_T(OR), e} 
*/
void logical_or_expression_dir(void) {

	if (lookahead.code == LOG_OP_T) {

		if (lookahead.attribute.log_op == OR) {

			match(LOG_OP_T, OR); logical_and_expression(); logical_or_expression_dir();
			gen_incode("PLATY: Logical OR expression parsed");

		}

	}

}

/* <logical  AND expression> ->
* <relational expression> <logical AND expression’>
*
*  FIRST(<logical AND expression>) = {AVID_T, FPL_T, INL_T, SVID_T, STR_T}
*/
void logical_and_expression(void) {

	relational_expression(); logical_and_expression_dir();

}

/* <logical AND expression’>
*	.AND. <relational expression> < logical AND expression’>
*	| e
*
*  FIRST(<logical AND expression’>) = {LOG_OP_T(AND), e} 
*/
void logical_and_expression_dir(void) {

	if (lookahead.code == LOG_OP_T) {

		if (lookahead.attribute.log_op == AND) {

			match(LOG_OP_T, AND); relational_expression(); logical_and_expression_dir();
			gen_incode("PLATY: Logical AND expression parsed");

		}

	}

}

/* <relational expression> ->
*	<primary a_relational expression>  <relational_op> <primary a_relational expression>
* | <primary s_relational expression>  <relational_op> <primary s_relational expression>
*
*  FIRST(<relational expression>) = {AVID_T, FPL_T, INL_T, SVID_T, STR_T}
*/
void relational_expression(void) {

	switch (lookahead.code) {

	case AVID_T:
	case FPL_T:
	case INL_T:

		primary_a_relational_expression(); relational_op(); primary_a_relational_expression();
		break;

	case SVID_T:
	case STR_T:

		primary_s_relational_expression(); relational_op(); primary_s_relational_expression();
		break;

	default:

		syn_printe();
		break;

	}

	gen_incode("PLATY: Relational expression parsed");

}

/* <primary a_relational expression> ->
*   AVID_T
* | FPL_T
* | INL_T
*
*  FIRST(<primary a_relational expression>) = {AVID_T, FPL_T, INL_T} 
*/
void primary_a_relational_expression(void) {

	switch (lookahead.code) {
		
	case AVID_T:

		match(AVID_T, NO_ATTR);
		break;

	case FPL_T:

		match(FPL_T, NO_ATTR);
		break;

	case INL_T:

		match(INL_T, NO_ATTR);
		break;

	default:

		syn_printe();
		break;

	}

	gen_incode("PLATY: Primary a_relational expression parsed");

}

/*<primary s_relational expression> ->
* <primary string expression>
*
*  FIRST(<primary s_relational expression>) = {SVID_T, STR_T}
*/
void primary_s_relational_expression(void) {

	primary_string_expression();

	gen_incode("PLATY: Primary s_relational expression parsed");

}

/*<relational_op> ->
*	==
*	| <>
*	| >
*	| <
*
*  FIRST(<relational_op>) = {REL_OP_T(EQ), REL_OP_T(NE), REL_OP_T(GT), REL_OP_T(LT)} 
*/
void relational_op(void) {

	if (lookahead.code == REL_OP_T) {

		switch (lookahead.attribute.rel_op) {

		case EQ:
			match(REL_OP_T, EQ);
			break;

		case NE:
			match(REL_OP_T, NE);
			break;

		case GT:
			match(REL_OP_T, GT);
			break;

		case LT:
			match(REL_OP_T, LT);
			break;

		default:

			syn_printe();
			break;

		}

	}
	else {

		syn_printe();

	}

}