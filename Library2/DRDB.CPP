//Library enabling dirty rectangle double buffering.

#include "Screamer.H"

struct stack
		{
		short unsigned int xpos;
		short unsigned int ypos;
		short unsigned int xlen;
		short unsigned int ylen;
		};

stack page1[100];
stack page2[100];
stack page2once[100];
extern spritedata *SPRITE,*_SPRITE;

short unsigned int page1_bot=0, page1_top=0, page2_bot=0, page2_top=0,
						 page2once_bot=0,page2once_top=0,
						 page1_lap=0, page2_lap=0, page2once_lap=0;

short unsigned int page1_buffer=0, page2_buffer=1, screen_buffer=255;

void page1_stack_add(int x,int y,int xlen,int ylen)
{
if((page1_top==page1_bot)&&(page1_lap==1)){printf("\nDirty rectangle double buffer page 1 stack overflow!\a");getch();exit(1);}
page1[page1_top].xpos=x;
page1[page1_top].ypos=y;
page1[page1_top].xlen=xlen;
page1[page1_top].ylen=ylen;
if(++page1_top==100) {page1_top=0; page1_lap=1;}
}

void page2_stack_add(int x,int y,int xlen,int ylen)
{
if((page2_top==page2_bot)&&(page2_lap==1)){printf("\nDirty rectangle double buffer page 2 stack overflow!\a");getch();exit(1);};
page2[page2_top].xpos=x;
page2[page2_top].ypos=y;
page2[page2_top].xlen=xlen;
page2[page2_top].ylen=ylen;
if(++page2_top==100) {page2_top=0; page2_lap=1;}
}

void page2once_stack_add(int x,int y,int xlen,int ylen)
{
if((page2once_top==page2once_bot)&&(page2once_lap==1)){printf("\nDirty rectangle double buffer page 2 once stack overflow!\a");getch();exit(1);};
// Add sprite 'clipping' here to reeduce the number of instances that it will be needed.
if(x+xlen>319) x=320-xlen;
if(y+ylen>199) y=200-ylen;
if(x<0) x=0;
if(y<0) y=0;
page2once[page2once_top].xpos=x;
page2once[page2once_top].ypos=y;
page2once[page2once_top].xlen=xlen;
page2once[page2once_top].ylen=ylen;
if(++page2once_top==100) {page2once_top=0; page2once_lap=1;}
}

void block1_frame(int x,int y,int xlen,int ylen)
{
if(x+xlen>319) x=320-xlen;
if(y+ylen>199) y=200-ylen;
if(x<0) x=0;
if(y<0) y=0;
add1_frame(x,y,xlen,ylen);
}

void block2_frame(int x,int y,int xlen,int ylen)
{
if(x+xlen>319) x=320-xlen;
if(y+ylen>199) y=200-ylen;
if(x<0) x=0;
if(y<0) y=0;
add2_frame(x,y,xlen,ylen);
}

void add1_frame(int x,int y,int xlen,int ylen)
{
page1_stack_add(x,y,xlen,ylen);
page2_stack_add(x,y,xlen,ylen);
}

void add2_frame(int x,int y,int xlen,int ylen)
{
page2_stack_add(x,y,xlen,ylen);
}

void update1_frame(void)
{
// While the first stack 'aint empty...
while((page1_bot!=page1_top)||(page1_lap!=0))
	{
	block_screen(page1_buffer, page1[page1_bot].xpos, page1[page1_bot].ypos, page1[page1_bot].xlen+4, page1[page1_bot].ylen, page2_buffer, page1[page1_bot].xpos, page1[page1_bot].ypos);
	if(++page1_bot==100) {page1_bot=0; page1_lap=0;}
	}
//page1_top=0; page1_bot=0; page1_lap=0;
}

void update2_frame(void)
{
// While the first stack 'aint empty...
while((page2_bot!=page2_top)||(page2_lap!=0))
	{
	block_screen(page2_buffer, page2[page2_bot].xpos, page2[page2_bot].ypos, page2[page2_bot].xlen+4, page2[page2_bot].ylen, screen_buffer, page2[page2_bot].xpos, page2[page2_bot].ypos);
	if(++page2_bot==100) {page2_bot=0; page2_lap=0;}
	}
// Update post draw Erase, by moving page2once to page2 stack...
while((page2once_bot!=page2once_top)||(page2once_lap!=0))
	{
	block1_frame(page2once[page2once_bot].xpos,page2once[page2once_bot].ypos,page2once[page2once_bot].xlen,page2once[page2once_bot].ylen);
	if(++page2once_bot==100) {page2once_bot=0; page2once_lap=0;}
	}
}

void drawon1_frame(int x,int y,int sprite)
{
putcm_sprite(0,0,319,199,x,y,sprite,page1_buffer);
SPRITE=_SPRITE;
SPRITE+=sprite;
block1_frame(x,y,SPRITE->xlen,SPRITE->ylen);
}

void blockon1_frame(int x,int y,int sprite)
{
SPRITE=_SPRITE;
SPRITE+=sprite;
block1_frame(x,y,SPRITE->xlen,SPRITE->ylen);
}

void drawon2_frame(int x,int y,int sprite)
{
putcm_sprite(0,0,319,199,x,y,sprite,page2_buffer);
SPRITE=_SPRITE;
SPRITE+=sprite;
block2_frame(x,y,SPRITE->xlen,SPRITE->ylen);
}

void blockon2_frame(int x,int y,int sprite)
{
SPRITE=_SPRITE;
SPRITE+=sprite;
block2_frame(x,y,SPRITE->xlen,SPRITE->ylen);
}

void drawon2once_frame(int x,int y,int sprite)
{
drawon2_frame(x,y,sprite);
SPRITE=_SPRITE;
SPRITE+=sprite;
page2once_stack_add(x,y,SPRITE->xlen,SPRITE->ylen);
}

void blockon2once_frame(int x,int y,int sprite)
{
SPRITE=_SPRITE;
SPRITE+=sprite;
page2once_stack_add(x,y,SPRITE->xlen,SPRITE->ylen);
}

void ini_frame(short unsigned int p1,short unsigned int p2,short unsigned int p3,int quiet)
{
if(quiet==0)
	{
	printf("\nInitialising frame using buffers %d & %d Main: ",p1,p2);
	if(p3==255) printf("screen"); else printf("%d",p3);
	}
page1_buffer=p1;
page2_buffer=p2;
screen_buffer=p3;
}

