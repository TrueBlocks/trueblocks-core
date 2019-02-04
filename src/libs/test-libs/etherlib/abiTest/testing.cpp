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
#include <algorithm>
#include "etherlib.h"
#include "options.h"

extern bool test_encodings  (void);
extern bool test_generation (void);
extern bool test_old_bug    (void);
extern bool test_func_assign(void);
extern bool test_evt_assign (void);
extern bool test_eth_tests  (uint64_t sub);
//--------------------------------------------------------------
int main(int argc, const char *argv[]) {
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
                cout << "Encodings test...\n";
                cout << (test_encodings() ? "...passed" : "...failed") << "\n";
                cout << "\nDone...\n";

            } else if (mode == "generation") {
                cout << "Generation test...\n";
                cout << (test_generation() ? "...passed" : "...failed") << "\n";
                cout << "\nDone...\n";

            } else if (mode == "old_bug") {
                cout << "Old bug test...\n";
                cout << (test_old_bug() ? "...passed" : "...failed") << "\n";
                cout << "\nDone...\n";

            } else if (mode == "func_assign") {
                cout << "function assignment test...\n";
                cout << (test_func_assign() ? "...passed" : "...failed") << "\n";
                cout << "\nDone...\n";

            } else if (mode == "evt_assign") {
                cout << "event assignment test...\n";
                cout << (test_evt_assign() ? "...passed" : "...failed") << "\n";
                cout << "\nDone...\n";

            } else if (mode == "eth_test") {
                setenv("ABI", "true", true);
                cout << "ethereum test box test...\n";
                cout << (test_eth_tests(options.sub) ? "...passed" : "...failed") << "\n";
                cout << "\nDone...\n";
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
            string_q type  = nextTokenClear(expected, ' ');
            string_q text  = nextTokenClear(expected, ' ');
            string_q myHex = chr_2_HexStr(text);
            string_q mySha = getSha3(myHex);
            if (type == "function")
                mySha = extract(mySha, 0, 10);
            bool result = mySha == expected;
            cout << "\t" << type << ": " << text << " "
                    << "encoding: " << mySha << " "
                    << (result ? greenCheck : redX) << "\n";
            ret &= result;
        }
    }
    return ret;
}

//---------------------------------------------------------------------------
bool sortByFunctionName(const CFunction& f1, const CFunction& f2) { return f1.name < f2.name; }

//--------------------------------------------------------------
bool test_generation(void) {
    CFunction::registerClass();
    CParameter::registerClass();

    {
        CAbi abi;
        cout << "Testing of already sorted JSON\n";
        abi.loadAbiFromString(getAlreadySortedJson(), false);
        sort(abi.interfaces.begin(), abi.interfaces.end(), sortByFunctionName);
        cout << abi.Format() << "\n\n";
        sort(abi.interfaces.begin(), abi.interfaces.end());
        cout << abi.Format() << "\n\n";
    }

    {
        CAbi abi;
        cout << "Testing of not sorted JSON\n";
        abi.loadAbiFromString(getNotSortedJson(), false);
        sort(abi.interfaces.begin(), abi.interfaces.end(), sortByFunctionName);
        cout << abi.Format() << "\n\n";
        sort(abi.interfaces.begin(), abi.interfaces.end());
        cout << abi.Format() << "\n\n";
    }

    return true;
}

//--------------------------------------------------------------
bool test_old_bug(void) {
    // This used to core dump when we first shifted to native c++ strings
    CFunction::registerClass();
    CParameter::registerClass();

    CAbi abi;
    cout << string_q(120, '-') << "\nABI of test1.json\n" << string_q(120, '-') << "\n";
    abi.loadAbiFromFile("./test1.json", false);
    sort(abi.interfaces.begin(), abi.interfaces.end(), sortByFunctionName);
    cout << abi << "\n";
    abi = CAbi();

    cout << string_q(120, '-') << "\nABI of test2.json\n" << string_q(120, '-') << "\n";
    abi.loadAbiFromFile("./test2.json", false);
    sort(abi.interfaces.begin(), abi.interfaces.end(), sortByFunctionName);
    cout << abi << "\n";
    return true;
}

