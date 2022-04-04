// This is compatible with graphics driver Screamer(Three) ONLY!

#include "C:\myc__p~1\Library2\Screamer.H"

int mx,my,mb,omb;//The mouse info!

char name[26];
float sonic_x=17,sonic_y=17;
int oldsnap=-1,sonic_facing=1,flash=0,draw_mode=1,draw_position=1,base_object=0,object=1,bg_on=1,fg_on=1,snap=0;

//In pixels...
#define WORLD_SIZE_X 1280
#define WORLD_SIZE_Y 1280

//Define as blocks of 16x16 pixels...
#define VIEW_PORT_X 18
#define VIEW_PORT_Y 10

#define MAX_MONSTERS 300

#define MAX_OBJECTS 43
#define FIRST_MONSTER 37
#define LAST_MONSTER 39

int object_vector[MAX_OBJECTS]=                                                                           /*37..39*/    /*40..41*/
	{                                                                                                      /*MON*/       /*SCONIC*/
	0,1,2,3,4,5,48,47,42,6,7,8,9,10,43,11,44,12,26,27,28,29,30,31,32,33,34,40,41,88,91,60,61,62,65,67,79,  83,100,101,   14,15, 49
	};

int map[(WORLD_SIZE_X/16)+2][(WORLD_SIZE_Y/16)+2];
int mapfront[(WORLD_SIZE_X/16)+2][(WORLD_SIZE_Y/16)+2];
//int world_size_x=WORLD_SIZE_X,world_size_y=WORLD_SIZE_Y;
int world_size_x=800,world_size_y=640,platform_type=0;
int view_port_x=VIEW_PORT_X,view_port_y=VIEW_PORT_Y;

int mapx=0,mapy=0;
struct amonster
				{
				int type; float xp; float yp; int dead; float baseyp; float yv;
				int jumpdelay;int visible;int killable;int alwaysupdate;
				};
struct aplatform{
	int used;
	int xp;
	int yp;
	int mlx;
	int mly;
	int lx;
	int ly;
	int directionx;
	int directiony;
	int delay;
	int length;
	int type;
	};

//Make them...
struct amonster monster[MAX_MONSTERS];
struct aplatform platform[30];

void make_sprites(void)
{
uncompress_screen("EDITOR_A.ASS",0,50,255);
printf("\nStoring Sprites in memory...");
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

add_sprite(96,160,16,16,42,0);//Spikes Down...
add_sprite(112,160,16,16,43,0);//Jump Right...
add_sprite(128,160,16,16,44,0);//Jump Left...
add_sprite(0  , 80,16,16,47,0);//Spikes Right
add_sprite(16 , 80,16,16,48,0);//Spikes Left
add_sprite(32 , 80,16,16,49,0);//Moving Platform
add_sprite(48 , 80,16,16,88,0);//Box; Sheild
add_sprite(96 ,80,16,16,91,0);//Speed Box!!!
add_sprite(112 ,80,16,16,92,0);//Collapsable platform

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

add_sprite(0 ,112,16,16,153,0);//Blanker!!
add_sprite(16,112,16,16,154,0);//Background on
add_sprite(32,112,16,16,155,0);//Foreground on
add_sprite(48,112,16,16,156,0);//Background off
add_sprite(64,112,16,16,157,0);//Foreground off
add_sprite(80,112,16,16,158,0);//Foreground signal

add_sprite(0 ,128,16,16,159,0);//Background Signal
add_sprite(16,128,16,16,160,0);//Monster Signal
add_sprite(32,128,16,16,161,0);//Clear screen icon
add_sprite(48,128,16,16,162,0);//Set Border Signal
add_sprite(64,128,16,16,163,0);//Platform signal  
add_sprite(80,128,16,16,164,0);//Sonic walking left 6

add_sprite(0 ,160,16,16,166,0);//Sonic balance right 1

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
add_sprite(48 ,144,16 ,16,182,0);//Pointer!
add_sprite(0  ,144,48 ,16,183,0);//Current

printf("O.K!");
}

void get_mouse(void)
{
mx=xcoord_mouse()>>1;
my=ycoord_mouse();
omb=mb;
mb=lbutton_mouse()*2;
mb+=rbutton_mouse();
}

