#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <bios.h>
#include <def.c>
#include <cursor.c>
#include <popup.c>
#include <error.c>

/******************************** DISKARC-Prg ******************************/

struct data
{
  char name[100];
  char wohnort[100];
  char diszi[100];
  char personalnr[100];
  char betr[100];
  char un_rast[100];
  char mj[4];
  char kfm[2];
  char rempf[5];
  char ausland;
  char bk[2];
  char rst;
  char storno1;
  char storno2;
  char summe[4];
} d;

struct kunde
{
  char name[100];
  char anschrift[100];
  char arbeitszeit[100];
  char storno1;
  char abr[2];
  char vo;
  char gk_art[2];
  char ga;
  char vk;
  char avsko[8];
  char tag;
  char kg;
  char tas[2];
  char apenvk[5];
  char art1[2];
  char art2[2];
  char art3[2];
  char art4[2];
  char art5[2];
  char art6[2];
  char norm_std[4];
  char mehrarb_std1[4];
  char mehrarb_std2[4];
  char davon_std[4];
  char pauschal_dm[4];
  char dients_abw[15];
  char art_rk[2];
  char art7[2];
  char art8[2];
  char art9[2];
  char art10[2];
  char art11;
  char art12;
  char fartkosten1[6];
  char fartkosten2[6];
  char ue_kosten[6];
  char km[3];
  char zkm[3];
  char storno2;
  char storno3;
  char zone1[3];
  char zone2[3];
  char u1;
  char ausl_art1[3];
  char u2;
  char ausl_art2[3];
  char tage1[3];
  char tage2[3];
  char erlkl[500];
} k;

const sl      = 68;
      abstand = 60;

char *prg_ver="9.0";

char *file="siemens.$$$";
FILE *f;
char s[255];
long pos;
int p, lpt;
char datum[9];
int zeile;

int file_app()
{
  if((f=fopen(file,"a+"))==NULL)
  {
    error("Konnte Datendatei \"siemens.$$$\" nicht zum Schreiben\
 ”ffnen!",1);
    exit(0);
  }

  return 0;
}

int file_read()
{
  if((f=fopen(file,"r"))==NULL)
  {
    error("Konnte Datendatei \"siemens.$$$\" nicht zum Lesen ”ffnen!",2);
    return -1;
  }

  return 0;
}

int file_read_app()
{
  if((f=fopen(file,"r+"))==NULL)
  {
    error("Konnte Datendatei \"siemens.$$$\" nicht zum Lesen/Schr.\
 ”ffnen!",3);

    return -1;
  }

  return 0;
}

/*************************** PRINT_DATA ************************************/

void printstring (char *str)
{
  while (*str)
  {
    _bios_printer(_PRINTER_WRITE,lpt,*str);
    str++;
  }
}

void leerzeile(int w)
{
  for (i=0; i<w; i++)
  {
    _bios_printer(_PRINTER_WRITE,lpt,0x0A);
    _bios_printer(_PRINTER_WRITE,lpt,0x0D);
  }
}

void print()
{
  if (file_read()!=0) { fclose(f); return; }

  printf("Drucken der Diskettenliste:\n");

  printf("Welcher Druckerport (1/2): LPT");
  scanf("%d",&lpt); lpt--;

  printf("\nDrucke \"siemens.$$$\" an LPT%d...",lpt+1);
}

/***************************************************************************/

int mask(int a)
{
char old[101];
  _clearscreen(0);

  printf("Name           : "); gets(d.name);

  if (a==1) if (strcmp(d.name,"")==0) strcpy(d.name,old);
  if (a==0 && strcmp(d.name,"")==0)
  {
    fclose(f);
    return -1;
  }

  printf("Wohnort        : "); gets(d.wohnort);

  printf("DISZI          : "); gets(d.diszi);

  printf("Personalnummer : "); gets(d.personalnr);

  printf("KFM            : "); gets(d.kfm);

  printf("Rempf.         : "); gets(d.rempf);

  printf("Betr.St„tte    : "); gets(d.betr);

  printf("Un/Rast        : "); gets(d.un_rast);

  printf("Ausland        : "); getch(d.ausland);

  printf("Bk             : "); gets(d.bk);

  printf("Rst            : "); getch(d.rst);

  printf("Monat/Jahr     : "); gets(d.mj);

  printf("Summe          : "); gets(d.summe);

  return 0;
}

