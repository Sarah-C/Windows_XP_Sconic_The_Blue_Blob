/*******************************************************************************
*  Screamer.CPP source code...
*  Mode 0x13 graphics library...
*  Written by D. Cartwright 1997...
*  Updates...
*       25/9/97 Remove CPP class's to increase speed of exe.
*       28/9/97 Redundant code removed from source code.
*      30/10/97 Added screen_buffer to _frame commands to allow output to
*               something other than the screen.
*      10/12/97 Fixed bug for variable declarations...
*               E.g. char huge* a,* b;   to... char huge* a, huge* b;
*      11/12/98 Fixed screen corruption of _frame commands by +4 due to the 32bit
					 block_screen alteration.
*******************************************************************************/

#include "screamer.h"

//#define _FRAME 0

//A place for storing different pages of video RAM.
unsigned char huge *buffers=0,huge *buffbak=0;

//Makes *screen a huge ptr to the screen RAM.
//static char huge *screen=(char huge*)MK_FP(0xA000,0);
unsigned char far *screen=(unsigned char far*)MK_FP(0xA000,0);

//Sprites . . .
unsigned char huge *spdata=0;
static unsigned long int sprptr=0;
spritedata *SPRITE,*_SPRITE;

/*

  For SPRITE DELETE....... Store size of SAT somewhere, so we can search it for sprites
  stored after the one we deleted!
  e.g,

  (here)
  static unsigned int SAT_size=0;

  (then in ini_sprite):
  SAT_size=number;

  (and in del_sprite)
  for(int a=0;a<SAT_size;a++)

  Oh, yeah, remember to FINISH IT OFFFF!!!!!!!!

*/


//Pallette...
static unsigned char huge *palpos;
int pals=0,colcount=0,targetpal=0;
float stepr[256],stepg[256],stepb[256],colr[256],colg[256],colb[256];

//Controll Keys 0:left hand set, 1: right hand set
int left[2]={0,0},right[2]={0,0},acc[2]={0,0};

void ini_mouse(int quiet)
{
if(quiet==0) printf("Mouse initialised!");
	union REGS regs;
	regs.x.ax=1;
	int86(0x33,&regs,&regs);
}

void end_mouse(void)
{
	union REGS regs;
	regs.x.ax=2;
	int86(0x33,&regs,&regs);
}

int lbutton_mouse(void)
{
	union REGS regs;
	int leftbutton;
	leftbutton=0;
	regs.x.ax=3;
	int86(0x33,&regs,&regs);
	if (regs.x.bx & 1) leftbutton=1;
	return (leftbutton);
}

int rbutton_mouse(void)
{
	union REGS regs;
	int rightbutton;
	rightbutton=0;
	regs.x.ax=3;
	int86(0x33,&regs,&regs);
	if (regs.x.bx & 2) rightbutton=1;
	return(rightbutton);
}

void show_mouse(void)
{
	union REGS regs;
	regs.x.ax=1;
	int86(0x33,&regs,&regs);
}

void hide_mouse(void)
{
	union REGS regs;
	regs.x.ax=2;
	int86(0x33,&regs,&regs);
}

int xcoord_mouse(void)
{
	union REGS regs;
	regs.x.ax=3;
	int86(0x33,&regs,&regs);
	return(regs.x.cx);
}

int ycoord_mouse(void)
{
	union REGS regs;
	regs.x.ax=3;
	int86(0x33,&regs,&regs);
	return(regs.x.dx);
}

void relcoord_mouse(int &px,int &py)//Force pass by reference, dont need to use pointers...
{
	union REGS regs;
	regs.x.ax=11;
	int86(0x33,&regs,&regs);
	px=regs.x.dx;
	py=regs.x.cx;
}

void setsens_mouse(int hspeed,int vspeed)
{
	union REGS regs;
	regs.x.ax=0x1a;
	regs.x.bx=hspeed;
	regs.x.cx=vspeed;
	int86(0x33,&regs,&regs);
}

void get_keys(void)
{
int acce=0,dir=0,keys;
keys=_bios_keybrd(_NKEYBRD_SHIFTSTATUS);
acce=keys;
dir=keys>>8;
if((dir&1)==1) {left[0]=1;}else{left[0]=0;}
if((dir&2)==2) {right[0]=1;}else{right[0]=0;}
if((acce&2)==2) {acc[0]=1;}else{acc[0]=0;}

if((dir&8)==8) {left[1]=1;}else{left[1]=0;}
if((dir&4)==4) {right[1]=1;}else{right[1]=0;}
if((acce&1)==1) {acc[1]=1;}else{acc[1]=0;}
}

void set_mode(byte mode)
{
asm mov ah,0x00
asm mov al,mode
asm int 0x10
}

void ppixelxor(int x,long int y,byte color,unsigned char huge *address)
{
short unsigned int xcol,ncol;
	ncol=color;
	y=((y << 8) + (y << 6) + x);
//	y=((y<<2)+y)<<6;
//	y+=x;
	xcol=*(address+y);
asm mov xcol,ax
asm xor ax,ncol
asm mov ncol,ax
	*(address+y)=ncol;
}

void ppixel(int x,long int y,byte color,unsigned char huge *address)
{
//	y=((y<<2)+y)<<6;
//	y+=x;
	*(address+(((y)<<8)+((y)<<6) + (x)))=color;
}

int gpixel(int x,long int y,unsigned char huge *address)
{
//	y=((y<<2)+y)<<6;
//	y+=x;
	return(*(address+(((y)<<8)+((y)<<6)+(x))));
}

void swap(int *f, int *s)
{
int t;
t=*f;
*f=*s;
*s=t;
}

void circle(word x,word y,word radius,byte col,int page)
{
int a, af, b, bf, target, r2, asp;
unsigned char huge *add=buffers;
if(page==255){add=screen;}else{add+=(page*64000l);}
asp=120;
target=0;
a=radius;
b=0;
r2=Sqr(radius);
while(a>=b)
{       b=Round(sqrt(r2-Sqr(a)));
	swap(&target,&b);
	while(b<target)
	{       af=(asp*a) / 100;
		bf=(asp*b) / 100;
		ppixel(x+af, y+b, col,add);
		ppixel(x+bf, y+a, col,add);
		ppixel(x-af, y+b, col,add);
		ppixel(x-bf, y+a, col,add);
		ppixel(x-af, y-b, col,add);
		ppixel(x-bf, y-a, col,add);
		ppixel(x+af, y-b, col,add);
		ppixel(x+bf, y-a, col,add);
		++b;
	}
	--a;
}
}

