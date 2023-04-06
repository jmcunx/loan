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
#include <math.h>
#include <string.h>
#include <stdlib.h>

#ifdef HAVE_JLIB
#include <j_lib2.h>
#include <j_lib2m.h>
#endif

#include "loan.h"

/*** globals ***/
char *buf = (char *) NULL;
size_t buf_size = 0;
char *exec_program = (char *) NULL;
char *program_name = (char *) NULL;
int run_mode = LOAN_BACKGROUND;

/*
 * rnddbl()
 */
double rnddbl(double a, int decimal)

{
  char n[83];
  char fmt[10];

  sprintf(fmt,"%%80.%df",decimal);
  sprintf(n,fmt,a);

  j2_justleft(n);
  
  return( (atof(n)) );

} /* rnddbl() */

/*
 * is_decimal()
 */
int is_decimal(char *s, char dec)

{
  for ( ; (*s) != JLIB2_CHAR_NULL; s++)
    {
      if ((*s) == dec)
        return((int) TRUE);
    }

  return((int) FALSE);

} /* is_decimal() */

/*
 * fmt_comma()
 */
void fmt_comma(char *s, char *tmp, int size)

{
  int i, j, k;
  int decimal_found;


  j2_justleft(tmp);
  strcpy(s, tmp);

  j2_clr_str(tmp, (char) ' ', size);
  decimal_found = is_decimal(s, DECIMAL_POINT);

  k = 0;
  i = strlen(s) - 1;
  j = size - 1;

  if ( decimal_found )
    {
      k = 1;
      for ( ; i >= 0; i--, j--)
	{
	  tmp[j] = s[i];
	  if (s[i] == DECIMAL_POINT)
	    {
	      --i;
	      --j;
	      break;
            }
	}
    }

  for ( ; i >= 0; i--, j--, k++)
    {
      if (j < 0)
        {
	  strcpy(tmp,s);
	  return;  /* ran out of space for commas, default */
        }
      if (k == 4)
	{
	  tmp[j] = THOUSAND_SEP;
	  j--;
	  k = 1;
	}
      if (j >= 0)
        tmp[j] = s[i];
    }

  if (tmp[0] == THOUSAND_SEP)
    strcpy(tmp,s);

} /* fmt_comma() */

/*
 * fmtlong()
 */
int fmtlong(long a, char *s, int size, int thou)

{
  char *tmp;
  char *t;
  char fmt[10];
  int i;

  --size;

  if (a < ((long) 0) )
    {
      a = labs(a);
      sprintf(fmt,"%%%dld-",size - 1);
    }
  else
    sprintf(fmt,"%%%dld ",size - 1);
  
  /*** make sure number within specified range */
  j2_clr_str(s, '9', size);
  if ( a > atol(s) )
    {
      j2_clr_str(s, '*', (++size));
      return((int) TRUE);
    }
  j2_clr_str(s, ' ', size);

  /*** no format it, in range ***/
  tmp = (char *) malloc( (sizeof(char) * size * 2) );
  if (tmp == (char *) NULL)
    return((int) FALSE);
  strcpy(tmp,s);

  sprintf(tmp, fmt, a);

  if ( thou == (int) TRUE )
    fmt_comma(s, tmp, size );

  i = size - strlen(tmp); 
  if (i < 0)
    t = s;
  else
   t = &(s[i]);
  
  j2_clr_str(s, ' ', size);
  strcpy(t, tmp);

  free(tmp);
  return((int) TRUE);

} /* fmtlong() */

/*
 * fmtdbl()
 */
int fmtdbl(double a, char *s, int size, int dec, int thou)

{
  char *tmp, *t;
  char fmt[10];
  int i;

  --size;

  if (a < ((double) 0) )
    {
      a = fabs(a);
      sprintf(fmt,"%%%d.%df-",size - 1, dec);
    }
  else
    sprintf(fmt,"%%%d.%df ",size - 1, dec);

  /*** make sure number within specified range */
  j2_clr_str(s, '9', size);
  s[(size - dec - 2)] = JLIB2_CHAR_NULL;
  if ( a > atof(s) )
    {
      j2_clr_str(s, '*', (size + 1));
      return((int) TRUE);
    }
  j2_clr_str(s, ' ', size);

  /*** now format it, in range ***/
  tmp = (char *) malloc( (sizeof(char) * size * 2) );
  if (tmp == (char *) NULL)
    return((int) FALSE);
  strcpy(tmp,s);

  sprintf(tmp, fmt, a);
  if (atof(tmp) == (double) 0)
    { /* get rid of negative zero issue */
      sprintf(fmt,"%%%d.%df ",size - 1, dec);
      sprintf(tmp, fmt, a);
    }

  if ( thou == (int) TRUE )
    fmt_comma(s, tmp, size);

  i = size - strlen(tmp); 
  if (i < 0)
    t = s;
  else
    t = &(s[i]);
  
  j2_clr_str(s, ' ', size);
  strcpy(t, tmp);

  free(tmp);
  return((int) TRUE);

} /* fmtdbl() */

