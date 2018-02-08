#include <ncurses.h>
#include <stdlib.h>
#include "etherlib.h"
#include "processing.h"
#include "debug.h"

//---------------------------------------------------------------------
static CParams debugCmds[] = {
    CParams("-(c)orrect",     "Correct the current imbalance and continue to the next imbalance"),
    CParams("-(a)utocorrect", "Turn on or off autocorrect (allows pressing enter to correct)"),
    CParams("-(e)thscan",     "Open a block, account, or transaction in http://ethscan.io"),
    CParams("-(s)ource",      "View the smart contract's source code (if found)"),
    CParams("-(b)uffer",      "Show the transaction buffer (including transaction hashes)"),
    CParams("-(l)ist",        "Show the list of accounts being debugged"),
    CParams("-confi(g)",      "Edit the config file"),
    CParams("-si(n)gle",      "Toggle single step"),
    CParams("-(t)race",       "Toggle display of trace"),
    CParams("-(d)ollars",     "Display US dollars as well"),
    CParams("-(q)uit",        "Quit the current monitor program"),
    CParams("-(h)elp",        "Display this screen"),
    CParams("",               "Press enter to continue without correction, up or down arrows to recall commands"),
};
static uint64_t nDebugCmds = sizeof(debugCmds) / sizeof(CParams);

//---------------------------------------------------------------------
#define isdelim(cc) ((cc) == ':' || (cc) == '.' || (cc) == ' ')

//---------------------------------------------------------------------
SFString completeCommand(const SFString& cmd) {

    for (uint32_t i=0;i<nDebugCmds-1;i++) {
        if (debugCmds[i].longName.substr(1,cmd.length()) == cmd) {
            return debugCmds[i].longName.substr(1);
        }

        if (debugCmds[i].shortName + " " == cmd) {
            return debugCmds[i].longName.substr(1);
        }
    }

    return cmd;
}

//---------------------------------------------------------------------
#define history(aa) { cmds[cmds.getCount()] = (aa); }

