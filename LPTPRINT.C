#include <bios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <def.c>

/* Werte gemessen an 10cpi-Schrift  */
/* DIN A4 */
#define A4_SEITE 68
#define A4_ZLAENGE 77

/* DIN A4quer */
#define A4q_SEITE 50
#define A4q_ZLAENGE 110

float PRG_ver=2.31;
char dateiname[67],
     print,
     p_z[5], s_z[5],
     *dina;
int  chars=1, geschars,
     zeile, geszeilen=1,
     seiten=1, zl, sl, NR,
     DOUBLE,
     LPT;
FILE *f;

void status()
{
  _settextposition(7,1);
  printf("Datei: %s -- LPT: %i -- DIN%s -- Zeilennummern: ",
  dateiname,LPT+1,dina);
  if (NR==YES) printf("Ja"); else printf("Nein");
  printf("\nDouble-Strike: ");
  if (DOUBLE==YES) printf("Ja"); else printf("Nein");
}

void unter(int a)
{
  if (a==1)
  {
    _bios_printer(_PRINTER_WRITE,LPT,0x1B);
    _bios_printer(_PRINTER_WRITE,LPT,0x2D);
    _bios_printer(_PRINTER_WRITE,LPT,0x01);
  }
    else
  {
    _bios_printer(_PRINTER_WRITE,LPT,0x1B);
    _bios_printer(_PRINTER_WRITE,LPT,0x2D);
    _bios_printer(_PRINTER_WRITE,LPT,0x00);
  }
}

void double_strike(int a)
{
  if (a==1)
  {
    _bios_printer(_PRINTER_WRITE,LPT,0x1B);
    _bios_printer(_PRINTER_WRITE,LPT,0x47);
  }
    else
  {
    _bios_printer(_PRINTER_WRITE,LPT,0x1B);
    _bios_printer(_PRINTER_WRITE,LPT,0x48);
  }
}

void leerzeile(int w)
{
  for (i=0; i<w; i++)
    _bios_printer(_PRINTER_WRITE,LPT,'\n');
}

void printstring (char *str)
{
  while (*str)
  {
    _bios_printer(_PRINTER_WRITE,LPT,*str);
    str++;
  }
}

void kopf()
{
  zeile+=2;
  printstring("Datei: ");
  for (i=0; i<strlen(dateiname); i++)
    _bios_printer(_PRINTER_WRITE,LPT,dateiname[i]);
  printstring(" -- Seite: "); itoa(seiten,s_z,10); printstring(s_z);
  leerzeile(1);
  for (i=0; i<zl; i++) _bios_printer(_PRINTER_WRITE,LPT,'Í');
  leerzeile(1);
}

void printzeilen()
{
int a=0, b=0;
char c[5];
  itoa(geszeilen,p_z,10); itoa(geszeilen,c,10); b=strlen(c);
  for (a=0; a<(5-b); a++) printstring(" ");
  printstring(p_z); printstring(": ");
}

void druck()
{
int k;
  status();
  _settextposition(10,1); printf("Drucke %s...!\n",dateiname);
  printf("Zeile:      ");
  if (DOUBLE==YES) double_strike(1);
  kopf();
  if (NR==YES) printzeilen();
  while(!feof(f))
  {
    k=kbhit();
    if (k!=0)
    {
      printf("\n\nAbbruch!\n");
      _bios_printer(_PRINTER_INIT,LPT,0);
      exit(1);
    }
    print=fgetc(f); chars++; geschars++; if(print=='\n') geschars++;
    if (chars>zl)
    {
      _bios_printer(_PRINTER_WRITE,LPT,'~');
      leerzeile(1); zeile++; chars=0;
    }
    if (zeile>sl)
    {
      leerzeile(3); zeile=0; seiten++;
      kopf();
    }
    if (print=='\n')
    {
      zeile++; geszeilen++; chars=1;
      if (NR==YES) { leerzeile(1); printzeilen(); } else leerzeile(1);
    }
      else _bios_printer(_PRINTER_WRITE,LPT,print);
    _settextposition (11,10); printf("%5i",geszeilen);
  }
  if (DOUBLE==YES) double_strike(0);
  geschars--;
  leerzeile(1);
  for (i=0; i<zl; i++) printstring("-");
  leerzeile((sl-zeile)+3);
  _settextposition(10,1); printf("Drucken beendet!");
  for(i=0;i<144;i++) printf(" ");
  printf("Gesamtzeilen: %5i\nZeichen     : %5i\nSeiten      : %5i\n",
    geszeilen,geschars,seiten);
}