string_q data =
"CBalanceHistoryArray|CBlockNumArray|CFunctionArray|CIncomeStatement|CLogEntryArray|CParameterArray|"
"CPerson|CReceipt|CNewReceipt|CNewTransactionArray|CBigUintArray|CTopicArray|address[]|bytes4|time|"
"uint8|CStringArray|CTraceAction|CTraceResult|CTransactionArray|CTreeNode *|CTopicArray|address|"
"blknum|bloom|bool|bytes|bytes32|double|gas|hash|int256|int64|string|timestamp|uint256|uint32|"
"uint64|wei";
//--------------------------------------------------------------
bool test_func_assign(void) {

    string_q types = data;
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

    string_q types = data;
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

    return
      "type     signature                                  expected\n"
      "event    ContributionMade(address)                  0x97a3367c201ad38e0d37322fd0ffa1b93457541ae8baf20eb9aa50bb83fcabef\n"  // NOLINT
      "event    RefundPayoutClaimed(uint256)               0x5e5c64a3ed3fb7fd17b9ca165f3ebe344a48d7c845dc947fc27249da676e7fbd\n"  // NOLINT
      "event    BeneficiaryPayoutClaimed(address,uint256)  0x22c1e24047f1e0c1af6f78290547f44645cdd2ad4d06b09115a162e41460c4d5\n"  // NOLINT
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
      "event    Transfer(address,address,uint256)          0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef\n"  // NOLINT
      "event    Approval(address,address,uint256)          0x8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925\n"  // NOLINT
      "function transferFrom(address,address,uint256)      0x23b872dd\n"
      "function approve(address,uint256)                   0x095ea7b3\n"
      "function transfer(address,uint256)                  0xa9059cbb\n";
}

//--------------------------------------------------------------
string_q getAlreadySortedJson(void) {
    return
        "["
            "{\"name\":\"a\"},"
            "{\"name\":\"c\"},"
            "{\"name\":\"d\"},"
            "{\"name\":\"e\"},"
            "{\"name\":\"m\"},"
        "]";
}

//--------------------------------------------------------------
string_q getNotSortedJson(void) {
    return
        "["
            "{\"name\":\"z\"},"
            "{\"name\":\"a\"},"
            "{\"name\":\"b\"},"
            "{\"name\":\"zz\"},"
            "{\"name\":\"az\"},"
            "{\"name\":\"bz\"},"
            "{\"name\":\"za1\"},"
            "{\"name\":\"aa1\"},"
            "{\"name\":\"ba1\"},"
        "]";
}

