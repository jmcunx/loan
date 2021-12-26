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
#include <stdlib.h>
#include <j_lib2.h>
#include "loan.h"

/*** globals ***/
extern char *program_name;
extern char *buf;
extern ssize_t buf_size;

/*
 * verify_key() -- Makes sure the key used is ok
 */
int verify_key(char key, int years)
     
{
  if ((key == QUIT) || (key == RERUN))
    return(LOAN_FALSE);
  else
    if (key == DETAIL)
      if (years < MAX_YEARS)
	return(LOAN_FALSE);
  
  return(LOAN_TRUE);

} /* verify_key() */

/*
 * loan_sumr() -- display loan summary info
 */
void loan_sumr(double prin, double payment, double rate, double total_payment, 
	       int years, char page, int print_feed)
     
{
  
  int i;
  char fmt_char[17];
  
  for (i = 0; i < 17; i++)
    fmt_char[i] = LOAN_NULL;
  
  if (page)
    {
      CLEAR_SCREEN;
    }
  else
    fprintf(stdout, "\n\n");
  
  fprintf(stdout, "%s                            %s                       %s", program_name, LIT_LOAN_01, LOAN_VERSION);
  fprintf(stdout, "\n%s", LIT_L80);
  
  fprintf(stdout, "\n\n\n     %s:\n", LIT_LOAN_18);
  fmtdbl(payment, fmt_char, 17, 2, TRUE);
  fprintf(stdout, "\n         %s %s %s\n", LIT_LOAN_19, LIT_ARROW_06, fmt_char);
  
  fmtdbl(prin,fmt_char,17,2,TRUE);
  fprintf(stdout, "\n         %s %s %s", LIT_LOAN_20, LIT_ARROW_07, fmt_char);
  fmtdbl((total_payment - prin),fmt_char,17,2,TRUE);
  fprintf(stdout, "\n         %s %s %s", LIT_LOAN_21, LIT_ARROW_07, fmt_char);
  fprintf(stdout, "\n                                 %s", LIT_ULINE_17);
  fmtdbl(total_payment,fmt_char,17,2,TRUE);
  fprintf(stdout, "\n         %s %s %s", LIT_LOAN_22, LIT_ARROW_07, fmt_char);
  fprintf(stdout, "\n                                 %s\n", LIT_DLINE_17);
  for (i = 0; i < 17; i++)
    fmt_char[i] = LOAN_NULL;
  
  fmtlong(years,fmt_char,14,TRUE);
  fprintf(stdout, "\n         %s %s %s", LIT_LOAN_05, LIT_ARROW_06, fmt_char);
  fmtlong(( (long) (years * 12)),fmt_char,14,TRUE);
  fprintf(stdout, "\n         %s %s %s", LIT_LOAN_23, LIT_ARROW_03, fmt_char);
  fmtdbl(rate,fmt_char,15,2,TRUE);
  fprintf(stdout, "\n         %s %s   %s%%\n\n\n\n", LIT_LOAN_03, LIT_ARROW_08, fmt_char);
  fprintf(stdout, "\n%s", LIT_L80);
  
  if (page)
    {
      if (years >= MAX_YEARS)
	fprintf(stdout, "\n%s %s %s '%c' %s;  '%c' %s;  '%c' %s ? ", LIT_TYPE, LIT_RETURN, LIT_OR, RERUN, LIT_RERUN, PRINT, LIT_LIST, QUIT, LIT_QUIT);
      else
	fprintf(stdout, "\n%s %s %s '%c' %s;  '%c' %s;  '%c' %s;  '%c' %s ? ", LIT_TYPE, LIT_RETURN, LIT_OR, RERUN, LIT_RERUN, DETAIL, LIT_DETAIL_INFO, PRINT, LIT_LIST, QUIT, LIT_QUIT);
      fflush(stdout);
    }
  else
    {
    fprintf(stdout, "\n");
    if (print_feed)
      fprintf(stdout, "\f");
    }
  
} /* loan_sumr() */

/*
 * show_summary() -- display summary information
 */
char show_summary(double prin, double payment, double rate, 
		  int years, int page, int print_feed)

{
  char action = LOAN_NULL;
  double total_payment;
  
  total_payment = payment * years * 12;
  
  for (; (verify_key(action,years)) ;)
    {
      if (action == PRINT)
	loan_lst(prin,rate,years);
      loan_sumr(prin,payment,rate,total_payment,years,page,print_feed);
      if (page)
	{
	  if (getline(&buf, &buf_size, stdin) > 0)
	    {
	      if (j2_bye_nl(buf) == 1)
		action = buf[0];
	      else
		{
		  if (j2_bye_nl(buf) == 0)
		    action = RERUN;
		  else
		    action = LOAN_NULL;
		}
	    }
	}
      else
	action = DETAIL;
    }
  
  return(action);	
  
} /* show_summary() */
