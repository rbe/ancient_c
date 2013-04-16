#include <def.c>
#include <stdio.h>

FILE *f,*f2;
char zeichen, fname[67];

void convert()
{
  printf("\nKonvertiere \"%s\"...",fname);
  while(!feof(f))
  {
    zeichen=fgetc(f);
    if(zeichen==0xfc) zeichen='î';
    if(zeichen==0xe4) zeichen='Ñ';
    if(zeichen==0xf6) zeichen='Å';
    if(zeichen==0xdf) zeichen='·';
    fputc(zeichen,f2);
  }
  printf("fertig!\n");
}

void files()
{
  fclose(f); fclose(f2);
  /*
  remove(fname);
  rename("change.tmp",fname);
  */
}

main(int argc, char *argv[])
{
  _clearscreen(0);
  argc--;
  if (argc!=1)
  {
    printf("\nCHANGER: Parameter nicht korrekt!\n");
    exit(1);
  } else strcpy(fname,argv[1]);
  if((f2=fopen("change.tmp","w"))==NULL)
  {
    printf("\nCHANGER: Konnte Datei \"CHANGE.TMP\" nicht îffnen!\n");
    exit(1);
  }
  if((f=fopen(fname,"r"))==NULL)
  {
    printf("\nCHANGER: Konnte Datei \"%s\" nicht îffnen!\n",fname);
    exit(1);
  } else convert();
  files();
  printf("\nDatei \"%s\" fertig konvertiert!\n",fname);
}