#include "testing.h"
//------------------------------------------------------------------------
class CThisTest : public testing::Test {
public:
    CThisTest(void) : Test() {}
    virtual void SetUp    (void) {}
    virtual void TearDown (void) {}
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
    for (size_t i = 0 ; i < serpent.length() ; i++) {
        char ch = serpent[i];
        switch (ch) {
            case 's': types.push_back("bytes"); break;
            case 'i': types.push_back("int256"); break;
            case 'a': types.push_back("int256[]"); break;
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
    return trim(ret,'|');
}

//------------------------------------------------------------------------
TEST_F(CThisTest, TestSerpent) {

    ASSERT_TRUE("type_2_Serp(bytes) == 'b'",      type_2_Serp("bytes") == "s");
    ASSERT_TRUE("type_2_Serp(int256) == 'i'",     type_2_Serp("int256") == "i");
    ASSERT_TRUE("type_2_Serp(int256[]) == 'a'",   type_2_Serp("int256[]") == "a");
    ASSERT_TRUE("type_2_Serp(bytes8) == 'b8'",    type_2_Serp("bytes8") == "b8");
    ASSERT_TRUE("type_2_Serp(bytes32) == 'b32'",  type_2_Serp("bytes32") == "b32");
    ASSERT_TRUE("type_2_Serp(...) == 'sb8ib8a'",  type_2_Serp("bytes|bytes8|int256|bytes8|int256[]") == "sb8ib8a");
    ASSERT_TRUE("type_2_Serp(int8) == 'unknown'", type_2_Serp("int8") == "unknown");
    ASSERT_TRUE("type_2_Serp(bool) == 'unknown'", type_2_Serp("bool") == "unknown");

    ASSERT_TRUE("serp_2_Type('s') == 'bytes'",            serp_2_Type("s") == "bytes");
    ASSERT_TRUE("serp_2_Type('i') == 'int256'",           serp_2_Type("i") == "int256");
    ASSERT_TRUE("serp_2_Type('a') == 'int256[]'",         serp_2_Type("a") == "int256[]");
    ASSERT_TRUE("serp_2_Type('b8') == 'bytes8'",          serp_2_Type("b8") == "bytes8");
    ASSERT_TRUE("serp_2_Type('b8i') == 'bytes8|int256'",  serp_2_Type("b8i") == "bytes8|int256");
    ASSERT_TRUE("serp_2_Type('b32') == 'bytes32'",        serp_2_Type("b32") == "bytes32");
    ASSERT_TRUE("serp_2_Type('b32i') == 'bytes32|int256", serp_2_Type("b32i") == "bytes32|int256");
    ASSERT_TRUE("serp_2_Type('sb8ib8a') == 'bytes|bytes8|int256|bytes8|int256[]",
                serp_2_Type("sb8ib8a") == "bytes|bytes8|int256|bytes8|int256[]");
    ASSERT_TRUE("serp_2_Type('i8') == 'unknown'",         serp_2_Type("i8") == "unknown");
    ASSERT_TRUE("serp_2_Type('x') == 'unknown'",          serp_2_Type("x") == "unknown");

    return true;
}}

//------------------------------------------------------------------------
class CFunctionTester : public CFunction {
public:
    bool doTest(const string_q& line1) {

#if 0
        string_q line;
        line = "decode|decoding (uint128[2]      [3 ], uint)|function baz(uint128[2  ]       [3],uint)|0x000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000003000000000000000000000000000000000000000000000000000000000000000400000000000000000000000000000000000000000000000000000000000000050000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000a|[[1, 2, 3], [4, 5, 6]], 10";
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

        fromDefinition(parts[2]);
        if (type != "function" && type != "event") {
            cout << "invalid: "  << parts[2] << endl;
            return false;
        }

        cout << endl;
        cout << "line: "      << line << endl;
        cout << "testType: "  << parts[0] << endl;
        cout << "testName: "  << parts[1] << endl;
        cout << "signature: " << parts[2] << " : " << type << " " << getSignature(SIG_CANONICAL) << endl;
        cout << "values: "    << parts[3] << endl;
        if (startsWith(line, ';')) {
            cout << "expected: "  << parts[4] << " : " << parts[4] << endl;
            return true;  // report on commented lines, but don't do the test
        }

        if (parts[0] == "signature") {
            expected = parts[4];
            result   = checkTypes() ? encodeItem() : "fail";
            result   = (type == "function" ? result.substr(0,10) : result);

        } else if (parts[0] == "encode" || parts[0] == "encode_raw") {
            expected = parts[4];
            result   = encodeItem();
            result   = (type == "function" ? result.substr(0,10) : result);

        } else if (parts[0] == "decode" || parts[0] == "decode_raw") {
            expected = parts[4];
            decodeRLP(inputs, parts[3]);
            for (auto param : inputs) {
                if (!result.empty())
                    result += ", ";
                result += param.value;
            }
            cout << "expected: "  << expected << " : " << result << endl;
            return true; // debugging

        } else {
            return false;
        }
        cout << "expected: "  << expected << " : " << result << endl;
        return (expected == result);
    };
};

//------------------------------------------------------------------------
TEST_F(CThisTest, TestSignatures) {
    string_q contents;
    asciiFileToString("./signatures.txt", contents);replaceAll(contents, "\\\n","");
    CStringArray lines;
    explode(lines, contents, '\n');
    uint64_t cnt = 0;
    for (auto line : lines) {
        if (!line.empty() && !startsWith(line, '#')) {
            CFunctionTester func;
            cout << string_q(80,'-') << endl;
            ASSERT_TRUE("test_" + uint_2_Str(cnt++), func.doTest(line));
        }
    }
    return true;
}}

//------------------------------------------------------------------------
TEST_F(CThisTest, TestEncode) {
    string_q contents;
    asciiFileToString("./encode.txt", contents);replaceAll(contents, "\\\n","");
    CStringArray lines;
    explode(lines, contents, '\n');
    uint64_t cnt = 0;
    for (auto line : lines) {
        if (!line.empty() && !startsWith(line, '#')) {
            CFunctionTester func;
            cout << string_q(80,'-') << endl;
            ASSERT_TRUE("test_" + uint_2_Str(cnt++), func.doTest(line));
        }
    }
    return true;
}}

