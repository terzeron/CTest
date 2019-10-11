#include <curses.h>

int main()
{
    initscr();

    attron(A_STANDOUT);
    mvaddstr(2, 8, "This is STANDOUT mode");
    attroff(A_STANDOUT);
    attron(A_REVERSE);
    mvaddstr(4, 8, "This is REVERSE mode");
    attroff(A_REVERSE);
    attron(A_BOLD);
    mvaddstr(6, 8, "This is BOLD mode");
    attroff(A_BOLD);
    attron(A_UNDERLINE);
    mvaddstr(8, 8, "This is UNDERLINE mode");
    attroff(A_UNDERLINE);
    attron(A_DIM);
    mvaddstr(10, 8, "This is DIM mode");
    attroff(A_DIM);
    attron(A_NORMAL);
    mvaddstr(12, 8, "This is NORMAL mode");
    attroff(A_NORMAL);
    attron(A_BLINK);
    mvaddstr(14, 8, "This is BLINK mode");
    attroff(A_BLINK);
    attron(A_BOLD | A_BLINK | A_UNDERLINE);
    mvaddstr(16, 8, "This is mixed mode");
    attrset(A_NORMAL);
    refresh();
    getch();
    endwin();
    return 0;
}
