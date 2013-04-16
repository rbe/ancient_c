#include <stdio.h>
#include <dir.h>
#include <process.h>
#include <string.h>
#include <dos.h>
#define BACK(n) {int i=n; while(i--) printf("\b \b");}
char path[MAXPATH],drive[MAXDRIVE],dir[MAXDIR],name[MAXFILE+MAXEXT],
     ext[MAXEXT];
main(int argc, char *argv[])
{
  if (argc<2)
    exit(printf("Aufruf: RUN [Lw:][Pfad\\]Programm"
		"[Parameter]\n"));
    fnsplit(strupr(argv[1]),drive,dir,name,ext);
    printf("%s%s",drive,dir);
    search();
    printf("\r%s nicht gefunden\n",argv[1]);
}
search(void)
{
  struct ffblk f;
  int done;
  char *dirend;

  if (*ext)
    check(ext);
  else {
    check(".com");
    check(".exe");
  }
  fnmerge(path,drive,dir,"*.*",0);
  dirend=dir+strlen(dir);
  done=findfirst(path,&f,0x10);
  while(!done) {
    if(*f.ff_name!='.' && f.ff_attrib&0x10) {
      strcpy(dirend,f.ff_name);
      strcat(dirend,"\\");
      printf("%s",dirend);
      search();
      BACK(strlen(dirend));
      *dirend=0;
    }
    done=findnext(&f);
  }
  return;
}
check(char *this_ext)
{
  struct ffblk f;

  fnmerge(path,drive,dir,name,this_ext);
  if(!findfirst(path,&f,0x27)) {
    fnmerge(path,drive,dir,f.ff_name,0);
    printf("\r%s\n",path);
    exit(execv(path,_argv+1));
  }
}