//---------------------------------------------------------------------
bool CVisitor::enterDebugger(const CBlock& block) {

    static SFStringArray cmds;
    SFString curCmd;
    uint32_t cursor=0;
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
                    uint32_t index = cmds.getCount() - (++cursor);
                    curCmd = cmds[index];
                }
                break;
            case KEY_DOWN:
                if (cursor > 0) {
                    curCmd = cursor > 0 ? cmds[cmds.getCount()-cursor--] : "";
                } else {
                    curCmd = "";
                }
                break;
            case KEY_LEFT:
                break;
            case KEY_RIGHT:
                break;
            case 127:  // backspace
                curCmd = curCmd.substr(0, curCmd.length()-1);
                break;
            case 9:  // tab
                curCmd = completeCommand(curCmd);
                break;
            case 10:  // 'enter'
                if (curCmd == "c" || curCmd == "correct") {
                    for (uint32_t i = 0 ; i < watches.getCount() ; i++)
                        watches[i].qbis.correct();
                    done = true;
                    history(curCmd);

                } else if (curCmd == "q" || curCmd == "quit" || curCmd == "exit") {
                    cout << "\r\n";
                    cout.flush();
                    opts.debugger_on = false;
                    return false;

                } else if (curCmd == "a" || curCmd == "autoCorrect") {
                    history(curCmd);
                    opts.autocorrect_on = !opts.autocorrect_on;
                    cout << "\tautoCorrect is " << (opts.autocorrect_on ? "on" : "off");
                    cout.flush();

                } else if (curCmd == "t" || curCmd == "trace") {
                    history(curCmd);
                    opts.trace_on = !opts.trace_on;
                    cout << "\ttrace display is " << (opts.trace_on ? "on" : "off");
                    cout.flush();

                } else if (curCmd == "n" || curCmd == "single") {
                    history(curCmd);
                    opts.single_on = !opts.single_on;
                    cout << "\tsingle step is " << (opts.single_on ? "on" : "off");
                    cout.flush();

                } else if (curCmd == "d" || curCmd == "dollar") {
                    history(curCmd);
                    expContext().asDollars = !expContext().asDollars;
                    cout << "\tdollar display is " << (expContext().asDollars ? "on" : "off");
                    cout.flush();

                } else if (curCmd == "b" || curCmd == "buffer") {
                    if (tBuffer.getCount()) {
                        cout << "\r\nTransaction buffer:\r\n";
                        for (uint32_t i=0;i<tBuffer.getCount();i++) {
                            cout << "    " << tBuffer[i].bn << "." << tBuffer[i].tx << " " << tBuffer[i].hash << "\r\n";
                        }
                    } else {
                        cout << "\r\nThere are no items in the transaction buffer\r\n";
                    }
                    history(curCmd);

                } else if (curCmd == "l" || curCmd == "list") {
                    cout << "\r\nAccounts:\r\n";
                    cout << cOff << "[";
                    for (uint32_t i=0;i<watches.getCount()-1;i++) {
                        cout << " { ";
                        cout << "\"index\": \""    <<                     watches[i].id              << "\", ";
                        cout << "\"address\": \""  << watches[i].color << watches[i].address    << cOff << "\", ";
                        cout << "\"firstBlock\": " << bRed                     << watches[i].firstBlock << cOff << ", ";
                        cout << "\"name\": \""     << watches[i].color << watches[i].name       << cOff << "\"";
                        cout << " }" << (i<watches.getCount()-2 ? ",\r\n " : " ]\r\n");
                    }
                    history(curCmd);

                } else if (curCmd.startsWith("s ") || curCmd.startsWith("s:") || curCmd.startsWith("source")) {
                    history(curCmd);
                    curCmd.Replace("s:","");
                    curCmd.Replace("s ","");
                    curCmd.Replace("source:","");
                    curCmd.Replace("source ","");
                    for (uint32_t i=0;i<watches.getCount();i++) {
                        if (watches[i].address == curCmd || watches[i].name == curCmd)
                            curCmd = ("source/" + watches[i].name + ".sol");
                    }
                    editFile(curCmd);

                } else if (curCmd == "g" || curCmd == "config") {
                    history(curCmd);
                    editFile("./config.toml");

                } else if (curCmd.startsWith("e ") || curCmd.startsWith("e:") || curCmd.startsWith("ethscan")) {
                    history(curCmd);
                    curCmd.Replace("e:","");
                    curCmd.Replace("e ","");
                    curCmd.Replace("ethscan:","");
                    curCmd.Replace("ethscan ","");
                    SFString cmd = "ethscan.py " + curCmd;
                    doCommand(cmd);

                } else if (curCmd == "h" || curCmd == "help") {
                    cout << "\r\n" << bBlue << "Help:" << cOff << "\r\n";
                    for (uint32_t i=0;i<nDebugCmds;i++) {
                        SFString name = debugCmds[i].longName;
                        SFString cmd;
                        if (name.length()) {
                            name.Replace(debugCmds[i].hotKey, "(" + debugCmds[i].hotKey + ")");
                            cmd = name.substr(1);
                        }
                        cout << "    " << padRight(cmd,15) << "    " << debugCmds[i].description << "\r\n";
                    }
                    cout << "\r\n";
                    history(curCmd);

                } else if (curCmd == "") {
                    cout << "\r\n";
                    cout.flush();
                    return true;

                } else {
                    cout << "invalid command: " << curCmd << SFString(' ',90) << "\r\n";
                    history(curCmd);
                }
                curCmd = "";
                cursor = 0;
                break;
            default: {
                bool allowDigits=curCmd.startsWith('t');
                bool allowHex=curCmd.startsWith('s')||curCmd.startsWith('e');
                if (ch == 107) // 'k'
                    showKeys = !showKeys;
                if (showKeys)
                    cout << "\t\t\t\t" << (char)ch << " : " << ch << "\r\n";
                if (
                        (islower(ch)) ||
                        (allowDigits && (isdelim(ch) || isdigit(ch))) ||
                        (allowHex    && (isdelim(ch) || isHex(ch)))
                    )
                    curCmd += (char)ch;
            }
        }
        cout << "\r>> " << curCmd;
        cout.flush();
    }
    return true;
}

//-----------------------------------------------------------------------
bool debugFile(void) {
    return fileExists("./cache/debug");
}
