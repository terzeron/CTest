#include <stdio.h>
#include <curses.h>

int main(void)
{
    int key, lines = 0;

    initscr();
    noecho();
    cbreak();
    meta(stdscr, TRUE);
    scrollok(stdscr, TRUE);
    keypad(stdscr, TRUE);

    while (1) {
	key = wgetch(stdscr);
	if (key == 'q')
	    break;
	printw("%d\n", key);
	wrefresh(stdscr);
	if (lines > LINES - 1)
	    scroll(stdscr);
	else
	    lines++;
    }
    return 0;
}
