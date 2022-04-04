***********************************************************************


			  Sconic The Blue Blob
			  'Sconic Goes Forth!'

			  BETA-Release V 1.6.1
			      Read-me File
			     Jun 17th  2000


***********************************************************************

				CONTENTS

***********************************************************************

			1. MINIMUM REQUIREMENTS
			2. DEMO OBJECTIVES
			3. INSTALLATION
			4. GAME CONTROLS
			5. FAQ'S
			6. LEVEL EDITOR 
			7. CONTACT & LEGAL INFORMATION



***********************************************************************

	1. MINIMUM REQUIREMENETS

***********************************************************************

Intel Pentium 200 MMX PC (or 100% compatible CPU)
32bit 2D Graphics PCB
32Mb RAM (64Mb recommended)
1Mb free Hard Drive 
Windows 95/98/NT/NT SERVER

The SCONIC THE BLUE BLOB  Demo is a work in-progress. Thus it is not
fully optimised and configured for lower spec machines. These issues
will be corrected for the full retail version, giving the end user
control over engine and special effect variables ensuring smooth
gameplay over a wide variety of CPUs, accelerator cards and RAM.

Due to the complexity of the SCONIC THE BLUE BLOB models, users with
32Mb of RAM may experience some memory paging, especially when exiting
the game and returning to Windows. These problems will be fixed for the
release version of the game.

KNOWN ISSUE WITH SOUND CARDS:

Currently with Sound Cards, there are no supported devices by the 
SCONIC THE BLUE BLOB engine. This issue will be corrected for the full
retail version.



***********************************************************************

	2. DEMO OBJECTIVES

***********************************************************************

As Sconic, you have decided to travel to a strange island were you will
take your vacation this year. But when you're plane is coming into
land, it crashes into the side of a huge mountain. It is your job to
guide Sconic through the 5 levels of this demo to finally reach the
runway, where your plane will pick you up.



***********************************************************************

	3. INSTALLATION

***********************************************************************

SCONIC THE BLUE BLOB has all the components it needs to run included in
the game's engine, which will be written to the Sconic directory when
you have installed Sconic. To install the SCONIC THE BLUE BLOB demo,
double click on Setup.exe. Follow the onscreen instructions.

Although the SCONIC THE BLUE BLOB Demo will work with other 
applications running in the background, for the best results it is
recommended that you run the game as a stand-alone application 
(with no other applications running). 

As well as avoiding possible conflicts from other applications, this 
will also help prevent Windows from accessing the hard drive Virtual
Memory swap file, and affecting the speed of the game.



***********************************************************************

	4. GAME CONTROLS

***********************************************************************

The controls for the game are:

Cursor Left	Left  (or Pad 4)
Cursor Right	Right (or Pad 6)
Cursor Up	Jump  (or Pad 8)
Space		Emergency Exit!(If the boss walks in!)

Q		Increase Window Height
A		Decrease Window Height
O		Decrease Window Width
P		Decrease Window Width

H		Alter Game Speed (Cycles through 3 speeds)

D		View Animation Buffer Usage



***********************************************************************

	5. FAQ'S

***********************************************************************

1) Sconic runs too slowly!
	Sconic will usually run with no problems whatsoever.
But if you find the game runs very sluggishly, you can speed it up by
pressing the 'H' key. There are 3 settings to choose from.
On the first press, all delays are turned off, on the second, some
delays are turned on, and on the third press, the game runs back at the
default speed. Experiment with each setting to find the most suitable.

2) The screen goes black and I get a message in white lettering at the
top of the screen!
	These are system messages, originally there to tell the
programming team that something bad has happened to the game. In
almost all circumstances, they are caused by lack of memory, so try
to quite some windows and try again.

3) I need advice on designing a level!
	Just send the programmers an e-mail, they will be glad to help.

4) Theres no sound!
	Sconic does not support sound at this stage.



***********************************************************************

	6. LEVEL EDITOR (Quick intro by Sarah Cartwright)

***********************************************************************

Don't be afraid to try things in the editor. If worse comes to worse,
you can always re-install anything that you break. Feel free to add
in creatures, screw with their properties, build crazily detailed
scenes, etc. The best way to learn is by doing.

To run the editor, click on 'Edit Sconic', from the START menu.

