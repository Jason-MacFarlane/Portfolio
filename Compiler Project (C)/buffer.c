/* File Name: buffer.c
*  Compiler: MS Visual Studio 2019
*  Author: Jason MacFarlane, 040-890-266
*  Course: CST8152 - Compilers, Lab Section: 012
*  Assignment: 03
*  Date: December 2nd, 2019
*  Professor: Sv. Ranev
*  Purpose: a muilti-mode buffer to temporarily hold data. 3 modes are fixed,
*			multiplicative, and additive. Takes parameters for initial capacity,
*			increment factor, and mode.
*  Function list: b_allocate(), b_addc(), b_clear(), b_free(), b_isfull(), b_limit(),
*				  b_capacity(), b_mark(), b_mode(), b_incfactor(), b_load(), b_isempty(),
*				  b_getc(), b_eob(), b_print(), b_compact(), b_rflag(), b_retract(), b_reset(),
*				  b_getoffset(), b_rewind(), b_location()
*/

#include "buffer.h"

/* Fuction Name: b_allocate
*  Purpose: Initializes a Buffer structure with a desired capacity, increment factor,
*			and mode.
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [October 2nd, 2019]
*  Called Functions: calloc(), malloc()
*  Parameters: init capacity: [Type: Short], [Range: 0 - 32766], [Description: Desired initial capacity]
*			   inc_factor: [Type: char], [Range: 0 - 255], [Description: Desired Increment factor]
*			   o_mode: [Type: char], [Valid Values: 'f', 'm', 'a'], [Description: Desired mode]
*  Return value: [Type: Buffer Pointer] a pointer to the constructed Buffer, or NULL if there is a failure
*  Algorithm: Checks for valid parameters, creates space on the heap, initializes values for buffer
*			  returns pointer.
*/
Buffer* b_allocate(short init_capacity, char inc_factor, char o_mode) {

	Buffer* pBuffer; /* declaring variable for the buffer pointer that is to be created */

	if (init_capacity < 0 || init_capacity > MAX_BUFFER_CAPACITY) {/* checking parameter for valid range */

		return NULL; /* if not valid return NULL */

	}

	if (o_mode == 'a') { /* if o_mode is additive check for the valid range of the the positive value of inc_factor */

		if ((unsigned char)inc_factor < 0 || (unsigned char)inc_factor > F_INC_FACTOR_MAX) { /* casted to unsigned to get positive value */

			return NULL;

		}

	}
	else if (o_mode == 'm') { /* if o_mode is multiplicative check for the valid range of the the value of inc_factor */

		if (inc_factor < 0 || inc_factor > M_INC_FACTOR_MAX) {

			return NULL;

		}

	}
	else if (o_mode == 'f') { /* if o_mode is fixed ignore any value for inc_factor in the parameter and set inc_factor to 0*/

		inc_factor = 0;

	}
	else {

		return NULL; /* if invalid o_mode return NULL */

	}

	pBuffer = (Buffer*)calloc(1, sizeof(Buffer)); /* reserve space on heap for 1 Buffer*/

	if (pBuffer == NULL) { /* if calloc() failed return NULL */

		return NULL;

	}

	if (init_capacity == 0) { /* if initial capacity is zero set default capacity and inc_factors*/

		init_capacity = DEFAULT_INIT_CAPACITY;

		if (o_mode == 'a' || o_mode == 'm') {

			inc_factor = DEFAULT_INC_FACTOR;

		}

	}
	else {

		if (inc_factor == 0) { /* is inc_factor is 0 and initial capacity is not 0 set mode to fixed */

			o_mode = 'f';

		}

	}

	pBuffer->cb_head = (char*)malloc(init_capacity); /* set memory aside on heap for the capacity of the buffer contents */

	if (pBuffer->cb_head == NULL) { /* if malloc() failed return NULL */

		free(pBuffer); /* free the memery assigned to pBuffer */
		pBuffer = NULL; /* avoid dangling pointer*/
		return NULL;

	}

	if (o_mode == 'f') { /* if o_mode is fixed set elements of buffer */

		pBuffer->mode = FIXED_MODE;
		pBuffer->inc_factor = 0;

	}
	else if (o_mode == 'a') {/* if o_mode is additive set elements of buffer */

		pBuffer->mode = ADDITIVE_MODE;
		pBuffer->inc_factor = inc_factor;

	}
	else { /* if o_mode is multiplicative set elements of buffer */

		pBuffer->mode = MULTIPLICATIVE_MODE;
		pBuffer->inc_factor = inc_factor;

	}

	/* set remaining elemets of Buffer to defaults*/
	pBuffer->capacity = init_capacity;
	pBuffer->flags = DEFAULT_FLAGS;
	pBuffer->addc_offset = 0;
	pBuffer->markc_offset = 0;
	pBuffer->getc_offset = 0;

	return pBuffer; /* return the pointer to the Buffer struct*/

}

