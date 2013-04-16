#include <errno.h>

void error (char *ERROR_STRING, int ERROR_NUMBER)
{
  if (strcmp(ERROR_STRING,"")!=0)
    printf("Programfehler %2d: %s\n(systemerror  %2d: %s)",ERROR_NUMBER,
	    ERROR_STRING,errno,sys_errlist[errno]);
  else printf("systemerror  %2d: %s",errno,sys_errlist[errno]);
  where=_gettextposition();
  taste(where.row+2,1);
}