Now, the editor is made up of two main viewing windows. At the bottom
of the screen we have your main toolbar, this has buttons for most of
your functions. At the top we have the 'World Viewer' window, this is
were part of the level you are currently editing appears.

-------------------------------
Control key overview
-------------------------------

L		: Load a level.
S		: Save a level.
Left Arrow	: Slide object selector left.
Right Arrow	: Slide object selector right.
Up Arrow	: Move map editor up.
Down Arrow	: Move map editor down.

-------------------------------
Loading
-------------------------------
Let's try loading a level to start. Using the keyboard press the 'L'
key to request the computer to load in a level. At the top of the
screen a prompt now appears asking you to enter a level name, all the
level's in Sconic take the form, level.? from 1 to 4. Lets load level
1. So type 'level.1' and press RETURN. If you have typed the level
name in correctly a loading progress bar will appear at the bottom of
the screen. When loaded, the level name (in this case "The Mountain!")
is displayed and a prompt asking you to press any key.

-------------------------------
Scrolling
-------------------------------
The level is now loaded. Try looking around the level, move the mouse
pointer to one of the screens edges, the level will scroll in that
direction. When you reach the edge of the level the map will stop
scrolling.

-------------------------------
The status window
-------------------------------
The Status window is were all the object selection and editing takes
place.
The top row of objects consists of:
1 Size selector 	: A checker board pattern, this you select to
			change the size of a level.
36 Standard objects	: These are all the objects the Sconic levels
			are made from.
3 Enemies		: These are the things that can kill Sconic.
2 Sconic placers	: These are used to place Sconic at the start
			of the level.
1 Platform		: This selects the platform function, repeatedly
			clicking with the left mouse button on this icon
			cycles through all the platforms. Right clicking
			selects the type of platform to be placed,
			either Moving platforms, or Collapsing platforms.

A thin red bar below this row of objects is a visual representation of
how many platforms have been used in this level.

Below this are 4 boxes, from left to right, they are:
BG ON/OFF : Turn background on/off
FG ON/OFF : Turn foreground on/off
These two options aid level design by unclutering the level area. They
have NO effect when the level is loaded into the game engine.
B/F       : Draw on BACKGOUND/FOREGROUND.This box selects what position
            the editor is to place the selected object on.
C         : CLEAR the current level.

Below the 4 boxes is another bar, this displays the number of enemies
that have been placed. When the bar is full you cannot place any more
enemies.

-------------------------------
Foreground & background
-------------------------------
The Sconic world is split into 4 main parts.

These are;
1 : The foreground, Sconic will go BEHIND these objects. Sconic will not
	interact with anything in the foreground (Except the water.)
2 : The background, this is were Sconic interacts with most of the
	objects in the world. If possible, and he is not blocked,
	Sconic goes infront of the background.
3 : Enemies, there are 3 types of Enemy, Homing Star, Lava blob, and
	Fish.
4 : Platforms, these allow Sconic to reach normally inaccessible
	parts of a level.

When you first enter the editor, the status window in the lower half of
the screen shows a large 'B', this indicates that any objects placed in
the level will be behind Sconic, and Sconic will be able to interact
with them. Generally, all objects placed in the foreground 'F' are just
for decoration.

N.B. All monsters and platforms are placed in the background.

-------------------------------
Monsters and things
-------------------------------
You may have noticed that objects within the Sconic world can only be
placed in discreet 16 pixel blocks. This is not true for monsters.
Monsters can be placed anywhere in the world, in a 1 pixel resolution.

	To place a monster on the map, press the right arrow key until
the homing star appears, then click on it. You will see the 'F' (or the 
'B') in the layer box turn to an 'M' indicating that you are now in
the monster editing mode.
	To place a large number of monsters, click and hold the mouse
button while you move the cursor around. The editor will ensure there
is sufficient space between them.

	Left clicking ON-TOP of an existing monster will do one of two
things:
1) A monster ATTRIBUTE editing menu will appear, if the monster editing
cursor is of the same type of monster that has been clicked.(See the
paragraph below, "The monster's attributes").
2) The monster on the map will be exchanged for the currently selected
monster, if it differs from the one on the map.

	Right clicking will erase a monster.

