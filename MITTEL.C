#include <stdio.h>

extern int mittel(int, int, int);

main()
{
    int z1, z2, z3;
    char zahl1[10], zahl2[10], zahl3[10];
      printf("\n\n       Mittelwert von max. 3 Zahlen berechnen. " );
      printf("\n\nErste Zahl: ");
      gets( zahl1 );
      z1 = atoi( zahl1 );
      printf("\nZweite Zahl: ");
      gets( zahl2 );
      z2 = atoi( zahl2 );
      printf("\nDritte Zahl: ");
      gets( zahl3 );
      z3 = atoi( zahl3 );

      printf("\nDer Mittelwert betr„gt%d",mittel(z1,z2,z3));

}