void make_palette(void)
{
int z,x;
load_pal("EDITOR_A.ASS");
for(z=0;z<100;z++)
	{
	changecol_pal(0,z+50,220,60,(z*2.55));
	for(x=0;x<160;x++)
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
	changecol_pal(0,z+50,220,60,((200-z)*2.55));
	for(x=0;x<160;x++)
		{
		ppixel(x,z,((200-z)*(1.8-(x/160.0)))+50,buffers+64000l);
		}
	for(x=160;x<320;x++)
		{
		ppixel(x,z,((200-z)*(1.8-((320.0-x))/160.0))+50,buffers+64000l);
		}
	}
changecol_pal(0,251,245,180,180);
for(z=0;z<100;z++) ppixel((rand()%320),(rand()%200),251,buffers+64000l);
}

void init_level(void)
{
int z,y;
//initialise game info.
sonic_x=17;sonic_y=17;sonic_facing=1;
world_size_x=1280;world_size_y=1280;
for(y=0;y<80;y++){for(z=0;z<80;z++){map[z][y]=0;mapfront[z][y]=0;}}
for(z=0;z<MAX_MONSTERS;z++)
	{
	monster[z].xp=0; monster[z].yp=0;
	monster[z].dead=1; monster[z].jumpdelay=0;
	monster[z].baseyp=0; monster[z].type=0;
	monster[z].visible=0;monster[z].killable=0;
	monster[z].alwaysupdate=0;
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
	platform[z].length=0;
   platform[z].type=0;
	}
for(z=0;z<25;z++) name[z]=32;
for(y=0;y<WORLD_SIZE_Y/16;y++)
	{
	map[0][y]=1;
	map[WORLD_SIZE_X/16-1][y]=1;
	}
for(z=0;z<WORLD_SIZE_X/16;z++)
	{
	map[z][0]=1;
	map[z][WORLD_SIZE_Y/16-1]=1;
	}
}

void loading_bar(int bar)
{
int co,z;
for(z=0;z<bar;z++)
	{
	co=((z/5+bar)%70)+50;
	ppixel(z,197,co,screen);
	ppixel(z,198,co,screen);
	ppixel(z,199,co,screen);
	}
}

void load_level(void)
{
FILE *in;
char level[15];
int y,z,count=0;
gotoxy(5,2);printf("Input Level!");
gotoxy(5,3);scanf("%12s", level);
fflush(stdin);
if ((in=fopen(level, "rt"))==NULL) {fprintf(stderr, "cant open\n");getch();return;}
fread(name,25,1,in);
name[25]=0;
fscanf(in,"%f %f %d ",&sonic_x,&sonic_y,&sonic_facing);
fscanf(in,"%d %d ",&world_size_x,&world_size_y);
for(y=0;y<80;y++)
	{
	for(z=0;z<80;z++)
		{
		fscanf(in,"%d %d ",&map[z][y],&mapfront[z][y]);
		loading_bar((count++)/20);
		}
	}
for(z=0;z<MAX_MONSTERS;z++)
	{
	fscanf(in,"%f %f %d %d %f %d %d %d %d ",&monster[z].xp,&monster[z].yp,&monster[z].dead,&monster[z].jumpdelay,&monster[z].baseyp,&monster[z].type,&monster[z].visible,&monster[z].killable,&monster[z].alwaysupdate);
	}
for(z=0;z<30;z++)
	{
	fscanf(in,"%d %d %d %d %d %d %d %d %d %d %d %d ",&platform[z].used,&platform[z].xp,&platform[z].yp,&platform[z].mlx,&platform[z].mly,&platform[z].lx,&platform[z].ly,&platform[z].directionx,&platform[z].directiony,&platform[z].delay,&platform[z].length,&platform[z].type);
	}
fclose(in);
gotoxy(5,6);printf("Loaded \"%s\"!",name);
gotoxy(5,8);printf("Press any key!");getch();
}

void save_level(void)
{
FILE *out;
int y,z,count=0,pos=0;
char key,level[15];
gotoxy(2,3);
printf("Saving!");
gotoxy(2,4);
printf("Please enter level FILENAME");
gotoxy(2,5);
scanf("%s",&level);
fflush(stdin);
if ((out=fopen(level, "wt"))==NULL) {fprintf(stderr, "cant open\n");return;}
//Store x,y and facing
gotoxy(2,6);
printf("%s",name);
gotoxy(2,7);
printf("Please enter level discription(25chrs)");
gotoxy(2,8);
printf("<");
do{
	key=getch();
	if((key==8)&&(pos>0)) {name[--pos]=0;gotoxy(pos+2,8);printf("< ");}
	if((pos<24)&&(key!=8)&&(key!=13)) {name[pos++]=key;gotoxy(pos+1,8);printf("%c<",key);}
}while(key!=13);
if(pos>0)
	{
	for(z=pos;z<25;z++) name[z]=32;
	}
fwrite(name,25,1,out);
fprintf(out,"%f %f %d ",sonic_x,sonic_y,sonic_facing);
fprintf(out,"%d %d ",world_size_x,world_size_y);
for(y=0;y<80;y++)
	{
	for(z=0;z<80;z++)
		{
		fprintf(out,"%d %d ",map[z][y],mapfront[z][y]);
		loading_bar((count++)/20);
		}
	}
for(z=0;z<MAX_MONSTERS;z++)
	{
	fprintf(out,"%f %f %d %d %f %d %d %d %d ",monster[z].xp,monster[z].yp,monster[z].dead,monster[z].jumpdelay,monster[z].baseyp,monster[z].type,monster[z].visible,monster[z].killable,monster[z].alwaysupdate);
	}
for(z=0;z<30;z++)
	{
	fprintf(out,"%d %d %d %d %d %d %d %d %d %d %d %d ",platform[z].used,platform[z].xp,platform[z].yp,platform[z].mlx,platform[z].mly,platform[z].lx,platform[z].ly,platform[z].directionx,platform[z].directiony,platform[z].delay,platform[z].length,platform[z].type);
	}
fclose(out);
}

