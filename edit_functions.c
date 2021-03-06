edit_functions.h                                                                                    0000644 0043077 0000017 00000002233 12520073623 012066  0                                                                                                    ustar                                   user                                                                                                                                                                                                                   #ifndef EDIT_FUNCTIONS_H
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
                                                                                                                                                                                                                                                                                                                                                                     editing.c                                                                                           0000644 0043077 0000017 00000003367 12520220701 010467  0                                                                                                    ustar                                   user                                                                                                                                                                                                                   #include <stdio.h>
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


                                                                                                                                                                                                                                                                         editing.h                                                                                           0000644 0043077 0000017 00000001270 12520176013 010471  0                                                                                                    ustar                                   user                                                                                                                                                                                                                   #ifndef EDITING_H
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
                                                                                                                                                                                                                                                                                                                                        edit_input.c                                                                                        0000644 0043077 0000017 00000007272 12520233067 011220  0                                                                                                    ustar                                   user                                                                                                                                                                                                                   #include<stdio.h>
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











                                                                                                                                                                                                                                                                                                                                      edit_input.h                                                                                        0000644 0043077 0000017 00000003610 12520232675 011221  0                                                                                                    ustar                                   user                                                                                                                                                                                                                   #ifndef EDIT_INPUT_H
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
                                                                                                                        edit_operator.h                                                                                     0000644 0043077 0000017 00000002323 12520014323 011701  0                                                                                                    ustar                                   user                                                                                                                                                                                                                   
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
  
                                                                                                                                                                                                                                                                                                             Makefile                                                                                            0000644 0043077 0000017 00000001012 12520233300 010320  0                                                                                                    ustar                                   user                                                                                                                                                                                                                   
CC = gcc
LD = $(CC)
DEPEND = mkdep

CFLAGS = -Wall -pedantic`


CFILES =\
       edit_input.c\
       edit_functions.c\
       editing.c\
       svi.c

HFILES =\
       edit_input.h\
       edit_functions.h\
       edit_operator.h\
       string_def.h\
       editing.h
   



svi : edit_input.o edit_functions.o editing.o svi.c 

editing.o : $(HFILES) editing.c 

edit_input.o : edit_functions.h string_def.h edit_operator.h edit_input.h edit_input.c

edit_functions.o : edit_functions.h string_def.h edit_functions.c


                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      mysession.vim                                                                                       0000644 0043077 0000017 00000025056 12520220677 011461  0                                                                                                    ustar                                   user                                                                                                                                                                                                                   let SessionLoad = 1
if &cp | set nocp | endif
let s:cpo_save=&cpo
set cpo&vim
nmap gx <Plug>NetrwBrowseX
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#NetrwBrowseX(expand("<cWORD>"),0)
let &cpo=s:cpo_save
unlet s:cpo_save
set backspace=indent,eol,start
set cscopeprg=/usr/bin/cscope
set cscopetag
set cscopeverbose
set fileencodings=ucs-bom,utf-8,latin1
set guicursor=n-v-c:block,o:hor50,i-ci:hor15,r-cr:hor30,sm:block,a:blinkon0
set helplang=en
set history=50
set hlsearch
set ruler
set viminfo='20,\"50
let s:so_save = &so | let s:siso_save = &siso | set so=0 siso=0
let v:this_session=expand("<sfile>:p")
silent only
cd ~/Stream\ Editor
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
set shortmess=aoO
badd +1 editing.c
badd +0 edit_functions.c
badd +0 edit_input.c
badd +0 svi.c
args editing.c edit_functions.c edit_input.c svi.c
edit editing.c
set splitbelow splitright
set nosplitbelow
set nosplitright
wincmd t
set winheight=1 winwidth=1
argglobal
setlocal keymap=
setlocal noarabic
setlocal noautoindent
setlocal nobinary
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal nocindent
setlocal cinkeys=0{,0},0),:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
setlocal comments=sO:*\ -,mO:*\ \ ,exO:*/,s1:/*,mb:*,ex:*/,://
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal completefunc=
setlocal nocopyindent
setlocal nocursorcolumn
setlocal nocursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal noexpandtab
if &filetype != 'c'
setlocal filetype=c
endif
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
setlocal foldmethod=manual
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=croql
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=0
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal nolist
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal modeline
setlocal modifiable
setlocal nrformats=octal,hex
setlocal nonumber
setlocal numberwidth=4
setlocal omnifunc=ccomplete#Complete
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal shiftwidth=8
setlocal noshortname
setlocal nosmartindent
setlocal softtabstop=0
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=3000
if &syntax != 'c'
setlocal syntax=c
endif
setlocal tabstop=8
setlocal tags=
setlocal textwidth=0
setlocal thesaurus=
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
silent! normal! zE
let s:l = 69 - ((21 * winheight(0) + 11) / 22)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
69
normal! 0
tabedit edit_functions.c
set splitbelow splitright
set nosplitbelow
set nosplitright
wincmd t
set winheight=1 winwidth=1
argglobal
2argu
setlocal keymap=
setlocal noarabic
setlocal noautoindent
setlocal nobinary
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal nocindent
setlocal cinkeys=0{,0},0),:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
setlocal comments=sO:*\ -,mO:*\ \ ,exO:*/,s1:/*,mb:*,ex:*/,://
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal completefunc=
setlocal nocopyindent
setlocal nocursorcolumn
setlocal nocursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal noexpandtab
if &filetype != 'c'
setlocal filetype=c
endif
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
setlocal foldmethod=manual
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=croql
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=0
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal nolist
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal modeline
setlocal modifiable
setlocal nrformats=octal,hex
setlocal nonumber
setlocal numberwidth=4
setlocal omnifunc=ccomplete#Complete
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal shiftwidth=8
setlocal noshortname
setlocal nosmartindent
setlocal softtabstop=0
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=3000
if &syntax != 'c'
setlocal syntax=c
endif
setlocal tabstop=8
setlocal tags=
setlocal textwidth=0
setlocal thesaurus=
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
silent! normal! zE
let s:l = 48 - ((10 * winheight(0) + 11) / 22)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
48
normal! 0
tabedit edit_input.c
set splitbelow splitright
set nosplitbelow
set nosplitright
wincmd t
set winheight=1 winwidth=1
argglobal
3argu
setlocal keymap=
setlocal noarabic
setlocal noautoindent
setlocal nobinary
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal nocindent
setlocal cinkeys=0{,0},0),:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
setlocal comments=sO:*\ -,mO:*\ \ ,exO:*/,s1:/*,mb:*,ex:*/,://
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal completefunc=
setlocal nocopyindent
setlocal nocursorcolumn
setlocal nocursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal noexpandtab
if &filetype != 'c'
setlocal filetype=c
endif
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
setlocal foldmethod=manual
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=croql
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=0
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal nolist
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal modeline
setlocal modifiable
setlocal nrformats=octal,hex
setlocal nonumber
setlocal numberwidth=4
setlocal omnifunc=ccomplete#Complete
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal shiftwidth=8
setlocal noshortname
setlocal nosmartindent
setlocal softtabstop=0
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=3000
if &syntax != 'c'
setlocal syntax=c
endif
setlocal tabstop=8
setlocal tags=
setlocal textwidth=0
setlocal thesaurus=
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
silent! normal! zE
let s:l = 39 - ((21 * winheight(0) + 11) / 22)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
39
normal! 023l
tabedit svi.c
set splitbelow splitright
set nosplitbelow
set nosplitright
wincmd t
set winheight=1 winwidth=1
argglobal
4argu
setlocal keymap=
setlocal noarabic
setlocal noautoindent
setlocal nobinary
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal nocindent
setlocal cinkeys=0{,0},0),:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
setlocal comments=sO:*\ -,mO:*\ \ ,exO:*/,s1:/*,mb:*,ex:*/,://
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal completefunc=
setlocal nocopyindent
setlocal nocursorcolumn
setlocal nocursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal noexpandtab
if &filetype != 'c'
setlocal filetype=c
endif
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
setlocal foldmethod=manual
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=croql
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=0
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal nolist
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal modeline
setlocal modifiable
setlocal nrformats=octal,hex
setlocal nonumber
setlocal numberwidth=4
setlocal omnifunc=ccomplete#Complete
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal shiftwidth=8
setlocal noshortname
setlocal nosmartindent
setlocal softtabstop=0
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=3000
if &syntax != 'c'
setlocal syntax=c
endif
setlocal tabstop=8
setlocal tags=
setlocal textwidth=0
setlocal thesaurus=
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
silent! normal! zE
let s:l = 1 - ((0 * winheight(0) + 11) / 22)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
1
normal! 04l
tabnext 1
if exists('s:wipebuf')
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20 shortmess=filnxtToO
let s:sx = expand("<sfile>:p:r")."x.vim"
if file_readable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &so = s:so_save | let &siso = s:siso_save
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  README                                                                                              0000644 0043077 0000017 00000000162 12520233446 007560  0                                                                                                    ustar                                   user                                                                                                                                                                                                                   Works perfectly on both test files from websites, however valgrind gives errors when freeing the array of EditOps
                                                                                                                                                                                                                                                                                                                                                                                                              string_def.h                                                                                        0000644 0043077 0000017 00000000311 12517500471 011172  0                                                                                                    ustar                                   user                                                                                                                                                                                                                   #ifndef STRING_H
#define STRING_H

#define LONG_STRING_LENGTH 256
#define SHORT_STRING_LENGTH 80

typedef char long_string[LONG_STRING_LENGTH];

typedef char short_string[SHORT_STRING_LENGTH];

#endif
                                                                                                                                                                                                                                                                                                                       svi                                                                                                 0000755 0043077 0000017 00000060211 12520233073 007424  0                                                                                                    ustar                                   user                                                                                                                                                                                                                   ELF          >    @
@     @       �G          @ 8  @ % "       @       @ @     @ @     �      �                           @      @                                          @       @     \      \                              `       `     �      �                    (       ( `     ( `     �      �                         @     @     D       D              P�td   �      �@     �@     �       �              Q�td                                                  /lib64/ld-linux-x86-64.so.2          GNU                       GNU �~q���}�'�|7,���               �         fUa9�                        �                      e                      L                                             G                      /                      �                      !                      �                      X                      �                      ^                      �                      '                      s                      l                      �                      |                      �                                            ?                      �                      �                      R     �"`            �     �"`             __gmon_start__ libc.so.6 strcpy fopen strrchr __isoc99_sscanf strncpy puts abort stdin fgets strlen memset strstr strtok_r memcpy malloc strcat __ctype_b_loc stderr fwrite fprintf strcmp __libc_start_main free GLIBC_2.3 GLIBC_2.7 GLIBC_2.2.5                                          ii   �      ii   �      ui	   �       �!`                   �"`                   �"`                   �!`                   �!`                   �!`                   �!`                   �!`                    "`                   "`                   "`        	           "`        
            "`                   ("`                   0"`                   8"`                   @"`                   H"`                   P"`                   X"`                   `"`                   h"`                   p"`                   x"`                   �"`                   H���  �:  ��  H����5�  �%�  @ �%�  h    ������%�  h   ������%�  h   ������%�  h   �����%�  h   �����%�  h   �����%�  h   �����%�  h   �p����%�  h   �`����%�  h	   �P����%�  h
   �@����%�  h   �0����%�  h   � ����%�  h   �����%�  h   � ����%z  h   ������%r  h   ������%j  h   ������%b  h   ������%Z  h   �����%R  h   �����%J  h   ����1�I��^H��H���PTI���@ H�� @ H��$@ ��������H��H�A  H��t��H��Ð������������UH��SH���=    uK� ` H��  H�� ` H��H��H9�s$fD  H��H��  �� ` H��  H9�r���  H��[��fff.�     H�=   UH��t�    H��t�  ` ����Ð�UH��H��P  ������H������������t.H�L  H�¸�@ H�Ѻ/   �   H�������   �   �@ H������H��H� H��H������H�E��E�   H������H�E�H�U�H������H��H���>  H��  H������H��H���R   H�E�H�}� t"�U�H������H������H��H���c
  �E�뺐H������H���  H�E�H���Q����    �Ð�UH��H��0  H������H������H������H�������   H������H�E�H�}� t3� @ H������H������H��H���+���H��H������H��H���f���H�E���UH��SH��(H�}�H�E�H���w���H�E�H�}� u,H�E؉»(@ H��  �   H��H�Ǹ    �E�������H�E�H��([��UH��H���X  ����H�E�H�E���UH��SH��H  H������H������H������H������H��H�������[@ H������H������H��H���H���H�E�H������H�E�H��H���n�����uH�������@P    H������� �K  H������� <suH�������@P    H������� �   ����H�H������� H��H�H�� ��%   ��t|H�������@P   �]@ H�E�H������H������H������I��H��H�Ǹ    �����������H�������������H�������PH������H��H������H��H�������xH������� </u[H�������@P   �E�    � �]�E�H�HE��H������HcÈ�E��}�O~�H������H��H������H��H���#�����h@ �'�������H��H  [��UH��H��0H�}�H�u�H�E��H�E؈�T  H�E���T  <stH�E�H�PH�E�H��TH��H�������n�[@ H�U�H�JH�U�H��H���O���H�E�[@ H�E�H�U�H��H���3���H�E�H�U�H�E�H��TH��H���h���H�E�H�U�H��TH��PH��H���M�����UH��H�� H�}�    �0���H�E�H�U�H�E�H��H���5���H�U�H�E�H��H������H�E���UH��SH��(  H������H�������E�    H������H������H��H�������H�E�H�}� u��F�E�H�H��H��H�����H������H���P���H��E�뮋E�H�H��H�����H�     �E��}�c~�H��(  [��UH��H��H�}�H�}� u��@ H�Ǹ    �"�����   H�E���T  �и�@ ��H�Ǹ    �������@ H�Ǹ    �����H�E��@P��t;��r��t�;��@ ������/H�E��PH�E����@ ��H�Ǹ    �����H�E�H�������H�E���T  <stH�E�H�PT��@ H��H�Ǹ    �j����,H�E�H��TH�PPH�E�H��TH����@ H��H�Ǹ    �<�����UH��H��H�}�H�E�H� H������H�E�H�     ��UH��H�� H�}��E�    �2�E�H�H��HE�H� H��tH�E�U�Hc�H��H�H�������E��}�c~��Ð��UH��H�� H�}�H�E�
   H���c���H�E�H�}� uH�E��H�E���UH��H�� H�}�H�u�H�E�H������H�E�H�U�H�E�H��H���I�����UH��H��   H������H������H������H���o���H�E�H�E�� <
