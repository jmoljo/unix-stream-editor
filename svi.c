#include <stdlib.h>
#include <stdio.h>

#include "editing.h"




int main(int argc, char * argv[])
{  if(argc != 2)
   {  fprintf(stderr,"Error : Please enter the name of the edit file\n");
      return EXIT_FAILURE;
   }
   else 
   { FILE* edit_file = fopen(argv[1],"r");
     edits_array my_edits;
     int i = 1;
     long_string current_line;
     char * c = current_line;
     fill_edits_array(my_edits,edit_file);
     while ( 1 )
     {  c = my_gets(current_line, stdin);
        if ( c == NULL )
          break;
        edit_line(my_edits,current_line,i);
        i+=1;
     }
     free_edits_array(my_edits);
     free(edit_file);
     return EXIT_SUCCESS;
   }
}
