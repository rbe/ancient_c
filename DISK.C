long disk_free ( unsigned char DRIV )
{
  struct diskfree_t D_FREE;

  _dos_getdiskfree ( DRIV, &D_FREE );

  if ( D_FREE.avail_clusters == -1 ) return FALSE;
    else return (long)D_FREE.avail_clusters *
		D_FREE.sectors_per_cluster *
		D_FREE.bytes_per_sector;
}
