#include <stdio.h>

struct data
{
  char name[80];
  char vorname[80];
  char strasse[80];
} d;

char n[80];
FILE *f;

void oeffnen()
{
  if((f=fopen("ralf.dat","w+r"))==NULL)
    exit(printf("Konnte Datei nicht ”ffnen!\n"));
}

void eingabe()
{
  clrscr();
  printf("Name    : ");
  scanf("%s",&d.name);
  fwrite(d.name,100,100,f);
  printf("Vorame    : ");
  scanf("%s",&d.vorname);
  fwrite(d.vorname,100,100,f);
}

void ausgabe()
{
  rewind(f);
  fread(n,100,100,f);
  printf("\nNAME: %s\n",&n);
  fread(n,100,100,f);
  printf("\nVORNAME: %s\n",&n);
}

void ende()
{
  fcloseall();
  exit(printf("Ende!"));
}

void main()
{
  oeffnen();
  eingabe();
  ausgabe();
  ende();
}
