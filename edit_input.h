#ifndef EDIT_INPUT_H
#define EDIT_INPUT_H

#include "edit_operator.h"

#define MAX_EDITS 100

typedef EditOp * edits_array[MAX_EDITS];

char * my_gets(long_string current_line, FILE * file);
/* used in place of the "dangerous: function gets, takes a line of input from file
 * and puts in in current_line, ommitting the new_line character. Only takes LONG_STRING_LENGTH
 * characters. If EOF is reached and no characters are read returns NULL, else returns a pointer to current_line
 */


void * Malloc(size_t size);
/*a wrapper function for malloc, checks to ensure that memory is allocated
 * if not, prints an error message to stderr and aborts
 */

EditOp * initialize_empty_EditOp();
/*initializes an empty edit operation in the heap
 */

EditOp * initialize_EditOp(long_string current_line);
/*uses a line from the edit operation file to initialize
 * an EditOp struct
 */

void set_range(EditOp* MyOp, char * current_line);
/* set the range_type and range of MyOp using the string current_line as edit command line
 * returns a pointer to the position in current_line where the edit operation specifier is
 */

void set_operator_and_string(EditOp * MyOp, char * edit_op_string);
/* sets the operator specifier, then sets the  StringType union MyOp->the_strings
 * to the proper string(s)
 */



void fill_edits_array(edits_array my_edits, FILE * edit_file);
/* takes a FILE *, from which to read edit commands 
 * uses this file to initialize EditOp' s, and places pointers to them 
 * in my_edits. Sets all unused pointers in edits_array to NULL
 */

void print_EditOp(EditOp * MyOp);
/* prints an edit op using the format:
 * Operation Specifier : [operation_specifier]
 * Range: [edit_range]
 * String(s): [edit_strings]
 */

void Free(void ** ptr);
/* frees ptr and sets it to a null pointer
 */


void free_edits_array(edits_array my_edits);
/* frees the memory allocated when the edits array is created
 */

#endif
