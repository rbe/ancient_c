#include <bios.h>
#include <stdio.h>
#include <ctype.h>

/* Makro zur Einsicht zu bestimmter Speicheradresse */
#define peek( adr )        (* (unsigned char far *) adr)
#define poke( adr )        (* (unsigned char far *) adr)
main()
{
   long int i;
   int kread = _NKEYBRD_READ;
   int kready = _NKEYBRD_READY;

   /* Wenn Bit 4 des Byte bei 0x0040:0x0096 geladen ist, liegt die neue
    * Tastatur vor.
    */

   /* Tasten einlesen und wiedergeben, bis die ESC-Taste gedrckt wird. */
   while( 1 == 1 )
   {
 /*     while( _bios_keybrd( kready ) )
	 _bios_keybrd( kread );
      taste = _bios_keybrd( kread );   */

      if( peek( 0x0040001A ) == peek( 0x0040001C ) )
	printf("\nKeine Taste gedrckt. ");
      else
	{
	printf("\nTaste gedrckt.");
	  for( i = 1; i< 35000; i++ );
	  poke( 0x0040001A ) = poke( 0x0040001C );
	  for( i = 1; i< 35000; i++ );
	/*_bios_keybrd( kread );   */
	}

     }
}

