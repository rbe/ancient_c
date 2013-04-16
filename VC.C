void vcheck()
{
char c;
long len;
int vc;
  if((vc=open("stoma.exe", 1))==FAILED)
    exit(printf("konnte virenprÅfung nicht durchfÅhren!"));
  len=filelength(vc);
  if (len!=48798) {
    printf("\nvirenprÅfung positiv!\ndatei ist wahrscheinlich von einem\
 virus befallen!\n");
    printf("programm trotzdem starten ? "); readkey; upcase;
    if (c!='J') abort();
  }
}

