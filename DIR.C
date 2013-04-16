#include <stdlib.h>
#include <dir.h>
#include <dos.h>
#include <string.h>
#include <conio.h>
#include <time.h>

struct ffblk ff;
struct tm t;
char *path,drive[MAXDRIVE],dir[MAXDIR],ext[MAXEXT],name[MAXFILE],
     *filename,*olddir, datum[10], zeit[10];
int i,j,k,c,r,d,lfw,attr,pause,uvz,unter,uges,ges,files,bild;
long speicher;

void getpath()
{
  getcurdir(lfw,olddir);
  getcurdir(lfw,path);
}

void wait()
{
  gotoxy(1,25);
  textcolor(LIGHTRED);
  cprintf("\"to be continued...\"");
  textcolor(YELLOW);
  c=getch();
  window(1,4,80,23); clrscr(); window(1,1,80,25);
  gotoxy(1,4);
  unter=0; bild=4;
}

void wait2()
{
  gotoxy(1,4);
  unter=0; bild=4;
  textcolor(YELLOW);
}

void directory()
{
  bild=4;
  findfirst("*.*",&ff,FA_HIDDEN + FA_SYSTEM + FA_DIREC);
  while (d==0)
  {
    speicher=speicher+ff.ff_fsize;
    fnsplit(ff.ff_name,drive,dir,name,ext);
    filename=ff.ff_name;
    if (ff.ff_attrib&FA_DIREC)
    {
      unter++;
      j=8-strlen(filename);
      if (strcmp(ff.ff_name,"..")==0) cprintf("\\%-12s  <getting up...>\n\r",
      filename); else if (strcmp(ff.ff_name,".")==0)
	cprintf("\\%-12s  <nothing will happen...>\n\r",filename);
      else
      {
	uges++;
	cprintf("dir  %4d -\\%-12s  <directory>\n\r",uges,filename);
      }
    }
    else if (uvz==0)
    {
      files++;
      textcolor(WHITE);
      if (ff.ff_attrib&0x5) cprintf("file %4d -%s",files,strlwr(name));
	else cprintf("file %4d -%s",files,name);
      for (j=1; j<=8-strlen(name); j++) printf(" ");
      if (ff.ff_attrib&0x5) cprintf("%s",strlwr(ext));
	else cprintf("%s",ext);
      for (j=1; j<=4-strlen(ext); j++) printf(" ");
      cprintf("%10ld Bytes %d.%d.%d   %d:%d:%d ",ff.ff_fsize,
      t.tm_mday,t.tm_mon,t.tm_year,t.tm_hour,t.tm_min,t.tm_sec);
      textcolor(YELLOW);
      if (attr==1)
      {
	textcolor(WHITE);
	switch (ff.ff_attrib)
	{
	  case  0 :cprintf("no attributs!\n\r"); break;
	  case  1 :cprintf("Read-Only\n\r"); break;
	  case  2 :cprintf("Hidden\n\r"); break;
	  case  3 :cprintf("Hidden, Read-Only\n\r"); break;
	  case  4 :cprintf("System\n\r"); break;
	  case  5 :cprintf("System, Read-Only\n\r"); break;
	  case  7 :cprintf("System, Hidden, Read-Only\n\r"); break;
	  case 32 :cprintf("Archieve\n\r"); break;
	  case 33 :cprintf("Archieve, Read-Only\n\r"); break;
	  case 34 :cprintf("Archieve, Hidden\n\r"); break;
	  case 35 :cprintf("Archieve, Hidden, Read-Only\n\r"); break;
	  case 37 :cprintf("Archieve, System, Read-Only\n\r"); break;
	  case 39 :cprintf("Archieve, System, Hidden, Read-Only\n\r"); break;
	  default :cprintf("unknown attribute: %d\n\r",ff.ff_attrib);
	}
      } else printf("\n");
      textcolor(YELLOW);
    }
    ges++; bild++;
    if (pause==1 && bild==24) wait();
    if (pause==0 && bild==24) wait2();
    d=findnext(&ff);
  }
  ges=files+uges;
}

void init()
{
  lfw=getdisk();
  findfirst("*.*",&ff,FA_LABEL);
  printf("label of disk in drive %c: %s (DIR Version 1)\ndirectory list\
 of path: %s\n",lfw+97,ff.ff_name,path);
  for (i=0; i<=79; i++) printf("-");
  gotoxy(1,24);
  for (i=0; i<=79; i++) printf("-");
  gotoxy(1,4);
}

main (int argc, char *argv[])
{
  textcolor(YELLOW);
  clrscr();
  pause=1; attr=0;
  getpath();
  if (argc>1) for (i=1; i<=argc; i++)
    switch (*argv[i])
    {
      case 'a' :attr=1; break;
      case 'p' :pause=0; break;
      case 'u' :uvz=1; break;
    }
  else
  {
    attr=0;
    pause=1;
    uvz=0;
  }
  if (argc>1)
  {
    path=argv[1];
    chdir(path);
  }
  init();
  directory();
  if (bild>=21)
    {
      c=getch();
      window(1,4,80,23);
      clrscr();
      window(1,1,80,25);
      gotoxy(1,4);
    }
  if (uvz==1) printf("\ndirectories: %d\n",uges);
    else printf("\ndirectories: %d, files: %d\nsum: %d\nbytes: %ld",
    uges,files,ges,speicher);
  chdir(olddir);
  gotoxy(1,24); textcolor(LIGHTGRAY); cprintf("THE END.\n\r");
  clreol();
  gotoxy(1,24);
  return;
}