/* Fuction Name: b_addc
*  Purpose: Adds character to the buffer
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [October 2nd, 2019]
*  Called Functions: realloc()
*  Parameters: pBD: [Type: Buffer Pointer], [Range: N/A], [Description: Pointer to buffer structure to add]
*			   symbol: [Type: char], [Range: 0 - 255], [Description: Character to be added to the buffer]
*  Return value: [Type: Pointer] a pointer to Buffer with the added character, or NULL if there is a failure
*  Algorithm: Checks for valid parameters, checks for space in the buffer and creates more space if needed.
*/
pBuffer b_addc(pBuffer const pBD, char symbol) {


	short newCapacity; /* variable to store the calculated new capacity if needed */
	short availableSpace; /* variable to store the caculated amount of available space if needed */
	long long testValue; /* variable to store calculations to avoid overflow of the short data */
	short newIncrement; /* variable to store the new increment calculations */
	char* temp; /* variable to store realloc'd space to avoid losing the contents of the buffer */

	if (pBD == NULL) { /* check parameter for validity */

		return NULL;

	}

	pBD->flags &= RESET_R_FLAG; /* reset r_flag to default if the parameter is valid */
	newCapacity = 0; /* initializing newCapacity variable */

	if (pBD->addc_offset == pBD->capacity) { /* if buffer is full try to make it bigger */

		if (pBD->mode == FIXED_MODE) { /* if the mode is fixed return NULL */

			return NULL;

		}
		else if (pBD->mode == ADDITIVE_MODE) { /* if mode is additive newCapacity is current capacity plus the positive value of inc_factor */

			newCapacity = pBD->capacity + (unsigned char)pBD->inc_factor;

			if (newCapacity > 0) { /* is there no overflow and variable > max buffer capacity-1 set capacity to max*/

				if (newCapacity > MAX_BUFFER_CAPACITY - 1) {

					newCapacity = MAX_BUFFER_CAPACITY;

				}

			}
			else { /* if there is overflow return NULL*/

				return NULL;

			}

		}
		else if (pBD->mode == MULTIPLICATIVE_MODE) {

			if (pBD->capacity == MAX_BUFFER_CAPACITY) { /* is capacity is already max return NULL */

				return NULL;

			}

			availableSpace = MAX_BUFFER_CAPACITY - pBD->capacity; /* calculate available space */

			testValue = (long long)availableSpace * pBD->inc_factor / M_DIVISION_FACTOR; /* try calculations in a larger variable type to avoid overflow*/

			if (testValue <= SHRT_MAX) { /* if calculations are within valid short range set to new increment */

				newIncrement = (short)testValue;

				if (newIncrement == 0) { /* if interger divion results in 0 use modulus */

					newIncrement = availableSpace * pBD->inc_factor % M_DIVISION_FACTOR;

				}

				newCapacity = pBD->capacity + newIncrement;

				if (newCapacity < 0 || newCapacity == SHRT_MAX) { /* if overflow set capacity to max */

					newCapacity = MAX_BUFFER_CAPACITY;

				}

			}
			else { /* if no valid mode return NULL */

				return NULL;

			}

		}

		temp = (char*)realloc(pBD->cb_head, newCapacity); /* make memory for a new capacity and save it to temp */

		if (temp == NULL) { /* if realloc() failed return NULL */

			return NULL;

		}

		if (temp != pBD->cb_head) { /* if realloc() changed the location of the memory*/

			pBD->flags |= SET_R_FLAG; /* set r_flag */
			pBD->cb_head = NULL; /* set old pointer to NULL to avoid dangling pointers*/
			pBD->cb_head = temp; /* set buffer to new pointer value*/

		}

		pBD->capacity = newCapacity; /* set capacity to newCapacity*/

	}

	pBD->cb_head[pBD->addc_offset++] = symbol; /* add the character and increment offset */

	return pBD; /* return pointer to the buffer */

}

