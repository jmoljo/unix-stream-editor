
#ifndef EDIT_OPERATOR_H
#define EDIT_OPERATOR_H


#include "string_def.h"

typedef enum { no_type , text_type, line_type } RangeType;
/* used to specify what type of range specification is being used
 * can be either no specification, in which case all lines are edited,
 * text specification, where lines with the given text are edited, or ranged specification
 * where lines in the range are n,m inclusive are edited
 */

typedef union 
{  short_string search_text; 
   int endpoints[2];
   char isitall;
} Range;
/* search_text stores the text in the specifier /<text>/
 * endpoints[0] is the first line in the range endpoints[1] is the last
 * isitall is either 0 or nonzero, if it is nonzero it indicates that all lines are specified
 */

typedef union
{  short_string old_and_new[2];
   long_string add_text;
} StringType;


typedef struct
{  Range edit_range;
   RangeType edit_range_type;
   StringType the_strings;
   char operator_specifier;
} EditOp;
/* operator specifier gives the letter for the operation. So A for append, I for insert, etc.
 * edit_strings is a pointer to strings. It either quites one or two strings depending on the edit
 * operation.(Two for search and replace, one for all others)
 */

 
#endif
  