void draw(int x1, int y1, int x2, int y2, byte col,int page)
{
int LgDelta, ShDelta, Cycle, LgStep, ShStep;
unsigned char huge *add=buffers;
if(page==255){add=screen;}else{add+=(page*64000l);}
LgDelta=x2-x1;
ShDelta=y2-y1;
if(LgDelta<0)
{       LgDelta=-LgDelta;
	LgStep=-1;
}
else LgStep=1;
if(ShDelta<0)
{       ShDelta=-ShDelta;
	ShStep=-1;
}
else ShStep=1;
if(ShDelta<LgDelta)
{       Cycle=LgDelta >> 1;
	while(x1 != x2)
	{       ppixel(x1, y1, col,add);
		Cycle+=ShDelta;
		if(Cycle>LgDelta)
		{       Cycle-=LgDelta;
			y1+=ShStep;
		}
		x1+=LgStep;
	}
	ppixel(x1, y1, col,add);
}
else
{       Cycle=ShDelta >> 1;
	swap(&LgDelta, &ShDelta);
	swap(&LgStep, &ShStep);
	while(y1 != y2)
	{       ppixel(x1, y1, col,add);
		Cycle+=ShDelta;
		if(Cycle>LgDelta)
		{       Cycle-=LgDelta;
			x1+=ShStep;
		}
		y1+=LgStep;
	}
	ppixel(x1, y1, col,add);
}
}

void triangle_noclip(int x1,int y1,int x2,int y2,int x3,int y3,int col,int page)
{
float xstep1,xstep2,xstep3,xpos1,xpos2,xx1,xx2,di1=0,di2=0,di3=0;
float xx[3]={x1,x2,x3},yy[3]={y1,y2,y3};
int yloop,tmp;
int top=0,mid=1,bot=2,x;
unsigned char huge *add=buffers;
if(page==255){add=screen;}else{add+=(page*64000l);}
if(yy[top]>yy[mid]) {tmp=mid; mid=top; top=tmp;}
if(yy[top]>yy[bot]) {tmp=bot; bot=top; top=tmp;}
if(yy[mid]>yy[bot]) {tmp=bot; bot=mid; mid=tmp;}
xpos1=xx[top];
xpos2=xx[top];
di1=yy[top]-yy[bot];
di2=yy[top]-yy[mid];
di3=yy[mid]-yy[bot];
if(di1!=0) xstep1=(xx[top]-xx[bot])/di1;
if(di2!=0) {xstep2=(xx[top]-xx[mid])/di2;} else {xpos2-=xx[top]-xx[mid];}
if(di3!=0) xstep3=(xx[mid]-xx[bot])/di3;
add+=(((long int)yy[top]<<2)+(long int)yy[top])<<6;
for(yloop=yy[top];yloop<yy[mid];yloop++)
	{
	xx1=Round(xpos1); xx2=Round(xpos2);
	if(xx1>xx2){tmp=xx1; xx1=xx2; xx2=tmp;}
	for(x=xx1;x<=xx2;x++)
		{
		*(add+x)=col;
		}
	add+=320;
	xpos1+=xstep1;
	xpos2+=xstep2;
	}
for(yloop=yy[mid];yloop<yy[bot];yloop++)
	{
	xx1=Round(xpos1); xx2=Round(xpos2);
	if(xx1>xx2){tmp=xx1; xx1=xx2; xx2=tmp;}
	for(x=xx1;x<=xx2;x++)
		{
		*(add+x)=col;
		}
	add+=320;
	xpos1+=xstep1;
	xpos2+=xstep3;
	}
}

void triangle(int x1,int y1,int x2,int y2,int x3,int y3,int col,int page)
{
float xstep1,xstep2,xstep3,xpos1,xpos2,xx1,xx2,di1=0,di2=0,di3=0;
float xx[3]={x1,x2,x3},yy[3]={y1,y2,y3};
int yloop,tmp,trans=0;
int top=0,mid=1,bot=2,x;
unsigned char huge *add=buffers;
if(col<0) {col=col*-1; trans=1;}
if(page==255){add=screen;}else{add+=(page*64000l);}
if(yy[top]>yy[mid]) {tmp=mid; mid=top; top=tmp;}
if(yy[top]>yy[bot]) {tmp=bot; bot=top; top=tmp;}
if(yy[mid]>yy[bot]) {tmp=bot; bot=mid; mid=tmp;}
xpos1=xx[top];
xpos2=xx[top];
di1=yy[top]-yy[bot];
di2=yy[top]-yy[mid];
di3=yy[mid]-yy[bot];
if(di1!=0) xstep1=(xx[top]-xx[bot])/di1; else xstep1=0;
if(di2!=0)
	{
	xstep2=(xx[top]-xx[mid])/di2;
	}
	else
	{
	xpos2-=xx[top]-xx[mid];
	xstep2=0;
	}
if(di3!=0) xstep3=(xx[mid]-xx[bot])/di3; else xstep3=0;
if(yy[bot]>200) yy[bot]=200;
if(yy[mid]>200) yy[mid]=200;
if(yy[top]<0)
	{
	if(yy[mid]<0)
		{
		xpos1+=(yy[top]*-1)*xstep1;
		xpos2+=(di2*-1)*xstep2; xpos2+=(yy[mid]*-1)*xstep3;
		yy[top]=0;
		yy[mid]=0;
		}
		else
		{
		xpos1+=(yy[top]*-1)*xstep1;
		xpos2+=(yy[top]*-1)*xstep2;
		yy[top]=0;
		}
	}
add+=(((long int)yy[top]<<2)+(long int)yy[top])<<6;
if(trans==0)
	{
	for(yloop=yy[top];yloop<yy[mid];yloop++)
		{
		xx1=Round(xpos1); xx2=Round(xpos2);
		if(xx1>xx2){tmp=xx1; xx1=xx2; xx2=tmp;}
		if(xx1<0) xx1=0;
		if(xx2>319) xx2=319;
		if(xx1<=xx2)
			{
			for(x=xx1;x<=xx2;x++)
				{
				*(add+x)=col;
				}
			}
		add+=320;
		xpos1+=xstep1;
		xpos2+=xstep2;
		}
	if(yy[mid]==200) return;
	for(yloop=yy[mid];yloop<yy[bot];yloop++)
		{
		xx1=Round(xpos1); xx2=Round(xpos2);
		if(xx1>xx2){tmp=xx1; xx1=xx2; xx2=tmp;}
		if(xx1<0) xx1=0;
		if(xx2>319) xx2=319;
		if(xx1<=xx2)
			{
			for(x=xx1;x<=xx2;x++)
				{
				*(add+x)=col;
				}
			}
		add+=320;
		xpos1+=xstep1;
		xpos2+=xstep3;
		}
	}
	else
	{
	for(yloop=yy[top];yloop<yy[mid];yloop++)
		{
		xx1=Round(xpos1); xx2=Round(xpos2);
		if(xx1>xx2){tmp=xx1; xx1=xx2; xx2=tmp;}
		if(xx1<0) xx1=0;
		if(xx2>319) xx2=319;
		if(xx1<=xx2)
			{
			for(x=xx1;x<=xx2;x++)
				{
				if((((int)(x+yloop)/2)*2==(int)(x+yloop))) *(add+x)=col;
				}
			}
		add+=320;
		xpos1+=xstep1;
		xpos2+=xstep2;
		}
	if(yy[mid]==200) return;
	for(yloop=yy[mid];yloop<yy[bot];yloop++)
		{
		xx1=Round(xpos1); xx2=Round(xpos2);
		if(xx1>xx2){tmp=xx1; xx1=xx2; xx2=tmp;}
		if(xx1<0) xx1=0;
		if(xx2>319) xx2=319;
		if(xx1<=xx2)
			{
			for(x=xx1;x<=xx2;x++)
				{
				if((((int)(x+yloop)/2)*2==(int)(x+yloop)))*(add+x)=col;
				}
			}
		add+=320;
		xpos1+=xstep1;
		xpos2+=xstep3;
		}
	}
}

