/* File Name: parser.h
*  Compiler: MS Visual Studio 2019
*  Author: Jason MacFarlane, 040-890-266
*  Course: CST8152 - Compilers, Lab Section: 012
*  Assignment: 03
*  Date: December 2nd, 2019
*  Professor: Sv. Ranev
*  Purpose: Header file for the parser with constant defines, function declarations
*  Function list: malar_next_token(), parser(), match(), syn_printe(),
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


#ifndef  PARSER_H_
#define  PARSER_H_ 

#ifndef TOKEN_H_
#include "token.h"
#endif

/* Defined constants */

#define NO_ATTR -1 /* Value assigned to NO_ATTR */
#define ELSE 0 /* Index value for ELSE in the kw_table */
#define KW_FALSE 1 /* Index value for FALSE in the kw_table */
#define IF 2 /* Index value for IF in the kw_table */
#define PLATYPUS 3 /* Index value for PLATYPUS in the kw_table */
#define READ 4 /* Index value for READ in the kw_table */
#define REPEAT 5 /* Index value for REPEAT in the kw_table */
#define THEN 6 /* Index value for THEN in the kw_table */
#define KW_TRUE 7 /* Index value for TRUE in the kw_table */
#define WHILE 8 /* Index value for WHILE in the kw_table */
#define WRITE 9 /* Index value for WRITE in the kw_table */

/* function declarations */
extern Token malar_next_token(void);
void parser(void);
void match(int, int);
void syn_printe(void);
void syn_eh(int);
void gen_incode(char*);
void program(void);
void opt_statements(void);
void statements(void);
void statement(void);
void statements_dir(void);
void assignment_statement(void);
void assignment_expression(void);
void selection_statement(void);
void iteration_statement(void);
void pre_condition(void);
void input_statement(void);
void variable_list(void);
void variable_list_dir(void);
void variable_identifier(void);
void output_statement(void);
void output_list(void);
void opt_variable_list(void);
void arithmetic_expression(void);
void unary_arithmetic_expression(void);
void additive_arithmetic_expression(void);
void additive_arithmetic_expression_dir(void);
void mulitplicative_arithmetic_expression(void);
void mulitplicative_arithmetic_expression_dir(void);
void primary_arithmetic_expression(void);
void string_expression(void);
void string_expression_dir(void);
void primary_string_expression(void);
void conditional_expression(void);
void logical_or_expression(void);
void logical_or_expression_dir(void);
void logical_and_expression(void);
void logical_and_expression_dir(void);
void relational_expression(void);
void primary_a_relational_expression(void);
void primary_s_relational_expression(void);
void relational_op(void);




#endif