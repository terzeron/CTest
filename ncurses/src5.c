#include <curses.h>
#include <stdlib.h>

void verify_color()
{
    if (!has_colors()) {
	endwin();
	fprintf(stderr, "\nIn your terminal Color not supported\n");
	return;
    }
    start_color();
    init_pair(1, COLOR_RED, COLOR_WHITE);
    init_pair(2, COLOR_GREEN, COLOR_BLUE);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
}

void help(WINDOW * mw)
{
    WINDOW *helpwin;

    helpwin = subwin(mw, 6, 11, 2, 2);

    wrefresh(mw);
    if (touchline(mw, 0, 23) == ERR) {
	fprintf(stderr, "falkfjal\n");
	exit(-1);
    }

    wattron(helpwin, 2 << 8);
    box(helpwin, 0, 0);
    mvwaddstr(helpwin, 1, 1, "F1: Help");
    mvwaddstr(helpwin, 2, 1, "F2: Save");
    mvwaddstr(helpwin, 3, 1, "F3: Load");
    mvwaddstr(helpwin, 4, 1, "F10: Menu");
    wrefresh(helpwin);
    wgetch(helpwin);
    delwin(helpwin);

    wtouchln(mw, 0, 23, 1);
}

int main()
{
    WINDOW *mw;
    char *top = "          Ncurses   Demo!          ";
    char *bottom = "                        Made by terzeron   ";
    int i;

    mw = initscr();

    verify_color();

    wattron(mw, 2 << 8);
    wmove(mw, 0, 0);
    wprintw(mw, top);
    wattroff(mw, 2 << 8);

    wattron(mw, 2 << 8);
    wattron(mw, A_BOLD);
    wmove(mw, 23, 0);
    wprintw(mw, bottom);
    wattroff(mw, 2 << 8);

    wattron(mw, 1 << 8);
    wmove(mw, 0, 0);
    for (i = 1; i < 369; i++)
	wprintw(mw, "Linux");
    wrefresh(mw);
    help(mw);
    wgetch(mw);
    endwin();
    return 0;
}
