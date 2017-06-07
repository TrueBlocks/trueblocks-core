#if 0
#include <ncurses.h>
#include "etherlib.h"
#include "support.h"
#include "debug.h"

static CParams debugCmds[] = {
    CParams("-quit",    "Quit the current monitor program"),
    CParams("-correct", "Correct the current imbalance and continue to the next imbalance"),
    CParams("-trace",   "Trace previous transaction (enter 't' plus tab to scroll through recent txs)"),
    CParams("-buffer",  "Show the transaction buffer"),
    CParams("-verbose", "Toggle auto trace"),
    CParams("-help",    "Display this screen"),
    CParams("",         "Press enter to continue without correction, up or down arrows to recall commands"),
};
static SFUint32 nDebugCmds = sizeof(debugCmds) / sizeof(CParams);
//cout << "\rt:bn.tn = trace at blockNum.transID, h = help, enter to continue\r\n";

//---------------------------------------------------------------------
SFString completeCommand(const SFString& cmd) {
    for (int i=0;i<nDebugCmds-1;i++) {
        if (debugCmds[i].longName.substr(1,cmd.length()) == cmd) {
            return debugCmds[i].longName.substr(1);
        }
    }
    return cmd;
}
extern void showColoredTrace(CVisitor *vis, const SFHash& hash);
//---------------------------------------------------------------------
bool CVisitor::enterDebugger(void) {

    static SFStringArray cmds;
    SFString curCmd;
    int cursor=0;
    bool showKeys = false;

    cout << ">> ";
    cout.flush();

    bool done=false;
    while (!done) {
        cout << "\r" << SFString(' ',80) << "\r";
        int ch = getch();
        switch(ch) {
            case KEY_UP:
                if (cursor < cmds.getCount()) {
                    int index = cmds.getCount() - (++cursor);
                    curCmd = cmds[index];
//                } else {
//                    cout << "\r\t\t\t\t\t\tup - no action";
                }
                break;
            case KEY_DOWN:
                if (cursor > 0) {
                    curCmd = cursor > 0 ? cmds[cmds.getCount()-cursor--] : "";
                } else {
                    curCmd = "";
//                    cout << "\r\t\t\t\t\t\tdown - no action";
                }
                break;
            case KEY_LEFT:
//                curCmd = "left";
                break;
            case KEY_RIGHT:
//                curCmd = "right";
                break;
            case 127:  // backspace
//                cout << curCmd << "\r\n";
                curCmd = curCmd.substr(0, curCmd.length()-1);
//                cout << curCmd << "\r\n";
                break;
            case 9:  // tab
//                cout << curCmd << "\r\n";
                curCmd = completeCommand(curCmd);
//                cout << curCmd << "\r\n";
                break;
            case 10:  // 'enter'
                if (curCmd == "c" || curCmd == "correct") {
                    for (int i = 0 ; i < watches.getCount() ; i++)
                        watches[i].qbis.correct();
                    done = true;
                    cmds[cmds.getCount()] = curCmd;

                } else if (curCmd == "q" || curCmd == "quit" || curCmd == "exit") {
                    cout << "\r\n";
                    cout.flush();
                    return false;

                } else if (curCmd == "v" || curCmd == "verbose") {
                    cmds[cmds.getCount()] = curCmd;
                    autoTrace = !autoTrace;
                    cout << "autoTrace is " << (autoTrace ? "on" : "off");
                    cout.flush();

                } else if (curCmd == "b" || curCmd == "buffer") {
                    cout << "\r\nTransaction buffer:\r\n";
                    for (int i=0;i<tBuffer.getCount();i++) {
                        cout << "    " << tBuffer[i].bn << "." << tBuffer[i].tx << "\r\n";
                    }
                    cmds[cmds.getCount()] = curCmd;

                } else if (curCmd.startsWith("t") || curCmd.startsWith("trace")) {
                    cmds[cmds.getCount()] = curCmd;
                    curCmd.Replace("t:","");
                    SFUint32 bn = toLongU(nextTokenClear(curCmd,'.'));
                    SFUint32 tn = toLongU(curCmd);
                    CTransaction trans;
                    getTransaction(trans,bn,tn);
                    showColoredTrace(this, trans.hash);

                } else if (curCmd == "h" || curCmd == "help") {
                    cout << "\r\n" << bBlue << "Help:" << cOff << "\r\n";
                    for (int i=0;i<nDebugCmds;i++) {
                        SFString name = debugCmds[i].longName;
                        SFString cmd;
                        if (name.length())
                            cmd = "(" + name.substr(1,1) + ")" + name.substr(2);
                        cout << "    " << padRight(cmd,15) << "    " << debugCmds[i].description << "\r\n";
                    }
                    cout << "\r\n";
                    cmds[cmds.getCount()] = curCmd;

                } else if (curCmd == "") {
                    cout << "\r\n";
                    cout.flush();
                    return true;

                } else {
                    cout << "invalid command: " << curCmd << SFString(' ',90) << "\r\n";
                    cmds[cmds.getCount()] = curCmd;
                }
                curCmd = "";
                cursor = 0;
                break;
            default: {
                if (ch == 107) // 'k'
                    showKeys = !showKeys;
                if (showKeys)
                    cout << "\t\t\t\t" << (char)ch << " : " << ch << "\r\n";
                if ((ch >= 'a' && ch <= 'z') ||
                        (curCmd.startsWith('t') && (ch == ':' || ch == '.' || ch == ' ' || (ch >= '0' && ch <= '9'))))
                    curCmd += ch;
            }
        }
        cout << "\r>> " << curCmd;
        cout.flush();
    }
    return true;
}