void display_map(int screenx,int screeny)
{
int bx,by,brick,ex=world_size_x/16,ey=world_size_y/16;
if(bg_on)
	{
	for(by=0;by<10;by++)
		{
		for(bx=0;bx<20;bx++)
			{
			brick=map[screenx+bx][screeny+by];
			putcm_sprite(0,0,319,199,bx*16,by*16,brick,0);
			}
		}
	}
if(fg_on)
	{
	for(by=0;by<10;by++)
		{
		for(bx=0;bx<20;bx++)
			{
			brick=mapfront[screenx+bx][screeny+by];
			if(brick!=0) putcm_sprite(0,0,319,199,bx*16,by*16,brick,0);
			}
		}
	}
for(by=0;by<10;by++)
	{
	for(bx=0;bx<20;bx++)
		{
	  if((bx+screenx>=ex)||(by+screeny>=ey))putcm_sprite(0,0,319,199,bx*16,by*16,153,0);
		}
	}
}

void display_flashing_platform(int mn)
{
int c1,pl,x=0,y=0,stx=0,sty=0;
static int flash=0;
if(platform[mn].used==0) return;
flash=1-flash;
//Draw were platform starts
if(platform[mn].type==0)
	{
	if(((((platform[mn].xp+platform[mn].lx)/16)-mapx)>-2)&&((((platform[mn].xp+platform[mn].lx)/16)-mapx)<21)&&(((platform[mn].yp+platform[mn].ly)/16)-mapy>-2)&&(((platform[mn].yp+platform[mn].ly)/16)-mapy<10))
		{
		putcm_sprite(0,0,319,160,((platform[mn].xp-(mapx*16))+platform[mn].lx),(platform[mn].yp-(mapy*16))+platform[mn].ly,49,0);
		}
	}
if(flash==1)
	{
	if(platform[mn].type==0)
		{
		stx=0;
		sty=0;
		for(c1=0;c1<(platform[mn].mlx+platform[mn].mly);c1++)
			{
			//Do x right...
			if(platform[mn].directionx==1)
				{
				stx+=8;
				if(stx>=platform[mn].mlx) stx=0;
				}
			//Do y down...
			if(platform[mn].directiony==1)
				{
				sty+=8;
				if(sty>=platform[mn].mly) sty=0;
				}
			if( (((platform[mn].xp+stx)/16)-mapx>-2)&&(((platform[mn].xp+stx)/16)-mapx<21)&&(((platform[mn].yp+sty)/16)-mapy>-2)&&(((platform[mn].yp+sty)/16)-mapy<10) )
				{
				putcm_sprite(0,0,319,160,((platform[mn].xp-(mapx*16))+stx),(platform[mn].yp-(mapy*16))+sty,49,0);
				}
			}
		}
		else
		{
		flash=1;
		}
	}
	else
	{
	if(platform[mn].directionx!=0) x=platform[mn].mlx;
	if(platform[mn].directiony!=0) y=platform[mn].mly;
	for(pl=0;pl<platform[mn].length;pl++)           
		{
		if( ((((platform[mn].xp+x)/16)-mapx)+pl>-2)&&((((platform[mn].xp+x)/16)-mapx)+pl<21)&&(((platform[mn].yp+y)/16)-mapy>-2)&&(((platform[mn].yp+y)/16)-mapy<10) )
			{
			putcm_sprite(0,0,319,160,(platform[mn].xp-(mapx*16))+x+(pl*16),(platform[mn].yp-(mapy*16))+y,49,0);
			}
		}
	}
}