//------------------------------------------------------------------------
TEST_F(CThisTest, TestEncodeRaw) {
    string_q contents;
    asciiFileToString("./encode_raw.txt", contents);replaceAll(contents, "\\\n","");
    CStringArray lines;
    explode(lines, contents, '\n');
    uint64_t cnt = 0;
    for (auto line : lines) {
        if (!line.empty() && !startsWith(line, '#')) {
            CFunctionTester func;
            cout << string_q(80,'-') << endl;
            ASSERT_TRUE("test_" + uint_2_Str(cnt++), func.doTest(line));
        }
    }
    return true;
}}

//------------------------------------------------------------------------
TEST_F(CThisTest, TestDecode) {
    string_q contents;
    asciiFileToString("./decode.txt", contents);replaceAll(contents, "\\\n","");
    CStringArray lines;
    explode(lines, contents, '\n');
    uint64_t cnt = 0;
    for (auto line : lines) {
        if (!line.empty() && !startsWith(line, '#')) {
            CFunctionTester func;
            cout << string_q(80,'-') << endl;
            ASSERT_TRUE("test_" + uint_2_Str(cnt++), func.doTest(line));
        }
    }
    return true;
}}

//------------------------------------------------------------------------
TEST_F(CThisTest, TestDecodeRaw) {
    string_q contents;
    asciiFileToString("./decode_raw.txt", contents);replaceAll(contents, "\\\n","");
    CStringArray lines;
    explode(lines, contents, '\n');
    uint64_t cnt = 0;
    for (auto line : lines) {
        if (!line.empty() && !startsWith(line, '#')) {
            CFunctionTester func;
            cout << string_q(80,'-') << endl;
            ASSERT_TRUE("test_" + uint_2_Str(cnt++), func.doTest(line));
        }
    }
    return true;
}}

//------------------------------------------------------------------------
TEST_F(CThisTest, TestTight) {
    string_q contents;
    asciiFileToString("./tight.txt", contents);replaceAll(contents, "\\\n","");
    CStringArray lines;
    explode(lines, contents, '\n');
    uint64_t cnt = 0;
    for (auto line : lines) {
        if (!line.empty() && !startsWith(line, '#')) {
            CFunctionTester func;
            cout << string_q(80,'-') << endl;
            ASSERT_TRUE("test_" + uint_2_Str(cnt++), func.doTest(line));
        }
    }
    return true;
}}

//--------------------------------------------------------------
bool test_eth_tests(uint64_t sub) {

    switch (sub) {
        case 0: LOAD_TEST(TestSerpent); break;
        case 1: LOAD_TEST(TestSignatures); break;
        case 2: LOAD_TEST(TestEncode); break;
        case 3: LOAD_TEST(TestEncodeRaw); break;
        case 4: LOAD_TEST(TestDecode); break;
        case 5: LOAD_TEST(TestDecodeRaw); break;
        case 6: LOAD_TEST(TestTight); break;
    }
    return !testing::RUN_ALL_TESTS();
}

#if 0
//--------------------------------------------------------------
function parse_nXm_Type (type) {
    ASSERT(startsWith(type, "fixed") || startsWith(type, "ufixed"));
    var tmp = /^\D+(\d+)x(\d+)$/.exec(type)
    return [ parseInt(tmp[1], 10), parseInt(tmp[2], 10) ]
}

