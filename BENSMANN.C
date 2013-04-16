#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <direct.h>
#include <graph.h>

/* -----------------------------Allgemeine Def.---------------------------- */

char c;
int i;
struct rccoord where;

#define FALSE    -1
#define TRUE     !FALSE
#define N        NULL
#define OKAY     0
#define FAILED   !OKAY
#define PRG_ENDE -10

#define readkey  c=getch()
#define upcase   c=toupper(c);
#define downcase c=tolower(c);
#define clrscr() _clearscreen(0);
#define gotoxy   _settextposition
#define STR_LEN  255;

struct diskfree_t D_FREE;

/* -----------------------------SYS_ERRLIST-------------------------------- */

#define NO_ERROR           	  0
#define	INVALID_FUNCTION_NR    	  1
#define NO_FILE_OR_DIR      	  2
#define PATH_NOT_FOUND       	  3
#define TOO_MANY_OPEN_FILES 	  4
#define PERMISSION_DENIED   	  5
#define BAD_FILE_NUMBER           6
#define MEM_ARENA_TRASHED   	  7
#define NOT_ENOUGH_MEMORY   	  8
#define INVALID_MEMORY_BLOCK_ADR  9
#define INVALID_ENVIROMENT       10
#define INVALID_FORMAT           11
#define INVALID_ACCESS_CODE      12
#define INVALID_DATA             13
#define NO_SUCH_DEVICE           15
#define REMOVE_CURRENT_DIR       16
#define NOT_SAME_DEVICE          17
#define NO_MORE_FILES            18
#define INVALID_ARGUMENT         19
#define ARGUMENT_LIST_TOO_BIG    20
#define EXEC_FORMAT_ERROR        21
#define CROSS_DEVICE_LINK        22


/* -------------------------------Tastatur-Def.---------------------------- */

#define ENTER  0x0D
#define ESC    0x1B
#define PGUP   0x49
#define PGDOWN 0x51
#define UP     0x48
#define DOWN   0x50
#define LEFT   0x4B
#define RIGHT  0x4D

/* -------------------------------MOUSE-Def.------------------------------- */

int MOUSE_X, MOUSE_Y,
    M_BILDX, M_BILDY,
    MOUSE_BUTTON;

#define NO_MOUSE       -1
#define MOUSE_OK       1
#define MOUSE          1
#define M_MIDDLE_X     312
#define M_MIDDLE_Y     88
#define M_BUTTONLEFT   1
#define M_BUTTONRIGHT  2
#define M_BUTTONLR     3
#define M_BUTTONMIDDLE 4
#define M_BUTTONLM     5
#define M_BUTTONMR     6
#define M_BUTTONALL    7

/* --------------------------------INTERRUPT-Def.-------------------------- */

#define VIDEO_INT         0x10
#define EQUIP_INT         0x11
#define MEM_INT           0x12
#define DISK_INT          0x13
#define SER_INT           0x14
#define SYS_INT           0x15
#define KEYB_INT          0x16
#define PRN_INT           0x17
#define BOOT_INT          0x19
#define DOS_TERMINATE     0x20
#define DOS_INT           0x21
#define ABS_DISKREAD_INT  0x25
#define ABS_DISKWRITE_INT 0x26
#define TSR_INT           0x27
#define MOUSE_INT         0x33

union REGS r;

/* ----------------------------------STRING-Def.--------------------------- */

char COPYRIGHT          []="Copyright (C) 1994 Ralf Bensmann\n";
char FILE_NOT_FOUND     []="Datei \"%s\" nicht gefunden!\n";
char CANNOT_OPEN        []="Konnte Datei \"%s\" nicht ”ffnen!\n";
char UNKOWN_ERROR       []="Unbekannter Fehler!\n";
char NO_MOUSE_INSTALLED []="Keine Maus angeschlossen bzw. kein Maustrieber\
 installiert!!\n";
