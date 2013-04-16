#include <malloc.h>

int i;
int *z;

main()
{
  z=(int*)malloc(sizeof(i));
  z=&i;
  for (i=0; i<256; i++) /* printf("%4i:%3c",*z,*z); */ printf("%2c",*z);
  free(z);
}
