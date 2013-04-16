char c;

void main()
{
  clrscr();
  while (c!=0x1b)
  {
    printf("ZEICHEN: ");
    c=getch();
    printf("\nSCANCODE %c: %i\n",c,c);
  }
}