-------------------------------
The monster's attributes
-------------------------------
When you click over an existing monster, with the cursor showing the
same type of monster, a small attributes menu will appear.
The menu has 3 options:

1) Jump Delay :
This allows you to set a delay at the start of a level, that stops
the monster moving for a certain period of time.
You can use this setting when the yellow death stars are visible when
Sconic starts a level, this will prevent them from killing Sconic
before he has a chance to move!

2) Killable :
This setting, (either a 0 for NOT killable, or a 1 for killable)
allows you to specify if a monster can be killed by Sconic. This
means if you wanted you could have killable lava!

3) Always Update :
Off by default (a zero), this option was generally for slower
compters. If a monster goes off screen, it is no longer updated.
This option overides that behaviour.
You may find that if lava goes above the top of the screen, it might
never come down! If you change this option to a 1, it will cure this,

-------------------------------
Platforms and things
-------------------------------
The platform editing mode has a lot in common with the monster editor.
You can place platforms anywhere in the world, just like with a monster.
But there are 5 main differences:

1) Repeatedly left clicking on the platform editor in the object select
strip will cycle through all of the placed platforms.

2) Right clicking on the platform icon in the object select strip will
toggle the type of platform to be created, either a COLLAPSING platform
or a MOVING platform, denoted by the blue or black 'P' status indicator
respectively.

3) Once you have placed a platform, the platforms ATTRIBUTES (see below)
menu will appear, here you can select how a platform will move or how
long it takes to drop if it is a collapsing platform.

4) Once a platform is placed, and the cursor is placed over it, the
platforms scope of travel will be displayed as a flashing line of
platforms.

5) When a platform's scope of travel is displayed (see 3 above), you can
click and drag either end of the platform's range. This enables you to
'fine tune' the platforms position.

-------------------------------
The platform's attributes
-------------------------------
When you create a purple platform, quite a large selection of options
appears for you to adjust. They control all the different aspects of the
platform in the game. Here they are:

For the purple sliding platforms:

1)  Delay to start:
This option can have a range of values, (anything under the value 1 means
something special, which you can read about below in the "Advanced
platform information!"), but for now it does exactly what it says it does,
allows you to enter the length of time into a level before the platform
starts to move.

2) Travel dist' X:
This option allows you to specify how far in the X direction the platform
will travel, before coming back.

3) Travel dist' Y:
The same as for the travel distance in X, except moving up and down.

4) Travelling in X:
This tells the game if the platform is to be moving in the horizontal
direction. A 0 here means it isn't moving horizontally and a 1 means it
is moving to the right. Don't use -1 for moving left, UNLESS you alter
the value for number 7 (below) to something other than 0.

5) Travelling in Y:
This is the same as number 4, but for the vertical direction of a
platform.

6) Platform Length:
This allows you to specify the length of the platform (in complete 1
platform sections).

7) Start X pos:
This value allows you to place the platform horizontally somewhere in
it's range of motion.

8) Start Y pos:
This value allows you to place the platform vertically somewhere in it's
range of motion.

So, if the attributes were set as follows:

1) Delay to start: 75
2) Travel dist' X: 100
3) Travel dist' Y: 0
4) Travelling in X: -1
5) Travelling in Y: 0
6) Platform Length: 2
7) Start X pos: 50
8) Start Y pos: 0

The platform is 2 sections wide and would start moving horizontally
(2 & 4), after 75 frames of animation(1)  (this is at 35 frames per
second), covering 100 pixels in total(2). It would start in the middle
of this sweep(7) and when it does move, it will slide to the left(4).
(N.B. The settings that influence a particular characteristic are
enclosed in brackets.)

For the collapsing platforms:

1) Delay to fall  :
This setting is used to specify a delay before the platform collapses
to give Sconic a chance to jump off. (This is not all it does; see the
advanced information below for auxiliary characteristics of this option.)

2) Platform Length:
Like the purple platforms, this allows you to set the length of the
platform in 16 pixel segments.

-------------------------------
Advanced platform information!
-------------------------------
Here is a brief summary about the initial state of a platforms delay
attribute (both sliding and collapsing), and how this setting effects
the platforms behaviour during the game.

For the purple moving platforms...
If the delay is:

