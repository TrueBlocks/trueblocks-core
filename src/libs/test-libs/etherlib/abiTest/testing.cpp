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
#include "etherlib.h"
#include "options.h"

extern bool test_encodings(void);
extern bool test_generation(void);
extern bool test_old_bug(void);
extern bool test_func_assign(void);
extern bool test_evt_assign(void);
extern bool test_eth_tests(uint64_t sub);
//--------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        while (!options.mode.empty()) {
            string_q mode = nextTokenClear(options.mode, '|');
            if (mode == "encoding") {
                cout << "Encodings test..." << endl;
                cout << (test_encodings() ? "...passed" : "...failed") << endl;
                cout << endl << "Done..." << endl;

            } else if (mode == "generation") {
                cout << "Generation test..." << endl;
                cout << (test_generation() ? "...passed" : "...failed") << endl;
                cout << endl << "Done..." << endl;

            } else if (mode == "old_bug") {
                cout << "Old bug test..." << endl;
                cout << (test_old_bug() ? "...passed" : "...failed") << endl;
                cout << endl << "Done..." << endl;

            } else if (mode == "func_assign") {
                cout << "function assignment test..." << endl;
                cout << (test_func_assign() ? "...passed" : "...failed") << endl;
                cout << endl << "Done..." << endl;

            } else if (mode == "evt_assign") {
                cout << "event assignment test..." << endl;
                cout << (test_evt_assign() ? "...passed" : "...failed") << endl;
                cout << endl << "Done..." << endl;

            } else if (mode == "eth_test") {
                setenv("ABI", "true", true);
                cout << "ethereum test box test..." << endl;
                cout << (test_eth_tests(options.sub) ? "...passed" : "...failed") << endl;
                cout << endl << "Done..." << endl;
            }
        }
    }
    return 0;
}

extern string_q getTestData(void);
extern string_q getAlreadySortedJson(void);
extern string_q getNotSortedJson(void);
//--------------------------------------------------------------
bool test_encodings(void) {
    bool ret = true;

    string_q contents = substitute(getTestData(), "  ", " ");
    nextTokenClear(contents, '\n');  // skip header row
    while (!contents.empty()) {
        string_q expected = nextTokenClear(contents, '\n');
        if (!startsWith(expected, ';')) {
            string_q type = nextTokenClear(expected, ' ');
            string_q text = nextTokenClear(expected, ' ');
            string_q myHex = chr_2_HexStr(text);
            string_q mySha = keccak256(myHex);
            if (type == "function")
                mySha = extract(mySha, 0, 10);
            bool result = mySha == expected;
            cout << "\t" << type << ": " << text << " "
                 << "encoding: " << mySha << " " << (result ? greenCheck : redX) << endl;
            ret &= result;
        }
    }
    return ret;
}

//--------------------------------------------------------------
bool test_generation(void) {
    CFunction::registerClass();
    CParameter::registerClass();

    {
        CAbi abi;
        cout << "Testing of already sorted JSON" << endl;
        loadAbiString(getAlreadySortedJson(), abi);
        cout << abi.Format() << endl << endl;
    }

    {
        CAbi abi;
        cout << "Testing of not sorted JSON" << endl;
        loadAbiString(getNotSortedJson(), abi);
        cout << abi.Format() << endl << endl;
    }

    return true;
}

//--------------------------------------------------------------
bool test_old_bug(void) {
    // This used to core dump when we first shifted to native c++ strings
    CFunction::registerClass();
    CParameter::registerClass();
    manageFields("CParameter:is_flags,is_enabled", false);

    CAbi abi;
    cout << string_q(120, '-') << "\nABI of test1.json" << endl << string_q(120, '-') << endl;
    loadAbiFile("./test1.json", &abi);
    cout << abi << endl;
    abi = CAbi();

    cout << string_q(120, '-') << "\nABI of test2.json" << endl << string_q(120, '-') << endl;
    loadAbiFile("./test2.json", &abi);
    cout << abi << endl;
    return true;
}

string_q data12 =
    "CEthStateArray|CBlockNumArray|CFunctionArray|CReconciliationOutput|CLogEntryArray|CParameterArray|"
    "CPerson|CReceipt|CNewReceipt|CNewTransactionArray|CBigUintArray|CTopicArray|address[]|bytes4|time|"
    // note - we leave 'CTreeNode *' here to show it gets fixed - should use 'CTreeNode* ' instead
    "uint8|CStringArray|CTraceAction|CTraceResult|CTransactionArray|CTreeNode *|CTopicArray|address|"
    "blknum|bool|bytes|bytes32|double|gas|hash|int256|int64|string|timestamp|uint256|uint32|"
    "uint64|wei";