#if 0
/*
 #include <ncurses.h>

int main()
{
    int ch;
    while ((ch = getch()) != '#') {
        switch(ch) {
            case KEY_UP:
                printw("\nUp Arrow");
                break;
            case KEY_DOWN:
                printw("\nDown Arrow");
                break;
            case KEY_LEFT:
                printw("\nLeft Arrow");
                break;
            case KEY_RIGHT:
                printw("\nRight Arrow");
                break;
            default: {
                printw("\nThe pressed key is ");
                attron(A_BOLD);
                printw("%c", ch);
                attroff(A_BOLD);
            }
        }
    }
    printw("\n\nBye Now!\n");
    return 0;
}

#if 0

#include <stdlib.h>
#include <curses.h>

#define MAIN_WIN_COLOR 1

int main(void) {

    /*  Create and initialize window  * /
    WINDOW * win;
    if ( (win = initscr()) == NULL ) {
        fputs("Could not initialize screen.", stderr);
        exit(EXIT_FAILURE);
    }

    /*  Initialize colors  * /
    if ( start_color() == ERR || !has_colors() || !can_change_color() ) {
        delwin(win);
        endwin();
        refresh();
        fputs("Could not use colors.", stderr);
        exit(EXIT_FAILURE);
    }

    init_pair(MAIN_WIN_COLOR, COLOR_WHITE, COLOR_BLUE);
    wbkgd(win, COLOR_PAIR(MAIN_WIN_COLOR));

    /*  Draw box  * /
    box(win, 0, 0);
    wrefresh(win);

    /*  Wait for keypress before ending  * /
    getch();

    /*  Clean up and exit  * /
    delwin(win);
    endwin();
    refresh();

    return EXIT_SUCCESS;
}
#endif

#if 0
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>                  /*  for sleep()  * /
#include <curses.h>

int main(void) {
    WINDOW * mainwin;

    if ( (mainwin = initscr()) == NULL ) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }

    start_color();                    /*  Initialize colours  * /
    mvaddstr(6, 32, " Hello, world! ");

    if ( has_colors() && COLOR_PAIRS >= 13 ) {
        int n = 1;
        init_pair(1,  COLOR_RED,     COLOR_BLACK);
        init_pair(2,  COLOR_GREEN,   COLOR_BLACK);
        init_pair(3,  COLOR_YELLOW,  COLOR_BLACK);
        init_pair(4,  COLOR_BLUE,    COLOR_BLACK);
        init_pair(5,  COLOR_MAGENTA, COLOR_BLACK);
        init_pair(6,  COLOR_CYAN,    COLOR_BLACK);
        init_pair(7,  COLOR_BLUE,    COLOR_WHITE);
        init_pair(8,  COLOR_WHITE,   COLOR_RED);
        init_pair(9,  COLOR_BLACK,   COLOR_GREEN);
        init_pair(10, COLOR_BLUE,    COLOR_YELLOW);
        init_pair(11, COLOR_WHITE,   COLOR_BLUE);
        init_pair(12, COLOR_WHITE,   COLOR_MAGENTA);
        init_pair(13, COLOR_BLACK,   COLOR_CYAN);

        while ( n <= 13 ) {
            color_set(n, NULL);
            mvaddstr(6 + n, 32, " Hello, world! ");
            mvprintw(6 + n, 45, "%c", ACS_BSSB);
            mvprintw(6 + n, 47, "%c", ACS_SSBB);
            mvprintw(6 + n, 49, "%c", ACS_BBSS);
            mvprintw(6 + n, 51, "%c", ACS_SBBS);
            mvprintw(6 + n, 53, "%c", ACS_SBSS);
            mvprintw(6 + n, 55, "%c", ACS_SSSB);
            mvprintw(6 + n, 57, "%c", ACS_SSBS);
            mvprintw(6 + n, 59, "%c", ACS_BSSS);
            mvprintw(6 + n, 61, "%c", ACS_BSBS);
            mvprintw(6 + n, 63, "%c", ACS_SBSB);
            mvprintw(6 + n, 65, "%c", ACS_SSSS);
            n++;
        }
    }

    refresh();
    sleep(3);

    delwin(mainwin);
    endwin();
    refresh();

    return EXIT_SUCCESS;
}
#endif
 */
#endif
#endif
