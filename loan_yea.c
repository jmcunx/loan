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

#define CHAR_SIZE 17
#define LOAN_PRINTF_1 "      Loan: %s  Interest: %.2f%%\n"

extern char *buf;
extern ssize_t buf_size;

/*
 * loan_year_ask() -- ask user for options
 */
char loan_year_ask(int *year, int num_years, double p, double i)
     
{
  int key = 0;
  
  if (*year == 1)
    {
      if (num_years == 1)
	{
	  printf("\nType: '%c' Summary; '%c' Rerun;  '%c' List  '%c' Quit ? ",
		 SUMMARY,RERUN,PRINT,QUIT);
	}
      else
	{
	  printf("\nType: '%c' Summary;  '%c' Next Year;  ",
		 SUMMARY,NEXT_SCREEN);
	  printf("'%c' Rerun;  '%c' List;  '%c' Quit ? ",RERUN,PRINT,QUIT);
	}
    }
  else
    {
      if (*year == num_years)
	{
	  printf("\nType: '%c' Summary;  '%c' Prev Year;  '%c' Rerun;  ",
		 SUMMARY,PREV_SCREEN,RERUN);
	  printf("'%c' List;  '%c' Quit ? ",PRINT,QUIT);
	}
      else
	{
	  printf("\nType: '%c' Summary; '%c' Next; '%c' Previous; ",
		 SUMMARY,NEXT_SCREEN,PREV_SCREEN);
	  printf("'%c' Rerun; '%c' List; '%c' Quit ? ",RERUN,PRINT,QUIT);
	}
    }
  
  fflush(stdout);
  
  if (getline(&buf, &buf_size, stdin) > -1)
    {
      if (j2_bye_nl(buf) == 1)
	key = buf[0];
      else
	{
	  if (j2_bye_nl(buf) == 0)
	    {
	    if (*year == num_years)
	      key = RERUN;
	    else
	      key = NEXT_SCREEN;
	    }
	  else
	    key = 0;
	}
    }
  
  if ((key == SUMMARY) || (key == RERUN) || (key == QUIT))
    return (key);
  else
    if ((key == NEXT_SCREEN) && (*year < num_years))
      (*year)++;
    else
      if ((key == PREV_SCREEN) && (*year > 1))
	(*year)--;
      else
	{
	  if (key == PRINT)
	    loan_lst(p,i,num_years);
	  key = DETAIL;
	}
  
  return(key);
  
} /* loan_year_ask() */

/*
 * print_year() -- displays 1 year on the screen
 */
char print_year(int *year, int num_years, 
		double balance, double prin, double rate, double payment, 
		double payment_total, int page, int print_feed)
{
  
  double pay_int = (double) 0.0;
  double pay_prin;
  int    per_left;
  double pay_prin_total = 0.0, pay_int_total = 0.0;
  char fmt_char1[CHAR_SIZE], fmt_char2[CHAR_SIZE];
  char fmt_char3[CHAR_SIZE];
  char fmt_pay[CHAR_SIZE];
  int i, j;
  
  fmtdbl(payment,fmt_pay,CHAR_SIZE,2,TRUE);
  fmtdbl(prin,fmt_char1,CHAR_SIZE,2,TRUE);
  j2_justleft(fmt_char1);
  
  i = ((*year) * 12) - 12;
  
  if (page)
    {
      CLEAR_SCREEN;
    }
  else
    printf("\n\n");
  
  printf("loan                            Loan Information         ");
  printf("              %s",LOAN_VERSION);
  printf("\n-----------------------------------------------------------");
  printf("--------------------");
  printf("\nYear %d of %d",*year,num_years);
  printf(LOAN_PRINTF_1,fmt_char1,rate);
  printf("\n                 Monthly        Ammt to          Ammt to   ");
  printf("       Balance");
  printf("\n Month           Payment        Interest        Principal  ");
  printf("       of Loan");
  printf("\n ----- ----------------- ---------------- ---------------- ");
  printf("----------------\n");
  
  per_left = (num_years * 12) - ((*year - 1)  * 12);
  
  for (j = 0; j < 12; i++, j++, per_left--)
    {
      pay_prin = get_per(rate,per_left) * payment;
      balance -= pay_prin;
      pay_int  = payment - pay_prin;
      pay_prin_total += pay_prin;
      pay_int_total  += pay_int;
      fmtdbl(pay_int,fmt_char1,CHAR_SIZE,2,TRUE);
      fmtdbl(pay_prin,fmt_char2,CHAR_SIZE,2,TRUE);
      fmtdbl(balance,fmt_char3,CHAR_SIZE,2,TRUE);
      printf("   %2d   %s %s %s %s\n",(j + 1),fmt_pay,fmt_char1,
	     fmt_char2,fmt_char3);
    }
  
  fmtdbl(pay_int_total,fmt_char1,CHAR_SIZE,2,TRUE);
  fmtdbl(pay_prin_total,fmt_char2,CHAR_SIZE,2,TRUE);
  fmtdbl(payment_total,fmt_pay,CHAR_SIZE,2,TRUE);
  
  printf(" ----- ----------------- ---------------- ---------------- ");
  printf("----------------");   
  printf("\n Total  %s %s %s\n",fmt_pay,fmt_char1,fmt_char2); 
  printf("       ================= ================ ================\n");
  printf("-----------------------------------------------------------");
  printf("-------------------");
  
  if (page)
    return(loan_year_ask(year,num_years,prin,rate));

  (*year)++;
  printf("\n");
  if (print_feed)
    printf("\f");

  return(0);
  
}  /* print_year */