/* Fuction Name: b_clear
*  Purpose: clears the buffer and starts back at the beginning
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [October 2nd, 2019]
*  Called Functions: N/A
*  Parameters: Buffer*: [Type: Buffer Pointer], [Range: N/A], [Description: Pointer to buffer structure to clear]
*  Return value: [Type: Int] RT_FAIL_1 if there is a failure and 0 is the function succeeded.
*  Algorithm: Checks for valid parameters, sets offsets and flags back to default.
*/
int b_clear(Buffer* const pBD) {

	if (pBD == NULL) { /* if parameter is not valid return RT_FAIL_1 */

		return RT_FAIL_1;

	}

	/* set values back to default */
	pBD->addc_offset = 0;
	pBD->getc_offset = 0;
	pBD->markc_offset = 0;
	pBD->flags = DEFAULT_FLAGS;

	return SUCCESS;

}

/* Fuction Name: b_free
*  Purpose: frees the memory that was allocated in the heap for the buffer
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [October 2nd, 2019]
*  Called Functions: free()
*  Parameters: pBD: [Type: Buffer Pointer], [Range: N/A], [Description: Pointer to buffer structure to free]
*  Return value: [Type: void]
*  Algorithm: Checks for valid parameters, frees the char* and then the Buffer*.
*/
void b_free(Buffer* const pBD) {

	if (pBD != NULL) { /* if parameter is valid */

		free(pBD->cb_head); /* free the cbhead */
		pBD->cb_head = NULL; /* set to NULL */

		free(pBD); /* free the buffer */

	}

}

/* Fuction Name: b_isfull
*  Purpose: checks to see if buffer is full
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [October 2nd, 2019]
*  Called Functions: N/A
*  Parameters: pBD: [Type: Buffer Pointer], [Range: N/A], [Description: Pointer to buffer structure to check]
*  Return value: [Type: Int] RT_FAIL_1 if there is a failure and TRUE/FALSE if the function succeeded.
*  Algorithm: Checks for valid parameters, checks if the buffer is full returns the result.
*/
int b_isfull(Buffer* const pBD) {

	if (pBD == NULL) { /* if parameter is not valid return RT_FAIL_1 */

		return RT_FAIL_1;

	}

	if (pBD->addc_offset == pBD->capacity) { /* if buffer is full return TRUE else return FALSE */

		return TRUE;

	}
	else {

		return FALSE;

	}

}

/* Fuction Name: b_limit
*  Purpose: gets the current limit of the buffer
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [October 2nd, 2019]
*  Called Functions: N/A
*  Parameters: Buffer*: [Type: Buffer Pointer], [Range: N/A], [Description: Pointer to buffer structure to check limit]
*  Return value: [Type: Short] RT_FAIL_1 if there is a failure and the value of the limit if the function succeeded.
*  Algorithm:
*/
short b_limit(Buffer* const pBD) {

	if (pBD == NULL) { /* if parameter is not valid return RT_FAIL_1 */

		return RT_FAIL_1;

	}

	return pBD->addc_offset; /* return addc_offset */

}

/* Fuction Name: b_capacity
*  Purpose: gets the current capacity of the buffer
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [October 2nd, 2019]
*  Called Functions: N/A
*  Parameters: pBD: [Type: Buffer Pointer], [Range: N/A], [Description: Pointer to buffer structure to check capacity]
*  Return value: [Type: Short] RT_FAIL_1 if there is a failure and the value of the capacity if the function succeeded.
*  Algorithm:
*/
short b_capacity(Buffer* const pBD) {

	if (pBD == NULL) { /* if parameter is not valid */

		return RT_FAIL_1;

	}

	return pBD->capacity; /* return capacity*/

}

/* Fuction Name: b_mark
*  Purpose: sets the mark of the buffer
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [October 2nd, 2019]
*  Called Functions: N/A
*  Parameters: pBD: [Type: Buffer Pointer], [Range: N/A], [Description: Pointer to buffer structure to set mark]
			   mark: [Type: short], [Range: 0 - 32766],  [Description: mark to be set]
*  Return value: [Type: Short] RT_FAIL_1 if there is a failure and the value of the mark if the function succeeded.
*  Algorithm:
*/
short b_mark(pBuffer const pBD, short mark) {

	if (pBD == NULL) { /* if parameter is invalid return RT_FAIL_1 */

		return RT_FAIL_1;

	}

	if (mark < 0 || mark > pBD->addc_offset) { /* if parameter is not within valid range return RT_FAIL_1 */

		return RT_FAIL_1;

	}

	pBD->markc_offset = mark; /* set mark */

	return pBD->markc_offset; /* return new mark */

}

