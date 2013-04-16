#include <dos.h>

void main()
{
  r.x.ax=0x0400;
  int86(0x31, &r, &r);
  printf("Prozessor: 80%d86", r.h.cl);
}

