/*  Prg.-Name    : CFLOPPY.C
    Programierer : Ralf Bensmann
    Auffruf      : CFLOPPY {[A:][B:]}
    Compiler     : Turbo-C 2.0
*/

#include <dos.h>
#include <bios.h>
#include <stdlib.h>
#include <conio.h>

#include "c:\prg\c\cfloppy.h"

struct DBB
  {
    char x1[2];
    char x2;
    char Kennung[8];
    int BytesPerSector;
    char SectorsPerCluster;
    int ResSectors;
    char Fats;
    int RootDirEntries;
    int Sectors;
    char MediaID;
    int SectorsPerFAT;
    int SectorsPerTrack;
    int Heads;
    int HiddenSectors;
    char BootPGM[483];
  };
  struct DDP_Block
  {
     char ParmBlock[10];
  };
  struct DDPB_Ptr
/* Diskette Drive Parameter-Block Ptr */
  {
     struct DDP_Block far *ptr2;
  };

char *buffer;
struct DDPB_Ptr far *ptr1 = MK_FP(0,0x78);

struct DBB *DiskBootBlock;

int drive,head,track,sector,nsects,cyls;
int i,n,terrors,perrors;
int status;
char antwort,eflag;

unsigned char BiosDiskRead
	      (lw,head,track,sector,
	       nsects,buffer)
char lw;
int head,track,sector,nsects;
char *buffer;
{
   union REGS rg;
   struct SREGS seg;
   segread(&seg);
   rg.h.al=nsects;
   rg.h.ah=0x2;
   rg.h.cl=sector;
   rg.h.ch=track;
   rg.h.dl=lw;
   rg.h.dh=head;
   rg.x.bx=FP_OFF(buffer);
   seg.es =FP_SEG(buffer);
   int86x(0x13,&rg,&rg,&seg);
   if (rg.x.cflag !=0) {
     return (rg.h.ah);
   }
   return(0);
}

int ResetDisk()
{
   union REGS rg;
   struct SREGS seg;
   segread(&seg);
   rg.h.ah=0x0;
   int86x(0x13,&rg,&rg,&seg);
   return (rg.h.al);
}

void PutIndent()
{
   clrscr();
   cputs
("\n\rÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ"
     "ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»\n\r"
     "º  CFLOPPY   -   Lesepruefung von Floppy-"
     "Disks                 R. Bensmann  º\n\r"
     "ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ"
     "ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼"
     "\n\r");
}

void Helptext()
{
   PutIndent();
   cputs
   ("\n\rProgrammaufruf   : cfloppy [A:][B:] ?"
    "\n\r-----------------           ³   ³"
    "   ³"
    "\n\r                  Laufwerk ÄÁÄÄÄÙ"
    "   ³"
    "\n\r                 (A: ohne Angabe)"
    "   ³"
    "\n\r                 Hilfetext ÄÄÄÄÄÄ"
    "ÄÄÄÙ\n"
    "\n\rProgrammfunktion: Zun„chst wird fe"
    "stgestellt, ob sich eine"
    "  DOS-Diskette "
    "\n\r----------------- in Laufwerk A:"
    " oder B: befindet.\n"
    "\n\r                  Ist das der Fall"
    ", so wird diese spurweise gelesen."
    "\n\r                  Tritt dabei ein "
    "Fehler auf, wird dieser als tempor„rer"
    "\n\r                  Lesefehler"
    " notiert, und die Sektoren dieser Spur"
    "\n\r                  werden einzeln"
    " berprft."
    "\n\r                  Tritt dabei"
    " ein weiterer Fehler auf, so ergibt"
    " sich"
    "\n\r                  daraus die"
    " Notierung eines permanenten"
    " Fehlers.\n"
    "\n\r                  Ein entsprechend"
    "es Protokoll wird am"
    " Bildschirm angezeigt."
    "\n\n\rweiter eine Taste drcken...");
    antwort=getch();
    exit(0);
}

void WriteFehler(status)
int status;
{
   switch (status) {
   case 0:
     break;
   case 1:
     cputs("-- ungltiger Befehl!\n");
     break;
   case 2:
     cputs
     ("-- Adreámarke nicht gefunden!\n");
     break;
   case 3:
     cputs
     ("-- Schreibschutz ist ein!\n");
     break;
   case 4:
     cputs
     ("-- Sektor nicht gefunden!\n");
     break;
   case 6:
     cputs
     ("-- Diskette gewechselt!\n");
     break;
   case 7:
     cputs
     ("-- Fehler beim Initalisieren!\n");
     break;
   case 8:
     cputs
     ("-- šberlauf im DMA-Controller!\n");
     break;
   case 9:
     cputs
     ("-- Segmentgrenze berschritten!\n");
     break;
   case 0x0C:
     cputs
     ("-- Media Type nicht gefunden!\n");
     break;
   case 0x10:
     cputs
     ("-- Lesefehler, ungltiger CRC!\n");
     break;
   case 0x20:
     cputs
     ("-- Controller Fehler!\n");
     break;
   case 0x40:
     cputs
     ("-- Seek Operation fehlerhaft\n");
     break;
   case 0x80:
     cputs
     ("-- Laufwerk nicht bereit!\n");
     break;
   default:
      cputs
      ("-- Fehlernummer unbekannt!\n");
   }
}

