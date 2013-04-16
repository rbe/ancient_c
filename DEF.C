#include <graph.h>
#include <dos.h>
#include <colors.h>
/* -----------------------------Allgemeine Def.--------------------------- */

char c;
int i;
struct rccoord where;

#define FALSE    -1
#define TRUE     !FALSE
#define N        NULL
#define OKAY     0
#define FAILED   !OKAY
#define YES      100
#define NO       -100
#define PRG_ENDE -10

#define readkey  c=getch()
#define upcase   c=toupper(c);
#define downcase c=tolower(c);
#define clrscr() _clearscreen(0);
#define gotoxy   _settextposition
#define STR_LEN  255;

struct diskfree_t D_FREE;

/* -----------------------------SYS_ERRLIST------------------------------- */

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

/* -------------------------------Tastatur-Def.--------------------------- */

#define ENTER  0x0D
#define ESC    0x1B
#define PGUP   0x49
#define PGDOWN 0x51
#define UP     0x48
#define DOWN   0x50
#define LEFT   0x4B
#define RIGHT  0x4D

/* --------------------------------INTERRUPT-Def.------------------------- */

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

/* ----------------------------------STRING-Def.-------------------------- */

char RB_COPYRIGHT       []="Copyright (C) 1994 Ralf Bensmann\n";
char FILE_NOT_FOUND     []="Datei \"%s\" nicht gefunden!\n";
char CANNOT_OPEN        []="Konnte Datei \"%s\" nicht ”ffnen!\n";
char UNKOWN_ERROR       []="Unbekannter Fehler!\n";
char NO_MOUSE_INSTALLED []="Keine Maus angeschlossen bzw. kein Maustreiber\
 installiert!!\n";
char WRONG_DOS          []="Falsche Dosversion (%.2f)!\n";

/* ------------------------allgemeine-FUNKTIONEN-------------------------- */

void taste (int Y, int X)
{
  _settextposition(Y,X);
  printf("Taste...");
  readkey;
}

int keypressed ()
{
static int KEY_PRESSED;
  while (KEY_PRESSED==0) KEY_PRESSED=kbhit();
  return KEY_PRESSED;
}
