#include <def.c>
#include <mouse.c>
#include <cursor.c>

int oldx, oldy, xspeed, yspeed;

void ende()
{
  _clearscreen(0);
  printf("\nmouseTST -- Ende!\n\n");
  printf(RB_COPYRIGHT);
  printf("\n");
  mouse_off();
  cursor_on();
  exit(1);
}

void status()
{
  change_mouse_coord();
  _settextposition(25,1);
  printf(" bildX:  %2d/80   bildY:  %2d/25",M_BILDX, M_BILDY);
  _settextposition(24,1);
  printf("mouseX: %3d/632 mouseY: %3d/192", MOUSE_X, MOUSE_Y);
  _settextposition(25,49);
  printf("mouseBUTTON: ");
}

void init()
{
int m;
  _clearscreen(0);
  mouse();
  _settextposition(1,18);
  printf("mouseTST -- ");
  printf(RB_COPYRIGHT);
  _settextposition(5,14);
  cprintf("Ende durch drcken aller Maustasten auf dieser Zeile!");
  _settextposition(10,32);
  cprintf("Mausspeed setzen");
  cursor_off();
  mouse_on();
  set_mouse_xy(M_MIDDLE_X, M_MIDDLE_Y);
  status();
}

void setspeed()
{
  _settextposition(11,1);
  printf("1=schnell, n=langsam, normal=10\n\n");
  printf("X-Speed: ");
  scanf("%d",&xspeed);
  printf("Y-Speed: ");
  scanf("%d",&yspeed);
  set_mouse_speed(xspeed,yspeed);
}

main()
{
  init();
  while (i!=1)
  {
    mouse_status();
    if ((oldx!=MOUSE_X) || (oldy!=MOUSE_Y) || (MOUSE_BUTTON>0))
    {
      status();
      if (MOUSE_BUTTON>0)
      {
	_settextposition(25,62);
	switch (MOUSE_BUTTON)
	{
	  case M_BUTTONLEFT   :printf("LINKS             "); break;
	  case M_BUTTONRIGHT  :printf("            RECHTS"); break;
	  case M_BUTTONMIDDLE :printf("      MITTE       "); break;
	  case M_BUTTONLM     :printf("LINKS+MITTE       "); break;
	  case M_BUTTONLR     :printf("LINKS   +   RECHTS"); break;
	  case M_BUTTONMR     :printf("      MITTE+RECHTS"); break;
	  case M_BUTTONALL    :printf("LINKS+MITTE+RECHTS"); break;
	}
	if (MOUSE_BUTTON==M_BUTTONLEFT && M_BILDY==10) setspeed();
	if (MOUSE_BUTTON==M_BUTTONALL && M_BILDY==5) i=1;
      }
      oldx=MOUSE_X; oldy=MOUSE_Y;
    }
  }
  ende();
}
