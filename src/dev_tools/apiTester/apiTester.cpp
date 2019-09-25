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

//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    string_q contents = substitute(asciiFileToString("./api_tests.csv"),"\r", "");
    string_q header = nextTokenClear(contents, '\n');
    cout << header << endl;
    CStringArray lines;
    explode(lines, contents, '\n');
    for (auto line : lines) {
        if (!startsWith(line, "#") && !startsWith(line, "off")) {
            CStringArray parts;
            explode(parts, line, ',');
            string_q num      = trim(parts[1]);
            string_q route    = trim(parts[2]);
            string_q path     = trim(parts[3]);
            string_q tool     = trim(parts[4]);
            string_q filename = trim(parts[5]);
            string_q post     = parts.size() > 6 ? trim(parts[6]) : "";
            string_q options  = parts.size() > 7 ? trim(parts[7]) : "";
            ostringstream os;
            os << "curl -s \"http://localhost:8080/";
            os << route;
            if (!options.empty())
                os << "?" << options;
            os << "\"";
            if (!post.empty())
                os << " | " << post << " ";
            string_q filePath = "../../../working/" + path + "/" + tool + "/api_tests/" + tool + "_" + filename + ".txt";
            os << " >" << filePath; //../../../working/" << path << "/" << tool << "/api_tests/" << tool << "_" << filename << ".txt";
            cout << os.str() << ": ";
            if (!system(os.str().c_str())) {
                string_q newText = asciiFileToString(filePath);
                string_q oldText = asciiFileToString(substitute(filePath, "/working/", "/gold/"));
                if (newText != oldText) {
                    cout << redX << endl;
//                    return 1;
                } else {
                    cout << greenCheck << endl;
                }
            } else {
                cout << redX << endl;
//                return 1;
            }
        }
    }
    return 0;
}
