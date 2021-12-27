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

#ifndef LOAN_H

#define LOAN_VERSION "     4.2"
#define LOAN_H "INCLUDED"

/*** loan specific messages ***/
#define LIT_LOAN_01 "Loan Information"
#define LIT_LOAN_02 "Optional command line usage:"
#define LIT_LOAN_03 "Interest Rate"
#define LIT_LOAN_04 "Amount of Loan (Principal)"
#define LIT_LOAN_05 "Number of Years"
#define LIT_LOAN_06 "NOTEs: 1. Arguments are optional, but may be specified in any order."
#define LIT_LOAN_07 "2. Optional argument"
#define LIT_LOAN_08 "will print a form feed,"
#define LIT_LOAN_09 "this allows redirection to a printer."
#define LIT_LOAN_10 "3. If Years are greater than"
#define LIT_LOAN_11 "Detail info CANNOT be generated"
#define LIT_LOAN_12 "4. If amounts entered are VERY large some results may be incorrect"
#define LIT_LOAN_13 "5. Enter 0 in any of the fields to exit"
#define LIT_LOAN_14 "INVALID Device or File Name, please Enter the device (Ex: printer)"
#define LIT_LOAN_15 "or File you want to send the listing to."
#define LIT_LOAN_16 "List on device/file"
#define LIT_LOAN_17 "****** Processing command *******"
#define LIT_LOAN_18 "Summary Information"
#define LIT_LOAN_19 "Monthly Payment"
#define LIT_LOAN_20 "Amount of Loan"
#define LIT_LOAN_21 "Total Interest"
#define LIT_LOAN_22 "Total Payments"
#define LIT_LOAN_23 "Number of Payments"

/*** common messages ***/
#define LIT_ARROW_03      "-->"
#define LIT_ARROW_04      "--->"
#define LIT_ARROW_06      "----->"
#define LIT_ARROW_07      "------>"
#define LIT_ARROW_08      "------->"
#define LIT_ARROW_09      "-------->"
#define LIT_ULINE_17      "-----------------"
#define LIT_L80           "--------------------------------------------------------------------------------"
#define LIT_DLINE_17      "================"

#define LIT_DETAIL_INFO   "Detail Info"

#define LIT_LOAN          "Loan"
#define LIT_INTEREST      "Interst"
#define LIT_YEARS         "Years"
#define LIT_COMMAND       "Command"
#define LIT_YN            "(y/n)"
#define LIT_TYPE          "Type"
#define LIT_QUIT          "Quit"
#define LIT_LIST          "List"
#define LIT_RERUN         "Rerun"
#define LIT_RETURN        "<RETURN>"
#define LIT_OR            "or"

#define MSG_LOAN_E01 "ERROR: Principal must be greater than 0"
#define MSG_LOAN_E02 "ERROR: Interest Rate must be greater than 0"
#define MSG_LOAN_E03 "ERROR: # of Years must be greater than 0"

#define MSG_LOAN_PI1 "What is the Amount of the loan ? "
#define MSG_LOAN_PI2 "What is the Amount of the loan [%.2f%%] ? "
#define MSG_LOAN_QI1 "What is the Interest Rate ? "
#define MSG_LOAN_QI2 "What is the Interest Rate [%.2f%%] ? "
#define MSG_LOAN_YI1 "How many Years is the loan for ? "
#define MSG_LOAN_YI2 "How many Years is the loan for [%i] ? "

/*** Other defaults ***/
#define LOAN_TRUE 1
#define LOAN_FALSE 0
#define LOAN_NULL 0

#define RERUN 'r'
#define SUMMARY 's'
#define DETAIL 'd'
#define PRINT 'l'
#define QUIT 'q'
#define NEXT_SCREEN 'n'
#define PREV_SCREEN 'p'
#define LOAN_PROGRAM_NAME "loan"
#define LOAN_INTERACTIVE 0
#define LOAN_BACKGROUND 1

#define MAX_YEARS 300

#define ARG_INTEREST       'i'
#define ARG_YEARS          'y'
#define ARG_PRINCIPAL      'p'
#define ARG_VERSION        'V'  /* Show Version Information       */
#define ARG_HELP           'h'  /* Show Help                      */
#define ARG_HELP_LONG      'H'  /* Show Long Help                 */
#define ARG_PRINT_FEED     'P'  /* Print a form feed              */

#define LIT_REV      "Revision"
#define LIT_INFO_01  "\tBuild: %s %s\n"
#define LIT_INFO_02  "j_lib2 built:"
#define USG_MSG_USAGE      "usage:\t%s [OPTIONS] [FILES ...]\n"
#define USG_MSG_OPTIONS    "Options\n"

#define MSG_HELP_11        "Calculate loan details, if Year, Principal and Interest"
#define MSG_HELP_12        "arguments are not specified will execute in interactive"
#define MSG_HELP_13        "mode."

#define USG_MSG_ARG_INTEREST    "\t%c%c num\t\t: Interest Rate\n"
#define USG_MSG_ARG_PRINCIPAL   "\t%c%c num\t\t: Principal Amount\n"
#define USG_MSG_ARG_YEARS       "\t%c%c num\t\t: Number of Years\n"
#define USG_MSG_ARG_PRINT_FEED  "\t%c%c\t\t: Print a form feed between pages\n"
#define USG_MSG_ARG_VERSION     "\t%c%c\t\t: Show revision information and exit\n"
#define USG_MSG_ARG_HELP        "\t%c%c\t\t: Show brief help and exit\n"
#define USG_MSG_ARG_HELP_LONG   "\t%c%c\t\t: Show detail help and exit\n"

#define MSG_ERR_00   "Try '%s %c%c' for more information\n"
#define MSG_ERR_67    "ERROR E67: Terminal not found for stdin OR stdout\n"
#define MSG_ERR_68    "ERROR E68: inconsistent arguments for '%c%c', '%c%c' and '%c%c'\n"

#if _MSDOS
#  define SWITCH_CHAR '/'
#  define LOAN_SCANF_DOUBLE "%lf"
#  define CLEAR_SCREEN system("cls")
#  define DEV_EX "Output to Device (Ex: PRN or LPT1 or LPT2...) OR File Name ? "
#  define BACKGROUND_STR " "
#  define DIR_SEPR '\\'
#else
   double power(); /* function prototype, remove when I can get pow() to work */
#  define SWITCH_CHAR '-'
#  define DEV_EX "Output to Device (Ex: /dev/lp or /dev/lp?) OR File Name ? "
#  define BACKGROUND_STR " &"
#  define DIR_SEPR '/'
#  if INIX
#    define LOAN_SCANF_DOUBLE "%F"
#    define CLEAR_SCREEN system("tput clear")
#  else
#    define LOAN_SCANF_DOUBLE "%lf"
#    define CLEAR_SCREEN system("clear")
#  endif
#endif

/* prototypes */
char print_year(int *, int, double, double, double, double, double, int, int);
char loan_year_ask(int *, int, double, double);
void load_table(double *, double, double, double, int, int);
char show_summary(double, double, double, int, int, int);
void get_loan(double *, double *, int *);
void loan_lst(double, double, int);
char print_year(int *, int, double, double, double, double, double, int, int);
double get_per(double, int);
double conv_dbl(char *);
void process_arg(int, char **, double *, double *, int *, int *);
int fmtdbl(double, char *, int, int, int);
int fmtlong(long, char *, int, int);

#endif