void plot(int op,int xplot,int yplot,int col,int page)
{
static px[3]={0,0,0}, py[3]={0,0,0};
px[0]=px[1]; py[0]=py[1];
px[1]=px[2]; py[1]=py[2];
px[2]=xplot ; py[2]=yplot;
switch(op)
	{
	case 5: draw(px[2],py[2],px[1],py[1],col,page);break;
	case 85: triangle(px[0],py[0],px[1],py[1],px[2],py[2],col,page);
				break;
	case 89: draw(px[0],py[0],px[1],py[1],col,page);
				draw(px[1],py[1],px[2],py[2],col,page);
				draw(px[2],py[2],px[0],py[0],col,page);
				break;
	default: ;break;// Do nothing!!!
	}
}

void change_col(unsigned int cc,unsigned int cr,unsigned int cg,unsigned int cb)
{
outp(0x3c6,0xfff);
outp(0x3c8,cc);outp(0x3c9,cr);outp(0x3c9,cg);outp(0x3c9,cb);
}

void exitgraph()
{
set_mode(3);
}

void movedw(void *a, void *b, int l)
{
asm             CLI
asm             CLD
asm             PUSH DS
asm             LDS SI, a
asm             LES DI, b
asm             MOV CX, l
asm             DB 0x66;
asm              REP MOVSW
asm             POP DS
asm             STI
}

void clear_screen(unsigned int col,int page)
{
unsigned char huge *saddr1, huge* saddr2;
unsigned long int p=page*64000l;
if(page==255) {saddr1=screen;saddr2=screen+4;} else {saddr1=buffers+p;saddr2=buffers+p+4;}
*(saddr1)=col;
*(saddr1+1)=col;
*(saddr1+2)=col;
*(saddr1+3)=col;
movedw(saddr1,saddr2,15999);
/*
asm             CLI
asm             CLD
asm             PUSH DS
asm             LDS SI, saddr1
asm             LES DI, saddr2
asm             MOV CX, xlen
asm             DB 0x66;
asm             REP MOVSW
asm             POP DS
asm             STI
*/
/*asm             CLI
asm             CLD
asm             LES DI, addr
asm             MOV CX, 16000
asm             DB 0x66
asm             MOV AX, WORD PTR c1
asm             DB 0x66
asm             REP STOSW
asm             STI*/
//for(pos=0;pos<64000l;pos++) *(add+pos)=col;
}

void ini_screen(int buff,int quiet)
{
int c=0;
unsigned long int memb=0;
if(quiet==0) printf("\nInitialising screen buffers; %d @ 64000 bytes...",buff);
memb=64000l*buff;
if((buffers=(unsigned char huge*) farmalloc(memb))==NULL)
	{
	printf("\nNot enough memory for screen pages!");getch;exit(1);
	}
buffbak=buffers;
if(quiet==0) printf("O.K!...\nClearing buffer: ");
for(c=0;c<buff;c++) {
	if(quiet==0) printf("%d:",c);
	clear_screen(0,c);
	if(quiet==0) printf("O.K! ");
	}
}

void move_screen(int page1,int page2)
{
//long int c=0;
unsigned char huge *ad1;
unsigned char huge *ad2;
if(page1!=255){ad1=buffers+(page1*64000l);}else{ad1=screen;}
if(page2!=255){ad2=buffers+(page2*64000l);}else{ad2=screen;}
//for(c=0;c<64000l;c++) *(ad2++)=*(ad1++);
movedw(ad1,ad2,16000);
}

void load_screen(char *path,byte page,byte palflag)
{
	FILE *stream;
	int error,x,y,r,g,b,col,l;
	unsigned long int pos=(page+1)*64000l;
	pos+=320;
	if(page==255)
		{
		pos=64320l;
		buffers=screen;
		}
	stream = fopen(path, "rb");
	if((error=fseek(stream, 18, SEEK_SET))!=0)
		{
		printf("\nload_screen(%s,%d,%d); FAILED!\nError code: %d",path,page,palflag,error);getch();exit(1);
		}

	for(l=0;l<256;l++)
	{
		b=(int) fgetc(stream);
		g=(int) fgetc(stream);
		r=(int) fgetc(stream);
		if(palflag!=0) change_col(l,r/4,g/4,b/4);
	}
	for(y=199;y>=0;y--)
	{
	pos-=640;
	for(x=0;x<320;x++)
		{
		col=(int) fgetc(stream);
		*(buffers+pos++)=col;
		}
	}
fclose(stream);
buffers=buffbak;
}