void do_keys(char key)
{
if(key==0) key=getch();
//75=l 77=r 72=u 80=d
if(key==72&&mapy>0) mapy--;
if(key==75&&base_object>0) --base_object;
if(key==77&&base_object<MAX_OBJECTS-19) ++base_object;
if(key==80&&mapy+10<(WORLD_SIZE_Y/16)) mapy++;
if(key=='l') load_level();
if(key=='s') save_level();
}

void process_commands()
{
int t,found=-1,x=(mx+8)/16,y=(my+14)/16;
//putcm_sprite(0,0,319,199,7+x*16,162,153,0);
if(y==11&&x>0&&x<20)
	{
	object=x+base_object-1;
	if((object==0)&&(draw_mode!=3)) draw_mode=3;
	if((object==MAX_OBJECTS-1)&&(draw_mode!=4)&&(mb==2)&&(omb==0)) draw_mode=4;
	if((object==MAX_OBJECTS-1)&&(omb==0)&&(mb==1)) platform_type=1-platform_type;
	if((object==MAX_OBJECTS-1)&&(draw_mode==4)&&(omb==0)&&(mb==2))
		{
		for(t=0;t<30;t++) if(platform[t].used==1) found=1;
		if(found==1)
			{
			found=-1;
			if(oldsnap!=-1) t=oldsnap+1; else t=0;
			if(t==30) t=0;
			do{
				if(platform[t].used==1) found=t;
				t++;
				if(t==30) t=0;
				}while(found==-1);
			oldsnap=found;
			}
		}
	if((object>0)&&(object<FIRST_MONSTER))     {draw_mode=draw_position;}
	if((object>FIRST_MONSTER-1)&&(draw_mode!=2)&&(object<MAX_OBJECTS-1)) draw_mode=2;
	}
y=(my+11)/16;
if(y==12)
	{
	if(x==1&&omb!=mb) bg_on=1-bg_on;
	if(x==3&&omb!=mb) fg_on=1-fg_on;
	if(x==5&&omb!=mb&&draw_mode!=2&&draw_mode!=3&&draw_mode!=4) {draw_position=1-draw_position;draw_mode=draw_position;}
	if(x==7) init_level();
	}
}

int find_new_monster(void)
{
int mon=-1,found=-1;
do{
if(monster[++mon].dead==1) found=mon;
}while((mon<MAX_MONSTERS)&&(found==-1));
return(found);
}

int find_new_platform(void)
{
int mon=0,found=-1;
do{
if(platform[mon].used==0) found=mon;
mon++;
}while((mon<30)&&(found==-1));
return(found);
}

void create_monster(int mon,int mn,int x, int y)
{
if(mon==FIRST_MONSTER+1)
	{
	monster[mn].xp=x;	  		monster[mn].yp=y;
	monster[mn].dead=0;		monster[mn].jumpdelay=0;
	monster[mn].baseyp=0; 	monster[mn].type=0;
	monster[mn].visible=1;	monster[mn].killable=1;
	monster[mn].alwaysupdate=0;
	}
if(mon==FIRST_MONSTER+2)
	{
	monster[mn].xp=x;      monster[mn].yp=y;
	monster[mn].dead=0;    monster[mn].jumpdelay=0;
	monster[mn].baseyp=y;  monster[mn].type=1; monster[mn].visible=0;
	monster[mn].visible=0; monster[mn].killable=1;
	monster[mn].alwaysupdate=0;
	}
if(mon==FIRST_MONSTER)
	{
	monster[mn].xp=x;        monster[mn].yp=y;
	monster[mn].dead=0;      monster[mn].jumpdelay=0;
	monster[mn].baseyp=y;    monster[mn].type=2;
	monster[mn].visible=0;   monster[mn].killable=0;
	monster[mn].alwaysupdate=0;
	}
}

void add_new_monster(int x,int y,int mon)
{
int mn=-1;
//83,100,101=30,31,32
mn=find_new_monster();
if(mn==-1) return;
create_monster(mon,mn,x,y);
}

int add_new_platform(int x,int y)
{
int mn=-1;
mn=find_new_platform();
if(mn==-1) return(-1);
platform[mn].used=1;
platform[mn].xp=x;
platform[mn].yp=y;
platform[mn].mlx=0;
platform[mn].mly=0;
platform[mn].lx=0;
platform[mn].ly=0;
platform[mn].directionx=0;
platform[mn].directiony=0;
platform[mn].delay=0;
platform[mn].length=1;
platform[mn].type=platform_type;
return(mn);
}

