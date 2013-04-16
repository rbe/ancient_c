int MOUSE_X, MOUSE_Y,
    M_BILDX, M_BILDY,
    MOUSE_BUTTON;

#define NO_MOUSE       -1
#define MOUSE_OK       1
#define MOUSE          1
#define M_MIDDLE_X     312
#define M_MIDDLE_Y     88
#define M_BUTTONLEFT   1
#define M_BUTTONRIGHT  2
#define M_BUTTONLR     3
#define M_BUTTONMIDDLE 4
#define M_BUTTONLM     5
#define M_BUTTONMR     6
#define M_BUTTONALL    7

int init_mouse ()
{
static int a;
  r.x.ax=0;
  int86(MOUSE_INT, &r, &r);
  a=r.x.ax;
  if (a!=-1) return NO_MOUSE; else return MOUSE_OK;
}

void mouse ()
{
static int a;
  a=init_mouse();
  if (a==NO_MOUSE) {
    printf(NO_MOUSE_INSTALLED);
    exit(1);
  }
}

void mouse_on ()
{
  r.x.ax=1;
  int86(MOUSE_INT, &r, &r);
}

void mouse_off ()
{
  r.x.ax=2;
  int86(MOUSE_INT, &r, &r);
}

void mouse_status ()
{
  r.x.ax=3;
  int86(MOUSE_INT, &r, &r);
  MOUSE_BUTTON=r.x.bx;
  MOUSE_X=r.x.cx;
  MOUSE_Y=r.x.dx;
}

void set_mouse_box (int M_MAX_X1, int M_MAX_Y1, int M_MAX_X2, int M_MAX_Y2)
{
  r.h.ah=7;
  r.x.cx=M_MAX_X1;
  r.x.dx=M_MAX_X2;
  int86(MOUSE_INT, &r, &r);
  r.h.ah=8;
  r.x.cx=M_MAX_Y1;
  r.x.dx=M_MAX_Y2;
  int86(MOUSE_INT, &r, &r);
}

void set_mouse_xy (int M_X, int M_Y)
{
  r.x.ax=4;
  r.x.cx=M_X;
  r.x.dx=M_Y;
  int86(MOUSE_INT, &r, &r);
}

void set_mouse_speed (int X_SPEED, int Y_SPEED)
{
  r.x.ax=15;
  r.x.cx=X_SPEED;
  r.x.dx=Y_SPEED;
  int86(MOUSE_INT, &r, &r);
}

void change_mouse_coord ()
{
  M_BILDX=(MOUSE_X/8)+1;
  M_BILDY=(MOUSE_Y/8)+1;
}

