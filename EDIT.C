#include <stdio.h>
#include <conio.h>
#include <def.c>

const k=79;
FILE *f;
char zeile[80][100],*file,c;
int i, von, bis, ges;

void quit()
{
   _settextwindow(1,1,25,80);
   printf("\nEDIT -- %s",RB_COPYRIGHT);
   exit(0);
}

void read_file()
{
  i=0;
  while (!feof(f))
  {
    i++;
    fgets(zeile[i],k,f);
  }
  ges=i+1;
}

void open_file()
{
  if ((f=fopen(file,"r"))==NULL)
  {
    printf("EDIT: ");
    printf(CANNOT_OPEN,file);
    printf("\n%s",RB_COPYRIGHT);
    quit();
  } else read_file();
}

void show()
{
  _clearscreen(0);
  for (i=von; i<bis; i++)
    if(strchr(zeile[i],0x09)!=NULL) printf("<tab> %s",zeile[i]);
      else printf("%s",zeile[i]);
}

void status()
{
  _settextwindow(1,1,25,80);
  _settextposition(25,1);
  printf("Datei: %s  |  Zeile: %d/%d",file,von,ges);
  _settextwindow(1,1,24,1);
}

void browse()
{
  status();
  while (c!=0x1B) {
    c=getch();
    switch(c)
    {
      case PGDOWN :{
		     von=bis; bis=bis+24;
		     if(bis>=ges) { von=ges-24; bis=ges; }
		     show();
		   } break;
      case PGUP   :{
		     if(bis==ges) { bis=ges-24; von=bis-24; }
		       else von=von-24; bis=bis-24;
		     if(von<1) { von=1; bis=24; }
		     show();
		   } break;
      case DOWN   :{
		     von++; bis++;
		     if (bis>=ges) { bis=ges; von=ges-25; }
		     show();
		   } break;
      case UP     :{
		     von--; bis--;
		     if (von<=1) { von=1; bis=25; }
		     show();
		   }  break;
      case ESC    :{ _clearscreen(0); quit(); }
    }
    status();
  }
}

void parameter()
{
  printf("\nparameterformat von edit:\n");
  printf("\nedit <dateiname>\n");
  printf("\nEDIT -- %s\n",RB_COPYRIGHT);
  exit();
}

void main(int argc, char *argv[])
{
  _clearscreen(0); argc--;
  if (argc<1) parameter(); else file=argv[1];
  open_file(); von=1; bis=25; show(); browse();
}
