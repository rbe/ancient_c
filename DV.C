#include <def.c>
#include <dos2.c>

void main()
{
float dver;
  dver=dosver();
  clrscr();
  if (dver<6.1) exit(printf(WRONG_DOS,dver));
  printf("DOS-Version:\n");
  printf("Dez: %.2f\n",dosver());
  printf("Str: %s\n",dosversion());
}
