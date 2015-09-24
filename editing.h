#ifndef EDITING_H
#define EDITING_H

#include "edit_input.h"


int to_edit(EditOp * MyOp, long_string current_line, int line_number);
/*determines whether or not current line is to be edited by MyOp
 * returns 0 if no edit is required and 1 if it is
 */




char make_edit(EditOp * MyOp, long_string current_line, int line_number);
/*using MyOp it takes the long_string current_line and conducts the appropriate
 * edit on it. Returns the identifier if an operation is made or the null char it no
 * operation is made
 */

void edit_line(edits_array my_edits, long_string current_line, int line_number);
/*conducts all the edit operations in my_edits on the long_string current_line
 */


#endif
