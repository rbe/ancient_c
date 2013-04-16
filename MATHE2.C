#include <stdio.h>
#include <math.h>

char c;

void xhochy()
{
  int basis, exponent;
  printf ( "Basis   : " );
  scanf ( "%d", &basis );

  printf ( "Exponent: " );
  scanf ( "%d", &exponent );

  printf ( "\n: %d hoch %d =\n%.50f", basis, exponent,
				     pow ( basis, exponent ) );
  printf ( "\nTaste..." );
  c = getch();
}

void exponential()
{
  float x;
  printf ( "Zahl x: " );
  scanf ( "%f", &x );

  printf ( "\nExponentialfunktion von %f =\n%.50f", x, exp ( x ) );
  printf ( "\nTaste..." );
  c = getch();
}

void nat_logarithmus()
{
  float x;
  printf ( "Zahl x: " );
  scanf ( "%f", &x );

  printf ( "\nnatÅrlicher Loagrithmus von %f =\n%.50f", x, log ( x ) );
  printf ( "\nTaste..." );
  c = getch();
}

void logarithmus_10()
{
  float x;
  printf ( "Zahl x: " );
  scanf ( "%f", &x );

  printf ( "\nLogarithmus von %f zur Basis 10 =\n%.50f", x, log10 ( x ) );
  printf ( "\nTaste..." );
  c = getch();
}

void main()
{
  int wahl;
  while ( wahl != 5 )
  {
    printf ( "\n\n" );
    printf ( "\n(1) x hoch y" );
    printf ( "\n(2) Exponentialfunktion" );
    printf ( "\n(3) Logarithmus zur Basis 10" );
    printf ( "\n(4) natÅrlicher Logarithmus" );
    printf ( "\n(5) Ende" );
    printf ( "\n\neingabe: " );
    scanf ( "%d", &wahl );
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