/* Fuction Name: b_mode
*  Purpose: gets the mode of the buffer
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [October 2nd, 2019]
*  Called Functions: N/A
*  Parameters: pBD: [Type: Buffer Pointer], [Range: N/A], [Description: Pointer to buffer structure to get mode]
*  Return value: [Type: Short] RT_FAIL_1 if there is a failure and the value of the mode if the function succeeded.
*  Algorithm:
*/
int b_mode(Buffer* const pBD) {

	if (pBD == NULL) { /* if parameter is invalid return NULL */

		return RT_FAIL_1;

	}

	return pBD->mode; /* return the mode */

}

/* Fuction Name: b_incfactor
*  Purpose: gets the postive value of the increment factor of the buffer
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [October 2nd, 2019]
*  Called Functions: N/A
*  Parameters: pBD: [Type: Buffer Pointer], [Range: N/A], [Description: Pointer to buffer structure to get mode]
*  Return value: [Type: size_t] SIZE_T_INC_FACTOR_FAIL if there is a failure and the value of the increment
*								factor if the function succeeded.
*  Algorithm:
*/
size_t b_incfactor(Buffer* const pBD) {

	if (pBD == NULL) { /* if parameter is invalid return fail value */

		return SIZE_T_INC_FACTOR_FAIL;

	}

	return (unsigned char)pBD->inc_factor; /* return positive value of inc_factor */

}

/* Fuction Name: b_load
*  Purpose: loads a file into the buffer
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [October 2nd, 2019]
*  Called Functions: fgetc(), feof(), b_addc(), ungetc()
*  Parameters:	fi: [Type: FILE Pointer], [Range: N/A], [Description: Pointer to FILE struct]
*				pBD: [Type: Buffer Pointer], [Range: N/A], [Description: Pointer to buffer structure to load]
*  Return value: [Type: Int] RT_FAIL_1 if there is a failure, LOAD_FAIL if the entire file cannot be loaded
							 into the buffer, or the number a characters loaded into the buffer.
*  Algorithm: in a loop load the file into the buffer char by char, check for EOF in each loop
*/
int b_load(FILE* const fi, Buffer* const pBD) {

	char temp; /* temp variable as char passes from file to buffer */
	int i; /* counter */

	if (pBD == NULL) { /* if parameter is invalid return fail value */

		return RT_FAIL_1;

	}

	if (fi == NULL) { /* if parameter is invalid return fail value */

		return RT_FAIL_1;

	}

	i = 0; /* initialize counter to 0*/

	do { /* loop until EOF */

		temp = (char)fgetc(fi); /* get char from file */

		if (feof(fi)) { /* check for EOF, if so break */

			break;

		}

		if (b_addc(pBD, temp) == NULL) { /* if add fails */

			ungetc(temp, fi); /* return character to file and return fail value */
			return LOAD_FAIL;

		}

		++i; /* increment counter */

	} while (TRUE);

	return i; /* return count */

}

/* Fuction Name: b_isempty
*  Purpose: checks if the buffer is empty
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [October 2nd, 2019]
*  Called Functions: N/A
*  Parameters: pBD: [Type: Buffer Pointer], [Range: N/A], [Description: Pointer to buffer structure to get check]
*  Return value: [Type: Int] RT_FAIL_1 if there is a failure or TRUE/FALSE is the function succeeded.
*  Algorithm:
*/
int b_isempty(Buffer* const pBD) {

	if (pBD == NULL) { /* if parameter is invalid return RT_FAIL_1 */

		return RT_FAIL_1;

	}

	if (pBD->addc_offset == 0) { /* if empty return TRUE else return FALSE */

		return TRUE;

	}
	else {

		return FALSE;

	}

}