void edit_platform_stats(int pf)
{
char key;
if(platform[pf].type==0)
	{
	do{
	gotoxy(2,2);
	printf("Editing platform atributes!!");
	gotoxy(2,4);printf("1) Delay to start : %d      ",platform[pf].delay);
	gotoxy(2,5);printf("2) Travel dist' X : %d      ",platform[pf].mlx);
	gotoxy(2,6);printf("3) Travel dist' Y : %d      ",platform[pf].mly);
	gotoxy(2,7);printf("4) Traveling in X : %d      ",platform[pf].directionx);
	gotoxy(2,8);printf("5) Traveling in Y : %d      ",platform[pf].directiony);
	gotoxy(2,9);printf("6) Platform Length: %d      ",platform[pf].length);
	gotoxy(2,10);printf("7) Start X pos    : %d      ",platform[pf].lx);
	gotoxy(2,11);printf("8) Start Y pos    : %d      ",platform[pf].ly);

	gotoxy(2,13);printf("Press number or Return to exit!");
	key=getch();
	if(key=='1')
		{
		gotoxy(21,4);printf(">      ");gotoxy(22,4);
		scanf("%d", &platform[pf].delay);
		fflush(stdin);
		}
	if(key=='2')
		{
		gotoxy(21,5);printf(">      ");gotoxy(22,5);
		scanf("%d", &platform[pf].mlx);
		if(platform[pf].lx>platform[pf].mlx-1) platform[pf].lx=platform[pf].mlx-1;
		if(platform[pf].lx<0) platform[pf].lx=0;
		fflush(stdin);
		}
	if(key=='3')
		{
		gotoxy(21,6);printf(">      ");gotoxy(22,6);
		scanf("%d", &platform[pf].mly);
		if(platform[pf].ly>platform[pf].mly-1) platform[pf].ly=platform[pf].mly-1;
		if(platform[pf].ly<0) platform[pf].ly=0;
		fflush(stdin);
		}
	if(key=='4')
		{
		gotoxy(21,7);printf(">      ");gotoxy(22,7);
		scanf("%d", &platform[pf].directionx);
		fflush(stdin);
		}
	if(key=='5')
		{
		gotoxy(21,8);printf(">      ");gotoxy(22,8);
		scanf("%d", &platform[pf].directiony);
		fflush(stdin);
		}
	if(key=='6')
		{
		gotoxy(21,9);printf(">      ");gotoxy(22,9);
		scanf("%d", &platform[pf].length);
		fflush(stdin);
		}
	if(key=='7')
		{
		gotoxy(21,10);printf(">      ");gotoxy(22,10);
		scanf("%d", &platform[pf].lx);
		if(platform[pf].lx>platform[pf].mlx-1) platform[pf].lx=platform[pf].mlx-1;
		if(platform[pf].lx<0) platform[pf].lx=0;
		fflush(stdin);
		}
	if(key=='8')
		{
		gotoxy(21,11);printf(">      ");gotoxy(22,11);
		scanf("%d", &platform[pf].ly);
		if(platform[pf].ly>platform[pf].mly-1) platform[pf].ly=platform[pf].mly-1;
		if(platform[pf].ly<0) platform[pf].ly=0;
		fflush(stdin);
		}
		}while(key!=13);
	}
if(platform[pf].type==1)
	{
	do{
	gotoxy(2,2);
	printf("Editing platform atributes!!");
	gotoxy(2,4);printf("1) Delay to fall  : %d      ",platform[pf].delay);
	gotoxy(2,5);printf("2) Platform Length: %d      ",platform[pf].length);
	gotoxy(2,7);printf("Press number or Return to exit!");
	key=getch();
	if(key=='1')
		{
		gotoxy(21,4);printf(">      ");gotoxy(22,4);
		scanf("%d", &platform[pf].delay);
		fflush(stdin);
		}
	if(key=='2')
		{
		gotoxy(21,5);printf(">      ");gotoxy(22,5);
		scanf("%d", &platform[pf].length);
		fflush(stdin);
		}
/*	if(key=='3')
		{
		gotoxy(21,6);printf(">      ");gotoxy(22,6);
		scanf("%d", &platform[pf].lx);
		if(platform[pf].lx>platform[pf].mlx) platform[pf].lx=platform[pf].mlx;
		fflush(stdin);
		}
	if(key=='4')
		{
		gotoxy(21,7);printf(">      ");gotoxy(22,7);
		scanf("%d", &platform[pf].ly);
		if(platform[pf].ly>platform[pf].mly) platform[pf].ly=platform[pf].mly;
		fflush(stdin);
		}*/
		}while(key!=13);
	}
}

