/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
/*
 * This code was generated automatically from grabABI and makeClass. You may
 * edit the file, but keep your edits inside the 'EXISTING_CODE' tags.
 */
#include <ncurses.h>
#include <stdlib.h>
#include "etherlib.h"
#include "processing.h"
#include "debug.h"

#ifdef DEBUGGER_ON
//---------------------------------------------------------------------
static const COption debugCmds[] = {
    COption("-(c)orrect", "c", "", OPT_SWITCH, "Correct the current imbalance and continue to the next imbalance"),
    COption("-(a)utocorrect", "a", "", OPT_SWITCH, "Turn on or off autocorrect (allows pressing enter to correct)"),
    COption("-(e)thscan", "e", "", OPT_SWITCH, "Open a block, account, or transaction in http://ethscan.io"),
    COption("-(s)ource", "s", "", OPT_SWITCH, "View the smart contract's source code (if found)"),
    COption("-(b)uffer", "b", "", OPT_SWITCH, "Show the transaction buffer (including transaction hashes)"),
    COption("-(l)ist", "l", "", OPT_SWITCH, "Show the list of accounts being debugged"),
    COption("-confi(g)", "g", "", OPT_SWITCH, "Edit the config file"),
    COption("-si(n)gle", "n", "", OPT_SWITCH, "Toggle single step"),
    COption("-(t)race", "t", "", OPT_SWITCH, "Toggle display of trace"),
    COption("-(d)ollars", "d", "", OPT_SWITCH, "Display US dollars as well"),
    COption("-!cmd", "c", "<cmd>", OPT_FLAG, "Run a system command"),
    COption("-(q)uit", "q", "", OPT_SWITCH, "Quit the current monitor program"),
    COption("-(h)elp", "h", "", OPT_SWITCH, "Display this screen"),
    COption("", "", "", OPT_DESCRIPTION,
            "Press enter to continue without correction, up or down arrows to recall commands"),
};
static const size_t nDebugCmds = sizeof(debugCmds) / sizeof(COption);

//---------------------------------------------------------------------
#define isdelim(cc) ((cc) == ':' || (cc) == '.' || (cc) == ' ')

//---------------------------------------------------------------------
string_q completeCommand(const string_q& cmd) {
    for (size_t i = 0; i < nDebugCmds - 1; i++) {
        if (extract(debugCmds[i].longName, 1, cmd.length()) == cmd) {
            return extract(debugCmds[i].longName, 1);
        }

        if (debugCmds[i].shortName + " " == cmd) {
            return extract(debugCmds[i].longName, 1);
        }
    }

    return cmd;
}

//---------------------------------------------------------------------
#define history(aa)                                                                                                    \
    { cmds.push_back(aa); }
#endif

