#include <curses.h>

int main()
{
    int i, j;

    initscr();
    noecho();
    for (i = 0; i < LINES; i++) {
	printw("%d %d", LINES, COLS);
	j = mvaddch(i, COLS - 1, 'A' + i);
	getch();
    }
    refresh();
    endwin();
    return 0;
}