void edit_monster_stats(int mn)
{
char key;
do{
gotoxy(2,2);
printf("Editing monster atributes!!");
gotoxy(2,4);printf("1) Jump Delay    : %d      ",monster[mn].jumpdelay);
gotoxy(2,5);printf("2) Killable      : %d      ",monster[mn].killable);
gotoxy(2,6);printf("3) Always Update : %d      ",monster[mn].alwaysupdate);
gotoxy(2,8);printf("Press number or Return to exit!");
key=getch();
if(key=='1')
	{
	gotoxy(20,4);printf(">      ");gotoxy(21,4);
	scanf("%d", &monster[mn].jumpdelay);
	fflush(stdin);
	}
if(key=='2')
	{
	gotoxy(20,5);printf(">      ");gotoxy(21,5);
	scanf("%d", &monster[mn].killable);
	fflush(stdin);
	}
if(key=='3')
	{
	gotoxy(20,6);printf(">      ");gotoxy(21,6);
	scanf("%d", &monster[mn].alwaysupdate);
	fflush(stdin);
	}
}while(key!=13);
}

int monster_collide(int x,int y)
{
int mxp,myp,difx,dify,mn;
for(mn=0;mn<MAX_MONSTERS;mn++)
	{
	if(monster[mn].dead==0)
		{
		mxp=monster[mn].xp; myp=monster[mn].yp;
		difx=x-mxp; dify=y-myp;
		if((difx>-1)&&(difx<16)&&(dify>-1)&&(dify<16)) return(mn);
		}
	}
return(-1);
}

int platform_collide(int x,int y)
{
int mxp,myp,difx,dify,mn;
for(mn=0;mn<30;mn++)
	{
	if(platform[mn].used==1)
		{
		mxp=platform[mn].xp; myp=platform[mn].yp;
		difx=x-mxp; dify=y-myp;
		if((difx>-1)&&(difx<platform[mn].length*16)&&(dify>-1)&&(dify<16)) return(mn);
		if(mn==oldsnap)
			{
			if((difx>-1)&&(difx<platform[mn].length*16)&&(dify>platform[mn].mly-1)&&(dify<platform[mn].mly+16)&&(platform[mn].directiony!=0)&&(platform[mn].directionx==0)) return(mn);
			if((difx>platform[mn].mlx-1)&&(difx<(platform[mn].mlx)+(platform[mn].length*16))&&(dify>-1)&&(dify<16)&&(platform[mn].directionx!=0)&&(platform[mn].directiony==0)) return(mn);
			if((difx>platform[mn].mlx-1)&&(difx<(platform[mn].mlx)+(platform[mn].length*16))&&(dify>platform[mn].mly-1)&&(dify<platform[mn].mly+16)&&(platform[mn].directionx!=0)&&(platform[mn].directiony!=0)) return(mn);
			}
		}
	}
return(-1);
}

int monster_convert(int m)
{
//object   monster type
//31 32 30=0 1 2
if(m==FIRST_MONSTER+1) return(0);
if(m==FIRST_MONSTER+2) return(1);
if(m==FIRST_MONSTER)   return(2);
return(-1);
}

