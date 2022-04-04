#include <dos.h>
#include <conio.h>
#include <alloc.h>
#include <stdio.h>
#include <bios.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <fcntl.h>
#include <io.h>

#define NO_REPORT 1
#define REPORT 0

#define Round(x) ((int)(x+0.5))
#define Trunc(x) ((int)(x))
#define Sqr(x)   (x*x)

typedef unsigned char byte;
typedef unsigned int word;
struct spritedata {int xlen;int ylen;int len;long int add;};
extern int max_sprite;
extern unsigned char KeyLeft,KeyRight,KeyUp,KeyDown,KeyFire;
extern unsigned char huge *buffers;
extern unsigned char huge *spdata;
extern unsigned char far *screen;
extern int left[2],right[2],acc[2];
extern short unsigned int page1_buffer, page2_buffer;
extern spritedata *SPRITE,*_SPRITE;

void ini_mouse(int quiet);
void end_mouse(void);
int lbutton_mouse(void);
int rbutton_mouse(void);
int xcoord_mouse(void);
int ycoord_mouse(void);
void relcoord_mouse(int &px,int &py);
void showmouse_mouse(void);
void hidemouse_mouse(void);
void setcursor_mouse(unsigned int *type,unsigned int xpos,unsigned int ypos);
void setsens_mouse(int hspeed,int vspeed);
void get_keys(void);
void set_mode(byte mode);
void ppixelxor(int x,long int y,byte color,unsigned char huge *address);
void ppixel(int x,long int y,byte color,unsigned char huge *address);
int gpixel(int x,long int y,unsigned char huge *address);
void swap(int *f, int *s);
void circle(word x,word y,word radius,byte col,int page);
void draw(int x1, int y1, int x2, int y2, byte col,int page);
void triangle(int x1,int y1,int x2,int y2,int x3,int y3,int col,int page);
void triangle_noclip(int x1,int y1,int x2,int y2,int x3,int y3,int col,int page);
void plot(int op,int xplot,int yplot,int col,int page);
void ini_screamer(int quiet);
void end_screamer(void);
void change_col(unsigned int cc,unsigned int cr,unsigned int cg,unsigned int cb);
void exitgraph(void);
void clear_screen(unsigned int col,int page);
void ini_screen(int buff,int quiet);
void move_screen(int page1,int page2);
void load_screen(char *path,byte page,byte palflag);
void compress_screen(char *name,int page1,int page2,int seed);
void uncompress_screen(char *filename,int page1,int seed,int coload);
void block_screen(int page1,int xpos1,int ypos1,int xlen,int ylen,int page2,int xpos2,int ypos2);
void pour_screen(byte page);
void ig_slide_col(int x,int topy);
void slide_screen(int effect,int from_page);
void end_screen(void);
void end_sprite(void);                
void end_pal(void);
void ini_sprite(int number,int quiet);
void add_sprite(int x,long int y,int xlen,int ylen,int sprite,int page);
void change_sprite(int x,long int y,int sprite, int page);
void put_sprite(int x,long int y,int sprite,int page);
void putm_sprite(int x,long int y,int sprite,int page);
void putmb_sprite(int x,long int y,int sprite,int page);
void putc_sprite(int screenx,int screeny,int screenxend,int screenyend,int spritex,int spritey,int sprite,int page);
void putcm_sprite(int screenx,int screeny,int screenxend,int screenyend,int spritex,int spritey,int sprite,int page);
void putcms_sprite(int screenx,int screeny,int screenxend,int screenyend,int spritex,int spritey,float scalex,float scaley,int sprite,int page);
void del_sprite(int sprite);
void ini_pal(int quiet);
void load_pal(char *path);
void use_pal(int pn);
void clear_pal(int pn);
void changecol_pal(int pala,int colour,int red,int green,int blue);
int getred_pal(int p1,int col);
int getgreen_pal(int p1,int col);
int getblue_pal(int p1,int col);
void add_pal(int noof);
void morphtoblack_pal(int p1,int steps);
void morphtocol_pal(int p1,int steps);
void morphcol_pal(int pal1,int pal2, int steps);
int morph(void);
void page1_stack_add(int x,int y,int xlen,int ylen);
void page2_stack_add(int x,int y,int xlen,int ylen);
void page2once_stack_add(int x,int y,int xlen,int ylen);
void add1_frame(int x,int y,int xlen,int ylen);
void add2_frame(int x,int y,int xlen,int ylen);
void update1_frame(void);
void update2_frame(void);
void block1_frame(int x,int y,int xlen,int ylen);
void block2_frame(int x,int y,int xlen,int ylen);
void drawon1_frame(int x,int y,int sprite);
void blockon1_frame(int x,int y,int sprite);
void drawon2_frame(int x,int y,int sprite);
void blockon2_frame(int x,int y,int sprite);
void drawon2once_frame(int x,int y,int sprite);
void ini_frame(short unsigned int p1,short unsigned int p2,short unsigned int p3,int quiet);
void end_frame(void);
void vsync_wait(int frames);
void trans_triangle(int x1,int y1,int x2,int y2,int x3,int y3,int col,int page);
void putcm_trans_sprite(int screenx,int screeny,int screenxend,int screenyend,int spritex,int spritey,int sprite,int page);
void putcms_trans_sprite(int screenx,int screeny,int screenxend,int screenyend,int spritex,int spritey,float scalex,float scaley,int sprite,int page);
void trans_convert_rgbtopal(int &r,int &g,int &b,int &col);
void trans_convert_paltorgb(int &r,int &g,int &b,int &col);
int trans_return_rgbtopal(int r,int g,int b);
void trans_ppixel(int x,long int y,unsigned short int r,unsigned short int g,unsigned short int b,unsigned char huge *address);
void trans_make_pal(void);
void trans_store_pal(int pal);
void ini_keyint(char *name,int quiet);
void end_keyint(void);

