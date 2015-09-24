#ifndef EDIT_FUNCTIONS_H
#define EDIT_FUNCTIONS_H



#include "string_def.h"


char * find_current_line(long_string current_line);
/* A helper function that is used to ensure that the lines added by
 * new_line_insert are not edited. Returns a pointer to the line that
 * must be edited.
 */


void append(long_string current_line, long_string append_text);
/* appends the append_text string to current line and prints the 
 * result to stdout
*/

void insert(long_string current_line, long_string insert_text);
/* inserts the insert_text ath the beginning of current_line
 * and outputs the result to stdout
 */

void new_line_insert(long_string current_line, long_string insert_text);
/* outputs insert_text to stdout, then the current_line on a new line
 * the insert text is simply printed and not put through editing
 */

void delete_line(long_string current_line);
/* placeholder function, takes the current line and does nothing
 * allows for syntactical symmetry
 */

void replace(long_string current_line, short_string old_text, short_string new_text);
/*searches for old_text in current_line, if found replaces with new_text
 * outputs new string to stdout
 */

#endif
