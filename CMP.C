/* STRING-Vergleich von 2 Dateien */
#include <stdio.h>
#include <io.h>
#include <conio.h>
#include "ralf.h"

FILE *f1,*f2;
int c,wx,wy;
string file1,file2;
char c1,c2;

void ende(char *message)
{
  fcloseall();
  exit(printf("%s",message));
}

void compare()
{
int i=0,fehler=0;
  printf("\n\n");
  wx=wherex();
  wy=wherey();
  while (!feof(f1) || !feof(f2))
  {
    if (feof(f1))
    {
      fcloseall();
      exit(printf("\nDateiende von %s erreicht. CMP abgebrochen!\n",file1));
    }
    if (feof(f2))
    {
      fcloseall();
      exit(printf("\nDateiende von %s erreicht. CMP abgebrochen!\n",file2));
    }
    i++;
    c1=fgetc(f1);
    c2=fgetc(f2);
    window(1,wy,80,wy+3);
    printf("Vergleiche Byte %d von %s mit %s\n",i,file1,file2);
    if (c1!=c2)
    {
      fehler++;
      if ((c1=='\n'))
	printf("Byte %d: %s: ENTER <> %s:'%c' ",i,file1,file2,c2);
      else if ((c2=='\n'))
	printf("Byte %d: %s:'%c' <> %s: ENTER ",i,file1,c1,file2);
      else if ((c1=='\n') || (c2=='\n'))
	printf("Byte %d: %s: ENTER <> %s: ENTER ",i,file1,file2);
      else printf("Byte %d: %s:'%c' <> %s:'%c' ",i,file1,c1,
	   file2,c2);
      clreol();
      printf("\nWeiter vergleichen ? ");
      c=cge;
      if ((c=='j')) clrscr(); else ende("\nProgramm abgebrochen!\n");
    }
  }
  if ((fehler==0))
  {
    printf("\nDateien sind identisch!");
    ende("\nProgramm erfolgreich beendet!\n");
  }
  if (fehler!=0) ende("\n\n\n\n\nCMP: Programm wegen Fehler abgebrochen!\n");
}

main(int argc, char *argv[])
{
  argc--;
  if (argc!=2) exit(printf("\nSyntax: CMP <datei1> <datei2>\n"));
  file1=argv[1];
  file2=argv[2];
  printf("\n\nCMP: %s und %s...\n\n",file1,file2);
  if ((f1=fopen(file1,"r"))==NULL)
    exit(printf("\nKonnte Datei %s nicht ”ffnen\n",file1));
  if ((f2=fopen(file2,"r"))==NULL)
    exit(printf("\nKonnte Datei %s nicht ”ffnen\n",file2));
  if (filelength(fileno(f1))!=filelength(fileno(f2)))
  {
    printf("\nDateien haben verschiedene Gr”áe!\n");
    printf("Beim vergleichen werden viele Fehler auftreten!\n");
    printf("Trotzdem vergleichen ? (J/N)");
    c=cge;
    if ((c=='j')) compare(); else exit(printf("\nProgramm abgebrochen!\n"));
  }
  compare();
  return;
}
