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
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <libgen.h>

#ifndef INIX
#include <stdlib.h>
#ifdef UNIX
#include <unistd.h>
#endif
#endif

#ifdef HAVE_JLIB
#include <j_lib2.h>
#include <j_lib2m.h>
#endif

#include "loan.h"

/*** globals ***/
extern char *program_name;
extern char *exec_program;

/*
 * show_brief_help()
 */
int show_brief_help(FILE *fp, char *pname)

{

  fprintf(fp, USG_MSG_USAGE, pname);
  fprintf(fp, "\t%s\n", MSG_HELP_11);
  fprintf(fp, "\t%s\n", MSG_HELP_12);
  fprintf(fp, "\t%s\n", MSG_HELP_13);
  fprintf(fp, USG_MSG_OPTIONS);

  fprintf(fp, USG_MSG_ARG_HELP,       SWITCH_CHAR, ARG_HELP);
  fprintf(fp, USG_MSG_ARG_INTEREST,   SWITCH_CHAR, ARG_INTEREST);
  fprintf(fp, USG_MSG_ARG_PRINT_FEED, SWITCH_CHAR, ARG_PRINT_FEED);
  fprintf(fp, USG_MSG_ARG_PRINCIPAL,  SWITCH_CHAR, ARG_PRINCIPAL);
  fprintf(fp, USG_MSG_ARG_VERSION,    SWITCH_CHAR, ARG_VERSION);
  fprintf(fp, USG_MSG_ARG_YEARS,      SWITCH_CHAR, ARG_YEARS);

  return(EXIT_FAILURE);

}  /* show_brief_help() */

/*
 * show_rev()
 */
int show_rev(FILE *fp, char *pname)

{

  fprintf(fp,"%s %s:\n", pname, LIT_REV);

#ifdef J_LIB2_H
  fprintf(fp, "\t%s %s\n", LIT_INFO_02, j2_get_build());
#endif

#ifdef OSTYPE
  fprintf(fp,"\t%s\n",OSTYPE);
#endif  /* OSTYPE  */
  fprintf(fp, LIT_INFO_01, __DATE__, __TIME__);

  return(EXIT_FAILURE);

}  /* show_rev() */

/*
 * conv_dbl() -- convert a string to a double
 */
double conv_dbl(char *s)
{
  char *c;
  double a = (double) 0;

  if (s == (char *) NULL)
    return(a);

  c = strdup(s);
  j2_bye_char(c, ',');
  a = atof(c);

  free(c);
  return(a);

} /* conv_dbl() */

/*
 * process_arg() -- process arguments
 */
void process_arg(int argc, char **argv, 
                 double *prin, 
                 double *rate, 
                 int *years, 
                 int *print_feed)

{
  char ckarg[80];
  int opt;
  int ok;
  int display_help = (int) FALSE;
  int display_rev  = (int) FALSE;

  (*prin)       = (double) 0;
  (*rate)       = (double) 0;
  (*years)      = (int) 0;
  (*print_feed) = (int) FALSE;

  program_name = j2_get_prgname(argv[0], LOAN_PROGRAM_NAME);

  sprintf(ckarg, "%c%c%c%c:%c:%c:", 
		 ARG_HELP, ARG_VERSION, ARG_PRINT_FEED,
		 ARG_INTEREST, ARG_YEARS, ARG_PRINCIPAL);

  while ((opt = getopt(argc, argv, ckarg)) != -1)
    {
      switch (opt)
	{
	  case ARG_PRINT_FEED:
	    (*print_feed) = (int) TRUE;
	    break;
	  case ARG_HELP:
	    display_help = (int) TRUE;
	    break;
	  case ARG_VERSION:
	    display_rev = (int) TRUE;
	    break;
	  case ARG_INTEREST:
	    *rate = conv_dbl(optarg);
	    break;
	  case ARG_YEARS:
	    *years = atoi(optarg);
	    break;
	  case ARG_PRINCIPAL:
	    *prin = conv_dbl(optarg);
	    break;
	  default:
	    fprintf(stderr, MSG_ERR_00, program_name, SWITCH_CHAR, ARG_HELP);
	    exit(EXIT_FAILURE);
	    break;
	}
    }

  /*** show help/rev and exit ? ***/
  ok = EXIT_SUCCESS;
  if (display_help)
    ok = show_brief_help(stdout, program_name);
  if (display_rev)
    ok = show_rev(stdout, program_name);
  if (ok != EXIT_SUCCESS)
    exit(ok);

  /*** validate arguments ***/
  ok = 0;
  if ((*rate) != 0)
    ok++;
  if ((*years) != (double) 0)
    ok++;
  if ((*prin) != (double) 0)
    ok++;
  if ((ok > 0) && (ok < 3))
    {
      fprintf(stderr, MSG_ERR_68, SWITCH_CHAR, ARG_INTEREST, SWITCH_CHAR, ARG_PRINCIPAL, SWITCH_CHAR, ARG_YEARS);
      fprintf(stderr, MSG_ERR_00, program_name, SWITCH_CHAR, ARG_HELP);
      exit(EXIT_FAILURE);
    }
  /* ok == 0 or 3, 0 means use interactive, 3 = arguments supplied */
  /* see if we have a terminal for interactive                     */
  if ( ok == 0 )
    {
      int o;
      int i;
      i = isatty(fileno(stdin));
      o = isatty(fileno(stdout));
      if ((i == 0) || (o == 0))
	{
	  fprintf(stderr, MSG_ERR_67);
	  fprintf(stderr, MSG_ERR_00, program_name, SWITCH_CHAR, ARG_HELP);
	  exit(EXIT_FAILURE);
	}
    }

} /* process_arg() */