/* Fuction Name: b_getc
*  Purpose: gets the next character from the buffer
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [October 2nd, 2019]
*  Called Functions: N/A
*  Parameters: pBD: [Type: Buffer Pointer], [Range: N/A], [Description: Pointer to buffer structure to get character]
*  Return value: [Type: char] RT_FAIL_2 if there is a failure, EOB if there is no more characters
*							  or the character that was successfully retrieved.
*  Algorithm: checks parameters, checks for EOB, and gets the next character and increments getc offset
*/
char b_getc(Buffer* const pBD) {

	if (pBD == NULL) { /* if parameter is invalid return fail value */

		return RT_FAIL_2;

	}

	if (pBD->getc_offset == pBD->addc_offset) { /* if at EOB set flag and return fail value */

		pBD->flags |= SET_EOB;
		return EOB;

	}
	else { /* else reset flag */

		pBD->flags &= RESET_EOB;

	}

	return pBD->cb_head[pBD->getc_offset++]; /* return and increment */

}

/* Fuction Name: b_eob
*  Purpose: checks to see if the eob flag is set
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [October 2nd, 2019]
*  Called Functions: N/A
*  Parameters: pBD: [Type: Buffer Pointer], [Range: N/A], [Description: Pointer to buffer structure to check flag]
*  Return value: [Type: int] RT_FAIL_1 if there is a failure and the value of the flag if the function succeeded.
*  Algorithm:
*/
int b_eob(Buffer* const pBD) {

	if (pBD == NULL) { /* if paramter is invalid return fail value */

		return RT_FAIL_1;

	}

	return pBD->flags & CHECK_EOB; /* return value of EOB flag */

}

/* Fuction Name: b_print
*  Purpose: prints the current contents of the buffer
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [October 2nd, 2019]
*  Called Functions: b_getc(), b_eob(), printf()
*  Parameters: pBD: [Type: Buffer Pointer], [Range: N/A], [Description: Pointer to buffer structure to print]
*			   nl: [Type: char], [Range: 0 - 255], [Description: if value is not 0 print \n character after printing the buffer]
*  Return value: [Type: int] RT_FAIL_1 if there is a failure and the number of characters printed if function is successful.
*  Algorithm: check for valid paramters and in a loop print the contents of the buffer one by one and in each loop check for EOB.
*/
int b_print(Buffer* const pBD, char nl) {

	char temp; /* temp variable while passing character from buffer to print */
	int i; /* counter */

	if (pBD == NULL) { /* if invalid parameter return fail value */

		return RT_FAIL_1;

	}

	i = 0; /* initialize counter to 0 */

	do { /* loop til EOB */

		temp = b_getc(pBD); /* get character */

		if (b_eob(pBD) != B_EOF_CHECK) { /* check for EOB  if so break */

			break;

		}

		printf("%c", temp); /* print character */

		++i; /* increment count */

	} while (TRUE);

	if (nl != FALSE) { /* if nl is not false print \n */

		printf("\n");

	}

	return i; /* return count */

}

/* Fuction Name: b_compact
*  Purpose: compacts the buffer to the eact size needed to hold its contents plus one and adds the character specified.
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [October 2nd, 2019]
*  Called Functions: realloc()
*  Parameters: pBD: [Type: Buffer Pointer], [Range: N/A], [Description: Pointer to buffer structure to compact]
*			   symbol: [Type: char], [Range: 0 - 255], [Description: character to be added after compact]
*  Return value: [Type: Pointer] NULL if there is a failure and pointer to the compacted buffer is function is successful.
*  Algorithm: check for valid paramters and resize the memory, add symbol to the end of the buffer and return pointer to resized buffer
*/
Buffer* b_compact(Buffer* const pBD, char symbol) {

	short newCapacity; /* variable to hold new capacity calculation */
	char* temp; /* test realloc variable */

	if (pBD == NULL) { /* if parameter is invalid return NULL */

		return NULL;

	}

	if (pBD->addc_offset == SHRT_MAX) { /* if max return NULL */

		return NULL;

	}

	newCapacity = pBD->addc_offset + 1; /* calculate new capacity */

	temp = (char*)realloc(pBD->cb_head, newCapacity); /* try and realloc new capacity */

	if (temp == NULL) { /* if realloc failed return NULL */

		return NULL;

	}

	if (temp != pBD->cb_head) { /* if pointer to buffer changed */

		pBD->flags |= SET_R_FLAG; /* set r_flag */
		pBD->cb_head = NULL; /* set old value to NULL */
		pBD->cb_head = temp; /* set cbhead to new buffer */

	}

	pBD->capacity = newCapacity; /* set new capacity */
	pBD->cb_head[pBD->addc_offset++] = symbol; /* add new character and increment offset */

	return pBD; /* return buffer pointer */

}

