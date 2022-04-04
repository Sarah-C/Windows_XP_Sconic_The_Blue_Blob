//Sconic the blue blob main games engine.
//Build 1.6.1

#include "C:\myc__p~1\Library2\Screamer.H"
#include "keyboard.h"
#include <ctype.h>
#include <time.h>

//Create data strustures for effects, rings, and platforms.
struct aneffect {int used;int delay;int timer; int xp; int yp; short int sprite; };
struct aring {float xp; float yp; float xv; float yv; int collect_delay;int timeout;int framenum;int animax;int ani;};
struct aplatform{short int used; int xp;float yp;int mlx;int mly;int lx;int ly;short int directionx;short int directiony;int delay;short int length;
	int type;float yvel;};

// Monster information structure.
struct amonster
	 {
	 short int type; float xp; float yp; short int dead; float baseyp; float yv;
	 int jumpdelay;short int visible;short int killable;short int alwaysupdate;
	 };

//Background animation
struct back_hue
	{
	short int red;
	short int green;
	short int blue;
	};

//Spring animation structure.
struct ajump
	{
	int mapx; int mapy; int time; int used; int orig;
	};

//The structure for the Sonic Animation data...
struct ananimation {int maxframe; int frame; int framevector[10]; int maxscreencount; int screencount;};

//Define the animation vector address, better than enum's as they're faster.
#define JUMP_LEFT_ANIMATION 0
#define JUMP_RIGHT_ANIMATION 1
#define WALK_LEFT_ANIMATION 2
#define WALK_RIGHT_ANIMATION 3
#define BALANCE_LEFT_ANIMATION 4
#define BALANCE_RIGHT_ANIMATION 5
#define RUN_LEFT_ANIMATION 6
#define RUN_RIGHT_ANIMATION 7
#define SCONIC_DEMO_ANIMATION 8
#define LAVA_TOP_ANIMATION 9
#define LAVA_BOTTOM_ANIMATION 10
#define SPINNING_RING_ANIMATION 11

//Rolling demo flags...
int _CREATE_DEMO,_DELAY=2;
FILE *demofile;
float demorecord=1000,maxdemorecord=demorecord;//Length of time to record

//In pixels...
#define WORLD_SIZE_X 1280
#define WORLD_SIZE_Y 1280

//Testing...
#define DEBUG 0

//Define as blocks of 16x16 pixels...
#define VIEW_PORT_X 18
#define VIEW_PORT_Y 10

//Define the possible outcomes of the game, with no data type.
enum {
	DEAD,
	WON,
	QUIT,
	UNKNOWN
}outcome;

//Define maximum possible number of ingame effects and monsters.
#define MAX_EFFECTS 350
#define MAX_MONSTERS 300

//Background
struct back_hue back_hues[200];

//Create the map array.
int map[(WORLD_SIZE_X/16)+2][(WORLD_SIZE_Y/16)+2];
int mapfront[(WORLD_SIZE_X/16)+2][(WORLD_SIZE_Y/16)+2];

//The animation array.
struct ananimation animation[12];

//Create the (map array) -> (graphic array) vectors.
int sprite_matrix[102];

//Calculate display information.
int world_size_x=WORLD_SIZE_X,world_size_y=WORLD_SIZE_Y;
int view_port_x=18,view_port_y=10;

//Make system display and control structures...
struct aneffect effect[MAX_EFFECTS];
struct aring ring[50];
struct amonster monster[MAX_MONSTERS];
struct ajump jump[5];
struct aplatform platform[30];