char WRONG_DOS          []="Falsche Dosversion (%.2f)!\n";

/* -----------------------------------POPUP-------------------------------- */

char *mpkt[1000], ch;
int  INTE, PAINT, MAXI,
     CHOOSEN;
long MFORE, MBACK;


/* ------------------------allgemeine-FUNKTIONEN--------------------------- */

void
taste (int X, int Y)
{
  _settextposition(X,Y);
  printf("Taste...");
  readkey;
}

int
keypressed ()
{
static int KEY_PRESSED;
  while (KEY_PRESSED==0) KEY_PRESSED=kbhit();
  return KEY_PRESSED;
}

/*
char
*copystr (char STR[10000], int von, int bis)
{
static char hilf[10000], hilf2[10000];
  strcpy(hilf2,STR);
  von--;
  hilf2[bis]='';
  strrev(hilf2);
  hilf2[bis-von]='';
  strrev(hilf2);
  strcpy(hilf,hilf2);
  return hilf;
}
*/

void
cursor_off ()
{
  r.h.ah=1;
  r.h.ch=255;
  r.h.cl=255;
  int86(VIDEO_INT, &r, &r);
}

void
cursor_on ()
{
  r.h.ah=1;
  r.h.ch=5;
  r.h.cl=7;
  int86(VIDEO_INT, &r, &r);
}

void
setcur (int CUR_VON, int CUR_BIS)
{
  r.h.ah=1;
  r.h.ch=CUR_VON;
  r.h.cl=CUR_BIS;
  int86(VIDEO_INT, &r, &r);
}

char
*ext_keyb ()
{
char *TASTE;
  r.h.ah=2;
  int86(KEYB_INT, &r, &r);
  switch(r.h.al)
  {
/*
    case 1   :TASTE="Shift rechts"; break;
    case 2   :TASTE="Shift links"; break;
    case 4   :TASTE="STRG"; break;
    case 8   :TASTE="ALT"; break;
*/
    case 16  :TASTE="         ROLLEN"; break;
    case 32  :TASTE="NUM"; break;
    case 48  :TASTE="NUM      ROLLEN"; break;
    case 64  :TASTE="    CAPS";break;
    case 80  :TASTE="    CAPS ROLLEN"; break;
    case 96  :TASTE="NUM CAPS"; break;
    case 112 :TASTE="NUM CAPS ROLLEN"; break;
    case 128 :TASTE="INSERT"; break;
  }
  return TASTE;
}
/*
  char *c;
  c=EXT_KEYB();
  printf("%s",c);
*/

void
init_prn (int COM)
{
  r.h.ah=1;
  r.x.dx=COM;
  int86(PRN_INT, &r, &r);
  if (r.h.ah<7) printf("DRUCKER-FEHLERCODE: %d\n",r.h.ah);
}

void
boot (char *TEXT)
{
  printf("%s",TEXT);
  int86(BOOT_INT, &r, &r);
}

void
error (char *ERROR_STRING, int ERROR_NUMBER)
{
  if (strcmp(ERROR_STRING,"")!=0)
    printf("Programfehler %2d: %s\n(systemerror  %2d: %s)",ERROR_NUMBER,
	    ERROR_STRING,errno,sys_errlist[errno]);
  else printf("systemerror  %2d: %s",errno,sys_errlist[errno]);
  where=_gettextposition();
  taste(1,where.col+1);
}

int
vcheck (char FNAME[1000], long int FLENGTH)
{
long len;
int vc;
  if((vc=open(FNAME, 1))==FAILED)
  {
    printf("konnte virenprfung nicht durchfhren!");
    return FAILED;
  }
  len=filelength(vc);
  if (len<0) return FAILED;
  if (len!=FLENGTH)
  {
    printf("\nvirenprfung positiv!\ndatei ist wahrscheinlich von einem\
 virus befallen!\n");
    printf("programm trotzdem starten ? "); readkey; upcase;
    printf("\n");
    if (c!='J') abort(); else return OKAY;
  }
  return OKAY;
}

