#include <def.c>
#include <disk.c>

void main()
{
  unsigned lfw;
  _dos_getdrive ( &lfw );
  printf ("laufwerk %c hat %d",'A'+lfw-1,disk_free(lfw));
}