void compress_screen(char *name,int page1,int page2,int seed)
{
//The picture to 0 compress goes in screen 0, output in 1.
FILE *out,*in;
unsigned long int off1=page1*64000l;
unsigned long int off2=page2*64000l;
unsigned char huge *sadd1, huge *sadd2;
char loadname[15],savename[15],handle[32]="ASS File Format (C)1999 Lab_Rat";
long int read=0,write=0,store=0;
int counter=0,xor=0;
unsigned int byte;
sadd1=buffers+off1;
sadd2=buffers+off2;
if(page1==255) sadd1=screen;
if(page2==255) sadd2=screen;
srand(seed);
strcpy(loadname,name);   strcpy(savename,name);
strcat(loadname,".TGA"); strcat(savename,".ASS");
load_screen(loadname,0,0);
for(read;read<64000l;read++)
	{
	byte=*(sadd1+read);
	//Is it a zero?
	if(byte==0)
		{
		//The byte==0, so check if it is the first one weve found, if it is, store it.
		if(counter==0)
			{
			*(sadd2+write)=xor;//Byte or 0, doesnt matter.
			xor=rand()%256;
			write++;
			if(write==64000l) {printf("Unable to compress picture!");getch();exit(1);}
			}
		counter++;
		//Does counter== maximum allowed or the end of the file?
		if((counter==255)||(read==63999l))
			{
			//Yes, so store the number of zeros counted, and reset counter
			*(sadd2+write)=counter^xor;
			xor=rand()%256;
			write++;
			if(write==64000l) {printf("Unable to compress picture!");getch();exit(1);}
			counter=0;
			}
		//Otherwise ignore...
		}
		else
		{
		//Byte is not a zero, but have we been counting zeros?
		if(counter!=0)
			{
			//Yes!
			//Store the number of zeros, and reset counter
			*(sadd2+write)=counter^xor;
			xor=rand()%256;
			write++;
			if(write==64000l) {printf("Unable to compress picture!");getch();exit(1);}
			counter=0;
			}
		//No, so just store the byte
		*(sadd2+write)=byte^xor;
		xor=rand()%256;
		write++;
		if(write==64000l) {printf("Unable to compress picture!");getch();exit(1);}
		}
	}
//Now write stores the length of the compressed image.
//So save it to disc with pallette information.
if((out=fopen(savename, "wb"))==NULL) {printf("cant open to save\n");getch();exit(1);}
if((in=fopen(loadname, "rb"))==NULL) {printf("cant open to load\n");getch();exit(1);}
if(fseek(in, 18, SEEK_SET)!=0) {printf("Cant seek!");getch();exit(1);}
fwrite(handle,32,1,out);

//First store colours...
srand(seed);
for(store=0;store<256;store++)
	{
	fputc(fgetc(in),out); fputc(fgetc(in),out); fputc(fgetc(in),out);
	}

for(store=0;store<write;store++)
	{
	byte=*(sadd2+store);
	fputc(byte,out);
	}
fclose(in);
fclose(out);
}//End compress

void uncompress_screen(char *filename,int page1,int seed,int coload)
{
int num,added,store;
unsigned long int write_pos=0;
int xor=0;
unsigned long int off1=page1*64000l;
unsigned char huge *sadd1;
clear_screen(0,page1);
sadd1=buffers+off1;
if(page1==255) sadd1=screen;
FILE *in;
srand(seed);
if((in=fopen(filename, "rb"))==NULL) {printf("cant open to load\n");getch();exit(1);}
if(fseek(in, 32, SEEK_SET)!=0) {printf("Cant seek!");getch();exit(1);}
if(coload)
	for(store=0;store<256;store++) change_col(store,fgetc(in)/4,fgetc(in)/4,fgetc(in)/4);
	else
	if(fseek(in,800, SEEK_SET)!=0) {printf("Cant seek!");getch();exit(1);}
	do{
	num=fgetc(in);
	num^=xor;
	xor=rand()%256;
	*(sadd1+write_pos)=num;
	if(num!=0) write_pos++;
	if(num==0)
		{
		added=fgetc(in);
		added^=xor;
		xor=rand()%256;
		write_pos+=added;
		}
	}while(write_pos<64000l);
}

void block_screen(int page1,int xpos1,int ypos1,int xlen,int ylen,int page2,int xpos2,int ypos2)
{
unsigned long int off1=page1*64000l;
unsigned long int off2=page2*64000l;
unsigned char huge *sadd1, huge *sadd2;
unsigned int x,y;//,add=320-xlen;
sadd1=buffers+off1;
sadd2=buffers+off2;
if(page1==255) sadd1=screen;
if(page2==255) sadd2=screen;
x=xpos1;y=ypos1;
y=((y<<2)+y)<<6;
y+=x;
sadd1+=y;
x=xpos2; y=ypos2;
y=((y<<2)+y)<<6;
y+=x;
sadd2+=y;
xlen=xlen/4;//32 bit moves...
for(y=0;y<ylen;y++)
	{
//	/*
	asm             CLI
	asm             CLD
	asm             PUSH DS
	asm             LDS SI, sadd1
	asm             LES DI, sadd2
	asm             MOV CX, xlen
	asm             DB 0x66;
	asm              REP MOVSW
	asm             POP DS
	asm             STI
//	*/
	/*
	for(x=0;x<xlen;x++)
		{
		*(sadd2++)=*(sadd1++);
		}
	*/
//	sadd2+=add;
//	sadd1+=add;
	sadd2+=320;
	sadd1+=320;
	}
}

void pour_screen(byte page)
{
	unsigned int lin[320];
	unsigned long int pos=page*64000l;
	pos+=64320l;
	int y2,x,y,col;
	for(y=199;y>=0;y--)
	{
	pos-=640;
		for(x=0;x<320;x++)
			{
			col=*(buffers+pos++);
			ppixel(x,y,col,screen);
			lin[x]=col;
			}
	for(y2=y;y2>=0;y2--)
		{
		for(x=0;x<320;x++)
			{
			ppixel(x,y2,lin[x],screen);
			}
		}
	delay(10);
	}
}