//Create sconics status flags (not using bits....)...
//Sonic_x & y are the TOP LEFT of the sonic sprite...
float sonic_x=17,sonic_y=17,sonic_xv=0,sonic_yv=0;
float frame_delay=0,frame_delay1,frame_delay2;
int sonic_facing=1,sonic_jumping=0,sonic_faling=1,sonic_blinking=0;
int sonic_sprite_on=1,sonic_stuned=0,sonic_invincible=0;
int jump_key=0,rings=0,key_press=0,delay_count=400,sonic_spring=0;
int sonic_shield=0,sonic_lives=0,sonic_protect=0,sonic_zoom=0,demo=0;
int sonic_level=1,win_count=0,no_rings,score=0,high_score=0,letgo=0,diag=0;
int solid_up[186]=   {0,1,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//                                No. 26 this is walk-through ground...--^
int solid_down[186]= {0,1,1,1,1,0,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int solid_left[186]= {0,1,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int solid_right[186]={0,1,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void putm_my_sprite(int x,long int y,int sprite)
{
//Replacement Sprite plotting routine for a 16x16 pixel sprite...
//Should be a bit faster...
unsigned char huge *add,huge *spradd;
add=buffers;
add+=((y<<2)+y)<<6;
add+=x;
SPRITE=_SPRITE;
SPRITE+=sprite;
spradd=spdata+SPRITE->add;
//1
for(short unsigned int yc=0;yc<16;yc++)
	{
	for(short unsigned int xc=0;xc<16;xc++)
		{
		if(*(spradd)!=0) *(add)=*(spradd); add++;spradd++;
		}
	add+=304;
	}
}

void loading_bar(int bar)
{
int co,x,y;
static int oldbar=0;
if(oldbar==bar) return;
oldbar=bar;
#if(!DEBUG)
	delay(15);
#endif
for(x=0;x<8;x++)
	{
	if(x==7)
		{
		for(y=160;y<193;y++)
			{
			co=gpixel(bar+54-x,y,screen);
			if(co==199) ppixel(bar+54-x,y,0,screen);//223
			}
		}
		else
		{
		for(y=160;y<193;y++)
			{
			co=gpixel(bar+55-x,y,screen);
			if((co==0)||(co>191)) ppixel(bar+55-x,y,223-(x*4),screen);//223

			co=gpixel(bar+63-x,y,screen);
			if((co==0)||(co>191)) ppixel(bar+63-x,y,199+(x*4),screen);//223
			}
		}
	}
}

void init_animation(void)
{
//Init jump left...
animation[JUMP_LEFT_ANIMATION].maxframe=3;
animation[JUMP_LEFT_ANIMATION].frame=0;
animation[JUMP_LEFT_ANIMATION].maxscreencount=2;
animation[JUMP_LEFT_ANIMATION].screencount=0;
animation[JUMP_LEFT_ANIMATION].framevector[0]=152;animation[JUMP_LEFT_ANIMATION].framevector[1]=151;
animation[JUMP_LEFT_ANIMATION].framevector[2]=150;animation[JUMP_LEFT_ANIMATION].framevector[3]=18;
//Init jump right...
animation[JUMP_RIGHT_ANIMATION].maxframe=3;
animation[JUMP_RIGHT_ANIMATION].frame=0;
animation[JUMP_RIGHT_ANIMATION].maxscreencount=2;
animation[JUMP_RIGHT_ANIMATION].screencount=0;
animation[JUMP_RIGHT_ANIMATION].framevector[0]=18;animation[JUMP_RIGHT_ANIMATION].framevector[1]=150;
animation[JUMP_RIGHT_ANIMATION].framevector[2]=151;animation[JUMP_RIGHT_ANIMATION].framevector[3]=152;
//init walk right...
animation[WALK_RIGHT_ANIMATION].maxframe=5;
animation[WALK_RIGHT_ANIMATION].frame=0;
animation[WALK_RIGHT_ANIMATION].maxscreencount=4;
animation[WALK_RIGHT_ANIMATION].screencount=0;
animation[WALK_RIGHT_ANIMATION].framevector[0]=153;animation[WALK_RIGHT_ANIMATION].framevector[1]=154;
animation[WALK_RIGHT_ANIMATION].framevector[2]=155;animation[WALK_RIGHT_ANIMATION].framevector[3]=156;
animation[WALK_RIGHT_ANIMATION].framevector[4]=157;animation[WALK_RIGHT_ANIMATION].framevector[5]=158;
//init walking left...
animation[WALK_LEFT_ANIMATION].maxframe=5;
animation[WALK_LEFT_ANIMATION].frame=0;
animation[WALK_LEFT_ANIMATION].maxscreencount=4;
animation[WALK_LEFT_ANIMATION].screencount=0;
animation[WALK_LEFT_ANIMATION].framevector[0]=159;animation[WALK_LEFT_ANIMATION].framevector[1]=160;
animation[WALK_LEFT_ANIMATION].framevector[2]=161;animation[WALK_LEFT_ANIMATION].framevector[3]=162;
animation[WALK_LEFT_ANIMATION].framevector[4]=163;animation[WALK_LEFT_ANIMATION].framevector[5]=164;
//init balance left...
animation[BALANCE_LEFT_ANIMATION].maxframe=1;
animation[BALANCE_LEFT_ANIMATION].frame=0;
animation[BALANCE_LEFT_ANIMATION].maxscreencount=20;
animation[BALANCE_LEFT_ANIMATION].screencount=0;
animation[BALANCE_LEFT_ANIMATION].framevector[0]=19;animation[BALANCE_LEFT_ANIMATION].framevector[1]=165;
//init balance right...
animation[BALANCE_RIGHT_ANIMATION].maxframe=1;
animation[BALANCE_RIGHT_ANIMATION].frame=0;
animation[BALANCE_RIGHT_ANIMATION].maxscreencount=20;
animation[BALANCE_RIGHT_ANIMATION].screencount=0;
animation[BALANCE_RIGHT_ANIMATION].framevector[0]=20;animation[BALANCE_RIGHT_ANIMATION].framevector[1]=166;
//init run right...
animation[RUN_RIGHT_ANIMATION].maxframe=3;
animation[RUN_RIGHT_ANIMATION].frame=0;
animation[RUN_RIGHT_ANIMATION].maxscreencount=2;
animation[RUN_RIGHT_ANIMATION].screencount=0;
animation[RUN_RIGHT_ANIMATION].framevector[0]=17;animation[RUN_RIGHT_ANIMATION].framevector[1]=172;
animation[RUN_RIGHT_ANIMATION].framevector[2]=173;animation[RUN_RIGHT_ANIMATION].framevector[3]=174;
//init run left...
animation[RUN_LEFT_ANIMATION].maxframe=3;
animation[RUN_LEFT_ANIMATION].frame=0;
animation[RUN_LEFT_ANIMATION].maxscreencount=2;
animation[RUN_LEFT_ANIMATION].screencount=0;
animation[RUN_LEFT_ANIMATION].framevector[0]=16;animation[RUN_LEFT_ANIMATION].framevector[1]=175;
animation[RUN_LEFT_ANIMATION].framevector[2]=176;animation[RUN_LEFT_ANIMATION].framevector[3]=177;
//Demo
animation[SCONIC_DEMO_ANIMATION].maxframe=1;
animation[SCONIC_DEMO_ANIMATION].frame=0;
animation[SCONIC_DEMO_ANIMATION].maxscreencount=20;
animation[SCONIC_DEMO_ANIMATION].screencount=0;
animation[SCONIC_DEMO_ANIMATION].framevector[0]=178;animation[SCONIC_DEMO_ANIMATION].framevector[1]=179;
//Lava Top
animation[LAVA_TOP_ANIMATION].maxframe=1;
animation[LAVA_TOP_ANIMATION].frame=0;
animation[LAVA_TOP_ANIMATION].maxscreencount=15;
animation[LAVA_TOP_ANIMATION].screencount=0;
animation[LAVA_TOP_ANIMATION].framevector[0]=65;animation[LAVA_TOP_ANIMATION].framevector[1]=66;
//Lava Bottom
animation[LAVA_BOTTOM_ANIMATION].maxframe=1;
animation[LAVA_BOTTOM_ANIMATION].frame=0;
animation[LAVA_BOTTOM_ANIMATION].maxscreencount=20;
animation[LAVA_BOTTOM_ANIMATION].screencount=0;
animation[LAVA_BOTTOM_ANIMATION].framevector[0]=62;animation[LAVA_BOTTOM_ANIMATION].framevector[1]=63;
//Spinning Rings
animation[SPINNING_RING_ANIMATION].maxframe=9;
animation[SPINNING_RING_ANIMATION].frame=0;
animation[SPINNING_RING_ANIMATION].maxscreencount=5;
animation[SPINNING_RING_ANIMATION].screencount=0;
animation[SPINNING_RING_ANIMATION].framevector[0]=12;animation[SPINNING_RING_ANIMATION].framevector[1]=167;
animation[SPINNING_RING_ANIMATION].framevector[2]=168;animation[SPINNING_RING_ANIMATION].framevector[3]=169;
animation[SPINNING_RING_ANIMATION].framevector[4]=170;animation[SPINNING_RING_ANIMATION].framevector[5]=171;
animation[SPINNING_RING_ANIMATION].framevector[6]=170;animation[SPINNING_RING_ANIMATION].framevector[7]=169;
animation[SPINNING_RING_ANIMATION].framevector[8]=168;animation[SPINNING_RING_ANIMATION].framevector[9]=167;
}

int cycle_animation_vector(int sprite)
{
if(++animation[sprite].screencount>animation[sprite].maxscreencount)
	{
	animation[sprite].screencount=0;
	if(++animation[sprite].frame>animation[sprite].maxframe)
		{
		animation[sprite].frame=0;
		}
	}
return(animation[sprite].framevector[animation[sprite].frame]);
}

void draw_border(void)
{
int xlength=10-view_port_x/2,ylength=6-view_port_y/2;
int x,y,tlx=16.0*(10.0-view_port_x/2.0)-1,tly=16.0*(6.0-view_port_y/2.0)+3;
int blx=1+tlx+view_port_x*16,bly=1+tly+view_port_y*16;
for(x=0;x<20;x++)
	{
	for(y=-1;y<ylength;y++)
		{
		put_sprite(x*16,y*16+4,181,255);
		}
	}
for(x=0;x<20;x++)
	{
	for(y=12;y>11-ylength;y--)
		{
		put_sprite(x*16,y*16+4,181,255);
		}
	}
for(x=0;x<xlength;x++)
	{
	for(y=ylength;y<6+view_port_y/2;y++)
		{
		put_sprite(x*16,y*16+4,181,255);
		}
	}
for(x=20-xlength;x<20;x++)
	{
	for(y=ylength;y<6+view_port_y/2;y++)
		{
		put_sprite(x*16,y*16+4,181,255);
		}
	}
for(x=0;x<5;x++)
	{
	draw(tlx-x,tly-x,tlx-x,bly+x,20,255);//l
	draw(tlx-x,tly-x,blx+x,tly-x,18,255);//t
	draw(blx+x,tly-x,blx+x,bly+x,19,255);//r
	draw(tlx-x,bly+x,blx+x,bly+x,17,255);//b
	}
}

void clear_map(void)
{
int z,y;
//initialise game info.
for(y=0;y<80;y++){for(z=0;z<80;z++){map[z][y]=0;mapfront[z][y]=0;}}
for(z=0;z<MAX_MONSTERS;z++)
	{
	monster[z].xp=0; monster[z].yp=0;
	monster[z].dead=1; monster[z].jumpdelay=0;
	monster[z].baseyp=0; monster[z].type=0;
	monster[z].visible=0;monster[z].killable=0;
	monster[z].alwaysupdate=0;
	}
for(z=0;z<MAX_EFFECTS;z++) effect[z].used=0;
for(z=0;z<5;z++) jump[z].used=0;
for(z=0; z<50; z++)
	{
	ring[z].xp=0; ring[z].yp=0;
	ring[z].xv=0;
	ring[z].yv=0;
	ring[z].collect_delay=0; ring[z].timeout=0;
	}
for(z=0;z<30;z++)
	{
	platform[z].used=0;
	platform[z].xp=0;
	platform[z].yp=0;
	platform[z].mlx=0;
	platform[z].mly=0;
	platform[z].lx=0;
	platform[z].ly=0;
	platform[z].directionx=0;
	platform[z].directiony=0;
	platform[z].delay=0;
	platform[z].type=0;
	platform[z].yvel=0;
	}
}

void load_level(char *level)
{
FILE *in;
char name[26];
int y,z,count=0,temp;
if ((in=fopen(level, "rt"))==NULL)
	{
	gotoxy(2,3);
	fprintf(stderr, " Sconic The Blue Blob can't find the  ");
	gotoxy(2,4);
	fprintf(stderr, " next level! This is probably due to  ");
	gotoxy(2,5);
	fprintf(stderr, " a bad instalation! Reinstall and try ");
	gotoxy(2,6);
	fprintf(stderr, " again!                               ");
	gotoxy(2,7);
	fprintf(stderr, " Please press any key to end SCONIC!  ");
	while(kbhit()) getch();
	getch();exit(0);}
fread(name,25,1,in);
name[25]=0;
use_pal(1);
move_screen(2,255);
gotoxy(16,2);//21
printf("Loading");
gotoxy(7,3);//12
printf("%s",name);
for(z=0;z<320;z++)
	{
	for(y=0;y<24;y++)
		{
		if(gpixel(z,y,screen)!=0) ppixel(z,y,(int)(64+(y%8)*6),screen);
		}
	}
fscanf(in,"%f %f %d ",&sonic_x,&sonic_y,&sonic_facing);
fscanf(in,"%d %d ",&world_size_x,&world_size_y);
#if(!DEBUG)
	delay(1000);
#endif
for(y=0;y<80;y++)
	{
	for(z=0;z<80;z++)
		{
		fscanf(in,"%d %d ",&map[z][y],&mapfront[z][y]);
		loading_bar((count++)/30);
		}
	}
for(z=0;z<MAX_MONSTERS;z++)
	{
	fscanf(in,"%f %f %d %d %f %d %d %d %d ",&monster[z].xp,&monster[z].yp,&monster[z].dead,&monster[z].jumpdelay,&monster[z].baseyp,&monster[z].type,&monster[z].visible,&monster[z].killable,&monster[z].alwaysupdate);
	}
for(z=0;z<30;z++)
	{
	fscanf(in,"%d %d %d %d %d %d %d %d %d %d %d %d ",&platform[z].used,&platform[z].xp,&temp,&platform[z].mlx,&platform[z].mly,&platform[z].lx,&platform[z].ly,&platform[z].directionx,&platform[z].directiony,&platform[z].delay,&platform[z].length,&platform[z].type);
	platform[z].yp=temp;
	}
fclose(in);
clear_screen(0,255);
use_pal(0);
}

void load_level(int level)
{
clear_map();
if(_CREATE_DEMO) level=_CREATE_DEMO;
switch(level)
	{
	case 0: load_level("Level.0"); break;
	case 1: load_level("Level.1"); break;
	case 2: load_level("Level.2"); break;
	case 3: load_level("Level.3"); break;
	case 4: load_level("Level.4"); break;
	default: load_level("Level.1");
	}
}

void make_sprites(void)
{
int z;
printf("\nGrabbing/Decrunching Sprite Sheet-A...");
uncompress_screen("SPRITE_A.ASS",0,50,255);
printf("O.K!");
printf("\nStoring Sheet-A Sprites in memory...");
add_sprite(0  ,0 ,16,16,0 ,0);//Blank
add_sprite(16 ,0 ,16,16,1 ,0);//Ground
add_sprite(32 ,0 ,16,16,2 ,0);//Grass
add_sprite(48 ,0 ,16,16,3 ,0);//Grass Edge Left
add_sprite(64 ,0 ,16,16,4 ,0);//Grass Edge Right
add_sprite(80 ,0 ,16,16,5 ,0);//Spikes Up
add_sprite(96 ,0 ,16,16,6 ,0);//Water Left
add_sprite(112,0 ,16,16,7 ,0);//Water Middle
add_sprite(128,0 ,16,16,8 ,0);//Water Right
add_sprite(144,0 ,16,16,9 ,0);//Pole
add_sprite(160,0 ,16,16,10,0);//Pole Top
add_sprite(176,0 ,16,16,11,0);//Jumper
add_sprite(192,0 ,16,16,12,0);//Ring
add_sprite(208,0 ,16,16,13,0);//Background star
add_sprite(224,0 ,16,16,14,0);//Sonic Standing Left
add_sprite(240,0 ,16,16,15,0);//Sonic Standing Right
add_sprite(256,0 ,16,16,16,0);//Sonic Running Left
add_sprite(272,0 ,16,16,17,0);//Sonic Running Right
add_sprite(288,0 ,16,16,18,0);//Sonic Jumping
add_sprite(272,16,16,16,19,0);//Sonic Balance Left
add_sprite(288,16,16,16,20,0);//Sonic Balance Right
add_sprite(208,16,16,16,21,0);//Sonic Bouncing Left
add_sprite(224,16,16,16,22,0);//Sonic Bouncing Right
add_sprite(240,16,16,16,23,0);//Sonic Skiding Left
add_sprite(256,16,16,16,24,0);//Sonic Skiding Right
add_sprite(16 ,16,56,16,25,0);//Ring: Logo
add_sprite(0  ,16,16,16,26,0);//Walk-through Ground
add_sprite(0  ,32,16,16,27,0);//Grass tops
add_sprite(16 ,32,16,16,28,0);//Door Left
add_sprite(32 ,32,16,16,29,0);//Door Right
add_sprite(48 ,32,16,16,30,0);//Bridge Left
add_sprite(64 ,32,16,16,31,0);//Bridge Middle
add_sprite(80 ,32,16,16,32,0);//Bridge Middle Bot
add_sprite(96 ,32,16,16,33,0);//Bridge Left
add_sprite(112,32,16,16,34,0);//Grass top flower
add_sprite(128,32,16,16,35,0);//Sonic Blinking Left
add_sprite(144,32,16,16,36,0);//Sonic Blinking Right
add_sprite(160,32,16,16,37,0);//Sonic Stuned Left
add_sprite(176,32,16,16,38,0);//Sonic Stuned Right
add_sprite(192,32,16,16,39,0);//Sonic Dead
add_sprite(208,32,16,16,40,0);//Box; Invincible
add_sprite(224,32,16,16,41,0);//Box; 10 Rings
add_sprite(96 ,80,16,16,91,0);//Speed Box!!!
add_sprite(112 ,80,16,16,92,0);//Collapsable platform
add_sprite(96,160,16,16,42,0);//Spikes Down...
add_sprite(112,160,16,16,43,0);//Jump Right...
add_sprite(128,160,16,16,44,0);//Jump Left...
add_sprite(112,144,16,16,45,0);//Jump Sprung Right...
add_sprite(128,144,16,16,46,0);//Jump Sprung Left...
add_sprite(0  , 80,16,16,47,0);//Spikes Right
add_sprite(16 , 80,16,16,48,0);//Spikes Left
add_sprite(32 , 80,16,16,49,0);//Moving Platform
add_sprite(48 , 80,16,16,88,0);//Box; Sheild
add_sprite(64 , 80,16,16,89,0);//Sheild Left
add_sprite(80 , 80,16,16,90,0);//Sheild Right
add_sprite(0  ,48,16,16,50,0);//No.0
add_sprite(16 ,48,16,16,51,0);//No.1
add_sprite(32 ,48,16,16,52,0);//No.2
add_sprite(48 ,48,16,16,53,0);//No.3
add_sprite(64 ,48,16,16,54,0);//No.4
add_sprite(80 ,48,16,16,55,0);//No.5
add_sprite(96 ,48,16,16,56,0);//No.6
add_sprite(112,48,16,16,57,0);//No.7
add_sprite(128,48,16,16,58,0);//No.8
add_sprite(144,48,16,16,59,0);//No.9
add_sprite(160,48,16,16,60,0);//Sunflower stalk
add_sprite(176,48,16,16,61,0);//Sunflower head
add_sprite(240,32,16,16,62,0);//Lava 1
add_sprite(256,32,16,16,63,0);//Lava 2
add_sprite(272,32,16,16,64,0);//Lava 3
add_sprite(288,32,16,16,65,0);//Lava top 1
add_sprite(240,48,16,16,66,0);//Lava top 2
add_sprite(256,48,16,16,67,0);//Grass bitty foreground
add_sprite(224,48,16,16,68,0);//Smoke 1
add_sprite(0  ,64,16,16,69,0);//Smoke 2
add_sprite(16 ,64,16,16,70,0);//Smoke 3
add_sprite(32 ,64,16,16,71,0);//Sparkle 1
add_sprite(48 ,64,16,16,72,0);//Sparkle 2
add_sprite(64 ,64,16,16,73,0);//Sparkle 3
add_sprite(80 ,64,16,16,74,0);//Sparkle 4
add_sprite(96 ,64,16,16,75,0);//Shield 1
add_sprite(112,64,16,16,76,0);//Shield 2
add_sprite(128,64,16,16,77,0);//Shield 3
add_sprite(144,64,16,16,78,0);//Shield 4
add_sprite(160,64,16,16,79,0);//Goal
add_sprite(176,64,16,16,80,0);//Splash 1
add_sprite(192,64,16,16,81,0);//Splash 2
add_sprite(208,64,16,16,82,0);//Splash 3
add_sprite(224,64,16,16,83,0);//Lava blob 1
add_sprite(240,64,16,16,84,0);//Lava blob 2
add_sprite(256,64,16,16,85,0);//Spring Sprung
add_sprite(272,64,16,16,86,0);//Lives Logo
add_sprite(272,48,16,16,100,0);//Enemy Fly
add_sprite(288,48,16,16,101,0);//Enemy Fish
add_sprite(0 , 96,16,16,150,0);//Sonic jumping 1
add_sprite(16, 96,16,16,151,0);//Sonic jumping 2
add_sprite(32, 96,16,16,152,0);//Sonic jumping 3
add_sprite(0 ,112,16,16,153,0);//Sonic walking right 0
add_sprite(16,112,16,16,154,0);//Sonic walking right 1
add_sprite(32,112,16,16,155,0);//Sonic walking right 2
add_sprite(48,112,16,16,156,0);//Sonic walking right 3
add_sprite(64,112,16,16,157,0);//Sonic walking right 4
add_sprite(80,112,16,16,158,0);//Sonic walking right 5
add_sprite(0 ,128,16,16,159,0);//Sonic walking left 1
add_sprite(16,128,16,16,160,0);//Sonic walking left 2
add_sprite(32,128,16,16,161,0);//Sonic walking left 3
add_sprite(48,128,16,16,162,0);//Sonic walking left 4
add_sprite(64,128,16,16,163,0);//Sonic walking left 5
add_sprite(80,128,16,16,164,0);//Sonic walking left 6
add_sprite(0 ,144,16,16,165,0);//Sonic balance left 1
add_sprite(0 ,160,16,16,166,0);//Sonic balance right 1
add_sprite(16,160,16,16,182,0);//T.V. Crackle 0
add_sprite(32,160,16,16,183,0);//T.V. Crackle 1
add_sprite(48,160,16,16,184,0);//T.V. Crackle 2
add_sprite(64,160,16,16,185,0);//T.V. Crackle 3
add_sprite(80,160,16,16,87 ,0);//T.V. Smashed
add_sprite(48 , 96,16,16,167,0);//Ring animation 1
add_sprite(64 , 96,16,16,168,0);//Ring animation 2
add_sprite(80 , 96,16,16,169,0);//Ring animation 3
add_sprite(96 , 96,16,16,170,0);//Ring animation 4
add_sprite(112, 96,16,16,171,0);//Ring animation 5
add_sprite( 96,112,16,16,172,0);//Sonic running right 1
add_sprite(112,112,16,16,173,0);//Sonic running right 2
add_sprite(128,112,16,16,174,0);//Sonic running right 3
add_sprite( 96,128,16,16,175,0);//Sonic running left 1
add_sprite(112,128,16,16,176,0);//Sonic running left 2
add_sprite(128,128,16,16,177,0);//Sonic running left 3
add_sprite(167,126,50,73,178,0);//Sonic demo 0
add_sprite(217,126,64,73,179,0);//Sonic demo 1
add_sprite(151,107,155,15,180,0);//Sonic Logo
add_sprite(16 ,144,16 ,16,181,0);//Border block
printf("O.K!");
printf("\nGrabbing/Decrunching Sprite Sheet-B...");
uncompress_screen("SPRITE_B.ASS",0,50,0);
printf("O.K!");
printf("\nStoring Sheet-B Sprites in memory...");
add_sprite(  0,  0,260, 33,93,0);//Game Over
add_sprite(  0, 35, 22, 15,94,0);//High Scores
add_sprite(  0, 53, 95, 31,95,0);//Sega!
printf("O.K!");

for(z=0;z<166;z++)
	{
	sprite_matrix[z]=z;
	}
}

void cycle_background_pallet(void)
	{
	static int timer=1;
   if(win_count) return;//Normally we morph pallet during win_count, so release control
	if(--timer==0)
		{
		timer=2;
		int tr=back_hues[0].red,tg=back_hues[0].green,tb=back_hues[0].blue;
		for(int z=0;z<199;z++)
			{
			back_hues[z].red=back_hues[z+1].red;
			back_hues[z].green=back_hues[z+1].green;
			back_hues[z].blue=back_hues[z+1].blue;
			}
		back_hues[199].red=tr;
		back_hues[199].green=tg;
		back_hues[199].blue=tb;
		for(z=0;z<200;z++)
			{
			changecol_pal(0,z+50,back_hues[z].red,back_hues[z].green,back_hues[z].blue);//needed when we morph
			change_col(z+50,back_hues[z].red/4,back_hues[z].green/4,back_hues[z].blue/4);//echo to screen
			}
		}
	}

void make_background(void)
{
for(int z=0;z<100;z++)
	{
	for(int x=0;x<160;x++)
		{
		ppixel(x,z,(z*(1.8-(x/160.0)))+50,buffers+64000l);
		}
	for(x=160;x<320;x++)
		{
		ppixel(x,z,(z*(1.8-((320.0-x))/160.0))+50,buffers+64000l);
		}
	}
for(z=100;z<200;z++)
	{
	for(int x=0;x<160;x++)
		{
		ppixel(x,z,((200-z)*(1.8-(x/160.0)))+50,buffers+64000l);
		}
	for(x=160;x<320;x++)
		{
		ppixel(x,z,((200-z)*(1.8-((320.0-x))/160.0))+50,buffers+64000l);
		}
	}
for(z=0;z<100;z++) ppixel((rand()%320),(rand()%200),251,buffers+64000l);//Stars
for(z=0;z<100;z++)
	{
	back_hues[z].red=220;
	back_hues[z].green=60;
	back_hues[z].blue=z*2.55;
	}
for(z=100;z<200;z++)
	{
	back_hues[z].red=220;
	back_hues[z].green=60;
	back_hues[z].blue=((200-z)*2.55);
	}
cycle_background_pallet();
}

void make_palette(void)
{
int z;
printf("\nGrabbing game palette from Sprite Sheet-A...");
load_pal("SPRITE_A.ASS");
printf("O.K!");
changecol_pal(0,251,245,180,180);
printf("\nGrabbing/Decrunching Loading Screen...");
uncompress_screen("LOADSC.ASS",2,50,255);
printf("\nGrabbing Loading Screen palette...");
load_pal("LOADSC.ASS");
printf("O.K!");
printf("\nCreating game intro morph palette..");
add_pal(1);//New pal (No.2) for morphing game intro...
changecol_pal(2,251,245,180,180);
printf("O.K!");
for(z=0;z<100;z++) changecol_pal(2,z+50,220,60,(z*2.55));
for(z=100;z<200;z++)	changecol_pal(2,z+50,220,60,((200-z)*2.55));
printf("\nGrabbing/Decrunching Ending Screen...");
uncompress_screen("ENDSC.ASS",3,50,255);
printf("O.K!");
printf("\nGrabbing Ending Screen palette...");
load_pal("ENDSC.ASS");
printf("O.K!");
printf("\nGrabbing System palette x 2...");
load_pal("SPRITE_A.ASS");
load_pal("SPRITE_A.ASS");
printf("O.K!");
use_pal(0);//So dos box can be seen.
}

int sprite_on_screen(float x,float y)
{
if((x>=-16.0)&&(x<((view_port_x*16.0)))&&(y>=-16.0)&&(y<((view_port_y*16.0)))) return(-1); else return(0);
}

void add_effect_request(int delay,int timer, int xp,int yp, int sprite)
{
int z=0, found=0;
do
	{
	if(effect[z].used==0)
		{
		effect[z].used=1;
		effect[z].delay=delay;
		effect[z].timer=timer;
		effect[z].xp=xp;
		effect[z].yp=yp;
		effect[z].sprite=sprite;
		found=1;
		}
	z++;
	}while((z<MAX_EFFECTS)&&(found==0));
}

void process_effects(int screenx,int screeny)
{
int z,logx,logy;
for(z=0;z<MAX_EFFECTS;z++)
	{
	if(effect[z].used==1)
		{
		if(effect[z].delay>0)
			{effect[z].delay--;}
			else
			{
			effect[z].timer--;
			if(effect[z].timer<=0) effect[z].used=0;
			logx=effect[z].xp-screenx;
			logy=effect[z].yp-screeny;
			if(sprite_on_screen(logx,logy)) putm_my_sprite(logx+20,logy+20,effect[z].sprite);
			}
		}
	}
}

void display_zoom(int sprite)
{
static timer=5;
int z;
timer--;
if(timer==0)
	{
	timer=5;
	for(z=0;z<10;z++)
		{
		add_effect_request(1+z*2,1,sonic_x+(sonic_xv*(z/7.0)),sonic_y+(sonic_yv*(z/7.0)),sprite);
		}
	}
}

void add_smoke(int x,int y)
{
add_effect_request(0,5,x,y,68);
add_effect_request(5,5,x,y,69);
add_effect_request(10,5,x,y,70);
}

void add_sparkle(int x,int y)
{
add_effect_request(0,5,x,y,71);
add_effect_request(5,5,x,y,72);
add_effect_request(10,5,x,y,73);
add_effect_request(15,5,x,y,74);
}

void add_winsparkle(int x,int y)
{
float ofdel=0,c,ang=0;
float ox,oy,a,r=0;
for(c=0;c<70;c++)
	{
	r=r+.8;
	a=ang/180.0*3.1415926;
	ox=sin(a);
	oy=cos(a);
	add_effect_request(ofdel,5,x+(ox*r),y+(oy*r),71);
	add_effect_request(ofdel+5,5,x+(ox*(r+2)),y+(oy*(r+2)),72);
	add_effect_request(ofdel+10,5,x+(ox*(r+5)),y+(oy*(r+5)),73);
	add_effect_request(ofdel+15,5,x+(ox*(r+9)),y+(oy*(r+9)),74);
	ang+=19;if(ang>=360) ang-=360;
	ofdel+=.5;
	}
}

void add_splash(int x,int y)
{
add_effect_request(0,2,x,y,80);
add_effect_request(2,2,x,y,81);
add_effect_request(4,2,x,y,82);
}

void expload_ring_request(float xp, float yp,int rings)
{
int z;
if(rings<0) rings=1;
if(rings>49) rings=49;
no_rings=1;
for(z=0; z<rings; z++)
	{
	ring[z].xp=xp; ring[z].yp=yp;
	ring[z].xv=((rand()%1000)-500.0)/100.0;
	ring[z].yv=-3-((rand()%500)/100.0);
	ring[z].collect_delay=20; ring[z].timeout=200+rand()%80;
	ring[z].framenum=z%10;
	ring[z].animax=3;
	ring[z].ani=0;
	}
}

void add_rings(int no)
{
rings+=no;
if(rings>=100)
	{
	rings-=100;
	sonic_lives++;
	}
}

void create_jump_object(int mapx,int mapy,int spriteno)
{
//Find spare spring storage space, change spring to sprung, and set timer...
int z=0,found=0;
do
	{
	if(jump[z].used==0)
		{
		jump[z].used=1; jump[z].mapx=mapx;
		jump[z].mapy=mapy; jump[z].time=8;
		jump[z].orig=map[mapx][mapy];
		map[mapx][mapy]=spriteno;
		found=1;
		}
	z++;
	}while((z<5)&&(found==0));
}

void process_jump_object(void)
{
//When timer falls to zero, reset map[mapx][mapy] to unsprung spring.
int z;
for(z=0;z<5;z++)
	{
	if(jump[z].used==1)
		{
		jump[z].time--;
		if(jump[z].time==0)
			{
			jump[z].used=0;
			map[jump[z].mapx][jump[z].mapy]=jump[z].orig;
			}
		}
	}
}

void process_rings(int sonicx, int sonicy,int screenx,int screeny)
{
int z,logx,logy,rx,ry,difx,dify,oldxp,oldyp,point1,point2,point3,point4,point5;
if(no_rings==0) return;
no_rings=0;
for(z=0;z<50;z++)
	{
	if(ring[z].timeout!=0)
		{
		ring[z].timeout--;
		no_rings++;
		//Update ring...
		oldxp=ring[z].xp; oldyp=ring[z].yp;
		ring[z].xp+=ring[z].xv;
		point1=map[int((ring[z].xp+5)/16.0)][int((ring[z].yp+5)/16)];
		point2=map[int((ring[z].xp+10)/16.0)][int((ring[z].yp+5)/16)];
		point3=map[int((ring[z].xp+10)/16.0)][int((ring[z].yp+10)/16)];
		point4=map[int((ring[z].xp+5)/16.0)][int((ring[z].yp+10)/16)];
		if(ring[z].xv<0)
			{
			if((solid_left[point1]==1)||(solid_left[point4]==1))
				{
				ring[z].xp=oldxp;
				ring[z].xv=abs(ring[z].xv);
				}
			}
		if(ring[z].xv>0)
			{
			if((solid_right[point2]==1)||(solid_right[point3]==1))
				{
				ring[z].xp=oldxp;
				ring[z].xv=-(abs(ring[z].xv));
				}
			}
		ring[z].yp+=ring[z].yv;
		point1=map[int((ring[z].xp+5)/16.0)][int((ring[z].yp+5)/16)];
		point2=map[int((ring[z].xp+10)/16.0)][int((ring[z].yp+5)/16)];
		point3=map[int((ring[z].xp+10)/16.0)][int((ring[z].yp+10)/16)];
		point4=map[int((ring[z].xp+5)/16.0)][int((ring[z].yp+10)/16)];
		point5=map[int((ring[z].xp+8)/16.0)][int((ring[z].yp+8)/16)];
		if((point5==62)||(point5==65)) {ring[z].timeout=0;add_smoke(ring[z].xp,ring[z].yp);}
		if(ring[z].yv<0)
			{
			if((solid_up[point1]==1)||(solid_up[point2]==1))
				{
				ring[z].yp=oldyp;
				ring[z].yv=abs(ring[z].yv);
				}
			}
		if(ring[z].yv>0)
			{
			if((solid_down[point3]==1)||(solid_down[point4]==1))
				{
				ring[z].yp=oldyp;
				ring[z].yv=-(abs(ring[z].yv));
				}
			}
		ring[z].xv/=1.005;ring[z].yv+=.2;
		logx=ring[z].xp-screenx;
		logy=ring[z].yp-screeny;
		if(++ring[z].ani>ring[z].animax)
			{
			ring[z].ani=0;
			if(++ring[z].framenum>9) ring[z].framenum=0;
			}
		if(sprite_on_screen(logx,logy)) putm_my_sprite(logx+20,logy+20,animation[SPINNING_RING_ANIMATION].framevector[ring[z].framenum]);
		if((ring[z].collect_delay)!=0)
			{
			ring[z].collect_delay--;
			}
			else
			{
			rx=ring[z].xp; ry=ring[z].yp;
			difx=sonicx-rx; dify=sonicy-ry;
			if((difx>-8)&&(difx<8)&&(dify>-8)&&(dify<8))
				{
				add_rings(1);
				ring[z].timeout=0;
				add_sparkle(rx,ry);
				}
			}
		}
	}
}


void update_lava_display_matrix(void)
{
sprite_matrix[65]=cycle_animation_vector(LAVA_TOP_ANIMATION);//Lava top
sprite_matrix[62]=cycle_animation_vector(LAVA_BOTTOM_ANIMATION);//Lava Bottom
}

void update_world_ring_matrix(void)
{
sprite_matrix[12]=cycle_animation_vector(SPINNING_RING_ANIMATION);//Rings
}

void update_tv_crackle_display_matrix(void)
{
static int tv1on=50,tv2on=50,tv3on=50,tv4on=50;;
if(tv1on==0)
	{
	if(rand()%10==1)
		{
		sprite_matrix[40]=40;
		tv1on=5+rand()%100;
		}
		else
		{
		sprite_matrix[40]=182+rand()%4;
		}
	}
	else
	{
	tv1on--;
	}
if(tv2on==0)
	{
	if(rand()%10==1)
		{
		sprite_matrix[41]=41;
		tv2on=5+rand()%100;
		}
		else
		{
		sprite_matrix[41]=182+rand()%4;
		}
	}
	else
	{
	tv2on--;
	}
if(tv3on==0)
	{
	if(rand()%10==1)
		{
		sprite_matrix[88]=88;
		tv3on=5+rand()%100;
		}
		else
		{
		sprite_matrix[88]=182+rand()%4;
		}
	}
	else
	{
	tv3on--;
	}
if(tv4on==0)
	{
	if(rand()%10==1)
		{
		sprite_matrix[91]=91;
		tv4on=5+rand()%100;
		}
		else
		{
		sprite_matrix[91]=182+rand()%4;
		}
	}
	else
	{
	tv4on--;
	}
}

void render_sky_effects(void)
{
for(int z=0;z<5000;z++) ppixel((rand()%320),(rand()%200),251,buffers);//Stars
}

void render_sky(int x,int y)
{
float vpx=view_port_x*16, vpy=view_port_y*16;
float background_scale_x=(500.0-(float)vpx)/(world_size_x-(float)vpx);
float background_scale_y=(500.0-(float)vpy)/(world_size_y-(float)vpy);
int xp=Round(x*background_scale_x);
int yp=Round(y*background_scale_y);
int b=xp%320, a=320-b;
int d=yp%200, c=200-d;
int xr=(b>(320-vpx));
int yr=(d>(200-vpy));
if((xr)&&(!yr))
	{
	// +4 because block_screen operates on multiples of 4 Bytes!!!!!(32Bit Code)
	block_screen(1,b,d,a+4,vpy,0,20  ,20);
	block_screen(1,0 ,d,(vpx-a)+4,vpy,0,a+20,20);
	}
if((!xr)&&(!yr))
	{
	block_screen(1,b,d,vpx,vpy,0,20,20);
	}
if((!xr)&&(yr))
	{
	block_screen(1,b,d ,vpx,c    ,0,20,20);
	block_screen(1,b,0 ,vpx,vpy-c,0,20,c+20);
	}
if((xr)&&(yr))
	{
	block_screen(1,b,d,a+4    ,c    ,0,20  ,20);
	block_screen(1,b,0,a+4    ,vpy-c,0,20  ,c+20);
	block_screen(1,0,0,vpx-a+4,vpy-c,0,a+20,c+20);
	block_screen(1,0,d,vpx-a+4,c    ,0,a+20,20);
	}
//render_sky_effects();
}

void render_background(int screenx,int screeny)
{
int brick,blockx,blocky,blockxpos,blockypos,bx,by;
blockx=(screenx/16); blocky=(screeny/16);
blockxpos=(screenx%16); blockypos=(screeny%16);
for(by=0;by<(view_port_y+1);by++)
	{
	for(bx=0;bx<(view_port_x+1);bx++)
		{
		brick=map[blockx][blocky];
		putm_my_sprite((20-blockxpos)+(bx*16),(20-blockypos)+(by*16),sprite_matrix[brick]);
		blockx++;;
		}
	blockx-=view_port_x+1;blocky++;
	}
}

void render_foreground(int screenx,int screeny)
{
int brick,blockx,blocky,blockxpos,blockypos,bx,by;
blockx=(screenx/16); blocky=(screeny/16);
blockxpos=(screenx%16); blockypos=(screeny%16);
for(by=0;by<(view_port_y+1);by++)
	{
	for(bx=0;bx<(view_port_x+1);bx++)
		{
		brick=mapfront[blockx][blocky];
		if(brick!=0) putm_my_sprite((20-blockxpos)+(bx*16),(20-blockypos)+(by*16),sprite_matrix[brick]);
		blockx++;;
		}
	blockx-=view_port_x+1;blocky++;
	}
}

float find_angle(float fmx,float fmy)
{
float ang=90;
if(fmx!=0.0){ang=atan(fmy/fmx);ang=(ang/3.141592653589793238)*180.0;}
if(ang<0.0) ang+=180.0;//Normalize Left/Right...
if((fmy>=0.0))
	{//Top/Bottom Don't ajust if at 0 degrees
	if(fmx>0.0) ang+=180.0;
	if((fmx<=0.0)&&(ang!=0.0)) ang+=180.0;
	}
if((ang+=90.0)>=360.0) ang-=360.0;//Rotate for SIN/COS; cant use '%' cos of float.
ang=360.0-ang;//Mirror
return(ang/180.0*3.141592653589793238);//Mirror.
}

void update_fly(int mn,int sonicx,int sonicy)
{
float ang=find_angle(sonicx-monster[mn].xp,sonicy-monster[mn].yp);
if(monster[mn].jumpdelay>0) {monster[mn].jumpdelay-=1; return;}
monster[mn].xp+=sin(ang)*2.0;
monster[mn].yp+=cos(ang)*2.0;
}

void update_fish(int mn,int sos)
{
if(monster[mn].jumpdelay>1) {monster[mn].jumpdelay-=1; return;}
if(monster[mn].jumpdelay==1)
	{
	monster[mn].visible=1;
	monster[mn].jumpdelay=0;
	monster[mn].yv=-10;
	if(sos) add_splash(monster[mn].xp,monster[mn].baseyp);
	return;
	}
monster[mn].yp+=monster[mn].yv;
monster[mn].yv++;
if(monster[mn].yp>monster[mn].baseyp)
	{
	monster[mn].yp=monster[mn].baseyp;monster[mn].visible=0; monster[mn].jumpdelay=12;
	if(sos) add_splash(monster[mn].xp,monster[mn].baseyp);
	}
}

void update_lavablob(int mn)
{
if(monster[mn].jumpdelay>1) {monster[mn].jumpdelay-=1; return;}
if(monster[mn].jumpdelay==1)
	{
	monster[mn].visible=1;
	monster[mn].jumpdelay=0;
	monster[mn].yv=-10;
	return;
	}
monster[mn].yp+=monster[mn].yv;
monster[mn].yv++;
if(monster[mn].yp>monster[mn].baseyp)
	{
	monster[mn].yp=monster[mn].baseyp-16;monster[mn].visible=0; monster[mn].jumpdelay=12;
	}
}

void update_monsters(int sonicx,int sonicy,int screenx,int screeny)
{
int mn,logx,logy,mx,my,difx,dify,sos=0;
for(mn=0;mn<MAX_MONSTERS;mn++)
	{
	if(monster[mn].dead==0)
		{
		logx=monster[mn].xp-screenx;
		logy=monster[mn].yp-screeny;
		sos=sprite_on_screen(logx,logy);
		if((sos)||(monster[mn].alwaysupdate==1))
			{
			//Monster's not dead or dying... so update it..(dead==0)
			switch(monster[mn].type)
				{
				case 0: if(demo==0)
								{
								monster[mn].alwaysupdate=1;//Once Sconics triggered it, theres no going back.
								update_fly(mn,sonicx,sonicy);
								}
								break;
				case 1: update_fish(mn,sos);break;//It should display the splash below, but I let update_fish() handle it, as it works out better.
				case 2: update_lavablob(mn);break;
				}
			//Display it.
			logx=monster[mn].xp-screenx;
			logy=monster[mn].yp-screeny;
			sos=sprite_on_screen(logx,logy);
			if(sos&&(monster[mn].visible==1))
				{
				switch(monster[mn].type)
					{
					case 0:        putm_my_sprite(logx+20,logy+20,100);break;
					case 1:        putm_my_sprite(logx+20,logy+20,101);break;
					case 2:        if(monster[mn].yv<4)
											{
											putm_my_sprite(logx+20,logy+20,83);
											if(monster[mn].yv<-3) add_smoke(monster[mn].xp,monster[mn].yp+16);
											}
										if(monster[mn].yv>=4) putm_my_sprite(logx+20,logy+20,84);
										break;
					}
				//Hit sonic? It's better checking monster to sonic than the other way
				//around...
				mx=monster[mn].xp; my=monster[mn].yp;
				difx=sonicx-mx; dify=sonicy-my;
				if((difx>-16)&&(difx<16)&&(dify>-16)&&(dify<16)&&(monster[mn].visible==1))
					{
					//Sonic has been hit!!!
					// Find out what happens now, Sonic Jumping or not?
					if((sonic_jumping==1)&&(monster[mn].killable==1))
						{
						//Yes, so kill monster...
						monster[mn].dead=1;
						monster[mn].visible=0;
						add_smoke(monster[mn].xp,monster[mn].yp);
						sonic_yv=-6;
						score+=50;
						}
						else
						{
						if(sonic_invincible==0)
							{
							//Owww!
							sonic_stuned=1;
							sonic_yv=-9;
							sonic_xv=(-1.5*sonic_xv);
							sonic_faling=1;
							sonic_jumping=0;
							sonic_invincible=100;
							if(sonic_protect==1)
								{
								sonic_protect=0;
								}
								else
								{
								if(rings!=0)
									{
									expload_ring_request(sonic_x,sonic_y,rings);
									rings=0;
									}
									else
									{
									outcome=DEAD;
									}//End else(rings!=0)
								}//End else(sonic_protect==1)
							}//End (sonic_invincible==0)
						}//End else sonic_jumping==1
					}//End overlap of dix, dify.
				}//End Collision with sonic
			}//End SOS && monsteralwaysupdate
		}//End monster dead
	}//End For(mn) Next Loop...
}//End method...

void update_protect(void)
{
static timer=3,flash=1;
timer--;
if(timer==0)
	{
	flash=1-flash;
	timer=3;
	if(flash==0) add_effect_request(0,1,sonic_x,sonic_y,89); else add_effect_request(0,1,sonic_x,sonic_y,90);
	}
}

void update_shield(void)
{
static timer=5,flash=1;
timer--;
flash=1-flash;
if(flash==1) add_effect_request(0,1,sonic_x,sonic_y,75);
if(timer==0)
	{
	timer=5;
	add_effect_request(0,4,sonic_x,sonic_y,76);//L.Blue wide circle
	add_effect_request(4,4,sonic_x,sonic_y,77);//M.Blue wide circle
	add_effect_request(8,4,sonic_x,sonic_y,78);//Dark.Blue wide circle
	}
}

void render_high_score(int x,int y)
{
int num_spr1,num_spr2,num_spr3,num_spr4,num_spr5,Score=high_score;
//Draw score counter...
putm_sprite(x,y+3,94,0);x+=19;
num_spr1=Score/10000; Score-=(num_spr1*10000);
putm_my_sprite(x,y,50+num_spr1); x+=11;
num_spr2=Score/1000; Score-=(num_spr2*1000);
putm_my_sprite(x,y,50+num_spr2); x+=11;
num_spr3=Score/100; Score-=(num_spr3*100);
putm_my_sprite(x,y,50+num_spr3); x+=11;
num_spr4=Score/10; Score-=(num_spr4*10);
putm_my_sprite(x,y,50+num_spr4); x+=11;
num_spr5=Score;
putm_my_sprite(x,y,50+num_spr5);
}

void render_player_info(void)
{
int num_spr1,num_spr2,num_spr3,num_spr4,num_spr5,rings_spr=rings,ringx=70,Score=score;
putm_sprite(20,6+view_port_y*16,25,0);//Ring logo...
num_spr1=rings_spr/100; rings_spr-=(num_spr1*100);
if(num_spr1!=0) {putm_my_sprite(ringx,6+view_port_y*16,50+num_spr1); ringx+=11;}
num_spr2=rings_spr/10; rings_spr-=(num_spr2*10);
if(num_spr2!=0) {putm_my_sprite(ringx,6+view_port_y*16,50+num_spr2); ringx+=11;}
num_spr3=rings_spr;
putm_my_sprite(ringx,6+view_port_y*16,50+num_spr3);
//Draw life counter...
putm_my_sprite(20,20,86);
num_spr1=sonic_lives<10?sonic_lives:9;
putm_my_sprite(33,28,50+num_spr1);
ringx=250-((18-view_port_x)*16);
num_spr1=Score/10000; Score-=(num_spr1*10000);
putm_my_sprite(ringx,23,50+num_spr1); ringx+=11;
num_spr2=Score/1000; Score-=(num_spr2*1000);
putm_my_sprite(ringx,23,50+num_spr2); ringx+=11;
num_spr3=Score/100; Score-=(num_spr3*100);
putm_my_sprite(ringx,23,50+num_spr3); ringx+=11;
num_spr4=Score/10; Score-=(num_spr4*10);
putm_my_sprite(ringx,23,50+num_spr4); ringx+=11;
num_spr5=Score;
putm_my_sprite(ringx,23,50+num_spr5);
if((view_port_x>9)&&(!_CREATE_DEMO)) render_high_score(250-((18-view_port_x)*16)-19,6+view_port_y*16);
}

void render_platforms(int screenx,int screeny)
{
int z,logx=0,logy=0,sos,pl;
for(z=0;z<30;z++)
	{
	if(platform[z].used==1)
		{
		for(pl=0;pl<platform[z].length;pl++)
			{
			logx=(platform[z].xp+platform[z].lx+(pl*16))-screenx;
			logy=(platform[z].yp+platform[z].ly)-screeny;
			sos=sprite_on_screen(logx,logy);
			if(sos)
				{
				//Display it.
				if(platform[z].type==0) putm_my_sprite(logx+20,logy+20,49); else putm_my_sprite(logx+20,logy+20,92);
				}
			}
		}
	}
}

void update_platforms(void)
{
int z;
for(z=0;z<30;z++)
	{
	if((platform[z].used==1)&&(platform[z].type==0))
		{
		if(platform[z].delay>0)
			{
			platform[z].delay--;
			}
		if(platform[z].delay==0)
			{
			//Do x right...
			if(platform[z].directionx==1)
				{
				platform[z].lx++;
				if(platform[z].lx==platform[z].mlx)
					{
					platform[z].directionx=-1;
					platform[z].lx+=1;
					}
				}
			//Do x left...
			if(platform[z].directionx==-1)
				{
				platform[z].lx--;
				if(platform[z].lx==0) platform[z].directionx=1;
				}
			//Do y down...
			if(platform[z].directiony==1)
				{
				platform[z].ly++;
				if(platform[z].ly==platform[z].mly) platform[z].directiony=-1;
				}
			//Do y up...
			if(platform[z].directiony==-1)
				{
				platform[z].ly--;
				if(platform[z].ly==0) platform[z].directiony=1;
				}
			}
		}
	if((platform[z].used==1)&&(platform[z].type==1))
		{
		if((platform[z].mlx==1)&&(platform[z].delay>0)) platform[z].delay--;
		if((platform[z].mlx==1)&&(platform[z].delay==0))
			{
			platform[z].yvel+=.4;
			platform[z].yp+=platform[z].yvel;
			if(platform[z].yp>2000) platform[z].mlx=0;
			}
		}
	}
}

int process_platform_collision(void)
{
int onplatform=-1,z,px,py,difx,dify;
for(z=0;z<30;z++)
	{
	if(platform[z].used==1)
		{
		px=platform[z].xp+platform[z].lx; py=platform[z].yp+platform[z].ly;
		difx=sonic_x-px; dify=sonic_y-py;
		//Sonic is collided with this platform, If this platform is higher than the one sconic is on, then make this the collided platform
		if((difx>-14)&&(difx<((16*platform[z].length)-16)+14)&&(dify>-17)&&(dify<0))
			{
			if(sonic_yv>0)// Anything Down...
				{
				if(onplatform==-1)
					{
					//This is the first collided platform, make topmost==0
					onplatform=z;
					}
					else
					{
					//This is another platform were on, so find which is higher...
					if(platform[z].yp+platform[z].ly<platform[onplatform].yp+platform[onplatform].ly)
						{
						onplatform=z;
						}//End Higher than other platform
					}//End else
				}//End Sonic is falling
			}//End Sonic collided
		}//End used
	}//End z loop
//Update initialise flags for the platform that is the highest, Sonic has hit platform...
if(onplatform!=-1)
	{
	/*A brief word about the initial state of a platforms delay attribute...
	For type 0's: (The moving platforms)
						If the delay is <-1, the platform just sits there forever(No variables updated).
						If the delay is =-1, the platform sits there until sconic stands on it.
						If the delay is =0 , the platform moves straight away.
						If the delay is >0 , the platform counts down, until it does =0, then it start moving.
	For type 1's: (The collapsable platforms)
						If the delay is <-1, the platform sits there.
						If the delay is =-1, the platform sits there until sconic JUMPS on it (NOT WALKS).
						If the delay is =0 , the platform drops straight away.
						If the delay is >0 , as soon as sconic triggers it, counts down to 0 (see up 1).
	*/
	if(platform[onplatform].type==0) if(platform[onplatform].delay==-1) platform[onplatform].delay=0;//Start a triggered platform...
	if(platform[onplatform].type==1)
		{
		platform[onplatform].mlx=1;//Trigger collapsable platform
		if(sonic_jumping==1)
			{
			if(platform[onplatform].delay==-1) {platform[onplatform].delay=0;}//If sconic jumps onto it, make it fall.
			}
		}
	}
return(onplatform);
}

void update_world(int spritex,int spritey,int screenx,int screeny,int sprite)
{
int logx,logy;
update_lava_display_matrix();
update_world_ring_matrix();
update_tv_crackle_display_matrix();
screenx-=Round(((view_port_x/2.0)*16.0))-8; screeny-=Round(((view_port_y/2.0)*16.0))-8;
if(screenx<0) screenx=0;
if(screenx>((world_size_x)-view_port_x*16)) screenx=((world_size_x)-view_port_x*16);
if(screeny<0) screeny=0;
if(screeny>((world_size_y)-view_port_y*16)) screeny=((world_size_y)-view_port_y*16);
render_sky(screenx,screeny);
render_background(screenx,screeny);
update_platforms();
render_platforms(screenx,screeny);
logx=spritex-screenx,logy=spritey-screeny;
if(sprite_on_screen(logx,logy)) putm_my_sprite(logx+20,logy+20,sprite);
process_effects(screenx,screeny);
render_foreground(screenx,screeny);
update_monsters(spritex,spritey,screenx,screeny);
process_rings(spritex,spritey,screenx,screeny);
process_jump_object();
if(demo==1)
	{
	if(sprite_on_screen(logx,logy)) putm_my_sprite(logx+20,logy+20,sprite);
	}
render_player_info();
}

void render_visual_display(void)
{
if(_DELAY) vsync_wait(_DELAY);
block_screen(0,20,20,16*view_port_x,16*view_port_y,255,160.0-Round(((view_port_x*16.0)/2.0)),100.0-Round(((view_port_y*16.0)/2.0)));
}

void trap_and_flush_system_keys(void)
{
char key;
if(_CREATE_DEMO)
	if(demorecord==0) outcome=QUIT;
while(kbhit()){
	key=toupper(getch());
	if(key==' ') outcome=QUIT;// Instant Closing...
	if((key=='O')&&(view_port_x>5)) {view_port_x--;draw_border();}
	if((key=='P')&&(view_port_x<VIEW_PORT_X)) {view_port_x++;draw_border();}
	if((key=='Q')&&(view_port_y<VIEW_PORT_Y)) {view_port_y++;draw_border();}
	if((key=='A')&&(view_port_y>3)) {view_port_y--;draw_border();}
	if(key=='D') {diag=1-diag;draw_border();}
	if(key=='H') {_DELAY++;if(_DELAY==3) _DELAY=0;}
	}
return;
}

void trap_control_keys(void)
{
int keyvalue=0;
if(sonic_stuned==1) return;
if(sonic_zoom>0) sonic_zoom--;
key_press=0;
if(_CREATE_DEMO)
	{
	int x,y;
	if((demorecord--)==100) putc(56,demofile);
	keyvalue=0;
	if(CursorUp) keyvalue=2;
	if(CursorLeft) keyvalue+=1;
	if(CursorRight) keyvalue+=4;
	putc(keyvalue+48,demofile);
	x=((maxdemorecord-demorecord)/maxdemorecord)*320.0;
	for(y=0;y<15;y++) *(screen+x+(y*320))=7;
	gotoxy(16,1);printf("RECORDING!");
	}
if((sonic_xv>-8)&&(CursorLeft))// Left
	{
	if(sonic_zoom>0)
		{
		sonic_xv-=.8;
		if(sonic_xv<-10) sonic_xv=-10;
		}
		else
		{
		sonic_xv-=.5;
		if(sonic_xv<-8) sonic_xv=-8;
		}
	if(!CursorRight) key_press=1;
	}
if((sonic_xv<8)&&(CursorRight))// Right
	{
	if(sonic_zoom>0)
		{
		sonic_xv+=.8;
		if(sonic_xv>10) sonic_xv=10;
		}
		else
		{
		sonic_xv+=.5;
		if(sonic_xv>8) sonic_xv=8;
		}
	if(!CursorLeft) key_press=2;
	}
if(CursorUp)// Jump_key:- If jump is held down from the start of one jump till landing, jump_key stops take-off again
	{           // letgo   :- If we let go of 'Jump' in a jump, locks out the jump key till we land.
	if(sonic_jumping==1)
		{
		if((sonic_yv<0)&(letgo==0)) sonic_yv-=.5;//If weve not let go and were going up, then decrease the decrease in upward velocity
		}
		else
		{
		if((sonic_faling==0)&&(jump_key==0))
			{
			if(sonic_zoom>0) sonic_yv=(-8)-(abs(sonic_xv)/3.0); else sonic_yv=(-7)-(abs(sonic_xv)/4.0);
			sonic_jumping=1;
			jump_key=1;//Were holding Jump, so set jump_key
			letgo=0;//Just pressed 'jump', so we havent let go yet!
			}
		}
	}
	else
	{
	if((sonic_jumping==0)&&(sonic_faling==0)) jump_key=0;//If were not in the air when we let go of 'Jump', reset jump_key
	letgo=1;
	}
}

//Don't ask why this function is almost a carbon copy of the one above. it's late, I'm tired, and I cant be arsed combining the two.
//I'll leave you to figure out my jump algorithem, I still can't and I WROTE it!
//That's what 1 curry pasty too many does to ya!...
void trap_demo_control_keys(void)
{
int acce=0,dir=0;
int keyp;
if(sonic_stuned==1) return;
if(sonic_zoom>0) sonic_zoom--;
key_press=0;
keyp=(int)getc(demofile);
if(((keyp&8)==8)&&(win_count==0)) {win_count=70;morphtoblack_pal(0,70);}
if((keyp&1)==1) dir=8;
if((keyp&4)==4) dir+=4;
if((keyp&2)==2) acce=1;
if((sonic_xv>-8)&&(dir&8)==8)// Left
	{
	if(sonic_zoom>0)
		{
		sonic_xv-=.8;
		if(sonic_xv<-10) sonic_xv=-10;
		}
		else
		{
		sonic_xv-=.5;
		if(sonic_xv<-8) sonic_xv=-8;
		}
	if(!CursorRight) key_press=1;
	}
if((sonic_xv<8)&&(dir&4)==4)// Right
	{
	if(sonic_zoom>0)
		{
		sonic_xv+=.8;
		if(sonic_xv>10) sonic_xv=10;
		}
		else
		{
		sonic_xv+=.5;
		if(sonic_xv>8) sonic_xv=8;
		}
	if(!CursorLeft) key_press=2;
	}
if((acce&1)==1)// Jump_key:- If jump is held down from the start of one jump till landing, jump_key stops take-off again
	{           // letgo   :- If we let go of 'Jump' in a jump, locks out the jump key till we land.
	if(sonic_jumping==1)
		{
		if((sonic_yv<0)&(letgo==0)) sonic_yv-=.5;//If weve not let go and were going up, then decrease the decrease in upward velocity
		}
		else
		{
		if((sonic_faling==0)&&(jump_key==0))
			{
			if(sonic_zoom>0) sonic_yv=(-8)-(abs(sonic_xv)/3.0); else sonic_yv=(-7)-(abs(sonic_xv)/4.0);
			sonic_jumping=1;
			jump_key=1;//Were holding Jump, so set jump_key
			letgo=0;//Just pressed 'jump', so we havent let go yet!
			}
		}
	}
	else
	{
	if((sonic_jumping==0)&&(sonic_faling==0)) jump_key=0;//If were not in the air when we let go of 'Jump', reset jump_key
	letgo=1;
	}
}

int process_collisions_and_sprite(void)
{
static int old_blink=0;
float old_sonic_y;
int sonic_sprite=15;
int point1,point2,point3,point4,point5,point6,point7,pointtopfront,pointbotfront,on_platform=0;
int balancing=0;
// Remember original positions...
old_sonic_y=sonic_y;
// Effects of gravity, air resistance...
sonic_yv+=1;
if(sonic_yv>8) sonic_yv=8;
if(sonic_xv<0) sonic_xv+=.25;
if(sonic_xv>0) sonic_xv-=.25;
if((sonic_xv>-.2)&&(sonic_xv<.2)) sonic_xv=0;
// MOVE SONIC LEFT/RIGHT!!!
sonic_x+=sonic_xv;

//check platform...
on_platform=process_platform_collision();

//move sonic on platform...
if(on_platform!=-1)
	{
	sonic_yv=platform[on_platform].yvel;
	//Move away from brick...
	sonic_y=(platform[on_platform].yp+platform[on_platform].ly)-16;
	sonic_x=sonic_x+platform[on_platform].directionx;
	sonic_y=sonic_y+platform[on_platform].directiony;
	sonic_jumping=0;
	sonic_faling=0;
	sonic_spring=0;
	sonic_stuned=0;
	}

// Map the high-res sonic position on to low-res map...
point1=map[int((sonic_x+1)/16.0)][int((sonic_y+2)/16)];
point2=map[int((sonic_x+14)/16.0)][int((sonic_y+2)/16)];
point3=map[int((sonic_x+14)/16.0)][int((sonic_y+15)/16)];
point4=map[int((sonic_x+1)/16.0)][int((sonic_y+15)/16)];
// Sonic is in which direction? (For sprites)
if(sonic_xv<0) sonic_facing=0;
if(sonic_xv>0) sonic_facing=1;
// Obstructions...
if((sonic_xv<0)||(on_platform!=-1))// Anything Left...
	{
	if((solid_left[point1]==1)||(solid_left[point4]==1))
		{
		//Only reset the velocity if player heading for the wall(stops getting stuck when a platform pushes you into a wall)
		if(sonic_xv<0) sonic_xv=0;
		sonic_x=((int)((sonic_x)/16)*16)+15;
		}
	}
if((sonic_xv>0)||(on_platform!=-1))// Anything Right...
	{
	if((solid_right[point2]==1)||(solid_right[point3]==1))
		{
		//Only reset the velocity if player heading for the wall(stops getting stuck when a platform pushes you into a wall)
		if(sonic_xv>0) sonic_xv=0;
		sonic_x=((int)((sonic_x+14)/16.0)*16)-14.25;
		}
	}
//MOVE SONIC UP/DOWN
sonic_y+=sonic_yv;

point1=map[int((sonic_x+1)/16.0)][int((sonic_y+2)/16)];//Tl
point2=map[int((sonic_x+14)/16.0)][int((sonic_y+2)/16)];//Tr
point3=map[int((sonic_x+14)/16.0)][int((sonic_y+15)/16)];//Bl
point4=map[int((sonic_x+1)/16.0)][int((sonic_y+15)/16)];//Br


// Work out balancing...
point5=map[int((sonic_x+7)/16.0)][int((sonic_y+15)/16)];
point6=map[int((sonic_x+8)/16.0)][int((sonic_y+15)/16)];

//Collisions
point7=map[int((sonic_x+8)/16.0)][int((sonic_y+7)/16)];

if(on_platform!=-1)
	{
	int px,difx;
	px=platform[on_platform].xp+platform[on_platform].lx;
	difx=sonic_x-px;
	if(difx>8+((platform[on_platform].length-1)*16)+platform[on_platform].directionx) {point3=0;point4=1;point5=0;point6=0;}//right balance
	if(difx<-7+platform[on_platform].directionx) {point3=1;point4=0;point5=0;point6=0;}//left balance
	}

//Splashes
pointtopfront=mapfront[int((sonic_x+7)/16.0)][int((sonic_y+6)/16)];
pointbotfront=mapfront[int((sonic_x+7)/16.0)][int((sonic_y+14)/16)];

if(sonic_xv==0)
	{
	if((solid_down[point3]==1)&&(solid_down[point6]==0)) {sonic_facing=0; balancing=1;}
	if((solid_down[point4]==1)&&(solid_down[point5]==0)) {sonic_facing=1; balancing=1;}
	// If sonic is still
	if((balancing==0)&&sonic_faling==0&&on_platform==-1)
		{
		if(delay_count==0)
			{
			if(rand()%50==1)
				{
				int t=rand()%10;
				if(t<=6) sonic_blinking=5;
				if((t>6)&&(t<9))
					{
					sonic_yv=(-5);
					sonic_jumping=1;
					jump_key=1;
					}
				if(t==9) sonic_facing=1-sonic_facing;
				}
			}
			else
			{
			delay_count--;
			}
		}
	}
	else
	{
	delay_count=400;
	}
if((sonic_yv<0)||(on_platform!=-1))// Anything Up...
	{
	if((solid_up[point1]==1)||(solid_up[point2]==1))
		{
		sonic_yv=0;
		sonic_y=old_sonic_y;
		}
	}
if((sonic_yv>0)||(on_platform!=-1))// Anything Down...
	{
	if((solid_down[map[int((sonic_x+1)/16.0)][int((sonic_y+15)/16)]]==1)||(solid_down[map[int((sonic_x+14)/16.0)][int((sonic_y+15)/16)]]==1))
		{
		//Integerize XV to the nearest whole number so that it can=0;
		sonic_yv=0;
		//Move away from brick...
		sonic_y=int(sonic_y/16.0)*16;
		sonic_jumping=0;
		sonic_faling=0;
		sonic_spring=0;
		sonic_stuned=0;
		}
		else
		{
		//Just falling
		if(on_platform==-1)
			{
			if(sonic_jumping==0)
				{
				sonic_faling=1;
				jump_key=1;
				}
			}
		}
	}


//Collisions...
//A splash in the water...
if((pointbotfront==7)&&(pointtopfront!=7)&&(sonic_yv!=0))
	{
	add_splash(sonic_x,((int((sonic_y)/16))*16));
	}
//Jumpers...
if((point5!=0)&&(point5!=27))
	{                          
	//Invincible box
	if((point5==40) && (((sonic_jumping==1)&&(sonic_yv>0)) || (sonic_spring==1)))
		{
		if(sonic_spring==0) sonic_yv=-7;
		sonic_invincible+=400;
		map[int((sonic_x+7)/16.0)][int((sonic_y+15)/16)]=87;
		add_smoke((int((sonic_x+7)/16.0)*16),(int((sonic_y+15)/16))*16);
		sonic_shield=1;
		score+=4;
		letgo=1;//Stops bouncing real high by holding jump...
		}
	//10 Rings box
	if((point5==41) && (((sonic_jumping==1)&&(sonic_yv>0)) ||  (sonic_spring==1)))
		{
		if(sonic_spring==0) sonic_yv=-7;
		add_rings(10);
		map[int((sonic_x+7)/16.0)][int((sonic_y+15)/16)]=87;
		add_smoke((int((sonic_x+7)/16.0)*16),(int((sonic_y+15)/16))*16);
		score+=4;
		letgo=1;//Stops bouncing real high by holding jump...
		}
	//Zoooooooooommmmmmmmm!!!
	if((point5==91) && (((sonic_jumping==1)&&(sonic_yv>0)) ||  (sonic_spring==1)))
		{
		if(sonic_spring==0) sonic_yv=-7;
		sonic_zoom+=1000;
		map[int((sonic_x+7)/16.0)][int((sonic_y+15)/16)]=87;
		add_smoke((int((sonic_x+7)/16.0)*16),(int((sonic_y+15)/16))*16);
		score+=4;
		letgo=1;//Stops bouncing real high by holding jump...
		}
	//Sheild box
	if((point5==88) && (((sonic_jumping==1)&&(sonic_yv>0)) ||  (sonic_spring==1)))
		{
		if(sonic_spring==0) sonic_yv=-7;
		sonic_protect=1;
		map[int((sonic_x+7)/16.0)][int((sonic_y+15)/16)]=87;
		add_smoke((int((sonic_x+7)/16.0)*16),(int((sonic_y+15)/16))*16);
		score+=4;
		letgo=1;//Stops bouncing real high by holding jump...
		}
	//Spikes
	if(((point7==5)||(point7==42)||(point7==47)||(point7==48))&&(sonic_invincible==0))
		{
		sonic_stuned=1;
		sonic_yv=-9;
		sonic_xv=(-1.5*sonic_xv);
		sonic_faling=1;
		sonic_jumping=0;
		sonic_invincible=100;
		if(sonic_protect==1)
			{
			sonic_protect=0;
			}
			else
			{
			if(rings!=0)
				{expload_ring_request(sonic_x,sonic_y,rings);rings=0;}else{outcome=DEAD;}
			}
		}
	//Lava...
	if((point7==62)||(point7==65)) {outcome=DEAD;sonic_shield=0;sonic_protect=0;}
	//Jumps...
	if(point7==43)
		{
		sonic_faling=0;
		sonic_yv=-12;
		sonic_xv=12;
		sonic_jumping=1;
		jump_key=1;
		sonic_stuned=0;
		sonic_spring=0;
		create_jump_object(int((sonic_x+8)/16.0),int((sonic_y+7)/16),45);
		}
	if(point7==44)
		{
		sonic_faling=0;
		sonic_yv=-12;
		sonic_xv=-12;
		sonic_jumping=1;
		jump_key=1;
		sonic_stuned=0;
		sonic_spring=0;
		create_jump_object(int((sonic_x+8)/16.0),int((sonic_y+7)/16),46);
		}
	if(point5==11)
		{
		if(sonic_yv>0)
			{
			sonic_faling=1;
			sonic_yv=-16;
			sonic_jumping=0;
			jump_key=1;
			sonic_stuned=0;
			sonic_spring=1;
			create_jump_object(int((sonic_x+7)/16.0),int((sonic_y+15)/16),85);
			}
		}
	//Ring...
	if(map[((int(sonic_x)+4)/16)][((int(sonic_y)+4)/16)]==12)
		{
		map[((int(sonic_x)+4)/16)][((int(sonic_y)+4)/16)]=0;
		add_rings(1);
		add_sparkle((((int(sonic_x)+4)/16)*16),((int(sonic_y)+4)/16)*16);
		score+=25;
		}
	if(map[((int(sonic_x)+11)/16)][((int(sonic_y)+4)/16)]==12)
		{
		map[((int(sonic_x)+11)/16)][((int(sonic_y)+4)/16)]=0;
		add_rings(1);
		add_sparkle((((int(sonic_x)+11)/16)*16),((int(sonic_y)+4)/16)*16);
		score+=25;
		}
	if(map[((int(sonic_x)+11)/16)][((int(sonic_y)+12)/16)]==12)
		{
		map[((int(sonic_x)+11)/16)][((int(sonic_y)+12)/16)]=0;
		add_rings(1);
		add_sparkle((((int(sonic_x)+11)/16)*16),((int(sonic_y)+12)/16)*16);
		score+=25;
		}
	if(map[((int(sonic_x)+4)/16)][((int(sonic_y)+12)/16)]==12)
		{

		map[((int(sonic_x)+4)/16)][((int(sonic_y)+12)/16)]=0;
		add_rings(1);
		add_sparkle((((int(sonic_x)+4)/16)*16),((int(sonic_y)+12)/16)*16);
		score+=25;
		}
	//Win flag...
	if((point7==79)&&(win_count==0))
		{
		win_count=70;
		morphtoblack_pal(0,70);
		add_winsparkle(sonic_x,sonic_y);
		score+=100;
		}
	}

//Work out Sprite...
if(sonic_facing==0)
	{
	if((sonic_xv<0)&&(sonic_faling==0))
		{
		if((key_press==2)&&(sonic_xv<0))
			{
			sonic_sprite=23;
			if((sonic_xv<-5)&&(rand()%4==1)&&(sonic_jumping==0))add_smoke(sonic_x-8,sonic_y+8);
			}
			else
			{
			if(sonic_xv<-5)
				{
				sonic_sprite=cycle_animation_vector(RUN_LEFT_ANIMATION);
				}
				else
				{
				animation[WALK_LEFT_ANIMATION].maxscreencount=4.0+sonic_xv;
				sonic_sprite=cycle_animation_vector(WALK_LEFT_ANIMATION);
				}
			}
		}
		else
		{
		if(balancing==0)//||(sonic_xv!=0))
			{
			if(sonic_blinking>0)
				{
				sonic_blinking--;
				sonic_sprite=35;
				}
				else
				{
				sonic_sprite=14;
				}
			}
			else
			{
			sonic_sprite=cycle_animation_vector(BALANCE_LEFT_ANIMATION);
			}
		}
	if(sonic_jumping==1) sonic_sprite=cycle_animation_vector(JUMP_LEFT_ANIMATION);
	//Zoom up...
	if((sonic_faling==1)&&(sonic_yv<0)) sonic_sprite=21;
	}
if(sonic_facing==1)
	{
	if((sonic_xv>0)&&(sonic_faling==0))
		{
		if((key_press==1)&&(sonic_xv>0))
			{
			sonic_sprite=24;
			if((sonic_xv>5)&&(rand()%4==1)&&(sonic_jumping==0))add_smoke(sonic_x+8,sonic_y+8);
			}
			else
			{
			if(sonic_xv>5)
				{
				sonic_sprite=cycle_animation_vector(RUN_RIGHT_ANIMATION);
				}
				else
				{
				animation[WALK_RIGHT_ANIMATION].maxscreencount=4.0-sonic_xv;
				sonic_sprite=cycle_animation_vector(WALK_RIGHT_ANIMATION);
				}
			}
		}
		else
		{
		if(balancing==0)
			{
			if(sonic_blinking>0)
				{
				sonic_blinking--;
				sonic_sprite=36;
				}
				else
				{
				sonic_sprite=15;
				}
			}
			else
			{
			sonic_sprite=cycle_animation_vector(BALANCE_RIGHT_ANIMATION);
			}
		}
	if(sonic_jumping==1) sonic_sprite=cycle_animation_vector(JUMP_RIGHT_ANIMATION);
	//Zoom up...
	if((sonic_faling==1)&&(sonic_yv<0)) sonic_sprite=22;
	}
if(sonic_blinking==old_blink) sonic_blinking=0;
if(sonic_invincible!=0)
	{
	sonic_invincible--;
	sonic_sprite_on=1-sonic_sprite_on;
	if(sonic_shield==1) sonic_sprite_on=1;
	if(sonic_invincible==0) {sonic_sprite_on=1; sonic_shield=0;}
	if(sonic_shield==1) update_shield();
	}
if(sonic_protect==1) update_protect();
if(sonic_zoom>0) display_zoom(sonic_sprite);
sonic_sprite=(sonic_sprite*sonic_sprite_on);
if(sonic_stuned==1)
	{
	if(sonic_facing==0) sonic_sprite=37; else sonic_sprite=38;
	}
if(win_count>1) {win_count--;morph();}
if(win_count==1) outcome=WON;
return(sonic_sprite);
}

void initialise_sonic(int x,int y,int facing,int lives,int protect)
{
// Sonic_x & y are the TOP LEFT of the sonic sprite...
sonic_x=x;sonic_y=y;sonic_xv=0;sonic_yv=0;
sonic_facing=facing;sonic_jumping=0;sonic_faling=1;sonic_blinking=0;
sonic_sprite_on=1;outcome=UNKNOWN;sonic_stuned=0;sonic_invincible=0;
jump_key=0;rings=0;key_press=0;delay_count=400;sonic_spring=0;
sonic_shield=0;sonic_lives=lives,win_count=0,no_rings=0;
sonic_protect=protect,sonic_zoom=0;
}

void dying(float posx,float posy)
{
int deadcount;
demo=1; sonic_yv=-3;sonic_xv=0;
for(deadcount=0;deadcount<150;deadcount++)
	{
	update_world(sonic_x,sonic_y,posx,posy,39);
	cycle_background_pallet();
	sonic_y+=sonic_yv;
	sonic_yv+=.125;
	render_visual_display();
	}
demo=0;
}

void debug_display_stat_bars(void)
{
int x;
float effect_count;
//Scan for effects used...
effect_count=0;
for(x=0;x<MAX_EFFECTS;x++) if(effect[x].used!=0) effect_count++;
effect_count=effect_count/MAX_EFFECTS*320;
for(x=0;x<effect_count;x++) *(screen+320+x)=8;   //Draw effects allocated
for(x=effect_count;x<320;x++) *(screen+320+x)=0; //Draw end of effects allocated
for(x=0;x<320;x+=10) *(screen+x+640)=6;          // Draw Scale
for(x=0;x<320;x+=50) *(screen+x+960)=6;          // Draw Scale
}

void play_life(void)
{
int startup=70,sonic_sprite;
float posx=sonic_x,posy=sonic_y;
clear_screen(0,255);
morphcol_pal(2,0,71);morph();
draw_border();
do
	{
	if(startup>0) {startup--;morph();} else {trap_control_keys();cycle_background_pallet();}
	sonic_sprite=process_collisions_and_sprite();
	posx+=(sonic_x-posx)/10; posy+=(sonic_y-posy)/10;
	trap_and_flush_system_keys();
	update_world(sonic_x,sonic_y,posx,posy,sonic_sprite);
	render_visual_display();
	if(diag==1) debug_display_stat_bars();
	if(score>high_score) high_score=score;
	}while(outcome==UNKNOWN);
if(outcome==DEAD) dying(posx,posy);
if(_CREATE_DEMO) outcome=QUIT;//Always quit program if we are making a demo!
}

float rv(int s)
{
float r=rand()%1000;
r=r/500;
r=r+0.5;
if(s==0) if((rand()%10)<5) r=-r;
if(s==-1) r=-r;
return(r);
}

int run_demo(void)
{
int end_demo=0,vpx=view_port_x,vpy=view_port_y;
int demotime=700;
float px=rand()%400+50,py=rand()%400+50,pxv=rv(0),pyv=rv(0),screenx,screeny;
demo=1;
load_level(0);
view_port_x=VIEW_PORT_X,view_port_y=VIEW_PORT_Y;//Draw the boder right size.
draw_border();
do
	{
	trap_and_flush_system_keys();
	view_port_x=VIEW_PORT_X,view_port_y=VIEW_PORT_Y;//Stop system keys changing values.
	update_lava_display_matrix();
	update_world_ring_matrix();
	update_tv_crackle_display_matrix();
   cycle_background_pallet();
	screenx=(int)(px+0.5);screeny=(int)(py+0.5);
	screenx-=Round(((view_port_x/2.0)*16.0))-8; screeny-=Round(((view_port_y/2.0)*16.0))-8;
	if(screenx<0) screenx=0;
	if(screenx>((world_size_x)-view_port_x*16)) screenx=((world_size_x)-view_port_x*16);
	if(screeny<0) screeny=0;
	if(screeny>((world_size_y)-view_port_y*16)) screeny=((world_size_y)-view_port_y*16);
	render_sky(screenx,screeny);
	render_background(screenx,screeny);
	render_foreground(screenx,screeny);
	update_monsters(px,py,screenx,screeny);
	process_effects(screenx,screeny);
	render_platforms(screenx,screeny);
	putm_my_sprite(20,22,cycle_animation_vector(WALK_RIGHT_ANIMATION));
	putm_my_sprite(292,22,cycle_animation_vector(WALK_LEFT_ANIMATION));
	putm_my_sprite(20,161,cycle_animation_vector(RUN_RIGHT_ANIMATION));
	putm_my_sprite(292,161,cycle_animation_vector(RUN_LEFT_ANIMATION));
	putm_sprite(90,30,180,0);
	putm_sprite(145,60,cycle_animation_vector(SCONIC_DEMO_ANIMATION),0);
	render_high_score(130,147);
	render_visual_display();
	px+=pxv;py+=pyv;
	if(px<136) {px=136; pxv=rv(1);}
	if(px>488) {px=488; pxv=rv(-1);}
	if(py<72)  {py=72 ; pyv=rv(1);}
	if(py>592) {py=592; pyv=rv(-1);}
	if(CursorUp) end_demo=1;
	if(kbhit())
		{
		char k=getch();
		if(k==' ') outcome=QUIT; else end_demo=1;
		}
	}while((end_demo==0)&&(outcome!=QUIT)&&(--demotime!=0));
demo=0;
view_port_y=vpy;view_port_x=vpx;
if((end_demo==1)||(outcome==QUIT)) return 0;else return 1;
}

void sega_intro(void)
{
int x;
float scale_increment=1,scale_ratio=0,width_in_pixels=94;
delay(1000);
for(x=41;x<50;x++) changecol_pal(4,x,0,0,99);changecol_pal(4,7,0,0,99);
for(x=41;x<50;x++) changecol_pal(5,x,255,255,255);changecol_pal(5,7,255,255,255);
init_animation();
clear_screen(40,0);
use_pal(4);
putm_sprite(112,84,95,0);
move_screen(0,255);
delay(2000);
morphcol_pal(4,5,15);for(x=0;x<15;x++) {morph();vsync_wait(1);}
morphcol_pal(5,0,20);for(x=0;x<20;x++) {morph();vsync_wait(1);}
delay(1000);
for(x=-20;x<320;x+=3)
	{
	block_screen(0,0,0,320,32,0,0,84);
	putm_sprite(112,84,95,0);
	putcm_sprite(0,0,319,199,x,100,cycle_animation_vector(RUN_RIGHT_ANIMATION),0);
	delay(6);
	block_screen(0,0,84,319,35,255,0,84);
	}
delay(3000);
morphcol_pal(0,4,80);for(x=0;x<99;x++)
	{
	morph();
	scale_ratio=width_in_pixels/94;
	putcms_sprite(0,0,319,199,159-(47*scale_ratio),99-(15*scale_ratio),scale_ratio,scale_ratio,95,255);
	width_in_pixels+=scale_increment;
	scale_increment+=.02;
	vsync_wait(1);
	}
clear_screen(0,255);
delay(200);
}

int rolling_demo(int rollevel)
{
char dnl[7]="";
int startup=70,sonic_sprite;
float posx,posy;
short int up=0;
int temp_high_score=high_score;
dnl[0]=(char)(rollevel+48);
strcat(dnl,".Dem");
if((demofile=fopen(dnl,"r"))==NULL) return 1;
sonic_lives=3;
sonic_level=1;
load_level(rollevel);
initialise_sonic(sonic_x,sonic_y,sonic_facing,sonic_lives,sonic_protect);//Just echo global variables to function.
posx=sonic_x;posy=sonic_y;
clear_screen(0,255);
morphcol_pal(2,0,71);morph();
draw_border();
do
	{
	if(startup>0) {startup--;morph();} else {trap_demo_control_keys();cycle_background_pallet();}
	sonic_sprite=process_collisions_and_sprite();
	posx+=(sonic_x-posx)/10; posy+=(sonic_y-posy)/10;
	update_world(sonic_x,sonic_y,posx,posy,sonic_sprite);
	render_visual_display();
	if(diag==1) debug_display_stat_bars();
	up=CursorUp;//CursorUp liable to change...
	}while((outcome==UNKNOWN)&&(!kbhit())&&(!up));
fclose(demofile);
if(outcome==DEAD) dying(posx,posy);
initialise_sonic(1,1,1,3,0);
score=0;
high_score=temp_high_score;
if(kbhit())
	{
	if(getch()==' ') outcome=QUIT;
	return 0;
	}
	else
	{
	if(up) return 0; else return 1;
	}
}

int run_rolling_demo()
{
static int level=0;
if(++level==5) level=1;
return(rolling_demo(level));
}

void game(void)
{
int nkey;
clear_screen(0,255);
make_background();
randomize();//The loading algorithem uses srand(), so we have to reset it.
if((_CREATE_DEMO==0)&&(!DEBUG)) sega_intro();
do
	{
	init_animation();
	if((_CREATE_DEMO==0)&&(!DEBUG))
		{
		do
			{
			nkey=run_demo();//Returns 0 if key or up pressed
			if(nkey==1) nkey=run_rolling_demo(); //Run only if no key pressed, returns 0 if key pressed
			init_animation();
			}while((outcome!=QUIT)&&(nkey==1));
		}
	if(outcome!=QUIT)
		{
		sonic_lives=3;
		sonic_level=1;
		do
			{
			load_level(sonic_level);
			initialise_sonic(sonic_x,sonic_y,sonic_facing,sonic_lives,sonic_protect);//Just echo global variables to function, v.bad practice, but fun!
			play_life();
			if(outcome==DEAD) sonic_lives--;
			if(outcome==WON)
				{
				//Do inter-level display here!!!
				sonic_level++;
				};
			}while((sonic_lives>0)&&(outcome!=QUIT)&&(sonic_level<5));
		}
	if(outcome!=QUIT)
		{
		if(score>high_score) high_score=score;
		score=0;
		if(sonic_level==5)
			{
			//Here we will write game finish sequence
			clear_screen(0,255);
			use_pal(3);
			pour_screen(3);
			getch();
			clear_screen(0,255);
			use_pal(0);
			}
		if(sonic_lives==0)
			{
			int c=0;
			//This is the game; over...
			//
			//EI
			//POP AF
			//POP BC
			//POP DE
			//RET
			clear_screen(0,0);
			putm_sprite(30,83,93,0);
			slide_screen(rand()%5,0);
			delay(1000);
			morphtoblack_pal(0,100); for(c=0;c<100;c++) {morph();delay(25);}
			clear_screen(0,255);
			}
		initialise_sonic(1,1,1,3,0);
		}
	}while(outcome!=QUIT);
//End unbracketed If.
}

void demo_query_start()
{
int dn=0;
char dnl[7]="";
if(kbhit())
	{
	if(getch()=='d')
		{
		printf("\n\nEnter level number to create demo for:");
		do{dn=getch();}while((dn<48)||(dn>52));
		_CREATE_DEMO=dn-48;
		}
	}
if(_CREATE_DEMO)
	{
	dnl[0]=(char)(_CREATE_DEMO+48);
	if((demofile=fopen(strcat(dnl,".Dem"),"w"))==NULL)
		{printf("Cannot open Demo File!");}
	}
}

void demo_query_end()
{
if(_CREATE_DEMO)
	fclose(demofile);
}

void main(void)
{
printf("Sconic Build 1.6.1");
randomize();

ini_screen(4,REPORT);
ini_sprite(186,REPORT);
make_sprites();
ini_pal(REPORT);
make_palette();
ini_sconic_keyint(REPORT);
ini_screamer(REPORT&DEBUG);

demo_query_start();

game();

demo_query_end();

end_screamer();
end_sconic_keyint();
end_pal();
end_screen();
end_sprite();
exit(1);
/*

"And always better in the ending than the beginning,
 the interval between being what it was."

*/
}

