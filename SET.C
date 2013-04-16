#include <stdlib.h>
#include <graph.h>

void strich()
{
int i;
  for (i=0; i<80; i++) printf("=");
}

void main()
{
  _clearscreen(0);
  printf("PATH     =%s\n",getenv("PATH"));
  strich();
  printf("COMSPEC  =%s\n",getenv("COMSPEC"));
  strich();
  printf("PROMPT   =%s\n",getenv("PROMPT"));
  strich();
  printf("BLASTER  =%s\n",getenv("BLASTER"));
  strich();
  printf("SOUND    =%s\n",getenv("SOUND"));
  strich();
  printf("TEMP     =%s\n",getenv("TEMP"));
  strich();
  printf("FBP_USER =%s\n",getenv("FBP_USER"));
  strich();
  printf("OS/VER   =%s %s\n",getenv("OS"),getenv("VER"));
  strich();
  printf("NWDOSCFG =%s\n",getenv("NWDOSCFG"));
  strich();
}