void ig_slide_col(int x,int topy,int pixel)
{
	register y;
	unsigned long int pos=64000l-(320-x);
	unsigned char far *splodge = screen;               // This is used with the pixel move statement inside the for loop if it is new line - IforG
	for( y=199; y>topy; y-- )     // Alteration we talked about where the number of pixels moved decreases - IforG
	{
		*(splodge+(int)pos)=*((splodge+(int)pos)-320);      // Use this line instead of above line so that it use far pointers not huge pointers
		pos-=320;                                                                               // this makes the calculations alot quicker - IforG
	}
*(splodge+(int)pos)=pixel;
//if(topy==199) {*(splodge+(int)pos)=rand()%10;getch();}
}

void slide_screen(int effect,int from_page)
{
	int    a;
	float       sinf,ang;
	int         sini,maxcount[320],count[320];
	int    moved[320];
	int    col[320];
	int         left = 0;
	unsigned char huge *colpos;
//	if(from_page==255)
//		{
//		printf("\nTrying to pour screen onto itself!");getch;exit(1);
//		}

	colpos=buffers+(64000l*from_page);

//	for(a=0;a<320;a++)
//	{
//		ppixel(a, (long) 0, (long) 0,screen);
//	}
	randomize();

switch(effect)
 {
 case 0: for(a=0;a<320;a++)
		{
		ang=a*2;
		ang/=180;
		ang*=3.141;
		sinf=sin(ang);
		sini=(int)abs((sinf*10));
		  maxcount[a]=10+(rand() % (sini+1));
		count[a]=maxcount[a];
		moved[a]=0;
		  col[a] = a;
		}
		break;

 case 1:for(a=0;a<320;a++)
		{
		sinf=sin(a*2.0/180*3.141);
		sini=(int)abs((sinf*20.0));
		maxcount[a]=sini+10;
		count[a]=maxcount[a];
		moved[a]=0;
		col[a] = a;
		}
		break;

 default:for(a=0;a<160;a++)
		{
		count[a+160]=160-a;
		maxcount[a+160]=160-a;
		count[a]=a+1;
		maxcount[a]=a+1;
		moved[a]=0;
		col[a] = a;
		moved[a+160]=0;
		col[a+160] = a+160;
		}
 }

if(from_page!=255)
	{
	while ( left < 320 ) {
		for ( a = left; a < 320; a++ ) {
			if ( count[a]-- == 0 ) {
				ig_slide_col(col[a], moved[a],*(colpos+(unsigned long int)col[a]+(unsigned long int)(moved[a]*320l)));
				count[a] = maxcount[a];
				if ( ++moved[a] == 200 ) {
					col[a] = col[left];
					count[a]=count[left];
					maxcount[a]=maxcount[left];
					moved[a] = moved[left++];
					}//End if moved<200
			}//End if count[a]==0
		}// Next A
	}// End While
	}// End func()
	else
	{
	while ( left < 320 ) {
		for ( a = left; a < 320; a++ ) {
			if ( count[a]-- == 0 ) {
				ig_slide_col(col[a], moved[a],0);
				count[a] = maxcount[a];
				if ( ++moved[a] == 200 ) {
					col[a] = col[left];
					count[a]=count[left];
					maxcount[a]=maxcount[left];
					moved[a] = moved[left++];
					}//End if moved<200
			}//End if count[a]==0
		}// Next A
	}// End While
	}//End if
}

void end_screen(void)
{
farfree(buffers);
exitgraph();
}

void end_sprite(void)
{
farfree(spdata);
free(_SPRITE);
}

void end_pal(void)
{
farfree(palpos);
}

void ini_sprite(int number,int quiet)
{
int a;
if(quiet==0) printf("\nCreating Sprite Allocation Table for %d sprites...",number);
if((_SPRITE=(spritedata*) calloc(number,sizeof(spritedata)))==NULL)
	{
	printf("\nUnable to claim memory for SAT!");getch();exit(1);
   }
SPRITE=_SPRITE;
for(a=0;a<number;a++)
	{
	SPRITE->xlen=-1;
	SPRITE->ylen=-1;
	SPRITE->len=-1;
	SPRITE->add=-1;
	SPRITE++;
	}
if(quiet==0) printf("O.K!");
}

void add_sprite(int x,long int y,int xlen,int ylen,int sprite,int page)
{
unsigned char huge *add,huge *oldptr;
unsigned long int p=page*64000l,oldsprptr;
long int xpos,ypos;
int length;
add=buffers+p;
if(page==255) add=screen;
//'add' now points to page position of sprite...(Vid RAM, Or normal RAM)
//Find out how big our sprite is...
length=xlen*ylen;
//Update the SAT (Sprite Allocation Table)
SPRITE=_SPRITE;
SPRITE+=sprite;
SPRITE->xlen=xlen;
SPRITE->ylen=ylen;
SPRITE->len=length;
SPRITE->add=sprptr;
//Allocate memory for sprite...
oldsprptr=sprptr;
sprptr+=length;
if((spdata=(unsigned char huge*)farrealloc(spdata,sprptr))==NULL)
	{
	printf("\nRequest: add_sprite(%p",SPRITE);
	printf(",%d",x);
	printf(",%d",y);
	printf(",%d",xlen);
	printf(",%d",ylen);
	printf(",%d,",sprite);
	if(page==255) printf("screen"); else printf("%d",page);
	printf(") failed!\nUnable to allocate memory!\a");getch();exit(1);
	}
oldptr=spdata+oldsprptr;
//Actually place the sprite into the sprite pool...
p=0;
add+=((y<<2)+y)<<6;
add+=x;
for(ypos=y;ypos<ylen+y;ypos++)
	{
	for(xpos=x;xpos<xlen+x;xpos++)
		{
		*(oldptr++)=*(add++);
		}
	add+=(320-xlen);
	}
}

