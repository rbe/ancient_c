#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <bios.h>
#include <assert.h>
#include <def.c>
#include <cursor.c>
#include <popup.c>
#include <error.c>

/******************************** DISKARC-Prg ******************************/

struct data
{
  char disknr[7];
  char titel[100];
  char lend[100];
  char deleted;
/*
  char diskof[5];
  char category[100];
  char description[100];
*/
} d;

const page_length = 68;
      space       = 60;

char *prg_ver="2.30";

char *file="diskarc.dat";
FILE *f;

long pos;

int p, lpt;
int line;

char datum[9];
char s[255];

int file_app ()
{
  if ( ( f = fopen ( file, "a+" ) ) == NULL )
  {
    error ( "Konnte Datendatei \"diskarc.dat\" nicht zum Schreiben\
 ”ffnen!", 1 );
    exit ( 0 );
  }
  return 0;
}

int file_read ()
{
  if ( ( f = fopen ( file, "r" ) ) == NULL )
  {
    error ( "Konnte Datendatei \"diskarc.dat\" nicht zum Lesen ”ffnen!", 2 );
    return -1;
  }
  return 0;
}

int file_read_app ()
{
  if ( ( f = fopen ( file, "r+" ) ) == NULL )
  {
    error ( "Konnte Datendatei \"diskarc.dat\" nicht zum Lesen/Schr.\
 ”ffnen", 3 );
    return -1;
  }
  return 0;
}

void define_disknr_string ()
{
  char *hilf;

  switch ( strlen ( d.disknr ) )
  {
    case 1 : strcpy ( hilf, "D0000" ); break;
    case 2 : strcpy ( hilf, "D000" ); break;
    case 3 : strcpy ( hilf, "D00" ); break;
    case 4 : strcpy ( hilf, "D0" );
  }
  strcat ( hilf, d.disknr );
  strcpy ( d.disknr, hilf );
}

void init ()
{
  if  ( file_read_app () != 0 )
  {
    fclose ( f );
    return;
  }
  while ( !feof ( f ) )
  {
    pos = ftell ( f );
    fread ( &d, sizeof ( d ), 1, f );
  }
  fclose ( f );
  define_disknr_string ();
}

/*************************** PRINT_DATA ************************************/

void printstring ( char *str )
{
  while  ( *str )
  {
    _bios_printer ( _PRINTER_WRITE, lpt, *str );
    str++;
  }
}

void leerzeile ( int w )
{
  for ( i = 0; i < w; i++ )
  {
    _bios_printer ( _PRINTER_WRITE, lpt, 0x0A );
    _bios_printer ( _PRINTER_WRITE, lpt, 0x0D );
  }
}

void headline ()
{
  _strdate ( datum );
  printstring ( "DISKARC - Diskettenliste, Stand: "); printstring ( datum );
  leerzeile ( 1 );
  for ( i = 0; i < 80; i++ ) printstring ( "Í" );
  leerzeile ( 1 ); printstring( "       Nr.   Name" ); leerzeile( 1 );
  line += 3;
}

void print_data ()
{
  int j;

  printstring ( "     " );

  printstring ( d.disknr ); printstring ( "  " ); printstring ( d.titel );
  leerzeile ( 1 );
}

void print ()
{
  if ( file_read () != 0 )
  {
    fclose ( f );
    return;
  }

  line = 0;

  printf ( "Drucken der Diskettenliste:\n" );

  printf ( "Welcher Druckerport (1/2): LPT" );
  scanf ( "%d", &lpt );

  printf ( "\nDrucke \"diskarc.dat\" an LPT%d...", lpt );

  lpt--;

  headline ();

  while ( !feof ( f ) )
  {
    fread ( &d, sizeof ( d ), 1, f );
    if ( feof ( f ) )
    {
      leerzeile ( ( page_length - line ) + 4 );
      init ();
      return;
    }
    if  ( line > page_length )
    {
      leerzeile ( 3 );
      line = 0;
      headline ();
    }

    print_data ();
    line++;
  }
}

/***************************************************************************/

int mask ( int a )
{
  char old[101], *eingabe;

  _clearscreen ( 0 );

  printf ( "Disknummer   : %s - ", d.disknr );
  if ( a == 1 ) strcpy ( old, d.disknr );
  gets ( eingabe );
  strcpy  ( d.disknr, eingabe );
  define_disknr_string ();

  if (a == 0 && strcmp ( eingabe, "" ) == 0 )
  {
    fclose ( f );
    return -1;
  }

  if ( a == 1 ) if ( strcmp ( eingabe, "" ) == 0 ) strcpy ( d.disknr, old );

  printf ( "Titel        : %s - ", d.titel );
  strcpy ( old,d.titel );
  gets ( d.titel );
  if ( strcmp ( d.titel, "" ) == 0 ) strcpy ( d.titel, old );

/*
  printf("Disk/Disks   : %s - ",d.diskof);
  strcpy(old,d.diskof);
  gets(d.diskof);
  if (strcmp(d.diskof,"")==0) strcpy(d.diskof,old);

  printf("Kategorie    : %s - ",d.category);
  strcpy(old,d.category);
  gets(d.category);
  if (strcmp(d.category,"")==0) strcpy(d.category,old);

  printf("Beschreibung : %s - ",d.description);
  strcpy(old,d.description);
  gets(d.description);
  if (strcmp(d.description,"")==0) strcpy(d.description,old);
*/

  strcpy ( d.lend, "" );
  d.deleted = 'n';

  return 0;
}

