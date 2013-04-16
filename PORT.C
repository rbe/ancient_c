#include <bensmann.h>

int i, k, port, p, pread, ms, times, pmax;
char c;

void ende()
{
  _clearscreen(0);
  printf("Ports gelesen: gesamt=%d/aktiv=%d\n\nPORTREAD -- %s\n\n",
	  port,pread,COPYRIGHT);
  cursor_on();
  exit(1);
}

void status1()
{
  _settextposition(1,1);
  cprintf("PORTREAD-PARAMETER: lesen/port: %3d mal -- max/lesen: %3d ports\
 -- warten: %3dms",times,pmax,ms);
  _settextposition(2,1);
  for (i=0; i<80; i++) cprintf("=");
  _settextposition(4,1);
  for (i=0; i<80; i++) cprintf("=");
  /*
  _settextposition(3,1);
  cprintf("scanning port %6Xh -- readed:    %5d...",port,pread);
  _settextposition(5,1);
  */
}

void status2()
{
  _settextposition(3,1);
  cprintf("scanning port %6Xh -- readed:    %5d...",port,pread);
  _settextposition(5,1);
}

void main(int argc, char *argv[])
{
  argc--; argc++;
  times=atoi(argv[1]);
  pmax=atoi(argv[2]);
  ms=atoi(argv[3]);
  _clearscreen(0);
  cursor_off();
  status1();
  while (c!=0x0d)
  {
    status2();
    k=kbhit();
    if (k!=0) ende();
    p=inp(port);
    if (p>1)
    {
      for (i=0; i<times; i++)
      {
	p=inp(port);
	cprintf("%4Xh",p);
      }
     pread++;
    }
    port++;
    if (pmax!=0) if (pread==pmax) ende();
  }
}
