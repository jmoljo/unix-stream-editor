#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "edit_input.h"

char * my_gets(long_string my_text,FILE * file)
{  long_string temp;
   char * foo;
   char * c = fgets(temp, LONG_STRING_LENGTH, file);
   if (c!= NULL)
     strcpy(my_text,strtok_r(temp,"\n",&foo));
   return c;
}


void * Malloc(size_t size)
{  void * c = malloc(size);
   if ( c == NULL )
   { fprintf(stderr, "Error : memory allocation failed at Malloc(%d): %d", (int)size, __LINE__);
     abort();
   }
   return c;
}


EditOp * initialize_empty_EditOp()
{  EditOp * MyOp = (EditOp*)Malloc(sizeof(EditOp)); 
   return MyOp;
}

void set_range(EditOp * MyOp, char * current_line)
{  long_string temp;
   char * save;
   char * range_identifier;
   strcpy(temp,current_line);
   range_identifier = strtok_r(temp,"/",&save); 
   if(  strcmp(range_identifier,current_line) == 0 )
   {  MyOp->edit_range_type = no_type;
      MyOp->edit_range.isitall = 1;
   }
   else if (current_line[0] == 's') 
   {  MyOp->edit_range_type = no_type;
      MyOp->edit_range.isitall = 1;
   }
   else if ( isdigit(current_line[0]) != 0 )
   {  int first_line, last_line;
      char comma;
      MyOp->edit_range_type = line_type;
      sscanf(range_identifier,"%d %c %d",&first_line,&comma,&last_line);
      MyOp->edit_range.endpoints[0] = first_line;
      MyOp->edit_range.endpoints[1] = last_line;
      strcpy(current_line,save);
   }
   else if (current_line[0] == '/' )
   {  int i;
      MyOp->edit_range_type = text_type;
      for(i = 0; i < SHORT_STRING_LENGTH; i+=1)
      {  MyOp->edit_range.search_text[i] = range_identifier[i];
      } 
      strcpy(current_line,save);
   }
   else   
   {  printf("There has been an invalid edit command\n");
      abort();
   }
}   
  
   
void set_operator_and_string(EditOp * MyOp, char * edit_string)
{ MyOp->operator_specifier = edit_string[0];
  if ( MyOp->operator_specifier != 's' )
  { strcpy(MyOp->the_strings.add_text, edit_string + 1);
  }
  else
  { char * old_text;
    char * new_text;
    char * save;
    old_text = strtok_r(edit_string + 1,"/",&save); 
    new_text = strtok_r(save,"/",&save); 
    strcpy(MyOp->the_strings.old_and_new[0],old_text);
    strcpy(MyOp->the_strings.old_and_new[1],new_text);
  }
}

EditOp * initialize_EditOp(long_string current_line)
{  EditOp * MyOp = initialize_empty_EditOp();
   set_range(MyOp,current_line);
   set_operator_and_string(MyOp,current_line);
   return MyOp;
}



void fill_edits_array(edits_array my_edits, FILE * edits_file)
{ long_string temp;
  char * c;
  int i = 0;
  while( 1 )
  {  c = my_gets(temp,edits_file);
     if (c == NULL)
       break;
     my_edits[i] = initialize_EditOp(temp);
     i++;
  }
  for( ; i < MAX_EDITS; i +=1)
  {  my_edits[i] = NULL;
  }
}


void print_EditOp(EditOp * MyOp)
{ if (MyOp == NULL)
    printf("This is not an edit op");
  else
  { printf("Operation Specifier: %c\n",MyOp->operator_specifier);
    printf("Range : ");
    switch( MyOp->edit_range_type )
    {  case no_type:  
         printf("all\n");
         break;
       case line_type:
         printf("%d , %d\n",MyOp->edit_range.endpoints[0],MyOp->edit_range.endpoints[1]);
         break;
       case text_type:
         printf("%s\n",MyOp->edit_range.search_text);
         break;
    }
    if (MyOp->operator_specifier != 's')
    {  printf("String : %s\n", MyOp->the_strings.add_text);
    } 
    else
    {  printf("Old Text : %s\n New Text : %s\n",MyOp->the_strings.old_and_new[0],MyOp->the_strings.old_and_new[1]);
    }
  }
}


void Free(void ** ptr)
{  free(*ptr);
   *ptr = NULL;
}



void free_edits_array(edits_array my_edits)
{  int i;
   for(i = 0; i < MAX_EDITS; i++)
   {  if(my_edits[i] != NULL)
       Free((void**)&my_edits[i]);
   }
}











