/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
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
    explicit CNode(string_q& str) {
        name = trim(nextTokenClear(str, ','));
        URL = trim(nextTokenClear(str, ','));
        APIKey = trim(str);
    }
};
typedef vector<CNode> CNodeArray;

class CTest {
  public:
    string_q num;
    string_q cmd;
    string_q name;
    string_q params;
    bool turbo{false};
    explicit CTest(string_q& str) {
        static uint64_t cnt = 0;
        replaceAll(str, "\t", " ");
        replaceAll(str, "  ", " ");
        if (countOf(str, ' ') == 1) {
            cmd = trim(nextTokenClear(str, ' '));
            params = trim(nextTokenClear(str, ' '));
        } else {
            num = trim(nextTokenClear(str, ' '));  // overwritten
            turbo = contains(num, "_turbo");
            replace(num, "_turbo", "");
            cmd = trim(nextTokenClear(str, ' '));
            name = trim(nextTokenClear(str, ' '));  // overwritten
            params = trim(str);
        }
        num = uint_2_Str(cnt++);
        name = cmd + "_" + num;
    }
};
typedef vector<CTest> CTestArray;

//--------------------------------------------------------------
int main(int argc, const char* argv[]) {
    string_q nodeStr = asciiFileToString(getConfigPath("testing_nodes"));
    CNodeArray nodes;
    while (!nodeStr.empty()) {
        string_q line = nextTokenClear(nodeStr, '\n');
        if (!startsWith(line, "#")) {
            CNode node(line);
            nodes.push_back(node);
        }
    }

    size_t passed{0}, failed{0};
    string_q testStr = asciiFileToString("./trace_tests");
    // cout << "TESTS: " << getCWD() << "\t" << testStr << endl;
    CTestArray tests;
    while (!testStr.empty()) {
        string_q line = trim(nextTokenClear(testStr, '\n'));
        if (!startsWith(line, "#")) {
            CTest test(line);
            tests.push_back(test);
        }
    }

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
                os << " | jq -S . ";
                string_q testFile = toLower(node.name) + "/" + test.name + ".txt";
                os << " >" << testFile;
                // cout << os.str() << endl;
                string_q t = (test.num + "." + test.name).substr(0, 20);
                if (node.name == "Erigon") {
                    cout << padRight(t, 20) << ": " << padRight(test.cmd, 20) << test.params << " ";
                    cout.flush();
                } else {
                    cerr << os.str() << "\r";
                    cerr.flush();
                }
                if (system(os.str().c_str())) {
                    return 0;
                }

                if (node.name == "Erigon") {
                    string_q turboFile = "./erigon/" + test.name + ".txt";
                    string_q parityFile = "./parity/" + test.name + ".txt";
                    string_q turboDat = asciiFileToString(turboFile);
                    string_q parityDat;
                    if (test.turbo) {
                        parityDat = turboDat;
                        stringToAsciiFile(parityFile, parityDat);
                    } else {
                        parityDat = asciiFileToString(parityFile);
                    }
                    cout << ((!turboDat.empty() && turboDat == parityDat) ? greenCheck : redX);
                    passed += (turboDat == parityDat);
                    failed += (turboDat != parityDat);
                    cout << endl;
                }
            }
        }
        cout << endl;
    }

    cout << passed << " tests passed " << (passed ? greenCheck : "") << " ";
    cout << failed << " tests failed " << (failed ? redX : "") << endl << endl;

    return 0;
}