void do_mouse(void)
{
static int drag=-1,ox=mx,oy=my,ofx=0,ofy=0;
int x,y;
if(mx==0&&my!=199&&my>159&&mb==2&&base_object>0) --base_object;
if(mx==319&&my!=199&&my>159&&mb==2&&base_object<MAX_OBJECTS-19) ++base_object;
if((my>159)&&(drag==-1))
	{
	if(mb!=0) process_commands();
	}
	else
	{
	if(draw_mode==4)
		{
		snap=platform_collide(mx+mapx*16,my+mapy*16);
		//if lmouse is clicking initially on an existing platform and moving...
		if((mb==2)&&(snap!=-1)&&(drag==-1)&&((ox!=mx)||(oy!=my))) {drag=snap;ofx=platform[drag].xp-((mapx*16)+ox);ofy=platform[drag].yp-((mapy*16)+oy);}
		ox=mx;oy=my;
		if((snap!=-1)&&(mb==1)) platform[snap].used=0;
		//if lmouse is clicked...
		//Add a new platform if there is none under it...
		if((mb==2)&&(snap==-1)&&(drag==-1))
			{
			snap=add_new_platform((mx-8)+mapx*16,(my-8)+mapy*16);
			if(snap!=-1) edit_platform_stats(snap);
			mb=0;omb=0;
			}
		if((mb==0)&&(omb==2))
			{
			//If it has not been dragged edit it...
			if((snap!=-1)&&(drag==-1)) edit_platform_stats(snap);
			}
		if(drag!=-1)
			{
			platform[drag].xp=(mx+ofx)+mapx*16;
			platform[drag].yp=(my+ofy)+mapy*16;
			if(mb!=2) drag=-1;
			//Stop platform cursor appearing while we drag...
			flash=1;
			}
		}
	if(draw_mode==2)
		{
		snap=monster_collide(mx+mapx*16,my+mapy*16);
		if((mb==2)&&((object==LAST_MONSTER+1)||(object==LAST_MONSTER+2))) {sonic_x=(mx-8)+mapx*16; sonic_y=(my-8)+mapy*16; sonic_facing=object-(LAST_MONSTER+1);}
		if((mb==1)&&(snap!=-1)) monster[snap].dead=1;
		if((mb==2)&&(snap==-1)) add_new_monster((mx-8)+mapx*16,(my-8)+mapy*16,object);
		if((mb==2)&&(snap!=-1)&&(mb!=omb))
			{
			if(monster_convert(object)!=monster[snap].type)
				{
				create_monster(object,snap,monster[snap].xp,monster[snap].yp);
				}
				else
				{
				edit_monster_stats(snap);
				}
			}
		}
	if((draw_mode==0)||(draw_mode==1)||(draw_mode==3))
	if(mb==2)
		{
		x=mx/16;y=my/16;
		if(draw_mode==1) map[mapx+x][mapy+y]=object_vector[object];
		if(draw_mode==0) mapfront[mapx+x][mapy+y]=object_vector[object];
		if(draw_mode==3) {world_size_x=16+(x*16)+(mapx*16);world_size_y=16+(y*16)+(mapy*16);}
		}
	if(mb==1)
		{
		x=mx/16;y=my/16;
		if(draw_mode==1) map[mapx+x][mapy+y]=0;
		if(draw_mode==0) mapfront[mapx+x][mapy+y]=0;
		}
	}
}

void draw_flashing(void)
{
int x,y;
if(flash==0) return;
if((draw_mode==0)||(draw_mode==1))
	{
	x=mx/16;y=my/16;
	putcm_sprite(0,0,319,160,x*16,y*16,object_vector[object],0);
	}
if(draw_mode==2)
	{
	snap=monster_collide(mx+mapx*16,my+mapy*16);
	if((snap==-1)||(object>LAST_MONSTER))
		{
		putcm_sprite(0,0,319,160,mx-8,my-8,object_vector[object],0);
		}
		else
		{
		putcm_sprite(0,0,319,160,monster[snap].xp-(mapx*16),monster[snap].yp-(mapy*16),object_vector[object],0);
		}
	}
if(draw_mode==4)
	{
	snap=platform_collide(mx+mapx*16,my+mapy*16);
	if(snap==-1)
		{
		putcm_sprite(0,0,319,160,mx-8,my-8,object_vector[object],0);
		}
		else
		{
		putcm_sprite(0,0,319,160,platform[snap].xp-(mapx*16),platform[snap].yp-(mapy*16),object_vector[object],0);
		}
	}
}

void draw_controls(void)
{
int x,y;
for(x=0;x<320;x++) {ppixel(x,160,8,buffers);ppixel(x,0,8,buffers);ppixel(x,199,8,buffers);}
for(y=0;y<199;y++) {ppixel(0,y,8,buffers);ppixel(319,y,8,buffers);}
for(x=0;x<19&&x+base_object<MAX_OBJECTS;x++)
	{
	putcm_sprite(0,0,319,199,8+x*16,162,153,0);
	putcm_sprite(0,0,319,199,8+x*16,162,object_vector[base_object+x],0);
	}
if(bg_on==1) putcm_sprite(0,0,319,199,8,181,154,0); else putcm_sprite(0,0,319,199,8,181,156,0);
if(fg_on==1) putcm_sprite(0,0,319,199,40,181,155,0); else putcm_sprite(0,0,319,199,40,181,157,0);
if(draw_mode==0) putcm_sprite(0,0,319,199,72,181,158,0);//Background
if(draw_mode==1) putcm_sprite(0,0,319,199,72,181,159,0);//Foreground
if(draw_mode==2) putcm_sprite(0,0,319,199,72,181,160,0);//Monster/Sonic
if(draw_mode==3) putcm_sprite(0,0,319,199,72,181,162,0);//Border
if(draw_mode==4)
	{
	putcm_sprite(0,0,319,199,72,181,163,0);//Platform
	if(platform_type==1)
		{
		for(x=0;x<16;x++)
			{
			for(y=0;y<16;y++)
				{
				if(*(buffers+57992l+x+(y*320))==0) *(buffers+57992l+x+(y*320))=15;
				}
			}
		}
	}
putcm_sprite(0,0,319,199,104,181,161,0);
putcm_sprite(0,0,319,199,154,181,183,0);
putcm_sprite(0,0,319,199,203,181,object_vector[object],0);

}