//---------------------------------------------------------------------
bool COptions::enterDebugger(const CBlock& block) {
#ifdef DEBUGGER_ON
    // TODO(tjayrush): global data
    static CStringArray cmds;
    string_q curCmd;
    size_t cursor = 0;
    bool showKeys = false;

    establishFolder(configPath("cache/tmp/"));

    cout << ">> ";
    cout.flush();

    bool done = false;
    while (!done) {
        cout << "\r" << string_q(80, ' ') << "\r";
        int ch = getch();
        switch (ch) {
            case KEY_UP:
                if (cursor < cmds.size()) {
                    size_t index = cmds.size() - (++cursor);
                    curCmd = cmds[index];
                }
                break;
            case KEY_DOWN:
                if (cursor > 0) {
                    curCmd = cursor > 0 ? cmds[cmds.size() - cursor--] : "";
                } else {
                    curCmd = "";
                }
                break;
            case KEY_LEFT:
                break;
            case KEY_RIGHT:
                break;
            case 127:  // backspace
                curCmd = extract(curCmd, 0, curCmd.length() - 1);
                break;
            case 9:  // tab
                curCmd = completeCommand(curCmd);
                break;
            case 10:  // 'enter'
                if (curCmd == "c" || curCmd == "correct") {
                    for (size_t i = 0; i < watches.size(); i++)
                        watches.at(i).statement.correct();
                    done = true;
                    history(curCmd);

                } else if (curCmd == "q" || curCmd == "quit" || curCmd == "exit") {
                    cout << "\r\n";
                    cout.flush();
                    debugger_on = false;
                    return false;

                } else if (curCmd == "a" || curCmd == "autoCorrect") {
                    history(curCmd);
                    autocorrect_on = !autocorrect_on;
                    cout << "\tautoCorrect is " << (autocorrect_on ? "on" : "off");
                    cout.flush();

                } else if (curCmd == "t" || curCmd == "trace") {
                    history(curCmd);
                    trace_on = !trace_on;
                    cout << "\ttrace display is " << (trace_on ? "on" : "off");
                    cout.flush();

                } else if (curCmd == "n" || curCmd == "single") {
                    history(curCmd);
                    single_on = !single_on;
                    cout << "\tsingle step is " << (single_on ? "on" : "off");
                    cout.flush();

                } else if (curCmd == "d" || curCmd == "dollar") {
                    history(curCmd);
                    expContext().asDollars = !expContext().asDollars;
                    cout << "\tdollar display is " << (expContext().asDollars ? "on" : "off");
                    cout.flush();

                } else if (curCmd == "b" || curCmd == "buffer") {
                    if (tBuffer.size()) {
                        cout << "\r\nTransaction buffer:\r\n";
                        for (size_t i = 0; i < tBuffer.size(); i++) {
                            size_t pointer =
                                ((tBuffer.writeCursor == 0) ? tBuffer.size() - 1 : tBuffer.writeCursor - 1);
                            cout << ((pointer == i) ? "==> " : "    ");
                            cout << i << " " << tBuffer[i].bn << "." << tBuffer[i].tx << " " << tBuffer[i].hash
                                 << "\r\n";
                        }
                    } else {
                        cout << "\r\nThere are no items in the transaction buffer\r\n";
                    }
                    history(curCmd);

                } else if (curCmd == "l" || curCmd == "list") {
                    cout << "\r\nAccounts:\r\n";
                    cout << cOff << "[";
                    for (size_t i = 0; i < watches.size() - 1; i++) {
                        cout << " { ";
                        cout << "\"address\": \"" << watches[i].color << watches[i].address << cOff << "\", ";
                        cout << "\"firstBlock\": " << bRed << watches[i].firstBlock << cOff << ", ";
                        cout << "\"name\": \"" << watches[i].color << watches[i].name << cOff << "\"";
                        cout << " }" << (i < watches.size() - 2 ? ",\r\n " : " ]\r\n");
                    }
                    history(curCmd);

                } else if (startsWith(curCmd, "s ") || startsWith(curCmd, "s:") || startsWith(curCmd, "source")) {
                    history(curCmd);
                    replace(curCmd, "s:", "");
                    replace(curCmd, "s ", "");
                    replace(curCmd, "source:", "");
                    replace(curCmd, "source ", "");
                    for (size_t i = 0; i < watches.size(); i++) {
                        if (watches[i].address == curCmd || watches[i].name == curCmd)
                            curCmd = ("source/" + watches[i].name + ".sol");
                    }
                    editFile(curCmd);

                } else if (startsWith(curCmd, "!") || startsWith(curCmd, "! ")) {
                    history(curCmd);
                    replace(curCmd, "! ", "");
                    replace(curCmd, "!", "");
                    string_q ret = substitute(doCommand("NO_COLOR=true " + curCmd + " 2>/dev/null "), "\n", "\r\n");
                    cout << "\r\n" << ret << "\r\n";

                } else if (curCmd == "g" || curCmd == "config") {
                    history(curCmd);
                    editFile("./con fig.toml");

                } else if (startsWith(curCmd, "e ") || startsWith(curCmd, "e:") || startsWith(curCmd, "ethscan")) {
                    history(curCmd);
                    replace(curCmd, "e:", "");
                    replace(curCmd, "e ", "");
                    replace(curCmd, "ethscan:", "");
                    replace(curCmd, "ethscan ", "");
                    string_q cmd = "ethscan.py " + curCmd;
                    doCommand(cmd);

                } else if (curCmd == "h" || curCmd == "help") {
                    cout << "\r\n" << bBlue << "Help:" << cOff << "\r\n";
                    for (size_t i = 0; i < nDebugCmds; i++) {
                        string_q name = debugCmds[i].longName;
                        string_q cmd;
                        if (name.length()) {
                            string_q hotKey = substitute(debugCmds[i].shortName, "-", "");
                            replace(name, hotKey, "(" + hotKey + ")");
                            cmd = extract(name, 1);
                        }
                        cout << "    " << padRight(cmd, 15) << "    " << debugCmds[i].description << "\r\n";
                    }
                    cout << "\r\n";
                    history(curCmd);

                } else if (curCmd == "") {
                    cout << "\r\n";
                    cout.flush();
                    return true;

                } else {
                    cout << "invalid command: " << curCmd << string_q(90, ' ') << "\r\n";
                    history(curCmd);
                }
                curCmd = "";
                cursor = 0;
                break;
            default: {
                bool allowDigits = startsWith(curCmd, 't');
                bool allowHex = startsWith(curCmd, 's') || startsWith(curCmd, 'e');
                bool isSys = startsWith(curCmd, "!");
                if (!isSys && ch == 107)  // 'k'
                    showKeys = !showKeys;
                if (showKeys)
                    cout << "\t\t\t\t" << static_cast<char>(ch) << " : " << ch << "\r\n";
                if ((islower(ch)) || ch == '!' || isSys || (allowDigits && (isdelim(ch) || isdigit(ch))) ||
                    (allowHex && (isdelim(ch) || ch == 'x' || isxdigit(ch))))
                    curCmd += static_cast<char>(ch);
            }
        }
        cout << "\r>> " << curCmd;
        cout.flush();
    }
#endif
    return true;
}

#ifdef DEBUGGER_ON
//-----------------------------------------------------------------------
bool debugFile(void) {
    return fileExists(configPath("cache/tmp/debug"));
}
#endif
