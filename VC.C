void vcheck()
{
char c;
long len;
int vc;
  if((vc=open("stoma.exe", 1))==FAILED)
    exit(printf("konnte virenpr�fung nicht durchf�hren!"));
  len=filelength(vc);
  if (len!=48798) {
    printf("\nvirenpr�fung positiv!\ndatei ist wahrscheinlich von einem\
 virus befallen!\n");
    printf("programm trotzdem starten ? "); readkey; upcase;
    if (c!='J') abort();
  }
}