//--------------------------------------------------------------
bool test_func_assign(void) {
    string_q types = data12;
    while (!types.empty()) {
        string_q type = nextTokenClear(types, '|');
        type += ((contains(type, "*") ? "" : " ") + string_q("_val"));
        CParameter param(type);
        cout << param.type << "\t" << param.getFunctionAssign(0);
    }

    return true;
}

//--------------------------------------------------------------
bool test_evt_assign(void) {
    string_q types = data12;
    while (!types.empty()) {
        string_q type = nextTokenClear(types, '|');
        type += ((contains(type, "*") ? "" : " ") + string_q("_val"));
        CParameter param(type);
        cout << param.type << "\t" << param.getEventAssign(0);
    }

    return true;
}

//--------------------------------------------------------------
string_q getTestData(void) {
    return "type     signature                                  expected\n"
           "event    ContributionMade(address)                  "
           "0x97a3367c201ad38e0d37322fd0ffa1b93457541ae8baf20eb9aa50bb83fcabef\n"  // NOLINT
           "event    RefundPayoutClaimed(uint256)               "
           "0x5e5c64a3ed3fb7fd17b9ca165f3ebe344a48d7c845dc947fc27249da676e7fbd\n"  // NOLINT
           "event    BeneficiaryPayoutClaimed(address,uint256)  "
           "0x22c1e24047f1e0c1af6f78290547f44645cdd2ad4d06b09115a162e41460c4d5\n"  // NOLINT
           "function emergencyCall()                            0x299ed37a\n"
           "function setup(address,address)                     0x2d34ba79\n"
           "function withdrawFunding()                          0x30b9af98\n"
           "function changeBaseValue(uint256)                   0x92a781d8\n"
           "function fund()                                     0xb60d4288\n"
           "function updateStage()                              0xc062f578\n"
           "function withdrawForWorkshop()                      0xf3a44fe1\n"
           "function issueTokens(address,uint256)               0x475a9fa9\n"
           "function depositRevenue()                           0x44e43cb8\n"
           "function withdrawRevenue()                          0x4f573cb2\n"
           "function softWithdrawRevenueFor(address)            0x5d0be9de\n"
           "event    Transfer(address,address,uint256)          "
           "0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef\n"  // NOLINT
           "event    Approval(address,address,uint256)          "
           "0x8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925\n"  // NOLINT
           "function transferFrom(address,address,uint256)      0x23b872dd\n"
           "function approve(address,uint256)                   0x095ea7b3\n"
           "function transfer(address,uint256)                  0xa9059cbb\n";
}

//--------------------------------------------------------------
string_q getAlreadySortedJson(void) {
    return "["
           "{\"name\":\"a\",\"type\":\"function\"},"
           "{\"name\":\"c\",\"type\":\"function\"},"
           "{\"name\":\"d\",\"type\":\"event\"},"
           "{\"name\":\"e\",\"type\":\"function\"},"
           "{\"name\":\"m\",\"type\":\"function\"},"
           "]";
}

//--------------------------------------------------------------
string_q getNotSortedJson(void) {
    return "["
           "{\"name\":\"z\",\"type\":\"function\"},"
           "{\"name\":\"a\",\"type\":\"function\"},"
           "{\"name\":\"b\",\"type\":\"event\"},"
           "{\"name\":\"zz\",\"type\":\"function\"},"
           "{\"name\":\"az\",\"type\":\"function\"},"
           "{\"name\":\"bz\",\"type\":\"event\"},"
           "{\"name\":\"za1\",\"type\":\"function\"},"
           "{\"name\":\"aa1\",\"type\":\"function\"},"
           "{\"name\":\"ba1\",\"type\":\"function\"},"
           "]";
}

#include "testing.h"
//------------------------------------------------------------------------
class CThisTest : public testing::Test {
  public:
    CThisTest(void) : Test() {
    }
    virtual void SetUp(void) {
    }
    virtual void TearDown(void) {
    }
};

//--------------------------------------------------------------------------------
string_q nominalSize(const string_q& strIn) {
    string_q str = strIn;
    reverse(str);
    string_q ret;
    for (auto ch : str) {
        if (isdigit(ch))
            ret += ch;
        else
            break;
    }
    reverse(ret);
    return ret;
}

