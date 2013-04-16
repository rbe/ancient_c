#include <dos.h>

union REGS r;
int i;

void ems (int fkt)
{
  r.x.ax=fkt;
  int86(0x67, &r, &r);
}

void strich()
{
  for (i=0; i<80; i++) printf("=");
}

void emsinfo()
{
  printf("\tGesamt\t\t\t\t\tFrei fr Betriebssystem\n"); strich();
  ems(0x4200);
  printf("EMS: %d KByte (%d Seiten)\t\t\tEMS: %d KByte (%d Seiten)\n",
	  r.x.dx*16, r.x.dx, r.x.bx*16, r.x.bx);
  printf("\n\nWeitere Infos:\n"); strich();
  ems(0x4600); printf("EMS-Treiberversion : %d",r.h.al);
}

void main()
{
  printf("\n\t\t\tEMS-Info (c) 1994 Ralf Bensmann\n\n");
  emsinfo();
  printf("\n\n");
}

