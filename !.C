/*  +---------------------------------------------------------------+
    |  !.C ("not")  -  versteckt Dateien vor DOS-Befehlen           |
    +----------------------------------------------------------------
    fÅr Turbo C ab Version 1.0, beliebiges Speichermodell          */

#include <stdio.h>
#include <stdlib.h>
#include <dir.h>
#include <process.h>

struct ffblk ff;
char drive[MAXDRIVE],dir[MAXDIR],name[MAXFILE],ext[MAXEXT];

main(int argc, char *argv[])
{
  if(argc<3)
    printf("Aufruf: ! Dateien Kommando [Parameter ....]");
    else {
      fnsplit(argv[1],drive,dir,name,ext);
      findfirst(argv[1],&ff,0x10);
      argv[0]=getenv("COMSPEC");
      argv[1]="/c";
      find(argv);
    }
}

find(char *arg[])
{
  char file[MAXPATH],attr;
  fnmerge(file,drive,dir,ff.ff_name,0);
  attr=_chmod(file,0)&0xef;
  _chmod(file,1,attr|3);
  if(findnext(&ff))
    spawnv(P_WAIT,arg[0],arg);
  else
    find(arg);
  _chmod(file,1,attr);
}