void input ()
{
  file_app ();

  _clearscreen ( 0 );

  d.deleted = 'n';

  if ( mask ( 0 ) == -1 ) return;
    else {
      fwrite ( &d, sizeof ( d ), 1, f );
      fclose ( f );
      input ();
    }
}

int output ( int a )
{
  _clearscreen ( 0 );

  _settextposition ( 1, 1 );
  printf ( "Disknummer    : %s", d.disknr );

  _settextposition ( 2, 1 );
  printf ( "Titel         : %s", d.titel );

/*
  _settextposition(3,1);
  printf("Disk/Disks    : %s",d.diskof);

  _settextposition(4,1);
  printf("Kategorie     : %s",d.category);

  _settextposition(5,1);
  printf("Beschreibung  : %s",d.description);

*/

  _settextposition ( 4, 1 );
  printf ( "Ausgeliehen an: " );
  if ( strcmp ( d.lend, "" ) == 0 ) printf ( "niemanden" );
    else printf ( "%s", d.lend );

  if ( a == 0 ) return taste ( 25, 1 ); else return;
}

void show_all ()
{
  if ( file_read () != 0 )
  {
    fclose ( f );
    return;
  }

  while ( !feof ( f ) )
  {
    for ( i = 0; i < 24 ; i++ )
    {
      fread ( &d, sizeof ( d ), 1, f );
      if ( feof ( f ) )
      {
	fclose ( f );
	taste ( 25, 1 );
	return;
      }
      if ( d.deleted == 'n' ) printf( "%s  %s\n", d.disknr, d.titel );
    }
    if ( taste ( 25, 1 ) == ESC ) return; else _clearscreen ( 0 );
  }
}

void search(int a)
{
  if (file_read()!=0) { fclose(f); return; }

  printf("Suchstring: ");
  gets(s);
  if (strcmp(s,"")==0) return;

  if (a==1) {
    printf("Welcher Druckerport (1/2): LPT");
    scanf("%d",&lpt); lpt--;
  }

  while (!feof(f))
  {
    fread(&d,sizeof(d),1,f);

    if (feof(f)) { fclose(f); return; }

    if (strcmp(d.titel,s)==0 || strcmp(d.disknr,s)==0

    /*  || strcmp(d.category,s)==0  */

       || strcmp(d.lend,s)==0
       || strcmp(d.lend,s)<0)
	  if (d.deleted=='n') if (a==1) print_data(); else if (output(0)==ESC)
							 return;
  }

  fclose(f);
}

void correct_data()
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

    if (strncmp(d.disknr,s,strlen(s))==0 || strncmp (d.titel,s,strlen(s))==0
       || strncmp(d.disknr,s,strlen(s))==0) if (d.deleted=='n')
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

void delete()
{
  if (file_read_app()!=0) { fclose(f); return; }

  printf("Welchen Datensatz l”schen (Suchstring): ");
  gets(s);

  while (!feof(f))
  {
    fread(&d,sizeof(d),1,f);
    pos=ftell(f);

    if (strncmp(d.titel,s,strlen(s))==0
       || strncmp(d.titel,s,strlen(s))<0
       || strncmp(d.disknr,s,strlen(s))==0)
      {
	fseek(f,pos,SEEK_SET);
	d.deleted='j';
	fwrite(&d,sizeof(d),1,f);
	fclose(f);

	printf("\"%s\" ge”scht!!",s);

	taste(25,1);
	return;
      }
  }

  fclose(f);
  printf("Dateiende - \"%s\" nicht gefunden!!!",s);
  taste(25,1);
}

