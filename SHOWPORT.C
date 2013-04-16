#include <conio.h>
#include <stdlib.h>

main(int argc, char *argv[])
{
int port, value;
  port=atoi(argv[1]);
  value=inp(port);
  printf("\n\nport: %4Xh, value: %4Xh\n\n",port,value);
}
