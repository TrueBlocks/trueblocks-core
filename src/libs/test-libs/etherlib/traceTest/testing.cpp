/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include <algorithm>
#include <vector>
#include "options.h"

class CNode {
    CNode() {
    }

  public:
    string_q name;
    string_q URL;
    string_q APIKey;
    CNode(string_q& str) {
        name = nextTokenClear(str, ',');
        URL = nextTokenClear(str, ',');
        APIKey = str;
    }
};
typedef vector<CNode> CNodeArray;

class CTest {
  public:
    string_q num;
    string_q cmd;
    string_q name;
    string_q params;
    bool turbo;
    CTest(string_q& str) {
        replaceAll(str, "\t", " ");
        replaceAll(str, "  ", " ");
        num = nextTokenClear(str, ' ');
        turbo = contains(num, "_turbo");
        replace(num, "_turbo", "");
        cmd = nextTokenClear(str, ' ');
        name = nextTokenClear(str, ' ');
        params = str;
    }
};
typedef vector<CTest> CTestArray;

//--------------------------------------------------------------
int main(int argc, const char* argv[]) {
    string_q nodeStr = asciiFileToString(configPath("testing_nodes"));
    CNodeArray nodes;
    while (!nodeStr.empty()) {
        string_q line = nextTokenClear(nodeStr, '\n');
        if (!startsWith(line, "#")) {
            CNode node(line);
            nodes.push_back(node);
        }
    }

    string_q testStr = asciiFileToString("./trace_tests");
    cout << "TESTS: " << getCWD() << "\t" << testStr << endl;
    CTestArray tests;
    while (!testStr.empty()) {
        string_q line = nextTokenClear(testStr, '\n');
        if (!startsWith(line, "#")) {
            CTest test(line);
            tests.push_back(test);
        }
    }

    uint32_t passed = 0;
    for (auto node : nodes) {
        cout << node.name << "\t" << node.URL << endl;
        for (auto test : tests) {
            if (!test.name.empty()) {
                ostringstream os;
                os << "curl -s ";
                os << "--data '{\"method\":\"" << test.cmd << "\",\"params\":" << test.params
                   << ",\"id\":\"1\",\"jsonrpc\":\"2.0\"}' ";
                os << "-H \"Content-Type: application/json\" ";
                if (!node.APIKey.empty())
                    os << "-H \"x-api-key: " << node.APIKey << "\" ";
                os << "-X POST " << node.URL;
                os << " | sed -f sed_file";
                os << " | jq | grep -v \"\\\"time\\\":\"";
                os << " >" << toLower(node.name) << "/" << test.cmd << "_" << test.num << "_" << test.name << ".txt";
                string_q t = (test.num + "." + test.name + ":").substr(0, 12);
                cout << padRight(t, 12) << " " << padRight(test.cmd, 20) << test.params << " ";
                cout.flush();
                if (system(os.str().c_str())) {
                    return 0;
                }

                if (node.name == "TurboGeth") {
                    string_q turbo =
                        asciiFileToString("./turbogeth/" + test.cmd + "_" + test.num + "_" + test.name + ".txt");
                    string_q parity;
                    if (test.turbo) {
                        parity = turbo;
                        stringToAsciiFile("./parity/" + test.cmd + "_" + test.num + "_" + test.name + ".txt", parity);
                    } else {
                        parity = asciiFileToString("./parity/" + test.cmd + "_" + test.num + "_" + test.name + ".txt");
                    }
                    cout << ((turbo == parity) ? greenCheck : redX);
                    passed += (turbo == parity);
                }

                cout << endl;
            }
        }
        cout << endl;
    }
    cout << passed << " tests passed " << greenCheck << " " << (tests.size() - passed) << " tests failed " << redX << endl << endl;

    return 0;
}
