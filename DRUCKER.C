void unter(int a)
{
  if (a==0) _bios_printer(_PRINTER_WRITE,LPT,0x1B+0x2D+0x01);
    else _bios_printer(_PRINTER_WRITE,LPT,0x1B+0x2D+0x00);
}

