#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <def.c>
#include <error.c>
#include <cursor.c>
#include <popup.c>

struct data
{
  char name[100], vorname[20], strasse[50];
} d;

FILE *stream;

void datei_auf()
{
  if ((stream=fopen("adress","a+"))==NULL)
  {
    error("ADR: Dateifehler",1);
    exit(errno);
  }
}

void eingabe()
{
  printf("Name    : "); gets(d.name);
  printf("Vorname : "); gets(d.vorname);
  printf("Straáe  : "); gets(d.strasse);
  fprintf(stream,"%s\n%s\n%s\n",d.name,d.vorname,d.strasse);
  taste(25,1);
}

void ausgabe()
{
  if (fseek(stream,0L,SEEK_SET)!=0)
  {
    error("Konnte Datei nicht lesen!",3);
    exit(errno);
  }
  while (!feof(stream))
  {
    fscanf (stream,"%s\n%s\n%s",d.name,d.vorname,d.strasse);
    printf("Name    : "); puts(d.name);
    printf("Vorname : "); puts(d.vorname);
    printf("Straáe  : "); puts(d.strasse);
    taste(25,1);
    _clearscreen(0);
  }
}

void sys_ende()
{
  if (fclose(stream)==EOF) error("Konnte Datei nicht schlieáen",3);
  _clearscreen(0);
  printf ("\n\nEnde von adr.c\n%s",RB_COPYRIGHT);
  exit(0);
}

void menu()
{
  while (CHOOSEN<4)
  {
    _settextwindow(5,10,8,80);
    i=pop_up(3,3);
    _settextwindow(1,1,25,80);
    _clearscreen(0);
    switch (i)
    {
      case 1:eingabe(); break;
      case 2:ausgabe(); break;
      case 3:sys_ende(); break;
    }
    _clearscreen(0);
  }
}

main()
{
  _clearscreen(0);
  datei_auf();
  mpkt[1]=" Eingabe ";
  mpkt[2]=" Ausgabe ";
  mpkt[3]="  Ende   ";
  PAINT=1; MFORE=RED; MBACK=LIGHTGRAY;
  _clearscreen(0);
  menu();
}
