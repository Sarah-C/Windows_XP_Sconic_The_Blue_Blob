// This is compatible with graphics driver Screamer(Three) ONLY!

#include "Screamer.H"

spritedata spinfo[50];

void main(void)
{
	ini_screen(3,NO_REPORT);
	initgraph(NO_REPORT);
	getch();
	end_screen();
	exit(1);
}