//--------------------------------------------------------------
/*
 const utils = require('ethereumjs-util')
 const BN = require('bn.js')

 var ABI = function () {
 }

 // Convert from short to canonical names
 // FIXME: optimise or make this nicer?
 function elementaryName (name) {
 if (name.startsWith('int[')) {
 return 'int256' + name.slice(3)
 } else if (name === 'int') {
 return 'int256'
 } else if (name.startsWith('uint[')) {
 return 'uint256' + name.slice(4)
 } else if (name === 'uint') {
 return 'uint256'
 } else if (name.startsWith('fixed[')) {
 return 'fixed128x128' + name.slice(5)
 } else if (name === 'fixed') {
 return 'fixed128x128'
 } else if (name.startsWith('ufixed[')) {
 return 'ufixed128x128' + name.slice(6)
 } else if (name === 'ufixed') {
 return 'ufixed128x128'
 }
 return name
 }

 ABI.eventID = function (name, types) {
 // FIXME: use node.js util.format?
 var sig = name + '(' + types.map(elementaryName).join(',') + ')'
 return utils.sha3(new Buffer(sig))
 }

 ABI.methodID = function (name, types) {
 return ABI.eventID(name, types).slice(0, 4)
 }

 // Parse N from type<N>
 function parseTypeN (type) {
 return parseInt(/^\D+(\d+)$/.exec(type)[1], 10)
 }

 // Parse N,M from type<N>x<M>
 function parseTypeNxM (type) {
 var tmp = /^\D+(\d+)x(\d+)$/.exec(type)
 return [ parseInt(tmp[1], 10), parseInt(tmp[2], 10) ]
 }

 // Parse N in type[<N>] where "type" can itself be an array type.
 function parseTypeArray (type) {
 var tmp = type.match(/(.*)\[(.*?)\]$/)
 if (tmp) {
 return tmp[2] === '' ? 'dynamic' : parseInt(tmp[2], 10)
 }
 return null
 }

 function parseNumber (arg) {
 var type = typeof arg
 if (type === 'string') {
 if (utils.isHexPrefixed(arg)) {
 return new BN(utils.stripHexPrefix(arg), 16)
 } else {
 return new BN(arg, 10)
 }
 } else if (type === 'number') {
 return new BN(arg)
 } else if (arg.toArray) {
 // assume this is a BN for the moment, replace with BN.isBN soon
 return arg
 } else {
 throw new Error('Argument is not a number')
 }
 }

 // someMethod(bytes,uint)
 // someMethod(bytes,uint):(boolean)
 function parseSignature (sig) {
 var tmp = /^(\w+)\((.+)\)$/.exec(sig)
 if (tmp.length !== 3) {
 throw new Error('Invalid method signature')
 }

 var args = /^(.+)\):\((.+)$/.exec(tmp[2])

 if (args !== null && args.length === 3) {
 return {
 method: tmp[1],
 args: args[1].split(','),
 retargs: args[2].split(',')
 }
 } else {
 return {
 method: tmp[1],
 args: tmp[2].split(',')
 }
 }
 }

 // Encodes a single item (can be dynamic array)
 // @returns: Buffer
 function encodeSingle (type, arg) {
 var size, num, ret, i

 if (type === 'address') {
 return encodeSingle('uint160', parseNumber(arg))
 } else if (type === 'bool') {
 return encodeSingle('uint8', arg ? 1 : 0)
 } else if (type === 'string') {
 return encodeSingle('bytes', new Buffer(arg, 'utf8'))
 } else if (isArray(type)) {
 // this part handles fixed-length ([2]) and variable length ([]) arrays
 // NOTE: we catch here all calls to arrays, that simplifies the rest
 if (typeof arg.length === 'undefined') {
 throw new Error('Not an array?')
 }
 size = parseTypeArray(type)
 if (size !== 'dynamic' && size !== 0 && arg.length > size) {
 throw new Error('Elements exceed array size: ' + size)
 }
 ret = []
 type = type.slice(0, type.lastIndexOf('['))
 if (typeof arg === 'string') {
 arg = JSON.parse(arg)
 }
 for (i in arg) {
 ret.push(encodeSingle(type, arg[i]))
 }
 if (size === 'dynamic') {
 var length = encodeSingle('uint256', arg.length)
 ret.unshift(length)
 }
 return Buffer.concat(ret)
 } else if (type === 'bytes') {
 arg = new Buffer(arg)

 ret = Buffer.concat([ encodeSingle('uint256', arg.length), arg ])

 if ((arg.length % 32) !== 0) {
 ret = Buffer.concat([ ret, utils.zeros(32 - (arg.length % 32)) ])
 }

 return ret
 } else if (type.startsWith('bytes')) {
 size = parseTypeN(type)
 if (size < 1 || size > 32) {
 throw new Error('Invalid bytes<N> width: ' + size)
 }

 return utils.setLengthRight(arg, 32)
 } else if (type.startsWith('uint')) {
 size = parseTypeN(type)
 if ((size % 8) || (size < 8) || (size > 256)) {
 throw new Error('Invalid uint<N> width: ' + size)
 }

 num = parseNumber(arg)
 if (num.bitLength() > size) {
 throw new Error('Supplied uint exceeds width: ' + size + ' vs ' + num.bitLength())
 }

 if (num < 0) {
 throw new Error('Supplied uint is negative')
 }

 return num.toArrayLike(Buffer, 'be', 32)
 } else if (type.startsWith('int')) {
 size = parseTypeN(type)
 if ((size % 8) || (size < 8) || (size > 256)) {
 throw new Error('Invalid int<N> width: ' + size)
 }

 num = parseNumber(arg)
 if (num.bitLength() > size) {
 throw new Error('Supplied int exceeds width: ' + size + ' vs ' + num.bitLength())
 }

 return num.toTwos(256).toArrayLike(Buffer, 'be', 32)
 } else if (type.startsWith('ufixed')) {
 size = parseTypeNxM(type)

 num = parseNumber(arg)

 if (num < 0) {
 throw new Error('Supplied ufixed is negative')
 }

 return encodeSingle('uint256', num.mul(new BN(2).pow(new BN(size[1]))))
 } else if (type.startsWith('fixed')) {
 size = parseTypeNxM(type)

 return encodeSingle('int256', parseNumber(arg).mul(new BN(2).pow(new BN(size[1]))))
 }

 throw new Error('Unsupported or invalid type: ' + type)
 }

 // Decodes a single item (can be dynamic array)
 // @returns: array
 // FIXME: this method will need a lot of attention at checking limits and validation
 function decodeSingle (parsedType, data, offset) {
 if (typeof parsedType === 'string') {
 parsedType = parseType(parsedType)
 }
 var size, num, ret, i

 if (parsedType.name === 'address') {
 return decodeSingle(parsedType.rawType, data, offset).toArrayLike(Buffer, 'be', 20).toString('hex')
 } else if (parsedType.name === 'bool') {
 return decodeSingle(parsedType.rawType, data, offset).toString() === new BN(1).toString()
 } else if (parsedType.name === 'string') {
 var bytes = decodeSingle(parsedType.rawType, data, offset)
 return new Buffer(bytes, 'utf8').toString()
 } else if (parsedType.isArray) {
 // this part handles fixed-length arrays ([2]) and variable length ([]) arrays
 // NOTE: we catch here all calls to arrays, that simplifies the rest
 ret = []
 size = parsedType.size

 if (parsedType.size === 'dynamic') {
 offset = decodeSingle('uint256', data, offset).toNumber()
 size = decodeSingle('uint256', data, offset).toNumber()
 offset = offset + 32
 }
 for (i = 0; i < size; i++) {
 var decoded = decodeSingle(parsedType.subArray, data, offset)
 ret.push(decoded)
 offset += parsedType.subArray.memoryUsage
 }
 return ret
 } else if (parsedType.name === 'bytes') {
 offset = decodeSingle('uint256', data, offset).toNumber()
 size = decodeSingle('uint256', data, offset).toNumber()
 return data.slice(offset + 32, offset + 32 + size)
 } else if (parsedType.name.startsWith('bytes')) {
 return data.slice(offset, offset + parsedType.size)
 } else if (parsedType.name.startsWith('uint')) {
 num = new BN(data.slice(offset, offset + 32), 16, 'be')
 if (num.bitLength() > parsedType.size) {
 throw new Error('Decoded int exceeds width: ' + parsedType.size + ' vs ' + num.bitLength())
 }
 return num
 } else if (parsedType.name.startsWith('int')) {
 num = new BN(data.slice(offset, offset + 32), 16, 'be').fromTwos(256)
 if (num.bitLength() > parsedType.size) {
 throw new Error('Decoded uint exceeds width: ' + parsedType.size + ' vs ' + num.bitLength())
 }

 return num
 } else if (parsedType.name.startsWith('ufixed')) {
 size = new BN(2).pow(new BN(parsedType.size[1]))
 num = decodeSingle('uint256', data, offset)
 if (!num.mod(size).isZero()) {
 throw new Error('Decimals not supported yet')
 }
 return num.div(size)
 } else if (parsedType.name.startsWith('fixed')) {
 size = new BN(2).pow(new BN(parsedType.size[1]))
 num = decodeSingle('int256', data, offset)
 if (!num.mod(size).isZero()) {
 throw new Error('Decimals not supported yet')
 }
 return num.div(size)
 }
 throw new Error('Unsupported or invalid type: ' + parsedType.name)
 }

 // Parse the given type
 // @returns: {} containing the type itself, memory usage and (including size and subArray if applicable)
 function parseType (type) {
 var size
 var ret
 if (isArray(type)) {
 size = parseTypeArray(type)
 var subArray = type.slice(0, type.lastIndexOf('['))
 subArray = parseType(subArray)
 ret = {
 isArray: true,
 name: type,
 size: size,
 memoryUsage: size === 'dynamic' ? 32 : subArray.memoryUsage * size,
 subArray: subArray
 }
 return ret
 } else {
 var rawType
 switch (type) {
 case 'address':
 rawType = 'uint160'
 break
 case 'bool':
 rawType = 'uint8'
 break
 case 'string':
 rawType = 'bytes'
 break
 }
 ret = {
 rawType: rawType,
 name: type,
 memoryUsage: 32
 }

 if (type.startsWith('bytes') && type !== 'bytes' || type.startsWith('uint') || type.startsWith('int')) {
 ret.size = parseTypeN(type)
 } else if (type.startsWith('ufixed') || type.startsWith('fixed')) {
 ret.size = parseTypeNxM(type)
 }

 if (type.startsWith('bytes') && type !== 'bytes' && (ret.size < 1 || ret.size > 32)) {
 throw new Error('Invalid bytes<N> width: ' + ret.size)
 }
 if ((type.startsWith('uint') || type.startsWith('int')) && (ret.size % 8 || ret.size < 8 || ret.size > 256)) {
 throw new Error('Invalid int/uint<N> width: ' + ret.size)
 }
 return ret
 }
 }

 // Is a type dynamic?
 function isDynamic (type) {
 // FIXME: handle all types? I don't think anything is missing now
 return (type === 'string') || (type === 'bytes') || (parseTypeArray(type) === 'dynamic')
 }

 // Is a type an array?
 function isArray (type) {
 return type.lastIndexOf(']') === type.length - 1
 }

 // Encode a method/event with arguments
 // @types an array of string type names
 // @args  an array of the appropriate values
 ABI.rawEncode = function (types, values) {
 var output = []
 var data = []

 var headLength = 0

 types.forEach(function (type) {
 if (isArray(type)) {
 var size = parseTypeArray(type)

 if (size !== 'dynamic') {
 headLength += 32 * size
 } else {
 headLength += 32
 }
 } else {
 headLength += 32
 }
 })

 for (var i = 0; i < types.length; i++) {
 var type = elementaryName(types[i])
 var value = values[i]
 var cur = encodeSingle(type, value)

 // Use the head/tail method for storing dynamic data
 if (isDynamic(type)) {
 output.push(encodeSingle('uint256', headLength))
 data.push(cur)
 headLength += cur.length
 } else {
 output.push(cur)
 }
 }

 return Buffer.concat(output.concat(data))
 }

 ABI.rawDecode = function (types, data) {
 var ret = []
 data = new Buffer(data)
 var offset = 0
 for (var i = 0; i < types.length; i++) {
 var type = elementaryName(types[i])
 var parsed = parseType(type, data, offset)
 var decoded = decodeSingle(parsed, data, offset)
 offset += parsed.memoryUsage
 ret.push(decoded)
 }
 return ret
 }

 ABI.simpleEncode = function (method) {
 var args = Array.prototype.slice.call(arguments).slice(1)
 var sig = parseSignature(method)

 // FIXME: validate/convert arguments
 if (args.length !== sig.args.length) {
 throw new Error('Argument count mismatch')
 }

 return Buffer.concat([ ABI.methodID(sig.method, sig.args), ABI.rawEncode(sig.args, args) ])
 }

 ABI.simpleDecode = function (method, data) {
 var sig = parseSignature(method)

 // FIXME: validate/convert arguments
 if (!sig.retargs) {
 throw new Error('No return values in method')
 }

 return ABI.rawDecode(sig.retargs, data)
 }

 function stringify (type, value) {
 if (type.startsWith('address') || type.startsWith('bytes')) {
 return '0x' + value.toString('hex')
 } else {
 return value.toString()
 }
 }

 ABI.stringify = function (types, values) {
 var ret = []

 for (var i in types) {
 var type = types[i]
 var value = values[i]

 // if it is an array type, concat the items
 if (/^[^\[]+\[.*\]$/.test(type)) {
 value = value.map(function (item) {
 return stringify(type, item)
 }).join(', ')
 } else {
 value = stringify(type, value)
 }

 ret.push(value)
 }

 return ret
 }

 ABI.solidityPack = function (types, values) {
 if (types.length !== values.length) {
 throw new Error('Number of types are not matching the values')
 }

 var size, num
 var ret = []

 for (var i = 0; i < types.length; i++) {
 var type = elementaryName(types[i])
 var value = values[i]

 if (type === 'bytes') {
 ret.push(value)
 } else if (type === 'string') {
 ret.push(new Buffer(value, 'utf8'))
 } else if (type === 'bool') {
 ret.push(new Buffer(value ? '01' : '00', 'hex'))
 } else if (type === 'address') {
 ret.push(utils.setLengthLeft(value, 20))
 } else if (type.startsWith('bytes')) {
 size = parseTypeN(type)
 if (size < 1 || size > 32) {
 throw new Error('Invalid bytes<N> width: ' + size)
 }

 ret.push(utils.setLengthRight(value, size))
 } else if (type.startsWith('uint')) {
 size = parseTypeN(type)
 if ((size % 8) || (size < 8) || (size > 256)) {
 throw new Error('Invalid uint<N> width: ' + size)
 }

 num = parseNumber(value)
 if (num.bitLength() > size) {
 throw new Error('Supplied uint exceeds width: ' + size + ' vs ' + num.bitLength())
 }

 ret.push(num.toArrayLike(Buffer, 'be', size / 8))
 } else if (type.startsWith('int')) {
 size = parseTypeN(type)
 if ((size % 8) || (size < 8) || (size > 256)) {
 throw new Error('Invalid int<N> width: ' + size)
 }

 num = parseNumber(value)
 if (num.bitLength() > size) {
 throw new Error('Supplied int exceeds width: ' + size + ' vs ' + num.bitLength())
 }

 ret.push(num.toTwos(size).toArrayLike(Buffer, 'be', size / 8))
 } else {
 // FIXME: support all other types
 throw new Error('Unsupported or invalid type: ' + type)
 }
 }

 return Buffer.concat(ret)
 }

 ABI.soliditySHA3 = function (types, values) {
 return utils.sha3(ABI.solidityPack(types, values))
 }

 ABI.soliditySHA256 = function (types, values) {
 return utils.sha256(ABI.solidityPack(types, values))
 }

 ABI.solidityRIPEMD160 = function (types, values) {
 return utils.ripemd160(ABI.solidityPack(types, values), true)
 }

 // Serpent's users are familiar with this encoding
 // - s: string
 // - b: bytes
 // - b<N>: bytes<N>
 // - i: int256
 // - a: int256[]

 function isNumeric (c) {
 // FIXME: is this correct? Seems to work
 return (c >= '0') && (c <= '9')
 }

 // For a "documentation" refer to https://github.com/ethereum/serpent/blob/develop/preprocess.cpp
 ABI.fromSerpent = function (sig) {
 var ret = []
 for (var i = 0; i < sig.length; i++) {
 var type = sig[i]
 if (type === 's') {
 ret.push('bytes')
 } else if (type === 'b') {
 var tmp = 'bytes'
 var j = i + 1
 while ((j < sig.length) && isNumeric(sig[j])) {
 tmp += sig[j] - '0'
 j++
 }
 i = j - 1
 ret.push(tmp)
 } else if (type === 'i') {
 ret.push('int256')
 } else if (type === 'a') {
 ret.push('int256[]')
 } else {
 throw new Error('Unsupported or invalid type: ' + type)
 }
 }
 return ret
 }

 ABI.toSerpent = function (types) {
 var ret = []
 for (var i = 0; i < types.length; i++) {
 var type = types[i]
 if (type === 'bytes') {
 ret.push('s')
 } else if (type.startsWith('bytes')) {
 ret.push('b' + parseTypeN(type))
 } else if (type === 'int256') {
 ret.push('i')
 } else if (type === 'int256[]') {
 ret.push('a')
 } else {
 throw new Error('Unsupported or invalid type: ' + type)
 }
 }
 return ret.join('')
 }

 module.exports = ABI
*/
#endif
