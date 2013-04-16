#include <stdio.h>
#include <math.h>
#include <assert.h>

char c;

void xhochy()
{
  int basis, exponent;

  printf ( "Bitte nur ganze Zahlen eingeben!\n\n" );

  printf ( "Basis   : " );
  assert ( scanf ( "%d", &basis ) );
  printf ( "Exponent: " );
  assert ( scanf ( "%d", &exponent ) );
  printf ( "\n%d hoch %d =\n\n%.50f\n\n", basis, exponent,
                                     pow ( basis, exponent ) );
}

void exponential()
{
  float x;
  printf ( "Zahl x: " );
  assert ( scanf ( "%f", &x ) );

  printf ( "\nexp ( %f ) = \n\n%.50f\n\n", x, exp ( x ) );

}

void nat_logarithmus()
{
  float x;
  printf ( "Zahl x: " );
  assert ( scanf ( "%f", &x ) );

  printf ( "\nnatÅrlicher Loagrithmus von %f =\n\n%.50f\n\n", x, log ( x ) );

}

void logarithmus_10()
{
  float x;
  printf ( "Zahl x: " );
  assert ( scanf ( "%f", &x ) );

  printf ( "\nLogarithmus von %f zur Basis 10 =\n\n%.50f\n\n", x, log10 ( x ) );

}

void main()
{
  int wahl;
  while ( wahl != 5 )
  {
    printf ( "\nMathe fÅr OS/2.");
    printf ( "\nCopyright (C) 1995 von Ralf Bensmann.");
    printf ( "\nErstellt mit GNU C/C++ fÅr OS/2.\n");
    printf ( "\n(1) x hoch y" );
    printf ( "\n(2) Exponentialfunktion" );
    printf ( "\n(3) Logarithmus zur Basis 10" );
    printf ( "\n(4) natÅrlicher Logarithmus" );
    printf ( "\n(5) Ende" );
    printf ( "\n\neingabe: " );
    assert ( scanf ( "%d", &wahl ) );
    printf ( "\n" );
    switch ( wahl )
    {
      case 1 : xhochy(); break;
      case 2 : exponential(); break;
      case 3 : logarithmus_10(); break;
      case 4 : nat_logarithmus(); break;
    }
  }
}