/* Fuction Name: b_rflag
*  Purpose: gets the value of rflag
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [October 2nd, 2019]
*  Called Functions: N/A
*  Parameters: pBD: [Type: Buffer Pointer], [Range: N/A], [Description: Pointer to buffer structure to get flag]
*  Return value: [Type: char] RT_FAIL_1 if there is a failure and the value of the rflag if the function is successful.
*  Algorithm:
*/
char b_rflag(Buffer* const pBD) {

	if (pBD == NULL) { /* if parameter is invalid return fail value */

		return RT_FAIL_1;

	}

	return pBD->flags & CHECK_R_FLAG; /* return value of r_flag */

}

/* Fuction Name: b_retract
*  Purpose: decrements the getc offset
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [October 2nd, 2019]
*  Called Functions: N/A
*  Parameters: pBD: [Type: Buffer Pointer], [Range: N/A], [Description: Pointer to buffer structure to retract]
*  Return value: [Type: short] RT_FAIL_1 if there is a failure and the value of getc_offset if the function is successful.
*  Algorithm:
*/
short b_retract(Buffer* const pBD) {

	if (pBD == NULL) { /* if invalid parameter return fail value */

		return RT_FAIL_1;

	}

	if (pBD->getc_offset <= 0) { /* if getc_offset cant go any lower return fail value */

		return RT_FAIL_1;

	}


	return --pBD->getc_offset; /*return value of decremented getc_offset*/

}

/* Fuction Name: b_reset
*  Purpose: reset getc_offset back to the value of the last mark
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [October 2nd, 2019]
*  Called Functions: N/A
*  Parameters: pBD: [Type: Buffer Pointer], [Range: N/A], [Description: Pointer to buffer structure to reset]
*  Return value: [Type: short] RT_FAIL_1 if there is a failure and the value of getc_offset if the function is successful.
*  Algorithm:
*/
short b_reset(Buffer* const pBD) {

	if (pBD == NULL) { /* if parameter is invalid return fail value */

		return RT_FAIL_1;

	}

	pBD->getc_offset = pBD->markc_offset; /* set getc_offset to markc_offset */

	return pBD->getc_offset; /* return getc_offset */

}

/* Fuction Name: b_getcoffset
*  Purpose: gets the value of getc_offset
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [October 2nd, 2019]
*  Called Functions: N/A
*  Parameters: pBD: [Type: Buffer Pointer], [Range: N/A], [Description: Pointer to buffer structure to get value]
*  Return value: [Type: short] RT_FAIL_1 if there is a failure and the value of getc_offset if the function is successful.
*  Algorithm:
*/

short b_getcoffset(Buffer* const pBD) {

	if (pBD == NULL) { /* if parameter is invalid return fail value */

		return RT_FAIL_1;

	}

	return pBD->getc_offset; /* return getc_offset */

}

/* Fuction Name: b_rewind
*  Purpose: sets getc_offset and markc_offset back to zero
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [October 2nd, 2019]
*  Called Functions: N/A
*  Parameters: pBD: [Type: Buffer Pointer], [Range: N/A], [Description: Pointer to buffer structure to rewind]
*  Return value: [Type: int] RT_FAIL_1 if there is a failure and REWIND_SUCCESS if the function is successful.
*  Algorithm:
*/
int b_rewind(Buffer* const pBD) {

	if (pBD == NULL) { /* if parameter is invalid return fail value */

		return RT_FAIL_1;

	}

	/* set get and mark back to 0 */
	pBD->getc_offset = 0;
	pBD->markc_offset = 0;

	return SUCCESS; /* return success value */

}

/* Fuction Name: b_location
*  Purpose: gets the pointer on the char that markc_offset currently points too
*  Author: Jason MacFarlane
*  History/Versions: 1.0 [October 2nd, 2019]
*  Called Functions: N/A
*  Parameters: pBD: [Type: Buffer Pointer], [Range: N/A], [Description: Pointer to buffer structure to retract]
*  Return value: [Type: Char Pointer] NULL if there is a failure and the value  if the function is successful.
*  Algorithm:
*/
char* b_location(Buffer* const pBD) {

	if (pBD == NULL) { /* if parameter is invalid return NULL */

		return NULL;

	}

	return &(pBD->cb_head[pBD->markc_offset]); /* return pointer to the character at the mark*/

}