//--------------------------------------------------------------------------------
string_q type_2_Serp(const string_q& typeList) {
    string_q ret;
    CStringArray types;
    explode(types, typeList, '|');
    for (auto type : types) {
        if (type == "bytes")
            ret += "s";
        else if (startsWith(type, "bytes"))
            ret += "b" + nominalSize(type);
        else if (type == "int256")
            ret += "i";
        else if (type == "int256[]")
            ret += "a";
        else
            return "unknown";
    }
    return ret;
}

//--------------------------------------------------------------------------------
string_q serp_2_Type(const string_q& serpent) {
    CStringArray types;
    for (size_t i = 0; i < serpent.length(); i++) {
        char ch = serpent[i];
        switch (ch) {
            case 's':
                types.push_back("bytes");
                break;
            case 'i':
                types.push_back("int256");
                break;
            case 'a':
                types.push_back("int256[]");
                break;
            case 'b': {
                string_q tmp = "bytes";
                size_t j = i + 1;
                while (j < serpent.length() && isdigit(serpent[j])) {
                    tmp += serpent[j];
                    j++;
                }
                i = j - 1;
                types.push_back(tmp);
                break;
            }
            default:
                types.clear();
                types.push_back("unknown");
                break;
        }
    }
    string_q ret;
    for (auto t : types)
        ret += (t + "|");
    return trim(ret, '|');
}

//------------------------------------------------------------------------
TEST_F(CThisTest, TestSerpent) {
    ASSERT_TRUE("type_2_Serp(bytes) == 'b'", type_2_Serp("bytes") == "s");            // NOLINT
    ASSERT_TRUE("type_2_Serp(int256) == 'i'", type_2_Serp("int256") == "i");          // NOLINT
    ASSERT_TRUE("type_2_Serp(int256[]) == 'a'", type_2_Serp("int256[]") == "a");      // NOLINT
    ASSERT_TRUE("type_2_Serp(bytes8) == 'b8'", type_2_Serp("bytes8") == "b8");        // NOLINT
    ASSERT_TRUE("type_2_Serp(bytes32) == 'b32'", type_2_Serp("bytes32") == "b32");    // NOLINT
    ASSERT_TRUE("type_2_Serp(...) == 'sb8ib8a'",                                      // NOLINT
                type_2_Serp("bytes|bytes8|int256|bytes8|int256[]") == "sb8ib8a");     // NOLINT
    ASSERT_TRUE("type_2_Serp(int8) == 'unknown'", type_2_Serp("int8") == "unknown");  // NOLINT
    ASSERT_TRUE("type_2_Serp(bool) == 'unknown'", type_2_Serp("bool") == "unknown");  // NOLINT

    ASSERT_TRUE("serp_2_Type('s') == 'bytes'", serp_2_Type("s") == "bytes");                         // NOLINT
    ASSERT_TRUE("serp_2_Type('i') == 'int256'", serp_2_Type("i") == "int256");                       // NOLINT
    ASSERT_TRUE("serp_2_Type('a') == 'int256[]'", serp_2_Type("a") == "int256[]");                   // NOLINT
    ASSERT_TRUE("serp_2_Type('b8') == 'bytes8'", serp_2_Type("b8") == "bytes8");                     // NOLINT
    ASSERT_TRUE("serp_2_Type('b8i') == 'bytes8|int256'", serp_2_Type("b8i") == "bytes8|int256");     // NOLINT
    ASSERT_TRUE("serp_2_Type('b32') == 'bytes32'", serp_2_Type("b32") == "bytes32");                 // NOLINT
    ASSERT_TRUE("serp_2_Type('b32i') == 'bytes32|int256", serp_2_Type("b32i") == "bytes32|int256");  // NOLINT
    ASSERT_TRUE("serp_2_Type('sb8ib8a') == 'bytes|bytes8|int256|bytes8|int256[]",                    // NOLINT
                serp_2_Type("sb8ib8a") == "bytes|bytes8|int256|bytes8|int256[]");
    ASSERT_TRUE("serp_2_Type('i8') == 'unknown'", serp_2_Type("i8") == "unknown");  // NOLINT
    ASSERT_TRUE("serp_2_Type('x') == 'unknown'", serp_2_Type("x") == "unknown");    // NOLINT

    return true;
}
}

