/*
 * Copyright (c) 1990 ... 2023 2024
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
#include <math.h>

#ifdef HAVE_JLIB
#include <j_lib2.h>
#include <j_lib2m.h>
#endif

#include "loan.h"

/*** globals ***/
extern char *buf;
extern size_t buf_size;
extern char *program_name;

/*
 * get_loan() -- Gets loan information
 */
void get_loan(double *p, double *i, int *y)

{

  CLEAR_SCREEN;
  
  fprintf(stdout, "%s                            %s                       %s", program_name, LIT_LOAN_01, LOAN_VERSION);
  fprintf(stdout, "\n%s", LIT_L80);
  
  fprintf(stdout, "\n\n%s", LIT_LOAN_02);
  fprintf(stdout, "\n\t%s [%c%c P %c%c I %c%c Y]", program_name, SWITCH_CHAR, ARG_PRINCIPAL,SWITCH_CHAR, ARG_INTEREST,SWITCH_CHAR, ARG_YEARS);
  fprintf(stdout, "\n\t     P = %s", LIT_LOAN_04);
  fprintf(stdout, "\n\t     I = %s", LIT_LOAN_03);
  fprintf(stdout, "\n\t     Y = %s", LIT_LOAN_05);
  fprintf(stdout, "\n%s", LIT_LOAN_06);
  fprintf(stdout, "\n       %s (%c%c) %s", LIT_LOAN_07, SWITCH_CHAR, ARG_PRINT_FEED, LIT_LOAN_08);
  fprintf(stdout, "\n          %s", LIT_LOAN_09);
  fprintf(stdout, "\n       %s %d, ", LIT_LOAN_10, (MAX_YEARS - 1));
  fprintf(stdout, "%s", LIT_LOAN_11); 
  fprintf(stdout, "\n       %s", LIT_LOAN_12);
  fprintf(stdout, "\n       %s\n\n\n", LIT_LOAN_13);

  if ((*p) > 0)
    fprintf(stdout, MSG_LOAN_PI2, (*p));
  else
    fprintf(stdout, MSG_LOAN_PI1);
  fflush(stdout); /* make sure message goes to screen */
  if (j2_getline(&buf, &buf_size, stdin) > -1)
    {
      if (j2_bye_nl(buf) > 0)
	(*p) = conv_dbl(buf);
    }
  else
    (*p) = (double) 0;
  if ((*p) <= 0)
    {
      fprintf(stdout, "\n%s\n", MSG_LOAN_E01);
      fprintf(stderr, MSG_ERR_00, program_name, SWITCH_CHAR, ARG_HELP);
      exit(EXIT_FAILURE);
    }
 
  fprintf(stdout, "\n");
  if ((*i) == (int) 0) 
    fprintf(stdout, MSG_LOAN_QI1);
  else
    fprintf(stdout, MSG_LOAN_QI2, (*i));
  
  if (j2_getline(&buf, &buf_size, stdin) > -1)
    {
      if (j2_bye_nl(buf) > 0)
	(*i) = conv_dbl(buf);
    }
  else
    (*i) = (double) 0;
  if ((*i) <= (double) 0)
    {
      fprintf(stdout, "\n%s\n", MSG_LOAN_E02);
      fprintf(stderr, MSG_ERR_00, program_name, SWITCH_CHAR, ARG_HELP);
      exit(EXIT_FAILURE);
    }
 
  fprintf(stdout, "\n");
  if ((*y) > 0) 
    fprintf(stdout, MSG_LOAN_YI2, (*y));
  else
    fprintf(stdout, MSG_LOAN_YI1);
  fflush(stdout); /* make sure message goes to screen */
  if (j2_getline(&buf, &buf_size, stdin) > -1)
    {
      if (j2_bye_nl(buf) > 0)
	(*y) = atoi(buf);
    }
  else
    (*y) = (int) 0;
  
  if ((*y) <= (int) 0)
    {
      fprintf(stdout, "\n%s\n", MSG_LOAN_E03);
      fprintf(stderr, MSG_ERR_00, program_name, SWITCH_CHAR, ARG_HELP);
      exit(EXIT_FAILURE);
    }

}   /* get_loan() */