int
disk_free (unsigned char DRIV)
{
static int a;
  _dos_getdiskfree(DRIV,&D_FREE);
  a=D_FREE.avail_clusters;
  if (a==-1) return FALSE;
    else return D_FREE.total_clusters*2;
}

/*
char
*disk_free_str (unsigned char DRIV)
{
static long a;
static char *FREE, b[100], *ch, *d;
  ch=""; d="";
  a=disk_free(DRIV);
  ltoa(a,b,10);
  switch (strlen(b))
  {
    case 5 :strncat(ch,b,2); break;
    case 6 :strncat(ch,b,3); break;
    case 7 :strncat(ch,b,1); break;
    case 8 :strncat(ch,b,2); break;
  }
  strcat(ch,",");
  strrev(b);
  switch (strlen(b))
  {
    case 5 :{ b[4]=''; b[3]=''; } break;
    case 6 :{ b[4]=''; b[4]=''; b[4]=''; } break;
    case 7 :b[6]=''; break;
    case 8 :{ b[7]=''; b[7]=''; } break;
  }
  strrev(b);
  for (a=0; a<strlen(b); a=a+3);
  {
    strncat(ch,b,3);
    b[0]=''; b[1]=''; b[1]='';
  }
  strcpy(b,ch);
  if (a<10) { FREE="       "; strcat(FREE,b); }
  else if (a>9) { FREE="      "; strcat(FREE,b); }
  else if (a>99) { FREE="     "; strcat(FREE,b); }
  else if (a>999) { FREE="    "; strcat(FREE,b); }
  else if (a>9999) { FREE="   "; strcat(FREE,b); }
  return FREE;
}
*/

char
*dosversion ()
{
static char a[2], b[2], *VERSION;
  VERSION="";
  itoa(_osmajor,a,10);
  itoa(_osminor,b,10);
  strcpy(VERSION,a);
  strcat(VERSION,".");
  strcat(VERSION,b);
  return VERSION;
}

float
dosver ()
{
static float a;
  a=_osmajor+(_osminor*0.01);
  return a;
}

/*-----------------------------------MOUSE----------------------------------*/

int
init_mouse ()
{
static int a;
  r.x.ax=0;
  int86(MOUSE_INT, &r, &r);
  a=r.x.ax;
  if (a!=-1) return NO_MOUSE; else return MOUSE_OK;
}

void
mouse ()
{
static int a;
  a=init_mouse();
  if (a==NO_MOUSE) {
    printf(NO_MOUSE_INSTALLED);
    exit(1);
  }
}

void
mouse_on ()
{
  r.x.ax=1;
  int86(MOUSE_INT, &r, &r);
}

void
mouse_off ()
{
  r.x.ax=2;
  int86(MOUSE_INT, &r, &r);
}

void
mouse_status ()
{
  r.x.ax=3;
  int86(MOUSE_INT, &r, &r);
  MOUSE_BUTTON=r.x.bx;
  MOUSE_X=r.x.cx;
  MOUSE_Y=r.x.dx;
}

void
set_mouse_box (int M_MAX_X1, int M_MAX_Y1, int M_MAX_X2, int M_MAX_Y2)
{
  r.h.ah=7;
  r.x.cx=M_MAX_X1;
  r.x.dx=M_MAX_X2;
  int86(MOUSE_INT, &r, &r);
  r.h.ah=8;
  r.x.cx=M_MAX_Y1;
  r.x.dx=M_MAX_Y2;
  int86(MOUSE_INT, &r, &r);
}

void
set_mouse_xy (int M_X, int M_Y)
{
  r.x.ax=4;
  r.x.cx=M_X;
  r.x.dx=M_Y;
  int86(MOUSE_INT, &r, &r);
}

