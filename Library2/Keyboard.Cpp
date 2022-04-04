//Keyboard handler.

#include "screamer.h"
#include <dos.h>
#include <conio.h>
#include <stdio.h>

void interrupt (*BIOSHandler)(...);
unsigned char KeyLeft=0,KeyRight=0,KeyUp=0,KeyDown=0,KeyFire=0;
unsigned char upon,upoff,lefton,leftoff,righton,rightoff,downon,downoff,fireon,fireoff;

void interrupt check_scancode(...)
{
int CallBIOSHandler=1;
unsigned char scanCode=inp(0x60);
if(scanCode==upon)
	{
	KeyUp=1;
	CallBIOSHandler=0;
	}
if(scanCode==upoff)
	{
	KeyUp=0;
	CallBIOSHandler=0;
	}
if(scanCode==lefton)
	{
	KeyLeft = 1;
	CallBIOSHandler = 0;
	}
if(scanCode==leftoff)
	{
	KeyLeft = 0;
	CallBIOSHandler = 0;
	}
if(scanCode==righton)
	{
	KeyRight = 1;
	CallBIOSHandler = 0;
	}
if(scanCode==rightoff)
	{
	KeyRight=0;
	CallBIOSHandler=0;
	}
if(scanCode==downon)
	{
	KeyDown = 1;
	CallBIOSHandler = 0;
	}
if(scanCode==downoff)
	{
	KeyDown=0;
	CallBIOSHandler=0;
	}
if(scanCode==fireon)
	{
	KeyFire = 1;
	CallBIOSHandler = 0;
	}
if(scanCode==fireoff)
	{
	KeyFire=0;
	CallBIOSHandler=0;
	}
if(CallBIOSHandler)
	{
	asm pushf
	BIOSHandler();
	}
	else
	{
	outportb(0x20,0x20);
	}
}

void load_keys(char *name)
{
FILE *in;
if ((in=fopen(name, "rt"))==NULL)
	{
	lefton=75;leftoff=203;upon=72;upoff=200;righton=77;rightoff=205;downon=80;downoff=208;fireon=29;fireoff=157;
	return;
	}
fscanf(in,"%c %c %c %c %c %c %c %c %c %c ",&lefton,&leftoff,&righton,&rightoff,&upon,&upoff,&downon,&downoff,&fireon,&fireoff);
fclose(in);
}

void ini_keyint(char *name,int quiet)
{
if(quiet==0) printf("\nCapturing keyboard interupt vector...");
load_keys(name);
BIOSHandler = getvect (9);
setvect(9,check_scancode);
if(quiet==0) printf("O.K!");
}

void end_keyint(void)
{
setvect(9,BIOSHandler);
}