void change_sprite(int x,long int y,int sprite, int page)
{
unsigned char huge *add,huge *oldptr;
unsigned long int p=page*64000l;
long int xpos,ypos,xlen,ylen;
add=buffers+p;
if(page==255) add=screen;
//'add' now points to page position of sprite...(Vid RAM, Or normal RAM)
SPRITE=_SPRITE;
SPRITE+=sprite;
xlen=SPRITE->xlen;
ylen=SPRITE->ylen;
oldptr=buffers+SPRITE->add;
//Actually replace the sprite into the sprite pool...
p=0;
add+=((y<<2)+y)<<6;
add+=x;
for(ypos=y;ypos<ylen+y;ypos++)
	{
	for(xpos=x;xpos<xlen+x;xpos++)
		{
		*(oldptr++)=*(add++);
		}
	add+=(320-xlen);
	}
}

//Put a DEFINITELY unclipped sprite to a page...
void put_sprite(int x,long int y,int sprite,int page)
{
unsigned char huge *add,huge *spradd;
long int length=0;
long int xpos,ypos;
unsigned long int p=page*64000l;
add=buffers+p;
SPRITE=_SPRITE;
SPRITE+=sprite;
length=320-SPRITE->xlen;
if(page==255) add=screen;
add+=((y<<2)+y)<<6;
add+=x;
spradd=spdata+SPRITE->add;
for(ypos=0;ypos<SPRITE->ylen;ypos++)
	{
	for(xpos=0;xpos<SPRITE->xlen;xpos++)
		{
		*(add++)=*(spradd++);
		}
	add+=length;
	}
}

//Put an unclipped MASKED sprite to a page...
void putm_sprite(int x,long int y,int sprite,int page)
{
unsigned char huge *add,huge *spradd;
long int length=0;
long int xpos,ypos;
unsigned long int p=page*64000l;
add=buffers+p;
SPRITE=_SPRITE;
SPRITE+=sprite;
length=320-SPRITE->xlen;
if(page==255) add=screen;
add+=((y<<2)+y)<<6;
add+=x;
spradd=spdata+SPRITE->add;
for(ypos=0;ypos<SPRITE->ylen;ypos++)
	{
	for(xpos=0;xpos<SPRITE->xlen;xpos++)
		{
		if(*(spradd)!=0) *(add)=*(spradd);
		add++;spradd++;
		}
	add+=length;
	}
}

//Put an unclipped MASKED sprite to a page, behind things...
void putmb_sprite(int x,long int y,int sprite,int page)
{
unsigned char huge *add,huge *spradd;
long int length=0;
long int xpos,ypos;
unsigned long int p=page*64000l;
add=buffers+p;
SPRITE=_SPRITE;
SPRITE+=sprite;
length=320-SPRITE->xlen;
if(page==255) add=screen;
add+=((y<<2)+y)<<6;
add+=x;
spradd=spdata+SPRITE->add;
for(ypos=0;ypos<SPRITE->ylen;ypos++)
	{
	for(xpos=0;xpos<SPRITE->xlen;xpos++)
		{
		if(*(add)==0) *(add)=*(spradd);
		add++;spradd++;
		}
	add+=length;
	}
}

//Put a CLIPPED UNMASKED sprite to a page...
void putc_sprite(int screenx,int screeny,int screenxend,int screenyend,int spritex,int spritey,int sprite,int page)
{
long int spritexend,spriteyend;
long int clipx,clipxend,clipyend;
long int clipy;
long int xx,yy;
long int leftinc=0,rightinc=0,screeninc;
unsigned char huge *add,huge *spradd;
unsigned long int p=page*64000l;
SPRITE=_SPRITE;
SPRITE+=sprite;
spritexend=spritex+SPRITE->xlen;
spriteyend=spritey+SPRITE->ylen;
clipx=spritex;
clipxend=spritexend;
clipyend=spriteyend;
clipy=spritey;
screeninc=320-SPRITE->xlen;
spradd=spdata+SPRITE->add;
screenxend++;screenyend++;
if(spritex<screenx)
	{
	leftinc=screenx-spritex;
	clipx=screenx;
	screeninc=320-(SPRITE->xlen-leftinc);
	}
if(spritey<screeny)
	{
	spradd+=(screeny-spritey)*SPRITE->xlen;
	clipy=screeny;
	}
if(spritexend>screenxend)
	{
	rightinc=spritexend-screenxend;
	clipxend=screenxend;
	screeninc=320-(SPRITE->xlen-rightinc);
	}
if(spriteyend>screenyend) clipyend=screenyend;
if(page==255)
	{add=screen;}
	else
	{add=buffers+p;}
add+=((clipy<<2)+clipy)<<6;
add+=clipx;
for(yy=clipy;yy<clipyend;yy++)
	{
	spradd+=leftinc;
		for(xx=clipx;xx<clipxend;xx++)
		{
		*(add++)=*(spradd++);
		}
	add+=screeninc;
	spradd+=rightinc;
	}
}

//(The slowest one)
//Put a (possibly)CLIPPED (definitely)MASKED sprite to a page...
void putcm_sprite(int screenx,int screeny,int screenxend,int screenyend,int spritex,int spritey,int sprite,int page)
{
long int spritexend,spriteyend;
long int clipx,clipxend,clipyend;
long int clipy;
long int xx,yy;
long int leftinc=0,rightinc=0,screeninc;
unsigned char huge *add,huge *spradd;
unsigned long int p=page*64000l;
SPRITE=_SPRITE;
SPRITE+=sprite;
spritexend=spritex+SPRITE->xlen;
spriteyend=spritey+SPRITE->ylen;
clipx=spritex;
clipxend=spritexend;
clipyend=spriteyend;
clipy=spritey;
screeninc=320-SPRITE->xlen;
spradd=spdata+SPRITE->add;
screenxend++;screenyend++;
if(spritex<screenx)
	{
	leftinc=screenx-spritex;
	clipx=screenx;
	screeninc=320-(SPRITE->xlen-leftinc);
	}
if(spritey<screeny)
	{
	spradd+=(screeny-spritey)*SPRITE->xlen;
	clipy=screeny;
	}
if(spritexend>screenxend)
	{
	rightinc=spritexend-screenxend;
	clipxend=screenxend;
	screeninc=320-(SPRITE->xlen-rightinc);
	}
if(spriteyend>screenyend) clipyend=screenyend;
if(page==255)
	{add=screen;}
	else
	{add=buffers+p;}
add+=((clipy<<2)+clipy)<<6;
add+=clipx;
for(yy=clipy;yy<clipyend;yy++)
	{
	spradd+=leftinc;
		for(xx=clipx;xx<clipxend;xx++)
		{
		if(*(spradd)!=0) *(add)=*(spradd);
		add++;spradd++;
		}
	add+=screeninc;
	spradd+=rightinc;
	}
}

