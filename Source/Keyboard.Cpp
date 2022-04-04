//Sconic keyboard handler.

#include <dos.h>
#include <conio.h>
#include <stdio.h>

void interrupt (*BIOSHandler)(...);
unsigned char CursorLeft=0,CursorRight=0,CursorUp=0;
unsigned char upon,upoff,lefton,leftoff,righton,rightoff;

void interrupt check_scancode(...)
{
int CallBIOSHandler=1;
unsigned char scanCode=inp(0x60);
if(scanCode==upon)
	{
	CursorUp=1;
	CallBIOSHandler=0;
	}
if(scanCode==upoff)
	{
	CursorUp=0;
	CallBIOSHandler=0;
	}
if(scanCode==lefton)
	{
	CursorLeft = 1;
	CallBIOSHandler = 0;
	}
if(scanCode==leftoff)
	{
	CursorLeft = 0;
	CallBIOSHandler = 0;
	}
if(scanCode==righton)
	{
	CursorRight = 1;
	CallBIOSHandler = 0;
	}
if(scanCode==rightoff)
	{
	CursorRight=0;
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

void load_keys(void)
{
FILE *in;
if ((in=fopen("Sconic.cfe", "rt"))==NULL)
	{
	lefton=75;leftoff=203;upon=72;upoff=200;righton=77;rightoff=205;
	return;
	}
fscanf(in,"%c %c %c %c %c %c ",&lefton,&leftoff,&righton,&rightoff,&upon,&upoff);
fclose(in);
}

void ini_sconic_keyint(int quiet)
{
if(!quiet) printf("\nCapturing keyboard interupt vector...");
load_keys();
BIOSHandler = getvect (9);
setvect(9,check_scancode);
printf("O.K!");
}

void end_sconic_keyint(void)
{
setvect(9,BIOSHandler);
}