void input()
{
  file_app();

  _clearscreen(0);

  if (mask(0)==-1) return; else
    {
      fwrite(&d,sizeof(d),1,f);
      fclose(f);

      input();
    }
}

void output()
{
  _clearscreen(0);

  _settextposition(1,1);
  printf("Name           : %s",d.name);

  _settextposition(2,1);
  printf("DISZI          : %s",d.diszi);

  _settextposition(3,1);
  printf("Wohnort        : %s",d.wohnort);

  _settextposition(4,1);
  printf("Personalnummer : %s",d.personalnr);

  _settextposition(6,1);
  printf("KFM            : %s",d.kfm);

  taste(25,1);
}

void search()
{
  if (file_read()!=0) { fclose(f); return; }

  printf("Suchstring: ");
  gets(s);
  if (strcmp(s,"")==0) return;

  lpt=0;

  while (!feof(f))
  {
    fread(&d,sizeof(d),1,f);

    if (strncmp(d.diszi,s,strlen(s))==0 || strncmp(d.diszi,s,strlen(s))>0
       || strncmp(d.name,s,strlen(s))==0
       || strncmp(d.personalnr,s,strlen(s))==0)
	  output();
  }

  fclose(f);
}

void change()
{
char old[150];

  if (file_read_app()!=0) { fclose(f); return; }

  printf("Welchen Datensatz „ndern (Suchstring): ");
  gets(s);
  if (strcmp(s,"")==0) return;

  while (!feof(f))
  {
    pos=ftell(f);
    fread(&d,sizeof(d),1,f);

    if (strncmp(d.name,s,strlen(s))==0 || strncmp (d.diszi,s,strlen(s))==0
       || strncmp(d.name,s,strlen(s))==0)
	  if (mask(1)==-1)
	  {
	    fclose(f);
	    return;
	  }
	else
	{
	  fseek(f,pos,SEEK_SET);
	  fwrite(&d,sizeof(d),1,f);
	  fclose(f);

	  return;
	}

  }
  fclose(f);
  printf("Dateiende - \"%s\" nicht gefunden!!",s);
  taste(25,1);
}

void del_data_file()
{
  printf("L™SCHEN DER DATENDATEI \"SIEMENS.PNT\"!!!");
  printf("\nWollen Sie wirklich ? ");
  c=getche();
  if (_toupper(c)=='J') system("del diskarc.dat >nul");
}

/************************************* MENš *******************************/

void primary_screen()
{
char datum[9], s[80];

  MFORE=WHITE; MBACK=RED;
  _clearscreen(0);

  for(i=0;i<80;i++) printf("-");

  colored(); _settextposition(2,1);
  for(i=0;i<10;i++)
  {
    sprintf(s,"%c",0xf9);
    _outtext(s);
  }

  sprintf(s,"Siemens PowerNT Version %s - Copyright (C) 1994 Ralf Bensmann\
",prg_ver);
  _outtext(s);

  for(i=0;i<8;i++) { sprintf(s,"%c",0xf9); _outtext(s); }

  uncolored();

  for(i=0;i<80;i++) printf("-");

  _settextposition(4,1); printf("Lizenziert fr: Siemens");

  _strdate(datum); _settextposition(4,66); printf("Datum: %s",datum);

  MFORE=WHITE; MBACK=BLUE;
}

void build_popup_menu()
{
  mpkt [1]=" þ Erstellen eines T„tigkeitsnachweises ";
  mpkt [2]=" þ T„tigkeitsnacheis ausdrucken         ";
  mpkt [3]=" þ Suchen von einem T„tigkeitsnachweis  ";
  mpkt [4]=" þ Žndern eines Nachweises              ";
  mpkt [5]=" þ L”schen der Datendatei               ";
  mpkt [6]=" þ Ende                                 ";

  PAINT=1;
}

/*********************************** Hauptprogramm *************************/

void main()
{
  _clearscreen(0);
  build_popup_menu();

  while(p<6)
  {
    primary_screen();

    _settextwindow(10,20,25,80);
    p=pop_up(6,6);

    _settextwindow(1,1,25,80);
    _clearscreen(0);

    switch(p)
    {
      case 1 :input(); break;
      case 2 :print(); break;
      case 3 :search(); break;
      case 4 :change(); break;
      case 5 :del_data_file(); break;
    }
  }

  _clearscreen(0);
  fclose(f);
}
