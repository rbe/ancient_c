void cursor_off ()
{
  r.h.ah=1;
  r.h.ch=255;
  r.h.cl=255;
  int86(VIDEO_INT, &r, &r);
}

void cursor_on ()
{
  r.h.ah=1;
  r.h.ch=5;
  r.h.cl=7;
  int86(VIDEO_INT, &r, &r);
}

void setcur (int CUR_VON, int CUR_BIS)
{
  r.h.ah=1;
  r.h.ch=CUR_VON;
  r.h.cl=CUR_BIS;
  int86(VIDEO_INT, &r, &r);
}

char *ext_keyb ()
{
char *TASTE;
  r.h.ah=2;
  int86(KEYB_INT, &r, &r);
  switch(r.h.al)
  {
/*
    case 1   :TASTE="Shift rechts"; break;
    case 2   :TASTE="Shift links"; break;
    case 4   :TASTE="STRG"; break;
    case 8   :TASTE="ALT"; break;
*/
    case 16  :TASTE="         ROLLEN"; break;
    case 32  :TASTE="NUM"; break;
    case 48  :TASTE="NUM      ROLLEN"; break;
    case 64  :TASTE="    CAPS";break;
    case 80  :TASTE="    CAPS ROLLEN"; break;
    case 96  :TASTE="NUM CAPS"; break;
    case 112 :TASTE="NUM CAPS ROLLEN"; break;
    case 128 :TASTE="INSERT"; break;
  }
  return TASTE;
}
/*
  char *c;
  c=EXT_KEYB();
  printf("%s",c);
*/
