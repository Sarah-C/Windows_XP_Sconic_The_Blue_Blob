#include "Screamer.H"

void trans_convert_rgbtopal(int &r,int &g,int &b,int &col)
{
col=((r/43)*6)+((g/43)*36)+(b/43);
}

int trans_return_rgbtopal(int r,int g,int b)
{
return ((r/43)*6)+((g/43)*36)+(b/43);
}

void trans_convert_paltorgb(int &r,int &g,int &b,int &col)
{
r=43*((col/6)%6);
g=43*((col/36)%6);
b=43*(col%6);
}

void trans_make_pal(void)
{
int r,g,b;
for(int col=0;col<216;col++)
	{
	trans_convert_paltorgb(r,g,b,col);
	change_col(col,r/4,g/4,b/4);
	}
}

void trans_store_pal(int pal)
{
int r,g,b;
for(int col=0;col<216;col++)
	{
	trans_convert_paltorgb(r,g,b,col);
	changecol_pal(pal,col,r,g,b);
	}
}

void trans_ppixel(int x,long int y,unsigned short int r,unsigned short int g,unsigned short int b,unsigned char huge *address)
{
unsigned int r1=0,g1=0,b1=0,col=0;
address=address+(((y)<<8)+((y)<<6)+(x));
col=*(address);
r1=43*((col/6)%6);
g1=43*((col/36)%6);
b1=43*(col%6);
r1+=r;
g1+=g;
b1+=b;
if(r1>255) r1=255;
if(g1>255) g1=255;
if(b1>255) b1=255;
col=(r1/43)*6;
col+=(g1/43)*36;
col+=b1/43;
*(address)=col;
}

void trans_triangle(int x1,int y1,int x2,int y2,int x3,int y3,int col,int page)
{
float xstep1,xstep2,xstep3,xpos1,xpos2,xx1,xx2,di1=0,di2=0,di3=0;
float xx[3]={x1,x2,x3},yy[3]={y1,y2,y3};
int yloop,tmp,trans=0;
int top=0,mid=1,bot=2,x;
unsigned char huge *add=buffers;
unsigned int redb,greenb,blueb,redf,greenf,bluef;
unsigned char colour;
if(col<0) {col=col*-1; trans=1;}
redf=43*((col/6)%6);
greenf=43*((col/36)%6);
bluef=43*(col%6);
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
				redb=43*((*(add+x)/6)%6);
				greenb=43*((*(add+x)/36)%6);
				blueb=43*(*(add+x)%6);
				redb+=redf;
				greenb+=greenf;
				blueb+=bluef;
				if(redb>255) redb=255;
				if(greenb>255) greenb=255;
				if(blueb>255) blueb=255;
				colour=(redb/43)*6;
				colour+=(greenb/43)*36;
				colour+=blueb/43;
				*(add+x)=colour;
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
				redb=43*((*(add+x)/6)%6);
				greenb=43*((*(add+x)/36)%6);
				blueb=43*(*(add+x)%6);
				redb+=redf;
				greenb+=greenf;
				blueb+=bluef;
				if(redb>255) redb=255;
				if(greenb>255) greenb=255;
				if(blueb>255) blueb=255;
				colour=(redb/43)*6;
				colour+=(greenb/43)*36;
				colour+=blueb/43;
				*(add+x)=colour;
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
				redb=43*((*(add+x)/6)%6);
				greenb=43*((*(add+x)/36)%6);
				blueb=43*(*(add+x)%6);
				redb+=redf;
				greenb+=greenf;
				blueb+=bluef;
				if(redb>255) redb=255;
				if(greenb>255) greenb=255;
				if(blueb>255) blueb=255;
				colour=(redb/43)*6;
				colour+=(greenb/43)*36;
				colour+=blueb/43;
				if((((int)(x+yloop)/2)*2==(int)(x+yloop))) *(add+x)=colour;
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
				redb=43*((*(add+x)/6)%6);
				greenb=43*((*(add+x)/36)%6);
				blueb=43*(*(add+x)%6);
				redb+=redf;
				greenb+=greenf;
				blueb+=bluef;
				if(redb>255) redb=255;
				if(greenb>255) greenb=255;
				if(blueb>255) blueb=255;
				colour=(redb/43)*6;
				colour+=(greenb/43)*36;
				colour+=blueb/43;
				if((((int)(x+yloop)/2)*2==(int)(x+yloop))) *(add+x)=colour;
				}
			}
		add+=320;
		xpos1+=xstep1;
		xpos2+=xstep3;
		}
	}
}

void putcm_trans_sprite(int screenx,int screeny,int screenxend,int screenyend,int spritex,int spritey,int sprite,int page)
{
long int spritexend,spriteyend;
long int clipx,clipxend,clipyend;
long int clipy;
long int xx,yy;
long int leftinc=0,rightinc=0,screeninc;
unsigned char huge *add,huge *spradd;
unsigned long int p=page*64000l;
unsigned int redb,greenb,blueb,redf,greenf,bluef;
unsigned char colour;
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
		redb=43*((*(add)/6)%6);
		greenb=43*((*(add)/36)%6);
		blueb=43*(*(add)%6);
		redf=43*((*(spradd)/6)%6);
		greenf=43*((*(spradd)/36)%6);
		bluef=43*(*(spradd)%6);
		redb+=redf;
		greenb+=greenf;
		blueb+=bluef;
		if(redb>255) redb=255;
		if(greenb>255) greenb=255;
		if(blueb>255) blueb=255;
		colour=(redb/43)*6;
		colour+=(greenb/43)*36;
		colour+=blueb/43;
		*(add)=colour;
		add++;spradd++;
		}
	add+=screeninc;
	spradd+=rightinc;
	}
}

void putcms_trans_sprite(int screenx,int screeny,int screenxend,int screenyend,int spritex,int spritey,float scalex,float scaley,int sprite,int page)
{
long int spritexend,spriteyend;
long int clipx,clipxend,clipyend;
long int clipy;
long int xx,yy;
int posy=1;
float leftinc=.1,rightinc=0,screeninc;
unsigned char huge *add,huge *spradd;
unsigned long int p=page*64000l;
unsigned int redb,greenb,blueb,redf,greenf,bluef;
unsigned char colour;
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
		redb=43*((*(add)/6)%6);
		greenb=43*((*(add)/36)%6);
		blueb=43*(*(add)%6);
		redf=43*((*(spradd+(int)scalepos)/6)%6);
		greenf=43*((*(spradd+(int)scalepos)/36)%6);
		bluef=43*(*(spradd+(int)scalepos)%6);
		redb+=redf;
		greenb+=greenf;
		blueb+=bluef;
		if(redb>255) redb=255;
		if(greenb>255) greenb=255;
		if(blueb>255) blueb=255;
		colour=(redb/43)*6;
		colour+=(greenb/43)*36;
		colour+=blueb/43;
		if(*(spradd+(int)scalepos)!=0) *(add)=colour;
		add++;scalepos+=1/scalex;
		}
	posy++;
	add+=(int)screeninc;
	scalepos+=rightinc;
	}
}

