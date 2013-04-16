/* DateiVIEWER Copyright (C) 1993 Ralf Bensmann */
#include <stdio.h>
#include <string.h>
#include <conio.h>

const k=79,true=1,false=0;

FILE *file1;
int zeile,bild,len,i,z;
char *f,line[80],c;

void parameter()
{ exit(printf("\nSyntaxfehler!!\nAufruf: VIEW <dateiname>\n\n")); }

void ende()
{
  printf("\nEnde der Datei %s!\nGesamtzeilen: %d\n",f,zeile-1);
  fclose(file1);
  if (bild<23) exit(printf("\nCopyright (C) 1993 Ralf Bensmann\n"));
    else exit(c=getch());
}

void view()
{
int wy;
  while (!feof(file1)||feof(file1))
  {
    for (i=0; i<24; i++)
    {
      fgets(line,k,file1);
      if (feof(file1)) ende();
      len=strlen(line);
      if (z==1)
      {
       printf ("%d %s",zeile,line);
       if (len>=77)
       {
	 wy=wherey();
	 gotoxy(1,wy-1);
       }
      }
      else printf("%s",line);
      zeile++;
    }
    bild=1;
    printf("Taste...");
    c=getch();
    clrscr();
  }
  ende();
}

main (int argc, char *argv[])
{
  zeile=1; argc--;
  if (argc<1) parameter();
  f=argv[1];
  if (strcmp("!",argv[2])==0) z=true; else z=false;
  clrscr();
  if ((file1=fopen(f,"r"))==NULL) printf("\nKonnte Datei %s nicht finden!",f);
    else view();
}