void lend()
{
char l[50];
  if (file_read_app()!=0) { fclose(f); return; }

  printf("Welches Programm ausleihen: ");
  gets(s);
  if (strcmp(s,"")==0) return;

  while (!feof(f))
  {
    pos=ftell(f);
    fread(&d,sizeof(d),1,f);

    if (strcmp(d.titel,s)==0 || strcmp(d.disknr,s)==0)
      {
	if (d.deleted=='n') output(1); else
	{
	  printf("Programm %s bereits gel”scht",d.titel);
	  taste(25,1);
	  return;
	}

	_settextposition(23,1);
	printf("Person eingeben (Enter fr zurcknehmen/Abbruch): ");

	gets(l);

	if (strcmp(l,"")==0) if (strcmp(d.lend,"")==0) return;
	if (strcmp(l,"")!=0) if (strcmp(d.lend,"")!=0 &&
				 strcmp(d.lend,l)!=0)
	{
	  printf("Programm %s ist schon an %s ausgeliehen",d.titel,
	    d.lend);
	  taste(25,1);
	  return;
	}

	_settextposition(24,1);
	printf("Programm %s",d.titel);

	if(strcmp(l,"")==0) printf(" zurckgenommen!");
	  else if (strcmp(d.lend,l)==0)
	    printf(" weiterhin an %s ausgeliehen!",l);
	      else printf(" an %s ausgeliehen!",l);

	strcpy (d.lend,l);
	fseek(f,pos,SEEK_SET);
	fwrite(&d,sizeof(d),1,f);
	fclose(f);

	taste(25,1); fclose(f); return;

      }
  }
  printf("Programm %s nicht gefunden!",s);
  taste(25,1);
}

void del_data_file ()
{
  printf("L™SCHEN DER DATENDATEI \"DISKARC.DAT\"!!!");
  printf("\nWollen Sie wirklich ? ");
  c=getche();
  if (_toupper(c)=='J') system("del diskarc.dat >nul");

  strcpy( d.disknr, "D00000" );
}

/******************************** ENDE ***********************************/

void end_prg ()
{
  _clearscreen ( 0 );

  printf ( "\nDISKARC %s beendet!\n", prg_ver );

  assert ( fclose ( f ) );
}

/************************************* MENš ******************************/

void primary_screen ()
{
  char datum[9], zeit[9], s[80];

  /* Vorder- und Hintergrund festlegen */
  MFORE = WHITE; MBACK = RED;

  _clearscreen ( 0 );

  /* Zeile mit - schreiben */
  for ( i = 0; i < 80; i++ ) printf ( "-" );

  colored ();
  _settextposition ( 2, 1 );

  /* i = 10 mal Zeichen 0xf9 ausgeben */
  for ( i = 0; i < 10; i++ )
  {
    sprintf ( s, "%c", 0xf9 );
    _outtext ( s );
  }

  /* Textzeile mit Copyright */
  sprintf ( s, "DISKARC%cVersion%c%s%c-%cCopyright%c(C)%c1994,1995\
%cRalf%cBensmann", 0xf9, 0xf9, prg_ver, 0xf9, 0xf9, 0xf9, 0xf9,
	    0xf9, 0xf9 );
  _outtext ( s );

  /* i = 10 mal Zeicehn 0xf9 ausgeben */
  for ( i = 0; i < 10; i++ )
  {
    sprintf ( s, "%c", 0xf9 );
    _outtext ( s );
  }

  uncolored ();

  for ( i = 0; i < 80; i++ ) printf ( "-" );

  _strdate ( datum );
  _strtime ( zeit );
  _settextposition ( 5, 64 );
  printf ( "%s %s", datum, zeit );

  MFORE = WHITE; MBACK = BLUE;
}

void build_popup_menu ()
{
  mpkt [1] = " þ Suchen von Disketten         ";
  mpkt [2] = " þ Neue Diskette(n) aufnehmen   ";
  mpkt [3] = " þ Ausleihen von Disketten      ";
  mpkt [4] = " þ Žndern von Eintr„gen         ";
  mpkt [5] = " þ Diskette l”schen             ";
  mpkt [6] = " þ Suchen und ausdrucken        ";
  mpkt [7] = " þ Alle Disketten anzeigen      ";
  mpkt [8] = " þ Drucken der Diskettenliste   ";
  mpkt [9] = " þ L”schen der Datendatei       ";
  mpkt[10] = " þ Ende                         ";

  PAINT = 1;
}

/*********************************** Hauptprogramm *************************/


void main ()
{
  _clearscreen ( 0 );
  strcpy ( d.disknr, "D00000" );
  init ();
  build_popup_menu ();

  while( p < 10 )
  {
    primary_screen ();

    _settextwindow ( 10, 24, 25, 80 );

    p = pop_up ( 10, 10 );

    _settextwindow ( 1, 1, 25, 80 );
    _clearscreen ( 0 );

    switch ( p )
    {
      case 1 : search ( 0 );     break;
      case 2 : input ();         break;
      case 3 : lend ();          break;
      case 4 : correct_data ();  break;
      case 5 : delete ();        break;
      case 6 : search ( 1 );     break;
      case 7 : show_all ();      break;
      case 8 : print ();         break;
      case 9 : del_data_file (); break;
    }
  }
  end_prg ();
}
