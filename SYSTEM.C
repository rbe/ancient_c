#include <stdio.h>
#include <direct.h>
#include <string.h>
#include <dos.h>
#include <time.h>
#include <def.c>

char command[1000], direc[1000], userid[20], password[9];

char *list_of_users[2][2] = {"ralf bensmann","developer",
			     "rb",           "dv2"};
char user[100];

int maxusers = 2;

int execute_system_command = FALSE;
int execute_shell_command = FALSE;
int execute_disk_command = FALSE;
int LOGOFF = FALSE;
int done_nothing = FALSE;

char *prg_name = "OPSfP";
float prg_ver = 1.5;

struct diskinfo
{
  unsigned laufw;
  struct diskfree_t laufwinfo;
  struct find_t find;
  char disk_name[100];
  long disk_avail;
} di;

void get_disk_information ()
{

  _dos_getdrive ( &di.laufw );
  _dos_getdiskfree ( di.laufw, &di.laufwinfo );

  if ( !_dos_findfirst( "\*.*", _A_VOLID, &di.find ) )
    strcpy ( di.disk_name, di.find.name );
      else strcpy ( di.disk_name, "[NOT IDENTIFIED]" );

  di.disk_avail = (long)di.laufwinfo.avail_clusters *
		  di.laufwinfo.sectors_per_cluster *
		  di.laufwinfo.bytes_per_sector;
}

void display_disk_information ()
{
  get_disk_information ();
  printf ( "\t\t\t\tDISK INFORMATION\n" );
  printf ( "\nDrive %c: ", 'A' + di.laufw - 1 );

  if ( di.laufwinfo.total_clusters == -1 ) printf ( "CD-ROM" );

  printf ( "\n\tName               : %s", di.disk_name );
  printf ( "\n\tBytes available    : %ld", di.disk_avail );

  if ( di.disk_avail == 0 ) printf ( "\n\tPer cent free/used : 0 / 100" );
    else printf ( "\n\t%% free/used        : %.2f%% / %.2f%%",
    (float)di.laufwinfo.total_clusters / di.laufwinfo.avail_clusters,
    100 - (float)di.laufwinfo.total_clusters / di.laufwinfo.avail_clusters );

  printf ( "\n\tClusters available : %d", di.laufwinfo.avail_clusters );
  printf ( "\n\tClusters total     : %d", di.laufwinfo.total_clusters );
  printf ( "\n\tBytes per sector   : %d", di.laufwinfo.bytes_per_sector );
  printf ( "\n\tSectors per cluster: %d", di.laufwinfo.sectors_per_cluster );
  printf ( "\n\n\tUser of disk       : %s", user );
  printf ( "\n" );
}

void command_shell ()
{
  char *date, *time_of_day;

  do
  {
    /* done_nothing auf FALSCH setzten, da sonst nichts passiert */

    done_nothing = FALSE;
    execute_shell_command = FALSE;

    /* wenn Befehl ausgefhrt, Speicherplatz angeben, sonst nicht */

    if ( execute_disk_command == TRUE )
    {
      execute_system_command = FALSE;
      execute_disk_command = FALSE;
    }

    /* aktuelles Verzeichnis ausgeben */

    getcwd ( direc, 1024 );
    printf ( "\nworking directory is %s.\n-> ", direc );

    /* auf Eingabe warten */

    gets ( command );

    /* shell-Befehle ausfhren */

    if ( strcmp ( command, "logoff" ) == 0 )
    {
      LOGOFF = TRUE;
      execute_system_command = FALSE;
      execute_shell_command = TRUE;
      execute_disk_command = FALSE;
    }

    if ( strcmp ( command, "wd" ) == 0 )
    {
      execute_system_command = FALSE;
      execute_shell_command = TRUE;
      execute_disk_command = TRUE;
      printf ( "\n%s\n", direc );
    }

    if ( strcmp ( command, "dt" ) == 0 )
    {
      execute_system_command = FALSE;
      execute_shell_command = TRUE;
      execute_disk_command = FALSE;
      _strdate ( date );
      _strtime ( time_of_day );
      printf ( "\ntoday's date: %s\ntoday's time: %s\n", date, time_of_day );
    }

    if ( strcmp ( command, "diskinfo" ) == 0 )
    {
      execute_system_command = FALSE;
      execute_shell_command = TRUE;
      execute_disk_command = TRUE;
      display_disk_information ();
    }

    /* ENTER, nichts machen */

    if ( strcmp ( command, "" ) ==0 )
    {
      done_nothing = TRUE;
      execute_system_command = FALSE;
      execute_shell_command = FALSE;
      execute_disk_command = FALSE;
      printf ( "\ndone nothing.\n" );
    }

    /* system-Befehl ausfhren */

    if ( LOGOFF == FALSE && done_nothing == FALSE
	 && execute_shell_command == FALSE)
    {
      system ( command );
      execute_system_command = TRUE;
      execute_shell_command = FALSE;
      execute_disk_command = FALSE;
    }
  }
  while ( LOGOFF == FALSE );
}

void init ()
{
  int i;

  printf ( "\nsystem is ready for a new user.\n-> " );
  gets ( command );
  if ( strcmp ( command, "logon" ) == 0 )
  {
    printf ( "\nuser id: " );
    gets ( userid );

      for ( i = 0; i < maxusers; ++i )
	if ( strcmp ( userid, list_of_users[i][0] ) == 0 )
	{
	  printf ( "\npassword: " );
	  gets ( password );
	  if ( strcmp ( password, list_of_users[i][1] ) == 0 )
	  {
	    printf ( "\n\t\t\tYou're welcome, %s!\n", list_of_users[i][0] );
	    printf ( "\n%s ver %.2f ready to operate.\n\n", prg_name,
		      prg_ver );
	    strcpy ( user, list_of_users[i][0] );
	    return;
	  } else init ();
	} if ( i == maxusers ) init ();
  } else init ();
}

void turn_off ()
{
  int i;
  long j;

  printf ( "\n logging off" );

  for ( i = 0; i < 3; ++i )
  {
    printf ( "." );
    for ( j = 0; j < 1000000; ++j ) ;
  }

  printf ( " \n Now you can turn off your machine or wait and \"C\"..." );
}

void main()
{
  init ();

  command_shell ();

  turn_off ();
}
