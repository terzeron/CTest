#include <curses.h>

int main()
{
    int i = 0;

    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    /*  scrollok(stdscr, TRUE); */
    while (getch() != 'q') {
	mvprintw(23, 0, "printing line # %d", i++);
	scroll(stdscr);
	refresh();
    }
    endwin();
    return 0;
}