void oeffne()
{
  if ((f=fopen(dateiname,"r"))==NULL)
  {
    printf("\n\nDatei nicht gefunden!\n\n");
    exit(errno);
  }
}

void param_abfrage()
{
  _settextposition(7,1);
  printf("Datei: ");
  gets(dateiname);
  if(strcmp(dateiname,"")==0)
  {
    printf("Abbruch!\n\n");
    exit(1);
  } else oeffne();
  printf("\nDINA[4]  oder DINA4quer[3] (ENTER=A4): ");
  c=getch(); i=atoi(&c);
  switch (i)
  {
    case 3 :{ sl=A4q_SEITE; zl=A4q_ZLAENGE; printf("A4quer");
	      dina="A4quer"; } break;
    default :{ sl=A4_SEITE; zl=A4_ZLAENGE; printf("A4"); dina="A4"; }
  }
  printf("\nDruckeranschluá LPT (ENTER=1)        : ");
  c=getch(); LPT=atoi(&c); if (LPT==0) LPT=1; LPT--;
  printf("LPT %i",LPT+1);
  printf("\nZeilennummern  (NEIN=ENTER)          : ");
  c=getche(); upcase; if (c==ENTER || c=='N') NR=NO; else NR=YES;
  printf("\nDouble-Strike (NEIN=ENTER)           : ");
  c=getche(); upcase; if (c==ENTER || c=='N') DOUBLE=NO; else DOUBLE=YES;
  _settextwindow(7,1,25,80);
  _clearscreen(_GWINDOW);
  _settextwindow(1,1,25,80);
}

void init()
{
int a;
  seiten=1;
  _clearscreen(0);
  printf("\t\t\t<<< LPT1/2-Druckerprogramm >>>\n\n");
  printf("Drucken von Dateien auf LPT1 oder LPT2!!\n\n\n");
  /*
  a=_bios_printer(_PRINTER_STATUS,1,0);
  if (a!=0x90)
  {
    printf("Drucker nicht bereit!\n\n");
    exit(1);
  }
  */
}

main(int argc, char *argv[])
{
  init();
  if (argc==1) param_abfrage();
    else if (strcmp(argv[1],"/?")==0)
    {
      printf("\nSyntax: lptprint <dateiname> <lpt> <din>\n");
      printf("\n<lpt>: 1 oder 2\n<din>: 3 (=A4quer) oder 4 (=A4)\n");
      printf("\nLPTPRINT %.2f -- %s\n",PRG_ver,RB_COPYRIGHT);
      exit(0);
    }
      else
    {
      strcpy(dateiname,argv[1]);
      oeffne();
      if (strlen(argv[2])==0) LPT=0; else LPT=atoi(argv[2])-1;
      i=atoi(argv[3]);
      switch (i)
      {
	case 3 :{ dina="A4quer"; sl=A4q_SEITE; zl=A4q_ZLAENGE; } break;
	case 4 :{ dina="A4"; sl=A4_SEITE; zl=A4_ZLAENGE; } break;
      }
      NR=NO; DOUBLE=NO;
    }
  druck();
  fclose(f);
  printf("\nLPTPRINT %.2f -- %s\n",PRG_ver,RB_COPYRIGHT);
}