uH�E�H������H������H��H���"���H�U�H������H��H�������H������H�E�H��H���������UH��H��   H������H������H������H�������H�E�H������H������H��H������H�E�� <
uH�E�H������� @ H�������H������H�E�H��H���������UH��H�� H�}�H�E�H���{���H�E��@ H�E��   H��H��������UH��SH��H  H������H������H������H������H������H��H�������H�E�H������H���o���HE�H�E�H������H�������H�E�H�}� ��   H�������   �    H������H�E�H���$���H��H�E�H������H��H)�H�M�H������H��H������H������H������H��H���0���H�U�H������H��H������H������H�E�H��H���4�����H��H  [�Ð��UH��H�� H�}�H�u��U�H�E��@P��t/��rP��uRH�E�� ;E�H�E��@;E�|�   �9�    �2H�U�H�E�H��H������H��t�   ��    ��   ��    ��UH��H�� H�}�H�u��U�U�H�M�H�E�H��H���U�������   H�E���T  ����A��2��   ��H��@ ��H�E�H�PTH�E�H��H�������A   �   H�E�H�PTH�E�H��H�������I   �jH�E�H�PTH�E�H��H�������O   �LH�E�H���o����d   �9H�E�H��TH�PPH�E�H��TH��H�E�H��H���y����s   ��    ��    ��UH��H��0H�}�H�u��U��E�    �D�E�H�H��HE�H� H��t+�E�H�H��HE�H� �U�H�M�H��H�������E��}�dt�E��}�c~���H�E�� ��tH�E�H���0����Ð���������������fffff.�     H�l$�L�d$�H�-�  L�%�  L�l$�L�t$�L�|$�H�\$�H��8L)�A��I��H��I���s���H��t1�@ L��L��D��A��H��H9�r�H�\$H�l$L�d$L�l$ L�t$(L�|$0H��8Ð������UH��SH��H�`  H���t�  ` D  H����H�H���u�H��[�Ð�H������H���                Error : Please enter the name of the edit file
 r       
       Error : memory allocation failed at Malloc(%d): %d / %d %c %d   There has been an invalid edit command This is not an edit op Operation Specifier: %c
 Range :  all %d , %d
 String : %s
 Old Text : %s
 New Text : %s
 
       �@     R@     R@     R@     R@     R@     R@     R@     �@     R@     R@     R@     R@     R@     �@     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     @     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     %@     ;�      �����   �����    ���  [���@  w���`  X����  ����  [����  �����  ���  .���0  ����P  ����p  �����  l����  �����  $����  <���  ����8  ����X  P���x  `����             zR x�        ����   A�C�     <   ����x    A�Cs  $   \   ����[    A�CP�F          �   ���    A�CW   $   �   ����   A�Cj��         �   �����    A�C�     �   e���F    A�CA  $     �����    A�Cq�h          4  ���   A�C    T  ����(    A�Cc      t  ����O    A�CJ     �  (���4    A�Co      �  <���5    A�Cp      �  Q����    A�C~     �  �����    A�C|       ���7    A�Cr   $   4  ,���   A�Cq��          \  ����    A�C}     |  ~���   A�C�     �  `����    A�C}     �  ����           $   �  �����    Q��_@����X                                                                                                                                                                                                                                                                                                                                                                                                                                           ��������        ��������                                     �@            �@     ���o    `@            �@            �@     
       �                                           �!`                                       �@            `@            H       	              ���o     @     ���o           ���o    �@                                                                                                             ( `                     �@     �@     	@     	@     &	@     6	@     F	@     V	@     f	@     v	@     �	@     �	@     �	@     �	@     �	@     �	@     �	@     �	@     
@     
@     &
@     6
@         GCC: (GNU) 4.4.7 20120313 (Red Hat 4.4.7-11) ,             $@                           ,    �       (@     �                      ,    �        @     �                      ,    ,       �@                                   �  �  main     �    �  �  @  my_gets �  Malloc �  initialize_empty_EditOp !  set_range �  set_operator_and_string d  initialize_EditOp �  fill_edits_array   print_EditOp =  Free q  free_edits_array     j    �  �  �   find_current_line �   append !  insert z  new_line_insert �  delete_line   replace     6    ,  �  �  to_edit �  make_edit L  edit_line     �       {     6   $@     &@            �8   ]   int b   �   �   �  C  �        �M   B  �M   ?   �   8    �   �  X   �  1�   	�  �Z  
�   ?    
   �   
�   �   
L  �   
�  �    
/  �   (
�  �   0
�  �   8
�   �   � 
V  �   � 
�  �   � 
Z   �   � 
�  "�  � 
   $�  � 
�   &?   � 
t  *?   � 
�   ,w   � 
�   0b   �
!  1i   �
#   2�  �
d  6�  �
�   ?�   �
g  H�   �
n  I�   �
u  J�   �
|  K�   �
�  L-   �
|  N?   �
�  P�  � �  �  ��  <  ��   =  ��  �  �?    a  �   �   �  8     Z  �   �  8    �  �  �   �  8   � �  	�  �   �  8   O      �  �     �  PJ  0  �    �   �  �    �       u    u  �  �   �   �  8   8   O �   U  X �  �   J   �  !  � o   "�  � �  #�   � �  $�  D  �  �  �  8   c �  �  	?   $@     &@     ��  �  	?   ��w�  	�  ��ws@     $@     j  �  �X-   �  ��wi ?   �d�  �  ��}c �   �h  �   �   )  ��  �  ��  )  ��  �  ��   �   J  {   P  6   (@     @           �8   ]   �  C  b   �    int �       �i   B  �i   b   �   8    �   �  �  1�   	�  �L  
�   b    
   �   
�   �   
L  �   
�  �    
/  �   (
�  �   0
�  �   8
�   �   � 
V  �   � 
�  �   � 
Z   �   � 
�  "�  � 
   $�  � 
�   &b   � 
t  *b   � 
�   ,p   � 
�   0F   �
!  1T   �
#   2�  �
d  6�  �
�   ?{   �
g  H�   �
n  I�   �
u  J�   �
|  K�   �
�  L-   �
|  Nb   �
�  P�  � �  �  ��  <  ��   =  ��  �  �b    S  �   �   �  8     L  �   �  8    �   X   1   S  �.  �  �  �$  �   �� m  ���  ��q    �  ?   �  +  �   ;  8   � �  	F  �   V  8   O q     �  �     V  P�  0  ;    �   �  �    �   |   �    �  �      �   �  8   8   O �   �  X /  �   �   �  !q  � o   "�  � �  #�   � �  $�  /  7  �   (@     �@     ��  e  �   ��}N  �  ��}�     ��}foo 	�   ��}c 
�   �h �     �   �@     �@     ��  �  -   �Hc �   �X �  :  �@     @     �!    :  �h g   @     �@     ��     :  ��}�   �   ��}�  !   ��}�  "�   ��}�  #�   �P�@     v@     �    /b   ��}]  /b   ��}�  0�   ��} �@     �@     i 8b   �\  '  F�@     �@     �d    F:  �H�  F�   �@E@     �@     �  L�   �`\  M�   �h�  N�   �X  z  V:  �@     �@     ��  �  V�   �X  W:  �h �  _�@     �@     �	  -   _	  ��}H  _�  ��}�  `   ��}c a�   �Pi bb   �\ :  �  p�@     �@     �=    p:  �h �  ��@     �@     �k  ptr �k  �h �   ?  ��@     @     ��  -   �	  �Xi �b   �l �  ��  �  ��   �   �  {   v  6    @     �@     m  ]   �  C  b   �    int �   k   �  �   X   �  �   k   �   -   � �  	e    @     T@     ��   	�  	e   �X
c 
e   �h �  T@     �@     �!  	�  e   �X	�  e   �P
c e   �h �  �@     @     �z  	�  e   ��}	�  e   ��}
c e   �h�  �   ��} �  $@     �@     ��  	�  $e   ��}	�  $e   ��}�  %�   ��}
c &e   �h �  /�@     �@     �  	�  /e   �X
c 0e   �h �  5�@     �@     �	�  5e   ��}	�  5e   ��}	\  5e   ��}�  6e   �H�  7e   �P
c 8e   �X�  9�   ��}  �   �  {   &  6   �@     �@       ]   �  C  b   �    int �   W   u   -    {   �  �   X   �  �   {   �   -   � �  	�   {   �   -   O �   	   	�  	�     �   
P  0  �     e   �  {    �   �    A    A  �  �    {   W  -   -   O �   !  X �  �      �  !�   � o   "W  � �  #{   � �  $b  �  �  W   �@     ^@     ��    �  �h�  u   �`  W   �\   {   ^@     `@     �L    �  �h�  u   �`  W   �\   7`@     �@     ��  -   7�  �X�  7u   �P  7W   �Li 8W   �hc 9{   �o �   %   :;I  $ >  $ >  I  ! I/      I  	:;  
 :;I8   :;  :;   :;I8  :;  (   :;   :;I  :;  :;  .?:;'I@
   :;I
    4 :;I
  4 :;I
  4 :;I?<   %   :;I  $ >  $ >  I  ! I/      I  	:;  
 :;I8   :;  :;   :;I8  :;  (   :;   :;I  :;  :;  .?:;'I@
   :;I
  4 :;I
  4 :;I
  .?:;I@
  .?:;'@
       :;I
  4 :;I?<   %  $ >  $ >   I   :;I  I  ! I/  .?:;'I@
  	 :;I
  
4 :;I
  .?:;'@
  4 :;I
  .?:;'@
   %  $ >  $ >  I  ! I/   I   :;I  :;  	(   
:;   :;I  :;  :;   :;I8  .?:;'I@
   :;I
  .?:;'@
  4 :;I
   �    �   �      /usr/lib/gcc/x86_64-redhat-linux/4.4.7/include /usr/include/bits /usr/include  svi.c    stddef.h   types.h   stdio.h   libio.h   string_def.h    edit_operator.h    edit_input.h      	$@     	t�$�"v�Z�v�K+$�Z i   �   �      /usr/lib/gcc/x86_64-redhat-linux/4.4.7/include /usr/include/bits /usr/include  edit_input.c    stddef.h   types.h   stdio.h   libio.h   string_def.h    ctype.h   edit_operator.h      	(@     ��u3K2��u'ZKx��K1��"������,�1����ב��˟Z������g�1��//K3�v�u$+%K0e���uh"!v�0�0���,2��3��K�� �    7   �       edit_functions.c    string_def.h      	 @     	�KuhK4��/2�0�Y�YY1�/��Y=Z1���2��g0��7�Yo&
 �    C   �       editing.c    string_def.h    edit_operator.h      	�@     .Z�sZvvuvvvZ2.�&g�gvgv�v&vxY20�K%c�#�� __off_t _IO_read_ptr _chain size_t _shortbuf my_edits /home/ugrad/jmol322/Stream Editor long long unsigned int the_strings GNU C 4.4.7 20120313 (Red Hat 4.4.7-11) long long int StringType Range _fileno _IO_read_end _flags _IO_buf_end _cur_column _old_offset old_and_new endpoints svi.c _IO_marker stdin _IO_write_ptr _sbuf short unsigned int _IO_save_base _lock edit_file _flags2 _mode text_type short_string _IO_write_end _IO_lock_t edit_range _IO_FILE _pos edit_range_type _markers unsigned char operator_specifier no_type short int RangeType _vtable_offset search_text _next __off64_t _IO_read_base _IO_save_end __pad1 __pad2 __pad3 __pad4 __pad5 _unused2 stderr argv add_text _IO_backup_base long_string line_type argc main _IO_write_base _IO_buf_base isitall _ISgraph _ISspace Malloc _ISalpha _ISdigit set_operator_and_string free_edits_array edit_input.c last_line set_range _ISblank initialize_EditOp Free comma _ISpunct save temp edit_string old_text size fill_edits_array print_EditOp initialize_empty_EditOp range_identifier first_line MyOp _IScntrl _ISxdigit _ISlower my_gets _ISalnum edits_file _ISupper new_text my_text _ISprint edit_functions.c old_text_pointer replace new_line_insert append_text find_current_line delete_line insert_text append output remainder to_edit edit_line line_number make_edit editing.c �        �  -   size_t w   __off_t �   __off64_t �   FILE Z  _IO_lock_t a  _IO_marker �   _IO_FILE �  long_string �  short_string   RangeType J  Range �  StringType �  EditOp �  edits_array     �    �  �  -   size_t p   __off_t {   __off64_t �   FILE L  _IO_lock_t S  _IO_marker �   _IO_FILE    long_string ;  short_string q  RangeType �  Range �  StringType /  EditOp         �  �  �   long_string     a    ,  �  �   long_string �   short_string �   RangeType   Range W  StringType �  EditOp      .symtab .strtab .shstrtab .interp .note.ABI-tag .note.gnu.build-id .gnu.hash .dynsym .dynstr .gnu.version .gnu.version_r .rela.dyn .rela.plt .init .text .fini .rodata .eh_frame_hdr .eh_frame .ctors .dtors .jcr .dynamic .got .got.plt .data .bss .comment .debug_aranges .debug_pubnames .debug_info .debug_abbrev .debug_line .debug_str .debug_pubtypes                                                                                  @                                          #             @                                          1             <@     <      $                              D   ���o       `@     `      (                             N             �@     �      p                          V             �@     �      �                              ^   ���o       �@     �      4                            k   ���o        @            @                            z             `@     `      H                            �             �@     �                                �             �@     �                                    �             �@     �      p                            �             @
@     @
      �                             �             �@     �                                    �             �@     �      �                             �             �@     �      �                              �             `@     `      �                             �               `                                           �              `                                          �               `                                           �             ( `     (       �                           �             �!`     �!                                   �             �!`     �!      �                             �             �"`     �"                                    �             �"`     �"                                     �      0               �"      -                             �                      �"      �                                                   y#      �                                                  %      �                             )                     �6      �                             7                     ;      �                             C     0               ?      0                            N                     DD                                                         WF      ^                                                   �P      �
      $   8                 	                      �[      �                                                            @                   @                   <@                   `@                   �@                   �@                   �@                    @                  	 `@                  
 �@                   �@                   �@                   @
@                   �@                   �@                   �@                   `@                     `                    `                     `                   ( `                   �!`                   �!`                   �"`                   �"`                                                                                                                                                                             !                      l
@                 ��                       `             *      `             8       `             E     �
@             [     �"`            j     �"`            x      @                 ��                �      `             �     X@             �       `             �     �@             �    ��                �    ��                �    ��                �    ��                �     �!`                    `                    `             )    ( `             2     �"`             =                     Q                     e    �@            u    @
@             |    @     �       �                     �                      �                      �                     �    �@     �       �                         �@                �@             %                     9                     L                     k     @     4       }                     �    �@     �       �    �@            �                     �                     �    �"`             �                     �    �"`                                 $    �@     �       ,                     B    �@           O                     c                     w   �@             �    T@     5       �    `             �     @     �       �                     �    �@           �    `@     �       �    ^@           �   ���"`             �    �@     7       �                                              (@     x           ���"`             %                     :    �"`            N    @     �      X                     l   ���"`             s    �@     �       z    �@     [       �                     �    �@     O       �    �@     F       �    �@     (       �    $@           �    �@              call_gmon_start crtstuff.c __CTOR_LIST__ __DTOR_LIST__ __JCR_LIST__ __do_global_dtors_aux completed.6349 dtor_idx.6351 frame_dummy __CTOR_END__ __FRAME_END__ __JCR_END__ __do_global_ctors_aux svi.c edit_input.c edit_functions.c editing.c _GLOBAL_OFFSET_TABLE_ __init_array_end __init_array_start _DYNAMIC data_start printf@@GLIBC_2.2.5 memset@@GLIBC_2.2.5 __libc_csu_fini _start new_line_insert abort@@GLIBC_2.2.5 __gmon_start__ _Jv_RegisterClasses puts@@GLIBC_2.2.5 set_operator_and_string __isoc99_sscanf@@GLIBC_2.7 initialize_empty_EditOp _fini malloc@@GLIBC_2.2.5 fopen@@GLIBC_2.2.5 __libc_start_main@@GLIBC_2.2.5 find_current_line fgets@@GLIBC_2.2.5 fill_edits_array _IO_stdin_used free@@GLIBC_2.2.5 strlen@@GLIBC_2.2.5 __data_start __ctype_b_loc@@GLIBC_2.3 stdin@@GLIBC_2.2.5 strrchr@@GLIBC_2.2.5 to_edit strtok_r@@GLIBC_2.2.5 print_EditOp strstr@@GLIBC_2.2.5 strcat@@GLIBC_2.2.5 __dso_handle append __DTOR_END__ __libc_csu_init memcpy@@GLIBC_2.2.5 replace edit_line make_edit __bss_start delete_line strcmp@@GLIBC_2.2.5 strcpy@@GLIBC_2.2.5 my_gets _end strncpy@@GLIBC_2.2.5 stderr@@GLIBC_2.2.5 set_range fwrite@@GLIBC_2.2.5 _edata insert Malloc fprintf@@GLIBC_2.2.5 free_edits_array initialize_EditOp Free main _init                                                                                                                                                                                                                                                                                                                                                                                        svi.c                                                                                               0000644 0043077 0000017 00000001243 12520227265 007650  0                                                                                                    ustar                                   user                                                                                                                                                                                                                   #include <stdlib.h>
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
                                                                                                                                                                                                                                                                                                                                                             test_files/                                                                                         0000755 0043077 0000017 00000000000 12520224033 011032  5                                                                                                    ustar                                   user                                                                                                                                                                                                                   test_files/output_file.txt                                                                          0000644 0043077 0000017 00000002062 12520233226 014136  0                                                                                                    ustar                                   user                                                                                                                                                                                                                   Adding a line to each line.
Adding a line again to left lines.
At the beginning -This is first line. RRR RRR RRR sss sss sss.
Adding a line to each line.
Adding a line again to left lines.
Adding a line to each line.
Adding a line to each line.
Adding a line again to left lines.
At the beginning -Line number four.
Adding a line to each line.
Adding a line again to left lines.
This is line number 5 -At the beginning -AAA RRR RRR RRR sss sss sss.
Adding a line to each line.
Adding a line to each line.
Adding a line again to left lines.
At the beginning -Seven. RRR RRR RRR rrr rrr rrr.
Adding a line to each line.
Adding a line to each line.
Adding a line again to left lines.
At the beginning -RRR RRR RRR rrr rrr rrr.
Adding a line to each line.
Adding a line again to left lines.
At the beginning -DUMB lines 1.
Adding a line to each line.
Adding a line again to left lines.
At the beginning -DUMb lines 2. ReplaceAll.
Adding a line to each line.
Adding a line to each line.
Adding a line again to left lines.
At the beginning -DUmb lines 3. RRR RRR RRR sss sss sss.
                                                                                                                                                                                                                                                                                                                                                                                                                                                                              test_files/test_edits.txt                                                                           0000644 0043077 0000017 00000000461 12520035033 013743  0                                                                                                    ustar                                   user                                                                                                                                                                                                                   /Never done/I---------------------------------------
1,3/IPrepended to 1,2 and 3 :
1,1/OThis must appear as the first line
A : Appended to all
/line for substitution/s/This is one/This has been substituted on a/
9,10/d
/deleted/IThis should not appear
/Never done/I---------------------------------------
                                                                                                                                                                                                               test_files/input_file2.txt                                                                          0000644 0043077 0000017 00000000704 12520064467 014031  0                                                                                                    ustar                                   user                                                                                                                                                                                                                   This is first line. SSS SSS SSS sss sss sss.
DDD DDD DDD. Anyway this will get deleted.
ReplaceAll. These lines need to be deleted.
Line number four.
AAA SSS SSS SSS sss sss sss.
Nothing. These lines need to be deleted. SSS SSS SSS sss sss sss.
Seven. SSS SSS SSS sss sss sss.
Eight. These lines need to be deleted.
SSS SSS SSS sss sss sss.
DUMB lines 1.
DUMb lines 2. ReplaceAll.
These lines need to be deleted.
DUmb lines 3. SSS SSS SSS sss sss sss.
                                                            test_files/svi                                                                                      0000755 0043077 0000017 00000060211 12520233110 011555  0                                                                                                    ustar                                   user                                                                                                                                                                                                                   ELF          >    @
@     @       �G          @ 8  @ % "       @       @ @     @ @     �      �                           @      @                                          @       @     \      \                              `       `     �      �                    (       ( `     ( `     �      �                         @     @     D       D              P�td   �      �@     �@     �       �              Q�td                                                  /lib64/ld-linux-x86-64.so.2          GNU                       GNU �~q���}�'�|7,���               �         fUa9�                        �                      e                      L                                             G                      /                      �                      !                      �                      X                      �                      ^                      �                      '                      s                      l                      �                      |                      �                                            ?                      �                      �                      R     �"`            �     �"`             __gmon_start__ libc.so.6 strcpy fopen strrchr __isoc99_sscanf strncpy puts abort stdin fgets strlen memset strstr strtok_r memcpy malloc strcat __ctype_b_loc stderr fwrite fprintf strcmp __libc_start_main free GLIBC_2.3 GLIBC_2.7 GLIBC_2.2.5                                          ii   �      ii   �      ui	   �       �!`                   �"`                   �"`                   �!`                   �!`                   �!`                   �!`                   �!`                    "`                   "`                   "`        	           "`        
            "`                   ("`                   0"`                   8"`                   @"`                   H"`                   P"`                   X"`                   `"`                   h"`                   p"`                   x"`                   �"`                   H���  �:  ��  H����5�  �%�  @ �%�  h    ������%�  h   ������%�  h   ������%�  h   �����%�  h   �����%�  h   �����%�  h   �����%�  h   �p����%�  h   �`����%�  h	   �P����%�  h
   �@����%�  h   �0����%�  h   � ����%�  h   �����%�  h   � ����%z  h   ������%r  h   ������%j  h   ������%b  h   ������%Z  h   �����%R  h   �����%J  h   ����1�I��^H��H���PTI���@ H�� @ H��$@ ��������H��H�A  H��t��H��Ð������������UH��SH���=    uK� ` H��  H�� ` H��H��H9�s$fD  H��H��  �� ` H��  H9�r���  H��[��fff.�     H�=   UH��t�    H��t�  ` ����Ð�UH��H��P  ������H������������t.H�L  H�¸�@ H�Ѻ/   �   H�������   �   �@ H������H��H� H��H������H�E��E�   H������H�E�H�U�H������H��H���>  H��  H������H��H���R   H�E�H�}� t"�U�H������H������H��H���c
  �E�뺐H������H���  H�E�H���Q����    �Ð�UH��H��0  H������H������H������H�������   H������H�E�H�}� t3� @ H������H������H��H���+���H��H������H��H���f���H�E���UH��SH��(H�}�H�E�H���w���H�E�H�}� u,H�E؉»(@ H��  �   H��H�Ǹ    �E�������H�E�H��([��UH��H���X  ����H�E�H�E���UH��SH��H  H������H������H������H������H��H�������[@ H������H������H��H���H���H�E�H������H�E�H��H���n�����uH�������@P    H������� �K  H������� <suH�������@P    H������� �   ����H�H������� H��H�H�� ��%   ��t|H�������@P   �]@ H�E�H������H������H������I��H��H�Ǹ    �����������H�������������H�������PH������H��H������H��H�������xH������� </u[H�������@P   �E�    � �]�E�H�HE��H������HcÈ�E��}�O~�H������H��H������H��H���#�����h@ �'�������H��H  [��UH��H��0H�}�H�u�H�E��H�E؈�T  H�E���T  <stH�E�H�PH�E�H��TH��H�������n�[@ H�U�H�JH�U�H��H���O���H�E�[@ H�E�H�U�H��H���3���H�E�H�U�H�E�H��TH��H���h���H�E�H�U�H��TH��PH��H���M�����UH��H�� H�}�    �0���H�E�H�U�H�E�H��H���5���H�U�H�E�H��H������H�E���UH��SH��(  H������H�������E�    H������H������H��H�������H�E�H�}� u��F�E�H�H��H��H�����H������H���P���H��E�뮋E�H�H��H�����H�     �E��}�c~�H��(  [��UH��H��H�}�H�}� u��@ H�Ǹ    �"�����   H�E���T  �и�@ ��H�Ǹ    �������@ H�Ǹ    �����H�E��@P��t;��r��t�;��@ ������/H�E��PH�E����@ ��H�Ǹ    �����H�E�H�������H�E���T  <stH�E�H�PT��@ H��H�Ǹ    �j����,H�E�H��TH�PPH�E�H��TH����@ H��H�Ǹ    �<�����UH��H��H�}�H�E�H� H������H�E�H�     ��UH��H�� H�}��E�    �2�E�H�H��HE�H� H��tH�E�U�Hc�H��H�H�������E��}�c~��Ð��UH��H�� H�}�H�E�
   H���c���H�E�H�}� uH�E��H�E���UH��H�� H�}�H�u�H�E�H������H�E�H�U�H�E�H��H���I�����UH��H��   H������H������H������H���o���H�E�H�E�� <
uH�E�H������H������H��H���"���H�U�H������H��H�������H������H�E�H��H���������UH��H��   H������H������H������H�������H�E�H������H������H��H������H�E�� <
uH�E�H������� @ H�������H������H�E�H��H���������UH��H�� H�}�H�E�H���{���H�E��@ H�E��   H��H��������UH��SH��H  H������H������H������H������H������H��H�������H�E�H������H���o���HE�H�E�H������H�������H�E�H�}� ��   H�������   �    H������H�E�H���$���H��H�E�H������H��H)�H�M�H������H��H������H������H������H��H���0���H�U�H������H��H������H������H�E�H��H���4�����H��H  [�Ð��UH��H�� H�}�H�u��U�H�E��@P��t/��rP��uRH�E�� ;E�H�E��@;E�|�   �9�    �2H�U�H�E�H��H������H��t�   ��    ��   ��    ��UH��H�� H�}�H�u��U�U�H�M�H�E�H��H���U�������   H�E���T  ����A��2��   ��H��@ ��H�E�H�PTH�E�H��H�������A   �   H�E�H�PTH�E�H��H�������I   �jH�E�H�PTH�E�H��H�������O   �LH�E�H���o����d   �9H�E�H��TH�PPH�E�H��TH��H�E�H��H���y����s   ��    ��    ��UH��H��0H�}�H�u��U��E�    �D�E�H�H��HE�H� H��t+�E�H�H��HE�H� �U�H�M�H��H�������E��}�dt�E��}�c~���H�E�� ��tH�E�H���0����Ð���������������fffff.�     H�l$�L�d$�H�-�  L�%�  L�l$�L�t$�L�|$�H�\$�H��8L)�A��I��H��I���s���H��t1�@ L��L��D��A��H��H9�r�H�\$H�l$L�d$L�l$ L�t$(L�|$0H��8Ð������UH��SH��H�`  H���t�  ` D  H����H�H���u�H��[�Ð�H������H���                Error : Please enter the name of the edit file
 r       
       Error : memory allocation failed at Malloc(%d): %d / %d %c %d   There has been an invalid edit command This is not an edit op Operation Specifier: %c
 Range :  all %d , %d
 String : %s
 Old Text : %s
 New Text : %s
 
       �@     R@     R@     R@     R@     R@     R@     R@     �@     R@     R@     R@     R@     R@     �@     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     @     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     R@     %@     ;�      �����   �����    ���  [���@  w���`  X����  ����  [����  �����  ���  .���0  ����P  ����p  �����  l����  �����  $����  <���  ����8  ����X  P���x  `����             zR x�        ����   A�C�     <   ����x    A�Cs  $   \   ����[    A�CP�F          �   ���    A�CW   $   �   ����   A�Cj��         �   �����    A�C�     �   e���F    A�CA  $     �����    A�Cq�h          4  ���   A�C    T  ����(    A�Cc      t  ����O    A�CJ     �  (���4    A�Co      �  <���5    A�Cp      �  Q����    A�C~     �  �����    A�C|       ���7    A�Cr   $   4  ,���   A�Cq��          \  ����    A�C}     |  ~���   A�C�     �  `����    A�C}     �  ����           $   �  �����    Q��_@����X                                                                                                                                                                                                                                                                                                                                                                                                                                           ��������        ��������                                     �@            �@     ���o    `@            �@            �@     
       �                                           �!`                                       �@            `@            H       	              ���o     @     ���o           ���o    �@                                                                                                             ( `                     �@     �@     	@     	@     &	@     6	@     F	@     V	@     f	@     v	@     �	@     �	@     �	@     �	@     �	@     �	@     �	@     �	@     
@     
@     &
@     6
@         GCC: (GNU) 4.4.7 20120313 (Red Hat 4.4.7-11) ,             $@                           ,    �       (@     �                      ,    �        @     �                      ,    ,       �@                                   �  �  main     �    �  �  @  my_gets �  Malloc �  initialize_empty_EditOp !  set_range �  set_operator_and_string d  initialize_EditOp �  fill_edits_array   print_EditOp =  Free q  free_edits_array     j    �  �  �   find_current_line �   append !  insert z  new_line_insert �  delete_line   replace     6    ,  �  �  to_edit �  make_edit L  edit_line     �       {     6   $@     &@            �8   ]   int b   �   �   �  C  �        �M   B  �M   ?   �   8    �   �  X   �  1�   	�  �Z  
�   ?    
   �   
�   �   
L  �   
�  �    
/  �   (
�  �   0
�  �   8
�   �   � 
V  �   � 
�  �   � 
Z   �   � 
�  "�  � 
   $�  � 
�   &?   � 
t  *?   � 
�   ,w   � 
�   0b   �
!  1i   �
#   2�  �
d  6�  �
�   ?�   �
g  H�   �
n  I�   �
u  J�   �
|  K�   �
�  L-   �
|  N?   �
�  P�  � �  �  ��  <  ��   =  ��  �  �?    a  �   �   �  8     Z  �   �  8    �  �  �   �  8   � �  	�  �   �  8   O      �  �     �  PJ  0  �    �   �  �    �       u    u  �  �   �   �  8   8   O �   U  X �  �   J   �  !  � o   "�  � �  #�   � �  $�  D  �  �  �  8   c �  �  	?   $@     &@     ��  �  	?   ��w�  	�  ��ws@     $@     j  �  �X-   �  ��wi ?   �d�  �  ��}c �   �h  �   �   )  ��  �  ��  )  ��  �  ��   �   J  {   P  6   (@     @           �8   ]   �  C  b   �    int �       �i   B  �i   b   �   8    �   �  �  1�   	�  �L  
�   b    
   �   
�   �   
L  �   
�  �    
/  �   (
�  �   0
�  �   8
�   �   � 
V  �   � 
�  �   � 
Z   �   � 
�  "�  � 
   $�  � 
�   &b   � 
t  *b   � 
�   ,p   � 
�   0F   �
!  1T   �
#   2�  �
d  6�  �
�   ?{   �
g  H�   �
n  I�   �
u  J�   �
|  K�   �
�  L-   �
|  Nb   �
�  P�  � �  �  ��  <  ��   =  ��  �  �b    S  �   �   �  8     L  �   �  8    �   X   1   S  �.  �  �  �$  �   �� m  ���  ��q    �  ?   �  +  �   ;  8   � �  	F  �   V  8   O q     �  �     V  P�  0  ;    �   �  �    �   |   �    �  �      �   �  8   8   O �   �  X /  �   �   �  !q  � o   "�  � �  #�   � �  $�  /  7  �   (@     �@     ��  e  �   ��}N  �  ��}�     ��}foo 	�   ��}c 
�   �h �     �   �@     �@     ��  �  -   �Hc �   �X �  :  �@     @     �!    :  �h g   @     �@     ��     :  ��}�   �   ��}�  !   ��}�  "�   ��}�  #�   �P�@     v@     �    /b   ��}]  /b   ��}�  0�   ��} �@     �@     i 8b   �\  '  F�@     �@     �d    F:  �H�  F�   �@E@     �@     �  L�   �`\  M�   �h�  N�   �X  z  V:  �@     �@     ��  �  V�   �X  W:  �h �  _�@     �@     �	  -   _	  ��}H  _�  ��}�  `   ��}c a�   �Pi bb   �\ :  �  p�@     �@     �=    p:  �h �  ��@     �@     �k  ptr �k  �h �   ?  ��@     @     ��  -   �	  �Xi �b   �l �  ��  �  ��   �   �  {   v  6    @     �@     m  ]   �  C  b   �    int �   k   �  �   X   �  �   k   �   -   � �  	e    @     T@     ��   	�  	e   �X
c 
e   �h �  T@     �@     �!  	�  e   �X	�  e   �P
c e   �h �  �@     @     �z  	�  e   ��}	�  e   ��}
c e   �h�  �   ��} �  $@     �@     ��  	�  $e   ��}	�  $e   ��}�  %�   ��}
c &e   �h �  /�@     �@     �  	�  /e   �X
c 0e   �h �  5�@     �@     �	�  5e   ��}	�  5e   ��}	\  5e   ��}�  6e   �H�  7e   �P
c 8e   �X�  9�   ��}  �   �  {   &  6   �@     �@       ]   �  C  b   �    int �   W   u   -    {   �  �   X   �  �   {   �   -   � �  	�   {   �   -   O �   	   	�  	�     �   
P  0  �     e   �  {    �   �    A    A  �  �    {   W  -   -   O �   !  X �  �      �  !�   � o   "W  � �  #{   � �  $b  �  �  W   �@     ^@     ��    �  �h�  u   �`  W   �\   {   ^@     `@     �L    �  �h�  u   �`  W   �\   7`@     �@     ��  -   7�  �X�  7u   �P  7W   �Li 8W   �hc 9{   �o �   %   :;I  $ >  $ >  I  ! I/      I  	:;  
 :;I8   :;  :;   :;I8  :;  (   :;   :;I  :;  :;  .?:;'I@
   :;I
    4 :;I
  4 :;I
  4 :;I?<   %   :;I  $ >  $ >  I  ! I/      I  	:;  
 :;I8   :;  :;   :;I8  :;  (   :;   :;I  :;  :;  .?:;'I@
   :;I
  4 :;I
  4 :;I
  .?:;I@
  .?:;'@
       :;I
  4 :;I?<   %  $ >  $ >   I   :;I  I  ! I/  .?:;'I@
  	 :;I
  
4 :;I
  .?:;'@
  4 :;I
  .?:;'@
   %  $ >  $ >  I  ! I/   I   :;I  :;  	(   
:;   :;I  :;  :;   :;I8  .?:;'I@
   :;I
  .?:;'@
  4 :;I
   �    �   �      /usr/lib/gcc/x86_64-redhat-linux/4.4.7/include /usr/include/bits /usr/include  svi.c    stddef.h   types.h   stdio.h   libio.h   string_def.h    edit_operator.h    edit_input.h      	$@     	t�$�"v�Z�v�K+$�Z i   �   �      /usr/lib/gcc/x86_64-redhat-linux/4.4.7/include /usr/include/bits /usr/include  edit_input.c    stddef.h   types.h   stdio.h   libio.h   string_def.h    ctype.h   edit_operator.h      	(@     ��u3K2��u'ZKx��K1��"������,�1����ב��˟Z������g�1��//K3�v�u$+%K0e���uh"!v�0�0���,2��3��K�� �    7   �       edit_functions.c    string_def.h      	 @     	�KuhK4��/2�0�Y�YY1�/��Y=Z1���2��g0��7�Yo&
 �    C   �       editing.c    string_def.h    edit_operator.h      	�@     .Z�sZvvuvvvZ2.�&g�gvgv�v&vxY20�K%c�#�� __off_t _IO_read_ptr _chain size_t _shortbuf my_edits /home/ugrad/jmol322/Stream Editor long long unsigned int the_strings GNU C 4.4.7 20120313 (Red Hat 4.4.7-11) long long int StringType Range _fileno _IO_read_end _flags _IO_buf_end _cur_column _old_offset old_and_new endpoints svi.c _IO_marker stdin _IO_write_ptr _sbuf short unsigned int _IO_save_base _lock edit_file _flags2 _mode text_type short_string _IO_write_end _IO_lock_t edit_range _IO_FILE _pos edit_range_type _markers unsigned char operator_specifier no_type short int RangeType _vtable_offset search_text _next __off64_t _IO_read_base _IO_save_end __pad1 __pad2 __pad3 __pad4 __pad5 _unused2 stderr argv add_text _IO_backup_base long_string line_type argc main _IO_write_base _IO_buf_base isitall _ISgraph _ISspace Malloc _ISalpha _ISdigit set_operator_and_string free_edits_array edit_input.c last_line set_range _ISblank initialize_EditOp Free comma _ISpunct save temp edit_string old_text size fill_edits_array print_EditOp initialize_empty_EditOp range_identifier first_line MyOp _IScntrl _ISxdigit _ISlower my_gets _ISalnum edits_file _ISupper new_text my_text _ISprint edit_functions.c old_text_pointer replace new_line_insert append_text find_current_line delete_line insert_text append output remainder to_edit edit_line line_number make_edit editing.c �        �  -   size_t w   __off_t �   __off64_t �   FILE Z  _IO_lock_t a  _IO_marker �   _IO_FILE �  long_string �  short_string   RangeType J  Range �  StringType �  EditOp �  edits_array     �    �  �  -   size_t p   __off_t {   __off64_t �   FILE L  _IO_lock_t S  _IO_marker �   _IO_FILE    long_string ;  short_string q  RangeType �  Range �  StringType /  EditOp         �  �  �   long_string     a    ,  �  �   long_string �   short_string �   RangeType   Range W  StringType �  EditOp      .symtab .strtab .shstrtab .interp .note.ABI-tag .note.gnu.build-id .gnu.hash .dynsym .dynstr .gnu.version .gnu.version_r .rela.dyn .rela.plt .init .text .fini .rodata .eh_frame_hdr .eh_frame .ctors .dtors .jcr .dynamic .got .got.plt .data .bss .comment .debug_aranges .debug_pubnames .debug_info .debug_abbrev .debug_line .debug_str .debug_pubtypes                                                                                  @                                          #             @                                          1             <@     <      $                              D   ���o       `@     `      (                             N             �@     �      p                          V             �@     �      �                              ^   ���o       �@     �      4                            k   ���o        @            @                            z             `@     `      H                            �             �@     �                                �             �@     �                                    �             �@     �      p                            �             @
@     @
      �                             �             �@     �                                    �             �@     �      �                             �             �@     �      �                              �             `@     `      �                             �               `                                           �              `                                          �               `                                           �             ( `     (       �                           �             �!`     �!                                   �             �!`     �!      �                             �             �"`     �"                                    �             �"`     �"                                     �      0               �"      -                             �                      �"      �                                                   y#      �                                                  %      �                             )                     �6      �                             7                     ;      �                             C     0               ?      0                            N                     DD                                                         WF      ^                                                   �P      �
      $   8                 	                      �[      �                                                            @                   @                   <@                   `@                   �@                   �@                   �@                    @                  	 `@                  
 �@                   �@                   �@                   @
@                   �@                   �@                   �@                   `@                     `                    `                     `                   ( `                   �!`                   �!`                   �"`                   �"`                                                                                                                                                                             !                      l
@                 ��                       `             *      `             8       `             E     �
@             [     �"`            j     �"`            x      @                 ��                �      `             �     X@             �       `             �     �@             �    ��                �    ��                �    ��                �    ��                �     �!`                    `                    `             )    ( `             2     �"`             =                     Q                     e    �@            u    @
@             |    @     �       �                     �                      �                      �                     �    �@     �       �                         �@                �@             %                     9                     L                     k     @     4       }                     �    �@     �       �    �@            �                     �                     �    �"`             �                     �    �"`                                 $    �@     �       ,                     B    �@           O                     c                     w   �@             �    T@     5       �    `             �     @     �       �                     �    �@           �    `@     �       �    ^@           �   ���"`             �    �@     7       �                                              (@     x           ���"`             %                     :    �"`            N    @     �      X                     l   ���"`             s    �@     �       z    �@     [       �                     �    �@     O       �    �@     F       �    �@     (       �    $@           �    �@              call_gmon_start crtstuff.c __CTOR_LIST__ __DTOR_LIST__ __JCR_LIST__ __do_global_dtors_aux completed.6349 dtor_idx.6351 frame_dummy __CTOR_END__ __FRAME_END__ __JCR_END__ __do_global_ctors_aux svi.c edit_input.c edit_functions.c editing.c _GLOBAL_OFFSET_TABLE_ __init_array_end __init_array_start _DYNAMIC data_start printf@@GLIBC_2.2.5 memset@@GLIBC_2.2.5 __libc_csu_fini _start new_line_insert abort@@GLIBC_2.2.5 __gmon_start__ _Jv_RegisterClasses puts@@GLIBC_2.2.5 set_operator_and_string __isoc99_sscanf@@GLIBC_2.7 initialize_empty_EditOp _fini malloc@@GLIBC_2.2.5 fopen@@GLIBC_2.2.5 __libc_start_main@@GLIBC_2.2.5 find_current_line fgets@@GLIBC_2.2.5 fill_edits_array _IO_stdin_used free@@GLIBC_2.2.5 strlen@@GLIBC_2.2.5 __data_start __ctype_b_loc@@GLIBC_2.3 stdin@@GLIBC_2.2.5 strrchr@@GLIBC_2.2.5 to_edit strtok_r@@GLIBC_2.2.5 print_EditOp strstr@@GLIBC_2.2.5 strcat@@GLIBC_2.2.5 __dso_handle append __DTOR_END__ __libc_csu_init memcpy@@GLIBC_2.2.5 replace edit_line make_edit __bss_start delete_line strcmp@@GLIBC_2.2.5 strcpy@@GLIBC_2.2.5 my_gets _end strncpy@@GLIBC_2.2.5 stderr@@GLIBC_2.2.5 set_range fwrite@@GLIBC_2.2.5 _edata insert Malloc fprintf@@GLIBC_2.2.5 free_edits_array initialize_EditOp Free main _init                                                                                                                                                                                                                                                                                                                                                                                        test_files/a.out                                                                                    0000755 0043077 0000017 00000037341 12520041717 012024  0                                                                                                    ustar                                   user                                                                                                                                                                                                                   ELF          >    �@     @       0*          @ 8  @ % "       @       @ @     @ @     �      �                           @      @                                          @       @     �      �                            `      `     4      H                    (      (`     (`     �      �                         @     @     D       D              P�td   $      $@     $@     \       \              Q�td                                                  /lib64/ld-linux-x86-64.so.2          GNU                       GNU �#��6��S.Q����X9��                                                     A                                             <                      ,                      !                      \                      &                      c                      O                      U                                            H                       __gmon_start__ libc.so.6 strcpy exit fopen __isoc99_sscanf puts printf strtok fgets strlen malloc __libc_start_main GLIBC_2.7 GLIBC_2.2.5                               ii   u      ui	          �`                   �`                   �`                   �`                   �`                   �`                    `                   `                   `        	           `        
            `                   (`                   H���  �  �M  H����5�  �%�  @ �%�  h    ������%�  h   ������%�  h   ������%�  h   �����%�  h   �����%�  h   �����%�  h   �����%�  h   �p����%�  h   �`����%�  h	   �P����%�  h
   �@���        1�I��^H��H���PTI��p@ H���@ H���@ �������H��H��
  H��t��H��Ð������������UH��SH���=H   uK�` H�B  H��` H��H��H9�s$fD  H��H�  ��` H�  H9�r���
  H��[��fff.�     H�=�   UH��t�    H��t� ` ����Ð�UH��H��   H������H������H������H�������   H������H�E�H�}� t,�h@ H������H��H������H��H������H��H������H�E���UH��H���X  �!���H�E�H�E���UH��SH��8H�}�H�u�H�E�H���:����Eܺj@ H�E�H��H���C���H�E�H�E�H�������U�Hc�H9�uH�E��@P    H�E�� H�E���   H�E�� </tZH�E��@P   �l@ H�E�H�u�H�M�H�U�I��H��H�Ǹ    �O����U�H�Eȉ�U�H�EȉP�u@ H�ƿ    �����jH�E�� </uKH�E��@P   �E�    ��]�E�H�HE��H�U�HcÈ�E��}�O~ݸu@ H�ƿ    �T�����x@ �����   �����H��8[��UH��H�� H�}�H�u�H�E��H�E舐T  H�E���T  <stH�E�H�PH�E�H��TH��H��������d�j@ H�U�H��H��H�������H�E�j@ H�ƿ    ����H�E�H�U�H�E�H��TH��H������H�E�H�U�H��TH��PH��H���v�����UH��H�� H�}�    �����H�E�H�U�H�E�H��H�������H�E�H�U�H�E�H��H���	���H�E���UH��SH��(  H������H�������E�    H������H������H��H�������H�E�H�}� u��F�E�H�H��H��H�����H������H���L���H��E�뮋E�H�H��H�����H�     �E��}�c~�H��(  [��UH��H��H�}�H�}� u��@ H�Ǹ    �������   H�E���T  �и�@ ��H�Ǹ    ������@ H�Ǹ    ����H�E��@P��t;��r��t�;��@ �����/H�E��PH�E����@ ��H�Ǹ    �Y����H�E�H���[���H�E���T  <stH�E�H�PT��@ H��H�Ǹ    �����,H�E�H��TH�PPH�E�H��TH����@ H��H�Ǹ    ������Ð��UH��H��0  �@ �@ H��H������H�E�H�U�H������H��H�������E�    �+�E�H�H�������H��t�E�H�H�������H�������E��}�c~��Ð���������������fffff.�     H�l$�L�d$�H�-o  L�%h  L�l$�L�t$�L�|$�H�\$�H��8L)�A��I��H��I�������H��t1�@ L��L��D��A��H��H9�r�H�\$H�l$L�d$L�l$ L�t$(L�|$0H��8Ð������UH��SH��H��  H���t� ` D  H����H�H���u�H��[�Ð�H������H���                
 / %d %c %d    There has been an invalid edit command This is not an edit op Operation Specifier: %c
 Range :  all %d , %d
 String : %s
 Old Text : %s
 New Text : %s
 r test_edits.txt    ;X   
   P���t   �����   �����   ����   �����   ���  ����D  ����d  L����  \����         zR x�        ����q    A�Cl     <   %���    A�CW   $   \   !���@   A�CT�'         �   9����    A�C�     �   ����J    A�CE  $   �   �����    A�Cq�h          �   l���   A�C      \���v    A�Cq     ,  ����           $   D  �����    Q��_@����X                           ��������        ��������                                     �@            H@     ���o    `@            �@            �@     
       �                                           �`                                       �@            �@                   	              ���o    `@     ���o           ���o    D@                                                                                                             (`                     �@     �@     �@     @     @     .@     >@     N@     ^@     n@     ~@         GCC: (GNU) 4.4.7 20120313 (Red Hat 4.4.7-11) ,             t@     u                      ,    �       �@     v                       �        �  �  my_gets G  initialize_empty_EditOp x  set_range 4  set_operator_and_string �  initialize_EditOp �  fill_edits_array W  print_EditOp         �  R  �  main     ~       {   �  6   t@     �@            �8   ]   o  �  b   q  �  int �       �i   �  �i   b   �   8    �   x  4  1�   	0  �L  
�   b    
   �   
�   �   
�  �   
�  �    
x  �   (
  �   0
�  �   8
�   �   � 
�  �   � 
b  �   � 
   �   � 
N  "�  � 
   $�  � 
�   &b   � 
�  *b   � 
"  ,p   � 
  0F   �
�  1T   �
#   2�  �
�  6�  �
&  ?{   �
  H�   �
!  I�   �
(  J�   �
/  K�   �
6  L-   �
�  Nb   �
=  P�  �   �a  ��  �  ��   �  ��  9  �b    S  �   �   �  8     L  �   �  8    �   X   r  �  �   �  8   � �  	�  �   �  8   O   �   �  ~   �  �  PJ  �  �  :  �   �  �    �       u  .  u  Y  �   �   �  8   8   O �   U  X �  %   J   >  !  � o   "�  � }  #�   � O  $�  �  F  �   t@     �@     �A  �  �   ��}T  A  ��}\  �  ��}c �   �h �   W  �  �@     @     �x  �  �  �h   �   @     A	@     �4  �  �  ���  �   ���  b   �L�  �   �Pv@     �@       �  b   �H  b   �DV  �   �C �@     &	@     i %b   �\  �   2A	@     �	@     ��  �  2�  �Xl  2�   �P�	@     �	@     �  8�   �`�  9�   �h  D  ?�  �	@     >
@     ��  �  ?�   �X�  @�  �`str A�   �h �  H>
@     �
@     �Q  -   HQ  ��}N  HA  ��}\  I�  ��}c J�   �Pi Kb   �\ �  �  Y�
@     �@     ��  Y�  �h  N   �  {   �  �  �@     b@     G     �8   ]   o  �  b   q  �  int �       �i   �  �i   b   �   8    �   x  4  1�   	0  �L  
�   b    
   �   
�   �   
�  �   
�  �    
x  �   (
  �   0
�  �   8
�   �   � 
�  �   � 
b  �   � 
   �   � 
N  "�  � 
   $�  � 
�   &b   � 
�  *b   � 
"  ,p   � 
  0F   �
�  1T   �
#   2�  �
�  6�  �
&  ?{   �
  H�   �
!  I�   �
(  J�   �
/  K�   �
6  L-   �
�  Nb   �
=  P�  �   �a  ��  �  ��   �  ��  9  �b    S  �   �   �  8     L  �   �  8    �   X   r  �  �   �  8   � �  	�  �   �  8   O   �   �  ~   �  �  PJ  �  �  :  �   �  �    �       u  .  u  Y  �   �   �  8   8   O �   U  X �  %   J   >  !  � o   "�  � }  #�   � O  $�  �  �  �  �  8   c �    	b   �@     b@     �K  �  
K  �`-   �  ��yi b   �l �    %   :;I  $ >  $ >  I  ! I/      I  	:;  
 :;I8   :;  :;   :;I8  :;  (   :;   :;I  :;  :;  .?:;'I@
   :;I
  4 :;I
  4 :;I
  .?:;I@
      .?:;'@
  .?:;'@
   %   :;I  $ >  $ >  I  ! I/      I  	:;  
 :;I8   :;  :;   :;I8  :;  (   :;   :;I  :;  :;  .?:;I@
  4 :;I
  4 :;I
   C   �   �      /usr/lib/gcc/x86_64-redhat-linux/4.4.7/include /usr/include/bits /usr/include  edit_input.c    stddef.h   types.h   stdio.h   libio.h   string_def.h    edit_operator.h      	t@     ��u,K1��K1�ug�u���(��>�����?��w����Yg�1��g/K3�v�u$+%K0e���uh"!v�0�0���, �    �   �      /usr/lib/gcc/x86_64-redhat-linux/4.4.7/include /usr/include/bits /usr/include ..  test_edit_input.c    stddef.h   types.h   stdio.h   libio.h   string_def.h   edit_operator.h   edit_input.h     	�@     	��Z�!H� __off_t _IO_read_ptr _chain size_t _shortbuf my_edits /home/ugrad/jmol322/Stream Editor long long unsigned int the_strings GNU C 4.4.7 20120313 (Red Hat 4.4.7-11) long long int StringType Range _fileno _IO_read_end set_operator_and_string _flags _IO_buf_end _cur_column last_line set_range _old_offset old_and_new endpoints initialize_EditOp comma temp _IO_marker edit_string _IO_write_ptr _sbuf old_text short unsigned int _IO_save_base fill_edits_array _lock _flags2 _mode current_line text_type short_string print_EditOp _IO_write_end _IO_lock_t edit_range _IO_FILE _pos edit_range_type _markers initialize_empty_EditOp unsigned char operator_specifier range_identifier no_type short int RangeType _vtable_offset search_text first_line MyOp length _next __off64_t _IO_read_base _IO_save_end __pad1 __pad2 __pad3 __pad4 __pad5 _unused2 my_gets edits_file add_text _IO_backup_base long_string line_type new_text my_text _IO_write_base _IO_buf_base isitall mine test_edit_input.c /home/ugrad/jmol322/Stream Editor/test_files main �        �  -   size_t p   __off_t {   __off64_t �   FILE L  _IO_lock_t S  _IO_marker �   _IO_FILE �  long_string �  short_string   RangeType J  Range �  StringType �  EditOp     �    �  R  -   size_t p   __off_t {   __off64_t �   FILE L  _IO_lock_t S  _IO_marker �   _IO_FILE �  long_string �  short_string   RangeType J  Range �  StringType �  EditOp �  edits_array      .symtab .strtab .shstrtab .interp .note.ABI-tag .note.gnu.build-id .gnu.hash .dynsym .dynstr .gnu.version .gnu.version_r .rela.dyn .rela.plt .init .text .fini .rodata .eh_frame_hdr .eh_frame .ctors .dtors .jcr .dynamic .got .got.plt .data .bss .comment .debug_aranges .debug_pubnames .debug_info .debug_abbrev .debug_line .debug_str .debug_pubtypes                                                                                 @                                          #             @                                          1             <@     <      $                              D   ���o       `@     `                                   N             �@     �      8                          V             �@     �      �                              ^   ���o       D@     D                                  k   ���o       `@     `      0                            z             �@     �                                  �             �@     �                                �             �@     �                                    �             �@     �      �                             �             �@     �      �                             �             H@     H                                    �             X@     X      �                              �             $@     $      \                              �             �@     �      l                             �              `                                          �             `                                         �              `                                          �             (`     (      �                           �             �`     �                                   �             �`     �      p                             �             0`     0                                    �             8`     4                                    �      0               4      -                             �                      a      `                                                   �      �                                                   |      �
                             )                     P      �                             7                     �       H                             C     0               >#                                  N                     D'      �                                                   �(      ^                                                   p3      @      $   6                 	                      �;      1                                                            @                   @                   <@                   `@                   �@                   �@                   D@                   `@                  	 �@                  
 �@                   �@                   �@                   �@                   H@                   X@                   $@                   �@                    `                   `                    `                   (`                   �`                   �`                   0`                   8`                                                                                                                                                                             !                      �@                 ��                      `             *     `             8      `             E     �@             [     8`            j     @`            x     P@                 ��                �     `             �     �@             �      `             �     @             �    ��                �    ��                �     �`             �       `                   `                 (`             #     0`             .                     B    p@            R    �@             Y                      h                      |                     �    A	@     �       �                     �                     �    �@            �    H@             �                                                               7                     J    >
@     �       [    X@            j                     ~    0`             �    �
@           �   `@             �   `             �    �@     �       �   ��4`             �                     �                     �    t@     q       �   ��H`                 @     @         ��4`                 �	@     J       &    �@     v       +    �@              call_gmon_start crtstuff.c __CTOR_LIST__ __DTOR_LIST__ __JCR_LIST__ __do_global_dtors_aux completed.6349 dtor_idx.6351 frame_dummy __CTOR_END__ __FRAME_END__ __JCR_END__ __do_global_ctors_aux edit_input.c test_edit_input.c _GLOBAL_OFFSET_TABLE_ __init_array_end __init_array_start _DYNAMIC data_start printf@@GLIBC_2.2.5 __libc_csu_fini _start __gmon_start__ _Jv_RegisterClasses puts@@GLIBC_2.2.5 set_operator_and_string __isoc99_sscanf@@GLIBC_2.7 exit@@GLIBC_2.2.5 initialize_empty_EditOp _fini malloc@@GLIBC_2.2.5 fopen@@GLIBC_2.2.5 __libc_start_main@@GLIBC_2.2.5 fgets@@GLIBC_2.2.5 fill_edits_array _IO_stdin_used strlen@@GLIBC_2.2.5 __data_start print_EditOp __dso_handle __DTOR_END__ __libc_csu_init __bss_start strcpy@@GLIBC_2.2.5 strtok@@GLIBC_2.2.5 my_gets _end set_range _edata initialize_EditOp main _init                                                                                                                                                                                                                                                                                                test_files/input_file.txt                                                                           0000644 0043077 0000017 00000000507 12520052756 013746  0                                                                                                    ustar                                   user                                                                                                                                                                                                                   1 To have text prepended
2 To have text prepended
3 To have text prepended
4 Unmodified except appended text
5 Unmodified except appended text
6 Unmodified except appended text
7 This is one line for substitution
8 This is one line for substitution
9 This is to be deleted
10 This is to be deleted
11 The last line of the file
                                                                                                                                                                                         test_files/simple_o_edits.txt                                                                       0000644 0043077 0000017 00000000101 12520166212 014566  0                                                                                                    ustar                                   user                                                                                                                                                                                                                   OAdding a line to each line.
OAdding a line again to left lines.
                                                                                                                                                                                                                                                                                                                                                                                                                                                               test_files/test_edits2.txt                                                                          0000644 0043077 0000017 00000000371 12520224033 014025  0                                                                                                    ustar                                   user                                                                                                                                                                                                                   IAt the beginning -
OAdding a line to each line.
/These lines need to be deleted./d
OAdding a line again to left lines.
/Incomplete lines/A - Now complete.
s/SSS SSS SSS/RRR RRR RRR/
2,2/d
6,9/s/sss sss sss/rrr rrr rrr/
5,5/IThis is line number 5 -
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       