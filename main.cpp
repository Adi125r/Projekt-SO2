#include <iostream>
#include <map>
#include "ncurses.h"
#include <curses.h>
#include <thread>
#include <vector>

#include "diningPhilosophersProblem.h"

#define PBAR "***************************************************"
#define LENGTH 30

std::vector<philosopher *> philosophers;

class ui {
private:
    int row;
    int col;
    int x;
    int y;
    const int max_len = 50;
    std::mutex m;
    std::map<int, const char *> states = {
            std::pair<int, const char *>(-1, "czeka na swoja kolej"),
            std::pair<int, const char *>(0, "filozofuje"),
            std::pair<int, const char *>(1, "ucztuje"),
            std::pair<int, const char *>(2, "czeka na widelce")};

public:
    ui();
    ~ui();
    void update();
};

ui::ui() {
    initscr();
    noecho();
    raw();
    nodelay(stdscr, TRUE);
    start_color();
    use_default_colors();
    curs_set(0);
    init_pair(3, COLOR_RED, -1);
    init_pair(2, COLOR_CYAN, -1);
    init_pair(1, COLOR_GREEN, -1);
    getmaxyx(stdscr, col, row);
    x = row / 2 - max_len;
    y = col / 2;
attron( COLOR_PAIR( 3 ) ); //4
        printw( "Problem filozofów " );
    
}

ui::~ui() {
    endwin();
}

void ui::update() {
WINDOW * win = newwin(8, 26, 2, 2);
WINDOW * win1 = newwin(8,40, 2,30);    
    refresh();
    while (true) {
        int key = getch();
        if (key == 27) { //esc
            for (auto phil : philosophers) {
                phil->exit = true;
            }
            return;
        }

        for (auto phil : philosophers) {
            int id = phil->id;
            int state = phil->state;
 

box(win, 0, 0);
    mvwprintw(win, 1, 1, "Widelec 1 : ");
    mvwprintw(win, 2, 1, "Widelec 2 : ");
    mvwprintw(win, 3, 1, "Widelec 3 : ");
    mvwprintw(win, 4, 1, "Widelec 4 : ");
    mvwprintw(win, 5, 1, "Widelec 5 : ");
    box(win1, 0, 0);
    mvwprintw(win1, 1, 1, "Filozof 1 : ");
    mvwprintw(win1, 2, 1, "Filozof 2 : ");
    mvwprintw(win1, 3, 1, "Filozof 3 : ");
    mvwprintw(win1, 4, 1, "Filozof 4 : ");
    mvwprintw(win1, 5, 1, "Filozof 5 : ");
    wrefresh(win);
    wrefresh(win1);   
    
    
    
            move(y + id - 2, 0);
            clrtoeol();
            move(y + id - 2, x);
            if (state == 2) {
                attron(COLOR_PAIR(3));
                 mvwprintw(win1,id, 13, states[state]);
                 printw("Filozof %d %s", id, states[state]);
                 
wrefresh(win);
            } else {
                int progress = phil->progress;
                int lpad = std::round(progress / 100.0f * LENGTH);
                int rpad = LENGTH - lpad;
                attron(COLOR_PAIR(state + 1));
                 mvwprintw(win1,id, 13, states[state]);
                printw("Filozof %d %s  %3d%% [%.*s%*s]", id, states[state], progress, lpad, PBAR, rpad, "");
            

if (phil->state==0){
            attron(COLOR_PAIR(2));
           if (id==5){
            mvwprintw(win, 1,14," jest wolny");
            mvwprintw(win, 5,14,"jest wolny" );
}
else{
            mvwprintw(win, id,14," jest wolny");
         
            mvwprintw(win,id+1,14,"jest wolny" );}
        } else {
            if (id==5){
            mvwprintw(win, 1,14," jest zajety");
            mvwprintw(win, 5,14,"jest zajety" );
}
else{
            mvwprintw(win, id,14," jest zajety");
         
            mvwprintw(win,id+1,14,"jest zajety" );} }
            wrefresh(win);
        }}
        
    }
}

int main() {
    diningPhilosophersProblem table;
    ui u;
    for (auto i = 0; i < 4; i++) {
        philosopher *p = new philosopher(i + 1, table, std::ref(table.forks[i]), std::ref(table.forks[i + 1]));
        philosophers.push_back(p);
    }

    philosophers.push_back(new philosopher(5, table, table.forks[4], table.forks[0]));
    std::this_thread::sleep_for(std::chrono::seconds(1));
    table.ready = true;
    std::thread t1(&ui::update, &u);

    t1.join();
    for (auto p : philosophers) {
        p->t.join();
    }
    endwin();
    return 0;
}
