/*
 * Copyright (c) 1990 ... 2021 2022
 *     John McCue <jmccue@jmcunx.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#ifndef _MSDOS
#include <sys/param.h>
#endif
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <j_lib2.h>
#include "loan.h"

#define CHAR_SIZE 17
#define MAX_CMD 2048

/*** globals ***/
char *loan_lst_c="$Id: loan_lst.c,v 3.1 2021/12/26 21:18:12 jmccue Exp $";
extern char *exec_program;
extern char *program_name;

/*
 * get_str() -- Get a string from stdin
 */
void get_str(FILE *question, char *title, char *string, int size)

{
  int user_size;
  char buffer[128];

  if (title != (char *) NULL)
    fprintf(question,"%s [%s] ",title,string);
  fflush(question);

  fgets(buffer, size, stdin);

  user_size = strlen(buffer) - 1;

  if (buffer[user_size] == '\n')
    {
      buffer[user_size] = JLIB2_CHAR_NULL;
      buffer[size - 1] = JLIB2_CHAR_NULL;
      if (user_size > 0)
	strcpy(string,buffer);
    }
 else
   {
     buffer[size - 1] = JLIB2_CHAR_NULL;
     if (user_size > 0)
       strcpy(string,buffer);
     for ( ; buffer[user_size] != '\n'; ) /** empties stdin **/
       {
	 fgets(buffer, size, stdin);
	 user_size = strlen(buffer) - 1;
       }
   }

  /** remove trailing spaces **/
  j2_rtw(string);

} /* get_str() */

/*
 * dev_ok()
 */
int dev_ok(char *x)

{
  if (isalnum( ((int) x[0]) ))
    return(LOAN_TRUE);

  switch (x[0])
    {
      case DIR_SEPR:
      case '$':
      case '@':
      case '~':
	return(LOAN_TRUE);
    }

  x[0] = LOAN_TRUE;
  return(LOAN_FALSE);

} /* dev_ok() */

/*
 * loan_lst()
 */
void loan_lst(double p, double i, int y)

{

  char fmt_p[CHAR_SIZE];
  char fmt_i[CHAR_SIZE];
  char route_device[MAX_CMD], y_n;
  char command[MAX_CMD];
  
#if MSDOS
  itoa(y,route_device,10);
#else
  fmtlong( ((long) y), route_device, 7, FALSE);
  j2_justleft(route_device);
#endif
 
  j2_clr_str(command, JLIB2_CHAR_NULL, MAX_CMD);
  j2_clr_str(fmt_p, JLIB2_CHAR_NULL, CHAR_SIZE);
  j2_clr_str(fmt_i, JLIB2_CHAR_NULL, CHAR_SIZE);
  fmtdbl(p,fmt_p,CHAR_SIZE,2,TRUE);
  fmtdbl(i,fmt_i,CHAR_SIZE,2,TRUE);
  j2_justleft(fmt_p);
  j2_justleft(fmt_i);
  sprintf(command, "%s %c%c %d %c%c %s %c%c %s > ",
          exec_program,  
          SWITCH_CHAR, ARG_YEARS, y,
          SWITCH_CHAR, ARG_PRINCIPAL, fmt_p,
          SWITCH_CHAR, ARG_INTEREST, fmt_i);

  j2_clr_str(route_device, JLIB2_CHAR_NULL, MAX_CMD);
  j2_clr_str(fmt_p, JLIB2_CHAR_NULL, CHAR_SIZE);
  j2_clr_str(fmt_i, JLIB2_CHAR_NULL, CHAR_SIZE);
  
  do
    {
      CLEAR_SCREEN;
      fprintf(stdout, "%s                            %s                       %s", program_name, LIT_LOAN_01, LOAN_VERSION);
      fprintf(stdout, "\n%s", LIT_L80);
      fmtdbl(p,fmt_p,CHAR_SIZE,2,TRUE);
      fmtdbl(i,fmt_i,CHAR_SIZE,2,TRUE);
      j2_justleft(fmt_i);
      j2_justleft(fmt_p);
      fprintf(stdout, "\n");
      fprintf(stdout, "\n%7s:  %s", LIT_LOAN,     fmt_p);
      fprintf(stdout, "\n%7s:  %s", LIT_INTEREST, fmt_i);
      fprintf(stdout, "\n%7s:  %-d", LIT_YEARS,   y);
      fprintf(stdout, "\n\n");
      if (route_device[0])
	{
	  fprintf(stdout, "\n%s",LIT_LOAN_14);
	  fprintf(stdout, "\n%s\n\n",LIT_LOAN_15);
	}
      fprintf(stdout, DEV_EX);
      fflush(stdout);
      get_str(stdin, (char *) NULL, route_device, 120);
    }
  while (!dev_ok(route_device));
  
  strcat(command,route_device);
  strcat(command,BACKGROUND_STR);
  
  for (y_n = ' '; ((y_n != 'n') && (y_n != 'y')); )
    {
      fprintf(stdout, "\n%s:\n%s", LIT_COMMAND, command);
      fprintf(stdout, "\n\n%s '%s' %s", LIT_LOAN_16, route_device, LIT_YN);
      fflush(stdout);
      y_n = getchar();
      while(getchar() != '\n')  /* clear buffer */
	;
    }
  
  if (y_n == 'y')
    {
      CLEAR_SCREEN;
      fprintf(stdout, "\n\n%s\n\n%s\n\n", LIT_LOAN_17, command);
      fflush(stdout);
      system(command);
    }
} /* loan_lst() */