void display_monsters(void)
{
int mn;
//Display monsters here..................................
for(mn=0;mn<MAX_MONSTERS;mn++)
	{
	if(monster[mn].dead==0)
		{
		if( ((monster[mn].xp/16)-mapx>-1)&&((monster[mn].xp/16)-mapx<20)&&((monster[mn].yp/16)-mapy>-1)&&((monster[mn].yp/16)-mapy<10) )
			{
			switch(monster[mn].type)
				{
				case 0:putcm_sprite(0,0,319,160,monster[mn].xp-mapx*16,monster[mn].yp-mapy*16,100,0);break;
				case 1:putcm_sprite(0,0,319,160,monster[mn].xp-mapx*16,monster[mn].yp-mapy*16,101,0);break;
				case 2:putcm_sprite(0,0,319,160,monster[mn].xp-mapx*16,monster[mn].yp-mapy*16,83,0);break;
				}
			}
		}
	}
if( ((sonic_x/16)-mapx>-1)&&((sonic_x/16)-mapx<20)&&((sonic_y/16)-mapy>-1)&&((sonic_y/16)-mapy<10) )
	{
	putcm_sprite(0,0,319,160,sonic_x-mapx*16,sonic_y-mapy*16,14+sonic_facing,0);
	}
}

void display_platforms(void)
{
int mn,pl;
for(mn=0;mn<30;mn++)
	{
	if(platform[mn].used==1)
		{
		for(pl=0;pl<platform[mn].length;pl++)
			{
			if( (((platform[mn].xp/16)-mapx)+pl>-2)&&(((platform[mn].xp/16)-mapx)+pl<21)&&((platform[mn].yp/16)-mapy>-2)&&((platform[mn].yp/16)-mapy<10) )
				{
				if(platform[mn].type==0) putcm_sprite(0,0,319,160,((pl*16)+platform[mn].xp)-(mapx*16),platform[mn].yp-mapy*16,49,0);
				if(platform[mn].type==1) putcm_sprite(0,0,319,160,((pl*16)+platform[mn].xp)-(mapx*16),platform[mn].yp-mapy*16,92,0);
				}
			}
		}
	}
}

void draw_stat_bar(void)
{
int z,x;
float mcount=0;
for(z=0;z<MAX_MONSTERS;z++) if(monster[z].dead==0) mcount++;
mcount=mcount/MAX_MONSTERS*318;
for(x=1;x<mcount;x++) *(buffers+63360l+x)=12;
for(x=mcount+1;x<319;x++) *(buffers+63360l+x)=0;
mcount=0;
for(z=0;z<30;z++) if(platform[z].used==1) mcount++;
mcount=mcount/30*318;
for(x=1;x<mcount;x++) *(buffers+57280l+x)=12;
for(x=mcount+1;x<319;x++) *(buffers+57280l+x)=0;
}

void editor(void)
{
char key;
int snap=-1;
use_pal(0);
clear_screen(0,255);
init_level();
do{
		flash=1-flash;
		clear_screen(0,0);
		display_map(mapx,mapy);//Draw the world map...
		display_monsters();
		display_platforms();
		get_mouse();
		if(draw_mode==4) snap=platform_collide(mx+mapx*16,my+mapy*16);
		if(snap!=-1) oldsnap=snap;
		if((oldsnap!=-1)&&(draw_mode==4)) display_flashing_platform(oldsnap);
		if(my<160) draw_flashing();
		draw_controls();
		draw_stat_bar();
		putcm_sprite(0,0,319,199,mx,my,182,0);
		vsync_wait(1);
		move_screen(0,255);
		if(mx==0&&mapx>0&&(my<161||my==199)) mapx--;
		if(my==0&&mapy>0) mapy--;
		if(mx==319&&mapx+19<(WORLD_SIZE_X/16)-1&&(my<161||my==199)) mapx++;
		if(my==199&&mapy+9<(WORLD_SIZE_Y/16)-1) mapy++;
		if(kbhit()) while(kbhit()){do_keys(key=getch());} else do_mouse();
	//Process commands here...
}while(key!=' ');
}

void main(void)
{
printf("Sconic Editor Build 1.6.1");
ini_screen(3,REPORT);
ini_sprite(190,REPORT);
make_sprites();
ini_pal(REPORT);
make_palette();
ini_screamer(REPORT);

editor();

end_screamer();
end_pal();
end_screen();
end_sprite();
exit(1);
}