//------------------------------------------------------------------------
class CFunctionTester : public CFunction {
  public:
    bool doTest(const string_q& line1) {
#if 0
        string_q line;
        line =
            "decode|decode donation tuple array (address token, uint256 amount, address recipient)[]|function donate((address+uint256+address)[])|\
0x\
0000000000000000000000000000000000000000000000000000000000000020\
0000000000000000000000000000000000000000000000000000000000000002\
0000000000000000000000006b175474e89094c44da98b954eedeac495271d0f\
000000000000000000000000000000000000000000000000869d529b714a0000\
000000000000000000000000f503017d7baf7fbc0fff7492b751025c6a78179b\
0000000000000000000000006b175474e89094c44da98b954eedeac495271d0f\
00000000000000000000000000000000000000000000000003782dace9d90000\
00000000000000000000000000de4b13153673bcae2616b67bf822500d325fc3\
|[{\"val_1\":\"0x6b175474e89094c44da98b954eedeac495271d0f\", \"val_2\":\"9700000000000000000\", \"val_3\":\"0xf503017d7baf7fbc0fff7492b751025c6a78179b\"}, {\"val_1\":\"0x6b175474e89094c44da98b954eedeac495271d0f\", \"val_2\":\"250000000000000000\", \"val_3\":\"0x00de4b13153673bcae2616b67bf822500d325fc3\"}]";
#else
        string_q line = line1;
#endif
        string_q expected, result;
        CStringArray parts;
        explode(parts, line, '|');
        if (parts.size() < 5)
            return false;
        for (auto& p : parts)
            p = trim(p);

        fromDefinition(*this, parts[2]);
        if (type != "function" && type != "event") {
            cout << "invalid: " << parts[2] << endl;
            return false;
        }

        cout << endl;
        cout << "line: " << line << endl;
        cout << "testType: " << parts[0] << endl;
        cout << "testName: " << parts[1] << endl;
        cout << "signature: " << parts[2] << " : " << type << " " << getSignature(SIG_CANONICAL) << endl;
        if (startsWith(line, ';')) {
            cout << (expected == result ? bGreen : bRed);
            cout << "expected: --" << parts[4] << "--?" << endl;
            cout << "result:   --" << parts[4] << "--? " << (parts[4] == parts[4]) << endl;
            cout << "testName: " << parts[1] << endl;
            cout << cOff;
            return true;  // report on commented lines, but don't do the test
        }

        if (parts[0] == "signature") {
            expected = parts[4];
            result = checkTypes() ? encodeItem() : "fail";
            result = (type == "function" ? result.substr(0, 10) : result);

        } else if (parts[0] == "encode" || parts[0] == "encode_raw") {
            expected = parts[4];
            result = encodeItem();
            result = (type == "function" ? result.substr(0, 10) : result);

        } else if (parts[0] == "decode" || parts[0] == "decode_raw") {
            cerr << endl << string_q(50, '-') << endl;
            decodeRLP(inputs, parts[2], parts[3]);
            expected = substitute(parts[4], "<empty>", "");
            for (auto param : inputs) {
                if (!result.empty())
                    result += ", ";
                result += param.value;
            }
            result = trim(result);
            cout << (expected == result ? bGreen : bRed);
            cout << "expected: " << expected << " ?" << endl;
            cout << "result:   " << result << " ? " << (expected == result) << endl;
            cout << "testName: " << parts[1] << endl;
            cout << cOff;
            if (!(expected == result))
                cout << "THIS IS AN ERROR, BUT WE DON'T QUIT." << endl;
            return true;  // debugging

        } else {
            return true;  // debugging
        }

        // cout << (expected == result ? bGreen : bRed);
        // cout << "expected: --" << expected << "--?" << endl;
        // cout << "result:   --" << result << "--? " << (expected == result) << endl;
        // cout << "testName: " << parts[1] << endl;
        // cout << cOff;
        ASSERT("Should not happen");
        return true;  // debugging
    }
};

//------------------------------------------------------------------------
TEST_F(CThisTest, TestSignatures) {
    string_q contents;
    asciiFileToString("./signatures.txt", contents);
    replaceAll(contents, "\\\n", "");
    CStringArray lines;
    explode(lines, contents, '\n');
    uint64_t cnt = 0;
    for (auto line : lines) {
        if (!line.empty() && !startsWith(line, '#')) {
            CFunctionTester func;
            cout << string_q(80, '-') << endl;
            ASSERT_TRUE("test_" + uint_2_Str(cnt++), func.doTest(line));
        }
    }
    return true;
}
}

