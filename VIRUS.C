int vcheck (char FNAME[1000], long int FLENGTH)
{
long len;
int vc;
  if((vc=open(FNAME, 1))==FAILED)
  {
    printf("konnte virenpr�fung nicht durchf�hren!");
    return FAILED;
  }
  len=filelength(vc);
  if (len<0) return FAILED;
  if (len!=FLENGTH)
  {
    printf("\nvirenpr�fung positiv!\ndatei ist wahrscheinlich von einem\
 virus befallen!\n");
    printf("programm trotzdem starten ? "); readkey; upcase;
    printf("\n");
    if (c!='J') abort(); else return OKAY;
  }
  return OKAY;
}