//This one puts a very amiturishly scaled sprite onto the screen...
void putcms_sprite(int screenx,int screeny,int screenxend,int screenyend,int spritex,int spritey,float scalex,float scaley,int sprite,int page)
{
long int spritexend,spriteyend;
long int clipx,clipxend,clipyend;
long int clipy;
long int xx,yy;
int posy=1;
float leftinc=.1,rightinc=0,screeninc;
unsigned char huge *add,huge *spradd;
unsigned long int p=page*64000l;
float scalepos;
if((scalex<=0.000)||(scaley<=0.000)) return;
SPRITE=_SPRITE;
SPRITE+=sprite;
spritexend=spritex+int((SPRITE->xlen*scalex)+0.5);
spriteyend=spritey+int((SPRITE->ylen*scaley)-0.5);
clipx=spritex;
clipxend=spritexend;
clipyend=spriteyend;
clipy=spritey;
screeninc=320-int((SPRITE->xlen*scalex)+0.5);
spradd=spdata+SPRITE->add;
screenxend++;screenyend++;
if(spritex<screenx)
	{
	leftinc=((screenx-spritex)*(1.0/scalex));
	clipx=screenx;
	screeninc=320-((int(0.5+SPRITE->xlen*scalex)-(screenx-spritex)));
	}
if(spritey<screeny)
	{
	posy=screeny-spritey+1;
	clipy=screeny;
	}
if(spritexend>screenxend)
	{
	rightinc=int((float)(spritexend-screenxend)*(1.0/scalex));
	clipxend=screenxend;
	screeninc=320-((int(0.5+SPRITE->xlen*scalex)-(spritexend-screenxend)));
	}
if((spritex<screenx)&&(spritexend>screenxend)) screeninc=0;
if(spriteyend>screenyend) clipyend=screenyend;
if(page==255)
	{add=screen;}
	else
	{add=buffers+p;}
add+=((clipy<<2)+clipy)<<6;
add+=clipx;
for(yy=clipy;yy<clipyend;yy++)
	{
	scalepos=(SPRITE->xlen*int(posy/scaley));
	scalepos+=leftinc;
	for(xx=clipx;xx<clipxend;xx++)
		{
		if(*(spradd+(int)scalepos)!=0) *(add)=*(spradd+(int)scalepos);
		add++;scalepos+=1/scalex;
		}
	posy++;
	add+=(int)screeninc;
	scalepos+=rightinc;
	}
}

void ini_pal(int quiet)
{
pals=0;
if(quiet==0) printf("\nInitialising palette...");
if((palpos=(unsigned char huge*) farmalloc(1))==NULL)
	{
	printf("\aFAILED! No memory.");getch();exit(1);
	}
printf("O.K!");
}

void load_pal(char *path)
{
unsigned char huge *pp;
FILE *stream;
int error,l,offset=18;//32
stream = fopen(path, "rb");
if((fgetc(stream)=='A')&&(fgetc(stream)=='S')&&(fgetc(stream)=='S')&&(fgetc(stream)==' ')&&(fgetc(stream)=='F')) offset=32;
if((error=fseek(stream, offset, SEEK_SET))!=0)
	{
	printf("\nload_pal(%s); FAILED!\nError code: %d",path,error);getch();exit(1);
	}
pals++;
if((palpos=(unsigned char huge*)farrealloc(palpos,(unsigned long)(pals*768)))==NULL)
	{
	printf("\nNot enough memory to add Palette!\a");getch();exit(1);
	}
pp=palpos;
pp+=((pals-1)*768);
	for(l=0;l<256;l++)
	{
		*(pp++)=(int) (fgetc(stream)/4);
		*(pp++)=(int) (fgetc(stream)/4);
		*(pp++)=(int) (fgetc(stream)/4);
	}
fclose(stream);
}

void use_pal(int pn)
{
int l;
unsigned char huge *p=palpos;
p+=(pn*768);
	for(l=0;l<256;l++)
	{
		change_col(l,*(p++),*(p++),*(p++));
	}
}

void clear_pal(int pn)
{
int l;
unsigned char huge *p=palpos;
p+=(pn*768);
	for(l=0;l<256;l++)
	{
		*(p++)=0;*(p++)=0;*(p++)=0;
	}
}

void changecol_pal(int pala,int colour,int red,int green,int blue)
{
unsigned char huge *pn=palpos;
pn+=(pala*768);
pn+=(colour*3);
*(pn++)=blue/4; *(pn++)=green/4; *(pn++)=red/4;
}

int getred_pal(int p1,int col)
{
unsigned char huge *p=palpos;
p+=(p1*768);
p+=col;
return(*(p+2)*4);
}

int getgreen_pal(int p1,int col)
{
unsigned char huge *p=palpos;
p+=(p1*768);
p+=col;
return(*(p+1)*4);
}

int getblue_pal(int p1,int col)
{
unsigned char huge *p=palpos;
p+=(p1*768);
p+=col;
return(*(p)*4);
}

void add_pal(int noof)
{
unsigned char huge *pp;
int l;
do{
	pals++;
	if((palpos=(unsigned char huge*)farrealloc(palpos,(unsigned long)(pals*768)))==NULL)
		{
		pals--;
		printf("\nadd_pal(%d); FAILED!\nNot enough memory to add Palette!",noof);getch();exit(1);
		}
	pp=palpos;
	pp+=((pals-1)*768);
		for(l=0;l<256;l++)
			{
			*(pp++)=0;
			*(pp++)=0;
			*(pp++)=0;
			}
}while(--noof!=0);
}

void morphtoblack_pal(int p1,int steps)
{
unsigned char huge *p=palpos;
p+=(p1*768);
int a;
for(a=0;a<256;a++)
	{
	unsigned char cr,cg,cb;
	cb=*(p++); cg=*(p++); cr=*(p++);
	stepr[a]=(float)cr/steps;
	stepg[a]=(float)cg/steps;
	stepb[a]=(float)cb/steps;
	colr[a]=cr; colg[a]=cg; colb[a]=cb;
	}
colcount=steps+1; targetpal=-1;
}

