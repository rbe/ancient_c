#include <graph.h>

int i;
char c;

void main()
{
  _clearscreen(0);
  for (i=0; i<8; i++)
  {
    _settextcolor(i);
    _outtext("FARBTEST!\n");
  }
  _settextwindow(1,10,25,80);
  for (i=8; i<16; i++)
  {
    _settextcolor(i);
    _outtext("FARBTEST!\n");
  }
}

