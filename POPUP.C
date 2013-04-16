char *mpkt[1000], ch;
int  INTE, PAINT, MAXI,
     CHOOSEN;
long MFORE, MBACK;

void colored()
{
  _settextcolor(MFORE);
  _setbkcolor(MBACK);
}

void uncolored()
{
  _settextcolor(7);
  _setbkcolor(*(long*)1);
}

void dodown()
{
  _settextposition(PAINT-1,1);
  uncolored(); if(PAINT!=1) _outtext(mpkt[PAINT-1]);
  _settextposition(PAINT,1);
  colored(); _outtext(mpkt[PAINT]);
  uncolored();
}

void doup()
{
  if(PAINT==MAXI)
  {
    PAINT=1;
    _settextposition(PAINT,1);
    uncolored(); _outtext(mpkt[PAINT]);
    _settextposition(MAXI,1);
    colored(); _outtext(mpkt[MAXI]);
    PAINT=MAXI;
  }
  else
  {
    _settextposition(PAINT+1,1);
    uncolored();
    _outtext(mpkt[PAINT+1]);
    _settextposition(PAINT,1);
    colored();
    _outtext(mpkt[PAINT]);
  }
  uncolored();
}

int pop_up (int MAX, int POPUP_ENDE)
{
static int k;
  cursor_off();
  MAXI=MAX;
  for (INTE=1; INTE<MAX+1; INTE++) {
    _settextposition(INTE,1);
    _outtext(mpkt[INTE]);
  }
  _settextposition(PAINT,1);
  colored();
  _outtext(mpkt[PAINT]);
  uncolored();
  while (ch!=ESC) {
    ch=getch();
    switch(ch)
    {
      case UP   : {
		    PAINT--;
		    if (PAINT<1) {
		     PAINT=MAX; _settextposition(MAX-MAX+1,1); uncolored();
		     _outtext(mpkt[1]);
		    }
		    doup();
		  } break;
      case DOWN : {
		    PAINT++;
		    if (PAINT>MAX) {
		      PAINT=1; _settextposition(MAX,1); uncolored();
		      _outtext(mpkt[MAX]);
		    }
		    dodown();
		  } break;
      case ENTER: { CHOOSEN=PAINT; cursor_on(); return CHOOSEN; }
      case ESC  : { CHOOSEN=POPUP_ENDE; cursor_on(); }
      default :;
    }
  }
  PAINT=CHOOSEN;
  cursor_on();
  return CHOOSEN;
}

/*
POPUP BEISPIEL:
---------------
  CHOOSEN = gew„hlte Menpunkt
  choose (param1, param2) = Prozedure; param1=Menge der Menpunkte
			    param2=END-Menpunkt
  PAINT=1; ==> INITaliaierung notwendig; nur einmal

void main()
{
  PAINT=1;
  MFORE=0; MBACK=15;
  while (CHOOSEN<3) {
    clrscr();
    mpkt[1]="PUNKT 1";
    mpkt[2]="PUNKT 2";
    mpkt[3]="ENDE DES MENUES!";
    popup(3,3);
    switch(CHOOSEN)
    {
      case 1 :printf("puh!"); break;
      case 2 :printf("p2!"); break;
    }
  }
}
*/
