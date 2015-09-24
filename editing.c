#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "editing.h"
#include "edit_functions.h"

int to_edit(EditOp * MyOp, long_string current_line, int line_number)
{  switch(MyOp->edit_range_type)
   {  case line_type :
        if( MyOp->edit_range.endpoints[0] <= line_number &&
            MyOp->edit_range.endpoints[1] >= line_number )
          return 1;
        else 
          return 0;
      case text_type : 
        if ( strstr(current_line,MyOp->edit_range.search_text) != NULL )
          return 1;
        else
          return 0;
      case no_type : 
        return 1;
      default :
        return 0;
   }
}


char make_edit(EditOp * MyOp, long_string current_line, int line_number)
{  if ( to_edit(MyOp,current_line,line_number) )
   {  switch (MyOp->operator_specifier)
      { case 'A' :
          append(current_line, MyOp->the_strings.add_text);
          return 'A';
        case 'I' :
          insert(current_line, MyOp->the_strings.add_text);
          return 'I';
        case 'O' :
          new_line_insert(current_line, MyOp->the_strings.add_text);
          return 'O';
        case 'd' :
          delete_line(current_line);
          return 'd';
        case 's' :
          replace(current_line, MyOp->the_strings.old_and_new[0], MyOp->the_strings.old_and_new[1]);
          return 's';
        default :
          return '\0';
      }
    }
    else
      return '\0';
}


void edit_line(edits_array my_edits, long_string current_line, int line_number)
{  int i;
   char  c;
   for(i = 0; i < MAX_EDITS; i+=1)
   {  if(my_edits[i] != NULL)
      { c = make_edit(my_edits[i],current_line,line_number);
        if ( c == 'd' )
          break;
      }  
   }
   if ( strcmp(current_line,"") != 0 )
     printf("%s\n",current_line);
}