void GetBuffers()
{
   DiskBootBlock=(struct DBB *)malloc
		 (sizeof(struct DBB));
   if (DiskBootBlock == NULL) {
      cputs
("\nKein Speicherplatz fr DiskBootBlock");
      exit(20);
   }
   buffer=malloc(512*27);
   if (buffer == NULL) {
      cputs
      ("\nKein Speicherplatz fur buffer");
      exit(20);
   }
}

void Cfloppy()
{
   PutIndent();
   cprintf("\nLaufwerk <%c:> ",drive+65);
   status=BiosDiskRead
	  (drive,0,0,1,1,DiskBootBlock);
   if (status==6) {
      status=BiosDiskRead
	    (drive,0,0,1,1,DiskBootBlock);
   }
   if (status!=0) {
      cputs("\r\n-- >Fehler ");
      WriteFehler(status);
      return(20);
   }
   ResetDisk();
   if ((DiskBootBlock->SectorsPerTrack <=0)
    || (DiskBootBlock->SectorsPerTrack >27)
    || (DiskBootBlock->Heads <=0)
    || (DiskBootBlock->Heads > 2)
    || (DiskBootBlock->Sectors<=0))
   {
      cputs
      ("\nKeine DOS-Diskette, Abbruch");
      return(20);
   }
   nsects=DiskBootBlock->SectorsPerTrack;
   cyls = DiskBootBlock->Sectors /
	  DiskBootBlock->Heads   /
	  DiskBootBlock->SectorsPerTrack;
   cprintf
   ("Parameter: Spuren=%d, K”pfe: %d,"
    " Sektoren/Spur = %d\n\r",
     cyls,DiskBootBlock->Heads,nsects);
   cprintf
   ("\rSpur: %2d, Kopf: %d",track,head);
   ResetDisk();
   ptr1->ptr2->ParmBlock[4] =
   DiskBootBlock->SectorsPerTrack;
   for (i=0;i<cyls*DiskBootBlock->Heads;
	i++ )
      {
      status = BiosDiskRead
      (drive,head,track,sector,nsects,buffer);
      if (status!=0) {
      cprintf
      ("\nTemp Diskfehler, Return Code"
      " : %X",status);
      terrors++;
      status = 0;
      for (status=1;
	   sector<DiskBootBlock->
	   SectorsPerTrack+1;
	   sector++ );
      {
	 status = BiosDiskRead
		  (drive,head,track,
		  sector,1,buffer);
		  if (status > 0) {
		     cprintf
		     ("\r -- Spur: %2d Kopf:"
		      " %d Sektor: %2d ",
		      head,track,sector);
		     WriteFehler(status);
		     eflag = 1;
		     perrors++;
		  }
	 }
	 sector = 1;
      }
      head++;
      if (head > (DiskBootBlock->Heads-1)) {
	  head = 0;
	  if (track > cyls-1) {
	     if (eflag) {
		cputs
		("\n\r>>> Diskette"
		 " berprfen,");
		cprintf
		("  %4d perm. Lesefehler"
		 " aufgetreten! <<<",
		 perrors);
		cprintf("\n\r            "
		"              %4d temp. "
		"Lesefehler aufgetreten!"
		" <<<",terrors);
	     } else {
		if (terrors) {
		   cputs
		   ("\n\r>>> Diskette"
		   " berprfen,");
		   cprintf
		   ("  %4d temp. Lesefehl"
		   "er aufgetreten !<<<",
		   terrors);
		} else {
		cputs
		("\n\r>>> Diskette ohne"
		"Lesefehler <<<");
		}
	     }
	     cputs("\n\r*** CFLOPPY  -"
	     "Programmende ***");
	     return(0);
	  }
      }
      cprintf("\rSpur: %d, Kopf: %d     "
      "	        	  ",track,head);
   }
}

main(argc, argv, envp)
   int argc;
   char *argv[];
   char *envp[];
{
   drive  = 0;
   if (argc > 1) {
      if (argv[1][0] == 'b') drive = 1;
      if (argv[1][0] == 'B') drive = 1;
      if (argv[1][0] == '?') Helptext();
   }
   GetBuffers();
   do {
      head    = 0;
      track   = 0;
      sector  = 1;
      eflag   = 0;
      terrors = 0;
      perrors = 0;
      Cfloppy();
      cputs
      ("\n\rSoll im gleichen Laufwerk"
      " eine weitere Diskette"
      " geprft werden J/N ");
      antwort = getch();
   } while ((antwort == 'j') ||
	    (antwort == 'J'));
}