Less than -1, the platform just sits there forever.
Equal to -1 , the platform sits there until Sconic stands on it.
Equal to 0  , the platform moves as soon as the level begins.
More than 0 , the delay counts down, until it does = 0, then it start
              moving.

For the collapsible platforms...
If the delay is:

Less than -1, the platform sits there.
Equal to -1 , the platform sits there until sconic JUMPS on it (NOT WALKS).
Equal to 0  , the platform drops as soon as Sconic makes contact.
More than 0 , as soon as sconic triggers it, counts down to 0 (see up 1).

Use these settings to add variety to the game, such as a
floor that collapses onto spikes if Sconic jumps on it.

-------------------------------
Saving the stuff
-------------------------------
O.K. So you've created the best level ever and you want to play it.
To do this just press the 'S' key. The save menu will appear.
First off, you are asked for the filename to save the level as. The
level's in Sconic are named: Level.1  Level.2  Level.3  Level.4
So if you want to try out your level straight away, type 'level.1'
and press return.

Next up the levels CURRENT in-game name is displayed. (The one that
appears while the level is loading.)
That is followed by a prompt asking you for the levels NEW
description. If you want to keep the original name JUST PRESS RETURN.
If not, you can type upto 25 characters in, then press return.

A Saving status bar now appears while the level saves.
When the level has finished saving you are returned to the editing
mode.

N.B. IMPORTANT MESSAGE!!!!
Remember to include a FINISH FLAG in each level, otherwise you will
be unable to finish it!
Also make sure Sconic is in a nice safe starting position; at the
start of a level he can't move for a couple of seconds, so don't
place him near any homing stars!

-------------------------------
Quittin'n stuff
-------------------------------
To exit the level editor press the SPACE BAR.(Make sure the levels
saved first!)

There's lot's to change and there are many ways that a level can be
built. To get an idea of what's possible play the game first, get to
know how the levels play and then load them into the editor. Here you
can mess with them to try out new ideas, or see how a part of a level
works. Don't be afraid to bodge a level, if you do you can always
reinstall Sconic.

-------------------------------
And Lastly, Rolling Demo's!
-------------------------------
After making your wonderful level, all that's left to do is to create
a 'rolling demo' of your level. To do this you need to press the 'd'
key just after Sconic has started loading (on the black and white text
screen with all the confusing techno babble on it). If you have timed
it right (it IS tricky!), the screen will go blank and a message
asking you to press the level key you want to create the demo for
appears.  So you have the choice of pressing keys 1 through 4.
Once you press the level key of your choice, the screen changes to the
'Loading Level' animation, Sconic is now loading the level for your
demo!
The level will appear just as it does in a normal game, but unlike
the real game, you are under a strict time limit of only a few seconds.
You will notice in the top middle of the screen the word "RECORDING!",
this means exactly what it says! You are now creating the rolling demo!
As you play a bright white bar will slowly lengthen across the screen,
when it reaches the far right hand side, your times up, and the demo
will finish. So,

It's time to SHOW OFF your skills!

When the game finishes recording, you will be returned
to Windows. To see yourself in action, just load the game again, but
this time just let it play, the game will cycle through all 4 level
demos in order.

IMPORTANT! As of yet there is now way of setting the direction of the
rings that Sconic drops when he hit's a bad guy, this means that if
you hit anything that causes you to drop your rings while you are
recording, when you come to play it back:

THEY WONT EXPLOAD IN THE SAME WAY!

So Sconic will look like he's chasing after ghost rings!
YOU HAVE BEEN WARNED!



***********************************************************************

	7. CONTACT AND LEGAL INFORMATION

***********************************************************************

CONTACT INFORMATION


For more information, or to communicate with us with regards to this or
any Fluid Vision product, please consult us at:

                      ElfLights@Yahoo.Com

...or for questions/technical issues send the programmer an email at:
                      
                       Viruslayer@Yahoo.com

LEGAL INFORMATION

SCONIC THE BLUE BLOB is published by and copyright of Fluid Vision and
developed by Sarah Cartwright.

SCONIC THE BLUE BLOB TM & (c) 1998, 1998 Sarah Cartwright.
All Rights Reserved. "Fluid Vision" "Fluid Engine," and 
"Twilight Games" and their associated logos are the property of
Fluid Vision. All Rights Reserved.

***********************************************************************