void morphtocol_pal(int p1,int steps)
{
unsigned char huge *p=palpos;
p+=(p1*768);
int a;
for(a=0;a<256;a++)
	{
	unsigned char cr,cg,cb;
	cb=*(p++); cg=*(p++); cr=*(p++);
	stepr[a]=-((float)cr/steps);
	stepg[a]=-((float)cg/steps);
	stepb[a]=-((float)cb/steps);
	colr[a]=0; colg[a]=0; colb[a]=0;
	}
colcount=steps+1; targetpal=p1;
}

void morphcol_pal(int pal1,int pal2, int steps)
{
unsigned char huge *p=palpos, huge *pp=palpos;
int a;
p=p+(pal1*768);
pp=pp+(pal2*768);
for(a=0;a<256;a++)
	{
	unsigned char cr,cg,cb,cr2,cg2,cb2;
	cb=*(p++); cg=*(p++); cr=*(p++);
	cb2=*(pp++); cg2=*(pp++); cr2=*(pp++);
	stepr[a]=-((float)(cr2-cr)/steps);
	stepg[a]=-((float)(cg2-cg)/steps);
	stepb[a]=-((float)(cb2-cb)/steps);
	colr[a]=cr; colg[a]=cg; colb[a]=cb;
	}
colcount=steps+1; targetpal=pal2;
}

int morph(void)
{
int a;
if(colcount==0) return(0);
if(--colcount==0)
	{
	if(targetpal!=-1)
		{
		use_pal(targetpal);
		}
	return(0);
	}
for(a=0;a<256;a++)
	{
	colr[a]-=stepr[a]; colg[a]-=stepg[a]; colb[a]-=stepb[a];
	outp(0x3c6,0xfff);
	outp(0x3c8,a);outp(0x3c9,(int)colr[a]);outp(0x3c9,(int)colg[a]);outp(0x3c9,(int)colb[a]);
	}
return(1);
}

void end_frame(void)
{
// Null
}

void message(void)
{
int x,y,c,z,t,ot=0;
float ang;
int r[250],g[250],b[250];
unsigned char huge *adr1=screen;
for(z=0;z<40;z++)
	{
	float up,down;
	up=z*1.615; down=(63.0-(z*1.615));
	r[z]=0;    r[z+40]=up; r[z+80]=63;   r[z+120]=63; r[z+160]=down; r[z+200]=0;
	g[z]=down; g[z+40]=0;  g[z+80]=0;    g[z+120]=up; g[z+160]=63;   g[z+200]=63;
	b[z]=63;   b[z+40]=63; b[z+80]=down; b[z+120]=0;  b[z+160]=0;    b[z+200]=up;
	}
printf("\n\n Screamer Graphics Library.\n\n By D. Cartwright.\n\n V 3.12\n\n (c)2000... Still Un-optimized");
for(y=0;y<200;y++)
	{
	c=y;
	for(x=0;x<320;x++)
		{
		c=(c%239)+1;
		if(*(adr1)!=0) {*(adr1)=0;}else{*(adr1)=c;}
		adr1++;
		}
	}
ang=0;
do{
	ang++;
	if(ang==360) ang=0;
	t=(sin((ang/180.0)*3.14159)*239);
	if(t<0) t=t+240;
	delay(5);
	if(t!=ot)
		{
		for(z=1;z<240;z++)
			{
			int o;
			o=(z+t)%240;
			change_col(z,r[o],g[o],b[o]);
			}
		ot=t;
		}
}while(!kbhit());
getch();
}

void vsync_wait(int frames)
{
do{delay(1);while(((inportb(0x3da)&8)!=8));frames--;}while(frames!=0);
}

void ini_screamer(int quiet)
{
if(quiet==0)
	{
	printf("\nScreamer graphics library V3.12 (c)2000 Fluid-Vision\nInitialization complete, opening DOS window.");
	delay(3000);
	}
set_mode(0x13);
// My info, backdoor.
get_keys();
if(left[0]==1&&right[0]==1) message();
}

void end_screamer()
{
set_mode(0x03);
}

void del_sprite(int sprite)
{
long int sprite_image_length,sprite_image_start,sprite_block_length;
SPRITE=_SPRITE;
SPRITE+=sprite;//Move to selected sprite to delete in the SAT.
sprite_image_length=SPRITE->len;
sprite_block_length=sprptr;
sprite_image_start=SPRITE->add;

//  sprite_block_length	= LENGTH OF THE SPRITE BLOCK.
//  spdata					= MEMORY ADDRESS OF SPRITE BLOCK.

//  sprite_image_length	= LENGTH IN BYTES OF SPRITE IMAGE.
//  sprite_image_start	= START OFFSET IN BYTES FOR SPRITE IMAGE.

//Step 1: Physically remove sprite from sprite block.
for(long int index=sprite_image_start;index<sprite_block_length-sprite_image_length;index++)
	*(spdata+index)=*(spdata+index+sprite_image_length);

//Step 2: Reallocate Memory...
sprptr-=sprite_image_length;
if((spdata=(unsigned char huge*)farrealloc(spdata,sprptr))==NULL)
	{
	printf("del_sprite error!");getch();exit(1);
	}
//Step 3: Update SAT.

}


//void del_sprite(int sprite)
//{
/* For this to work you must do the following...

A)
Imagine the block of memory containing the sprite
bitmaps (NOT the sprite allocation table.).

##########$$$$@@@@@@@@@@@@

#=Sprite info before the sprite that is deleted...
$=Sprite bitmap to delete...
@=Sprite bitmaps after the sprite to be deleted...
The above info can be found in the Sprite Allocation Table.

B)
Sooo....(slide bitmaps after sprite to left)

			 <-----------!!!!
##########$$$$@@@@@@@@@@@@

Leaving...

##########@@@@@@@@@@@@::::

:=Memory now not needed...

C)
Then realloc the sprite bitmaps to free up space...

##########@@@@@@@@@@@@

D)
You then need to search the Sprite Allocation Table for all
pointers refering to sprites AFTER the one that has been
deleted, when one is found subtract the length of the deleted
sprite from it.

E)
And finally update the sprite allocation table for the
deleted sprite...

Just set things to -1's

P.S. If the sprite is then added later it is just added
to the end of the sprite bitmap table as usuall...
//}
*/

