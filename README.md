## loan -- Show Loan Details

loan(1) will create a report showing Loan Details
over the length of the loan.

This requires [j\_lib2](https://github.com/jmcunx/j_lib2) to build.

[GNU automake](https://en.wikipedia.org/wiki/Automake)
only confuses me, but this seems to be good enough for me.

**To compile:**
* Set "DESTDIR" for where you want this to install.  Examples:
  * setenv DESTDIR /usr/local
  * export DESTDIR=/usr/local
* this assumes [j\_lib2](https://github.com/jmcunx/j_lib2)
  is also installed under "DESTDIR".
* Edit Makefile, uncomment areas associated to the OS
  you want to compile on.
  OS Sections:
  * Linux 64 bit (default)
  * Linux 32 bit
  * BSD 64 bit
  * BSD 32 bit
  * AIX

_To uninstall_, execute
"make uninstall"
from the source directory
