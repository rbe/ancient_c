void init_prn (int PRN)
{
  r.h.ah=1;
  r.x.dx=PRN;
  int86(PRN_INT, &r, &r);
  if (r.h.ah<7) printf("DRUCKER-FEHLERCODE: %d\n",r.h.ah);
}

void boot (char *TEXT)
{
  printf("%s",TEXT);
  int86(BOOT_INT, &r, &r);
}

char *dosversion ()
{
static char a[2], b[2], *VERSION;
  VERSION="";
  itoa(_osmajor,a,10);
  itoa(_osminor,b,10);
  strcpy(VERSION,a);
  strcat(VERSION,".");
  strcat(VERSION,b);
  return VERSION;
}

float dosver ()
{
static float a;
  a=_osmajor+(_osminor*0.01);
  return a;
}
