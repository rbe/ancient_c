#include <stdio.h>
#include <math.h>
#include <assert.h>

void main()
{
  int i, x;
  printf ( "\n2^x fÅr OS/2." );
  printf ( "\nCopyright (C) 1995 von Ralf Bensmann" );
  printf ( "\nErstellt in GNU C/C++ fÅr OS/2.");
  printf ( "\n\nGeben Sie x ein: " );
  assert ( scanf ( "%d", &x ) );
  printf ( "\n2 hoch x. x e [0; %d]\n\n", x );
  for ( i = 0; i < x+1; i++ )
    printf ( "2^%d = %.0f\n", i, pow ( 2, i ) );
}
