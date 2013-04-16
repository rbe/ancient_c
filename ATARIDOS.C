/* Programm ATARIDOS.C konvertiert Textdateien von ATARI- nach */
/* DOS-Format. Dazu mÅssen diese auf einer DOS-formatierten Diskette */
/* auf dem ATARI abgespeichert worden sein (720 KB Diskette). */

#include <stdio.h>

main()
{
FILE *stream;
FILE *in;
char datei_out[67];
char datei_in[67];
int j, ch, chsp1, chsp2, chsp3, chsp4;

  printf("…ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕª" );
  printf("∫               ATARIDOS (c) 1992 Rolf Bernhardt, Tel. 02533/2174              ∫" );
  printf("∫                                                                              ∫" );
  printf("∫ Mit Hilfe dieses Programmes kînnen ATARI-Textdateien in DOS-Textdateien      ∫" );
  printf("∫ ÅberfÅhrt werden. Formatierungen, Trennungen usw. werden weitgehend aus der  ∫" );
  printf("∫ Atari-Textdatei entfernt. Ca. 98%% der unter DOS nicht nutzbaren Formatierun- ∫" );
  printf("∫ gen werden durch dieses Programm Åberschrieben. Sie mÅssen den Text nach der ∫" );
  printf("∫ Konvertierung nach eventuell verbliebenen unbrauchbaren Zeichen durchsuchen! ∫" );
  printf("∫                                                                              ∫" );
  printf("∫ Im aktuellen Pfad mu· die temporÑre Datei 'atari.tmp' speicherbar sein. Eine ∫" );
  printf("∫ eventuell existierende Datei gleichen Namens wird Åberschrieben.             ∫" );
  printf("∫ DrÅcken Sie bei den gefragten Dateinamen nur RETURN, so endet das Programm.  ∫" );
  printf("»ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕº" );
  printf("\n  Aus welcher ATARI-Datei lesen? ");
  gets( datei_out );
  if( strlen(datei_out) == 0 ) exit(0);

  printf("\n  In welche DOS-Datei schreiben? ");
  gets( datei_in );
  if( strlen(datei_in) == 0 ) exit(0);

  printf( "\n\n  %s -> %s        ", datei_out, datei_in);

  if( (stream = fopen(datei_out,"r")) == NULL)
    {
    printf("\nDatei %s konnte zum Lesen nicht geîffnet werden!", datei_out);
    exit(1);
    }
  if( (in = fopen(datei_in,"w")) == NULL)
    {
    printf("\nDatei %s konnte zum Schreiben nicht geîffnet werden!", datei_in);
    exit(1);
    }

   ch = fgetc(stream);
   if( ch == 30 )
       ch = 32;
   if( ch == 158 )
       ch = 225;

   while( !feof(stream) )
   {
   if( ( (ch > 31) || (ch == 10) ) && ( (ch != 128) && (ch != 133) ) )
	fputc( ch, in);             /* ATARI-Steuerzeichen nicht Åbersetzen */

   ch = fgetc(stream);                            /* nÑchstes Zeichen holen */
   if( ch == 30 ) ch = 32;
   if( ch == 158 ) ch = 225;
   if( ch == 27 ) ch = 13;

   if( ch == '-' )                                         /* Trennzeichen? */
     {
     chsp1 = ch;                               /* Zeichen erstmal speichern */
     ch = fgetc(stream);                          /* Noch ein Zeichen holen */
     if( ch == 30 ) ch = 32;
     chsp2 = ch;                                          /* Auch speichern */
     ch = fgetc(stream);                          /* Noch ein Zeichen holen */
     if( ch == 30 ) ch = 32;
     chsp3 = ch;                                          /* Auch speichern */
     ch = fgetc(stream);                          /* Noch ein Zeichen holen */
     if( ch == 30 ) ch = 32;
     chsp4 = ch;                                          /* Auch speichern */

     if( chsp3 == 10 || chsp3 == 29 )                          /* Folgt CR? */
	 {
	 fputc( chsp4, in );
	 ch = fgetc(stream);               /* '-' und ' ' und CR ignorieren */
	 if( ch == 30 ) ch = 32;
	 }
     else if( chsp2 == 10 )                                    /* Folgt CR? */
	 {
	 fputc( chsp3, in );
	 fputc( chsp4, in );
	 ch = fgetc(stream);               /* '-' und ' ' und CR ignorieren */
	 if( ch == 30 ) ch = 32;
	 }
     else
	 {
	 fputc( chsp1, in );
	 fputc( chsp2, in );
	 fputc( chsp3, in );
	 fputc( chsp4, in );
	 ch = fgetc( stream );
	 if( ch == 30 ) ch = 32;
	 }
     }


   if( ch == 10 )                                                    /* CR? */
     {
     chsp1 = ch;
     ch = fgetc(stream);
     if( ch == 30 ) ch = 32;
     chsp2 = ch;
     if( chsp2 != 10 )                                     /* Zeilenwechsel */
       {
       fputc( 13, in );                         /* 'Harten' Absatz ausgeben */
       fputc( chsp2, in );
       ch = fgetc( stream );
       if( ch == 30 ) ch = 32;
       }
     }


   }
   fclose(stream);
   fclose(in);

   stream = fopen(datei_in,"r");
   if( (in = fopen("atari.tmp","w")) == NULL)
    {
    printf("\nDatei 'atari.tmp' konnte zum Schreiben nicht geîffnet werden!" );
    exit(1);
    }

   chsp1 = fgetc( stream );
   chsp2 = fgetc( stream );
   if( chsp1 == 32 && chsp2 == 32 )
     {
     fputc( 32, in );
     }

   while( !feof(stream) )
     {
     chsp1 = fgetc( stream );
     chsp2 = fgetc( stream );
     if( chsp1 == 32 && chsp2 == 32 )
       {
       fputc( 32, in );
       }
     else
       {
       fputc( chsp1, in );
       fputc( chsp2, in );
       }
     }

   fclose(stream);
   fclose(in);

   stream = fopen("atari.tmp","r");
   in = fopen(datei_in,"w");

   while( !feof(stream) )
     {
     ch = fgetc(stream);
     fputc( ch, in );
     }

   fclose(stream);
   fclose(in);

   printf( "\x1B[%i;%iH",20,0 );
   printf( "  Konvertierung von %s nach %s beendet. \n\n", datei_out, datei_in);

}
