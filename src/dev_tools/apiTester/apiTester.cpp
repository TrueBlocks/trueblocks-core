#if 0
/*
 string_q a = "xxx world world world end";
 string_q b = "world";
 { cout << "Test 1:\n"; size_t f = find_nth(a, b, 0); cout << "\tresult: " << f << "\t" << (f != string::npos ? a.substr(f) : "-1") << endl; }
 { cout << "Test 2:\n"; size_t f = find_nth(a, b, 1); cout << "\tresult: " << f << "\t" << (f != string::npos ? a.substr(f) : "-1") << endl; }
 { cout << "Test 3:\n"; size_t f = find_nth(a, b, 2); cout << "\tresult: " << f << "\t" << (f != string::npos ? a.substr(f) : "-1") << endl; }
 { cout << "Test 4:\n"; size_t f = find_nth(a, b, 3); cout << "\tresult: " << f << "\t" << (f != string::npos ? a.substr(f) : "-1") << endl; }
 */
#endif

/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "utillib.h"

size_t find_nth(const string& haystack, size_t pos, const string& needle, size_t nth) {
    size_t found_pos = haystack.find(needle, pos);
    if (0 == nth || string::npos == found_pos)
        return found_pos;
    return find_nth(haystack, found_pos + 1, needle, nth-1);
}
size_t find_nth(const string& haystack, const string& needle, size_t nth) {
    return find_nth(haystack, 0, needle, nth);
}

//#define ALL
#ifdef ALL
bool quit_on_fail = true;
bool ignoreOff = true;
string_q tests[] = { "abi", "logs", "receipts", "state", "tokens", "traces", "transactions", "when", "where" };
#else
bool quit_on_fail = false;
bool ignoreOff = false;
string_q tests[] = { "abi", };
#endif
size_t nTests = sizeof(tests) / sizeof(string_q);

//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    string_q contents;
    for (size_t i = 0 ; i < nTests ; i++)
        contents += asciiFileToString(tests[i] + ".csv") + "\n";
    replaceAll(contents, "\n\n", "\n");

    CStringArray lines;
    explode(lines, contents, '\n');

    for (auto line : lines) {
        bool ignore = startsWith(line, "#") || (!ignoreOff && startsWith(line, "off")) || contains(line, ",route,");
        if (ignore) {
            size_t found = find_nth(line, ",", 5);
            if (found != string::npos) {
                cerr << cYellow << "Skipping: " << cOff << line.substr(0,found) << string(30,' ') << "\r";
                cerr.flush();
            }
        } else {
            CStringArray parts;
            explode(parts, line, ',');
            string_q num      = trim(parts[1]);
            string_q route    = trim(parts[2]);
            string_q path     = trim(parts[3]);
            string_q tool     = trim(parts[4]);
            string_q filename = trim(parts[5]);
            string_q post     = parts.size() > 6 ? trim(parts[6]) : "";
            string_q options  = parts.size() > 7 ? trim(parts[7]) : "";
            string_q filePath = "../../../working/" + path + "/" + tool + "/api_tests/";
            string_q fileName = tool + "_" + filename + ".txt";

            ostringstream cmd;
            cmd << "curl -s \"http:/""/localhost:8080/" << route;
            if (!options.empty())
                cmd << "?" << options;
            cmd << "\"";
            if (!post.empty())
                cmd << " | " << post << " ";
            string_q dispCmd = cmd.str();
            cmd << " >" << filePath + fileName;
            if (!system(cmd.str().c_str())) {
                string_q newText = asciiFileToString(filePath + fileName);
                string_q oldText = asciiFileToString(substitute(filePath + fileName, "/working/", "/gold/"));
                if (!newText.empty() && newText != oldText) {
                    cerr << dispCmd << ": " << redX << endl;
                    if (quit_on_fail)
                        return 1;
                } else if (newText.empty()) {
                    cerr << dispCmd << ": " << redX << endl;
                    if (quit_on_fail)
                        return 1;
                } else {
                    cerr << dispCmd << ": " << greenCheck << endl;
                }
            } else {
                cerr << dispCmd << ": " << redX << endl;
                if (quit_on_fail)
                    return 1;
            }
        }
        usleep(50000);
    }
    return 0;
}