/*
 * power() -- raise a double to a power
 */
double power(double x, int i)
{
  double a = 1.0;

  for (; i > 0; i--)
    a *= x;

  return(a);

} /* power() */

/*
 * get_per() -- gets percentage of payment that relates to principal
 */
double get_per(double rate, int periods)

{
  return((1 / power(((rate / 1200) + 1),periods)));
  /* pow() does not seem to work, use my own routine */

} /* get_per() */

/*
 * comp() -- calculates the monthly period
 */
double comp(double rate, double prin, int years)

{
  return(prin * (rate / 1200) / (1 - (get_per(rate, years * 12))));
} /* comp() */

/*
 * load_table() -- loads balance table
 */
void load_table(double *tbl_balance, double prin, double payment, 
	        double rate, int years, int page)
     
{
  
  int i, j, process_mm = 1, process_yy = 1;
  double pay_prin;
  
  tbl_balance[0]  = prin;
  
  if (page) /* This if is NOT necessary, but I felt like seeing it */
    {
      CLEAR_SCREEN;
      printf("\n\n\n\n\n\n\n\n\n\n\n                        Generating ");
      printf("Detail for Year:  %d",process_yy);
      fflush(stdout);
    } 
  
  for (j = 1, i = (years * 12); i > 0; i--, process_mm++)
    {
      if (process_mm > 12)
	{
	  process_mm = 1;
	  process_yy++;
	  if (page) /* This if is not needed, I felt like seeing if */
	    {
	      printf("\b\b\b");
	      printf("%3d",process_yy);
	      fflush(stdout);
	    }
	}
      pay_prin = get_per(rate,i) * payment;
      prin -= pay_prin;
      if (process_mm == 12)
	{
	  tbl_balance[j]	= prin;
	  j++;
	}
    }
  
  j--;
  
  if ((tbl_balance[j] < 0.01) && (tbl_balance[j] > -0.01))
    tbl_balance[j] = 0.0;
  
} /* load_table() */

/*
 * process_loan -- show detail information
 */
int process_loan(double *prin, double *rate, int *years, int print_feed)

{
  double payment, payment_total;
  int year_process = 1, i = 2;
  int page = LOAN_FALSE;
  char key;
  double tbl_balance[MAX_YEARS];
  
  if (run_mode == LOAN_INTERACTIVE)
    {
      page = LOAN_TRUE;
      get_loan(prin, rate, years);
    }
  
  payment = comp((*rate),(*prin),(*years));
  payment_total = rnddbl(payment,2) * 12;
  
  key = show_summary((*prin),payment,(*rate),(*years),page, print_feed);
  
  if ((key == DETAIL) && ((*years) < MAX_YEARS))
    {
      i = 0;
      load_table(tbl_balance, (*prin), payment, (*rate), (*years), page);
    }
  
  if (page)
    {
      for (; i == 0;)
	{
	  if ((*years) < MAX_YEARS) /* prevent table overflow */
	    key = print_year(&year_process,(*years),
			     tbl_balance[year_process - 1],
			     (*prin), (*rate), payment, payment_total,page, print_feed);
	  else
	    key = SUMMARY;
	  if (key == SUMMARY)
	    {
	      key = show_summary((*prin),payment,(*rate),(*years),page, print_feed);
	      if (key != DETAIL)
		i = 1;
	    }
	  else
	    if (!((key == DETAIL)      || (key == NEXT_SCREEN) ||
		  (key == PREV_SCREEN)))
	      i = 1;
	}
    }
  else
    {
      for (; year_process <= (*years); )
	print_year(&year_process,(*years),tbl_balance[year_process - 1],
		   (*prin), (*rate), payment, payment_total,page, print_feed);
      key = QUIT;
      printf("\n");
    }
  
  return (key);
  
} /* process_loan() */

/*
 * main()
 */
int main(int argc, char **argv)

{
  int i;
  int years = 0;
  double prin = 0; 
  double rate = 0;
  int print_feed = (int) FALSE;
  
  exec_program = argv[0];

  process_arg(argc, argv, &prin, &rate, &years, &print_feed);
  
  if ((prin <= 0) || (rate <= 0) || (years <= 0))
    {
      run_mode = LOAN_INTERACTIVE;
      prin = (double) 0;
      rate = (double) 0;
      years = (int) 0;
    }
  
  for (i = RERUN; i == RERUN; )
    i = process_loan(&prin, &rate, &years, print_feed);
  
  if (program_name != (char *) NULL)
    free(program_name);
  exit(EXIT_SUCCESS);

} /* main() */