//------------------------------------------------------------------------
TEST_F(CThisTest, TestEncode) {
    string_q contents;
    asciiFileToString("./encode.txt", contents);
    replaceAll(contents, "\\\n", "");
    CStringArray lines;
    explode(lines, contents, '\n');
    uint64_t cnt = 0;
    for (auto line : lines) {
        if (!line.empty() && !startsWith(line, '#')) {
            CFunctionTester func;
            cout << string_q(80, '-') << endl;
            ASSERT_TRUE("test_" + uint_2_Str(cnt++), func.doTest(line));
        }
    }
    return true;
}
}

//------------------------------------------------------------------------
TEST_F(CThisTest, TestEncodeRaw) {
    string_q contents;
    asciiFileToString("./encode_raw.txt", contents);
    replaceAll(contents, "\\\n", "");
    CStringArray lines;
    explode(lines, contents, '\n');
    uint64_t cnt = 0;
    for (auto line : lines) {
        if (!line.empty() && !startsWith(line, '#')) {
            CFunctionTester func;
            cout << string_q(80, '-') << endl;
            ASSERT_TRUE("test_" + uint_2_Str(cnt++), func.doTest(line));
        }
    }
    return true;
}
}

//------------------------------------------------------------------------
TEST_F(CThisTest, TestDecode1) {
    string_q contents;
    asciiFileToString("./decode.txt", contents);
    replaceAll(contents, "\\\n", "");
    CStringArray lines;
    explode(lines, contents, '\n');
    uint64_t cnt = 0;
    for (auto line : lines) {
        if (!line.empty() && !startsWith(line, '#')) {
            CFunctionTester func;
            cout << string_q(80, '-') << endl;
            ASSERT_TRUE("test_" + uint_2_Str(cnt++), func.doTest(line));
        }
    }
    return true;
}
}

//------------------------------------------------------------------------
TEST_F(CThisTest, TestDecode2) {
    string_q contents;
    asciiFileToString("./decode2.txt", contents);
    replaceAll(contents, "\\\n", "");
    CStringArray lines;
    explode(lines, contents, '\n');
    uint64_t cnt = 0;
    for (auto line : lines) {
        if (!line.empty() && !startsWith(line, '#')) {
            CFunctionTester func;
            cout << string_q(80, '-') << endl;
            ASSERT_TRUE("test_" + uint_2_Str(cnt++), func.doTest(line));
        }
    }
    return true;
}
}

//------------------------------------------------------------------------
TEST_F(CThisTest, TestDecodeRaw) {
    string_q contents;
    asciiFileToString("./decode_raw.txt", contents);
    replaceAll(contents, "\\\n", "");
    CStringArray lines;
    explode(lines, contents, '\n');
    uint64_t cnt = 0;
    for (auto line : lines) {
        if (!line.empty() && !startsWith(line, '#')) {
            CFunctionTester func;
            cout << string_q(80, '-') << endl;
            ASSERT_TRUE("test_" + uint_2_Str(cnt++), func.doTest(line));
        }
    }
    return true;
}
}

//------------------------------------------------------------------------
TEST_F(CThisTest, TestTight) {
    string_q contents;
    asciiFileToString("./tight.txt", contents);
    replaceAll(contents, "\\\n", "");
    CStringArray lines;
    explode(lines, contents, '\n');
    uint64_t cnt = 0;
    for (auto line : lines) {
        if (!line.empty() && !startsWith(line, '#')) {
            CFunctionTester func;
            cout << string_q(80, '-') << endl;
            ASSERT_TRUE("test_" + uint_2_Str(cnt++), func.doTest(line));
        }
    }
    return true;
}
}

//--------------------------------------------------------------
bool test_eth_tests(uint64_t sub) {
    switch (sub) {
        case 0:
            LOAD_TEST(TestSerpent);
            break;
        case 1:
            LOAD_TEST(TestSignatures);
            break;
        case 2:
            LOAD_TEST(TestEncode);
            break;
        case 3:
            LOAD_TEST(TestEncodeRaw);
            break;
        case 4:
            LOAD_TEST(TestDecode1);
            break;
        case 7:
            LOAD_TEST(TestDecode2);
            break;
        case 5:
            LOAD_TEST(TestDecodeRaw);
            break;
        case 6:
            LOAD_TEST(TestTight);
            break;
    }
    return !testing::RUN_ALL_TESTS();
}
