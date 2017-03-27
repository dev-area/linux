/*
   curses.c  Chapter 15

   When compiling, specify -lcurses
*/

#include <curses.h>

int main (int argc, char *argv[])
{
   int i;

   initscr();
   nonl(); 
   cbreak(); 
   noecho(); 
   clear();

   for (i=0; i<25; i++) 
   {
     move(i, i+i);
     addstr("curses");
   }

   refresh(); 
   getch();
   endwin();

   return 0;
}