void
set_mouse_speed (int X_SPEED, int Y_SPEED)
{
  r.x.ax=15;
  r.x.cx=X_SPEED;
  r.x.dx=Y_SPEED;
  int86(MOUSE_INT, &r, &r);
}

void
change_mouse_coord ()
{
  M_BILDX=(MOUSE_X/8)+1;
  M_BILDY=(MOUSE_Y/8)+1;
}

/*-----------------------------------POPUP----------------------------------*/

void
colored()
{
  _settextcolor(MFORE);
  _setbkcolor(MBACK);
}

void
uncolored()
{
  _settextcolor(7);
  _setbkcolor(*(long*)1);
}

void
dodown()
{
  _settextposition(1,PAINT-1);
  uncolored(); if(PAINT!=1) cprintf("%s",mpkt[PAINT-1]);
  _settextposition(1,PAINT);
  colored(); cprintf("%s",mpkt[PAINT]);
  uncolored();
}

void
doup()
{
  if(PAINT==MAXI)
  {
    PAINT=1;
    _settextposition(1,PAINT);
    uncolored(); cprintf("%s",mpkt[PAINT]);
    _settextposition(1,MAXI);
    colored(); cprintf("%s",mpkt[MAXI]);
    PAINT=MAXI;
  }
  else
  {
    _settextposition(1,PAINT+1);
    uncolored();
    cprintf("%s",mpkt[PAINT+1]);
    _settextposition(1,PAINT);
    colored();
    cprintf("%s",mpkt[PAINT]);
  }
  uncolored();
}

int
pop_up (int MAX, int POPUP_ENDE, int MOUSE_ENDE)
{
static int k;
  cursor_off();
  MAXI=MAX;
  for (INTE=1; INTE<MAX+1; INTE++) {
    _settextposition(1,INTE);
    cprintf("%s",mpkt[INTE]);
  }
  _settextposition(1,PAINT);
  colored();
  cprintf("%s",mpkt[PAINT]);
  uncolored();
  while (ch!=ESC) {
    k=kbhit();
    if (k!=0) ch=getch();
    else {
      mouse_status();
      change_mouse_coord();
      if (M_BILDY==MOUSE_ENDE && MOUSE_BUTTON==M_BUTTONMIDDLE)
      {
	cursor_on();
	return POPUP_ENDE;
      }
    }
    _settextposition(1,25); printf("%d",k);
    switch(ch)
    {
      case UP   : {
		    PAINT--;
		    if (PAINT<1) {
		     PAINT=MAX; _settextposition(1,MAX-MAX+1); uncolored();
		     cprintf("%s",mpkt[1]);
		    }
		    doup();
		  } break;
      case DOWN : {
		    PAINT++;
		    if (PAINT>MAX) {
		      PAINT=1; _settextposition(1,MAX); uncolored();
		      cprintf("%s",mpkt[MAX]);
		    }
		    dodown();
		  } break;
      case ENTER: { CHOOSEN=PAINT; cursor_on(); return CHOOSEN; }
      case ESC  : { CHOOSEN=POPUP_ENDE; cursor_on(); }
      default :;
    }
  }
  PAINT=CHOOSEN;
  cursor_on();
  return CHOOSEN;
}

/*
POPUP BEISPIEL:
---------------
  CHOOSEN = gew„hlte Menpunkt
  choose (param1, param2) = Prozedure; param1=Menge der Menpunkte
			    param2=END-Menpunkt
  PAINT=1; ==> INITaliaierung notwendig; nur einmal

void main()
{
  PAINT=1;
  MFORE=0; MBACK=15;
  while (CHOOSEN<3) {
    clrscr();
    mpkt[1]="PUNKT 1";
    mpkt[2]="PUNKT 2";
    mpkt[3]="ENDE DES MENUES!";
    popup(3,3);
    switch(CHOOSEN)
    {
      case 1 :printf("puh!"); break;
      case 2 :printf("p2!"); break;
    }
  }
}
*/
