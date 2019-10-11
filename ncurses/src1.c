#include <curses.h>

int main()
{
    int x;
    WINDOW *win1;

    win1 = initscr();
    meta(win1, TRUE);
    cbreak();
    noecho();
    scrollok(win1, TRUE);
    keypad(win1, TRUE);

    refresh();

    while ((x = wgetch(win1)) != 'q') {
	printw("%d %x\n", x, x);
	refresh();
    }
    endwin();
    return 0;
}
