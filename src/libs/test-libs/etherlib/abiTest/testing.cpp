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

    CAbi::registerClass();
    CParameter::registerClass();

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
                cout << "\n";

            } else if (mode == "generation") {
                cout << "Generation test...\n";
                cout << (test_generation() ? "...passed" : "...failed") << "\n";
                cout << "\n";

            } else if (mode == "old_bug") {
                cout << "Old bug test...\n";
                cout << (test_old_bug() ? "...passed" : "...failed") << "\n";
                cout << "\n";

            } else if (mode == "func_assign") {
                cout << "function assignment test...\n";
                cout << (test_func_assign() ? "...passed" : "...failed") << "\n";
                cout << "\n";

            } else if (mode == "evt_assign") {
                cout << "event assignment test...\n";
                cout << (test_evt_assign() ? "...passed" : "...failed") << "\n";
                cout << "\n";
            } else if (mode == "eth_test") {
                cout << "ethereum test box test...\n";
                cout << (test_eth_tests(options.sub) ? "...passed" : "...failed") << "\n";
                cout << "\n";
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
string_q parseTypeN(const string_q& strIn) {
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
string_q type_2_Serp(const string_q& strIn) {

    string_q ret;
    string_q str = strIn;
    CStringArray types;
    explode(types, str, '|');
    for (auto type : types) {
        if (type == "bytes")
            ret += "s";
        else if (startsWith(type, "bytes"))
            ret += "b" + parseTypeN(type);
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
string_q serp_2_Type(const string_q& strIn) {
    CStringArray types;
    for (size_t i = 0 ; i < strIn.length() ; i++) {
        char ch = strIn[i];
        switch (ch) {
            case 's': types.push_back("bytes"); break;
            case 'i': types.push_back("int256"); break;
            case 'a': types.push_back("int256[]"); break;
            case 'b': {
                string_q tmp = "bytes";
                size_t j = i + 1;
                while (j < strIn.length() && isdigit(strIn[j])) {
                    tmp += strIn[j];
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
    bool doTest(const string_q& line) {
        string_q expected, result;
        CStringArray parts;
        explode(parts, line, '|');
        if (parts.size() < 5)
            return false;

        CFunction func;
        func.fromDefinition(parts[2]);
        if (func.type != "function" && func.type != "event") {
            cout << "invalid: "  << parts[2] << endl;
            return false;
        }

        cout << "line: "      << line << endl;
        cout << "testType: "  << parts[0] << endl;
        cout << "testName: "  << parts[1] << endl;
        cout << "signature: " << parts[2] << " : " << func.type << " " << func.getSignature(SIG_CANONICAL) << endl;
        cout << "values: "    << parts[3] << endl;

        if (parts[0] == "encode") {
            expected = parts[4];
            result   = func.encodeItem();
            result   = (func.type == "function" ? result.substr(0,10) : result);
        } else if (parts[0] == "decode") {
            expected = parts[4];
            string_q types[256];
            size_t nTypes = 0;
            for (auto param : func.inputs)
                types[nTypes++] = param.type;
            result = substitute(decodeRLP(func.name, parts[3], nTypes, types), "\"", "");
            nextTokenClear(result,',');
            result = trim(result);
        } else {
            return false;
        }
        cout << "expected: "  << expected << " : " << result << endl;
        return (parts[0] == "decode") && (expected == result);
    };
};

//------------------------------------------------------------------------
TEST_F(CThisTest, TestEncoding) {
    string_q contents;
    asciiFileToString("./encode.txt", contents);
    CStringArray lines;
    explode(lines, contents, '\n');
    uint64_t cnt = 0;
    for (auto line : lines) {
        if (!startsWith(line, ";")) {
            CFunctionTester func;
            ASSERT_TRUE("test_" + uint_2_Str(cnt++), func.doTest(line));
        }
    }
    return true;
}}

//------------------------------------------------------------------------
TEST_F(CThisTest, TestEncodingRaw) {
    string_q contents;
    asciiFileToString("./encode_raw.txt", contents);
    CStringArray lines;
    explode(lines, contents, '\n');
    uint64_t cnt = 0;
    for (auto line : lines) {
        if (!startsWith(line, ";")) {
            CFunctionTester func;
            ASSERT_TRUE("test_" + uint_2_Str(cnt++), func.doTest(line));
        }
    }
    return true;
}}

//------------------------------------------------------------------------
TEST_F(CThisTest, TestDecoding) {
    string_q contents;
    asciiFileToString("./decode.txt", contents);
    CStringArray lines;
    explode(lines, contents, '\n');
    uint64_t cnt = 0;
    for (auto line : lines) {
        if (!startsWith(line, ";")) {
            CFunctionTester func;
            ASSERT_TRUE("test_" + uint_2_Str(cnt++), func.doTest(line));
        }
    }
    return true;
}}

//------------------------------------------------------------------------
TEST_F(CThisTest, TestDecodingRaw) {
    string_q contents;
    asciiFileToString("./decode_raw.txt", contents);
    CStringArray lines;
    explode(lines, contents, '\n');
    uint64_t cnt = 0;
    for (auto line : lines) {
        if (!startsWith(line, ";")) {
            CFunctionTester func;
            ASSERT_TRUE("test_" + uint_2_Str(cnt++), func.doTest(line));
        }
    }
    return true;
}}

//--------------------------------------------------------------
bool test_eth_tests(uint64_t sub) {

    switch (sub) {
        case 0: LOAD_TEST(TestSerpent); break;
        case 1: LOAD_TEST(TestEncoding); break;
        case 2: LOAD_TEST(TestEncodingRaw); break;
        case 3: LOAD_TEST(TestDecoding); break;
        case 4: LOAD_TEST(TestDecodingRaw); break;
    }
    return !testing::RUN_ALL_TESTS();
}

#if 0
// Homebrew decoding tests

describe('decoding uint256[]', function () {
    it('should equal', function () {
        var a = abi.rawDecode([ 'uint256[]' ], new Buffer('00000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000003000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000003', 'hex'))
        var b = new BN(1)
        var c = new BN(2)
        var d = new BN(3)

        assert.equal(a.length, 1)
        assert.equal(a[0].length, 3)
        assert.equal(a[0][0].toString(), b.toString())
        assert.equal(a[0][1].toString(), c.toString())
        assert.equal(a[0][2].toString(), d.toString())
    })
})

describe('decoding fixed-array', function () {
    it('uint[3]', function () {
        var a = abi.rawDecode([ 'uint[3]' ], new Buffer('000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000003', 'hex'))
        assert.equal(a.length, 1)
        assert.equal(a[0].length, 3)
        assert.equal(a[0][0].toString(10), 1)
        assert.equal(a[0][1].toString(10), 2)
        assert.equal(a[0][2].toString(10), 3)
    })
})

describe('decoding (uint[2], uint)', function () {
    it('should work', function () {
        var a = abi.rawDecode([ 'uint[2]', 'uint' ], new Buffer('0000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000005c0000000000000000000000000000000000000000000000000000000000000003', 'hex'))
        assert.equal(a.length, 2)
        assert.equal(a[0].length, 2)
        assert.equal(a[0][0].toString(10), 1)
        assert.equal(a[0][1].toString(10), 92)
        assert.equal(a[1].toString(10), 3)
    })
})

/* FIXME: should check that the whole input buffer was consumed
 describe('decoding uint[2] with [1,2,3]', function () {
 it('should fail', function () {
 assert.throws(function () {
 abi.rawDecode([ 'uint[2]' ], new Buffer('00000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000003000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000003', 'hex'))
 }, Error)
 })
 })
 */

describe('stringify', function () {
    it('should be hex prefixed for address', function () {
        assert.deepEqual(abi.stringify([ 'address' ], [ new BN('1234', 16) ]), [ '0x1234' ])
    })

    it('should be hex prefixed for bytes', function () {
        assert.deepEqual(abi.stringify([ 'bytes' ], [ new Buffer('1234', 'hex') ]), [ '0x1234' ])
    })

    it('should be hex prefixed for bytesN', function () {
        assert.deepEqual(abi.stringify([ 'bytes32' ], [ new Buffer('1234', 'hex') ]), [ '0x1234' ])
    })

    it('should be a number for uint', function () {
        assert.deepEqual(abi.stringify([ 'uint' ], [ 42 ]), [ '42' ])
    })

    it('should be a number for uintN', function () {
        assert.deepEqual(abi.stringify([ 'uint8' ], [ 42 ]), [ '42' ])
    })

    it('should be a number for int', function () {
        assert.deepEqual(abi.stringify([ 'int' ], [ -42 ]), [ '-42' ])
    })

    it('should be a number for intN', function () {
        assert.deepEqual(abi.stringify([ 'int8' ], [ -42 ]), [ '-42' ])
    })

    it('should work for bool (true)', function () {
        assert.deepEqual(abi.stringify([ 'bool' ], [ true ]), [ 'true' ])
    })

    it('should work for bool (false)', function () {
        assert.deepEqual(abi.stringify([ 'bool' ], [ false ]), [ 'false' ])
    })

    it('should work for address[]', function () {
        assert.deepEqual(abi.stringify([ 'address[]' ], [ [ new BN('1234', 16), new BN('5678', 16) ] ]), [ '0x1234, 0x5678' ])
    })

    it('should work for address[2]', function () {
        assert.deepEqual(abi.stringify([ 'address[2]' ], [ [ new BN('1234', 16), new BN('5678', 16) ] ]), [ '0x1234, 0x5678' ])
    })

    it('should work for bytes[]', function () {
        assert.deepEqual(abi.stringify([ 'bytes[]' ], [ [ new Buffer('1234', 'hex'), new Buffer('5678', 'hex') ] ]), [ '0x1234, 0x5678' ])
    })

    it('should work for bytes[2]', function () {
        assert.deepEqual(abi.stringify([ 'bytes[2]' ], [ [ new Buffer('1234', 'hex'), new Buffer('5678', 'hex') ] ]), [ '0x1234, 0x5678' ])
    })

    it('should work for uint[]', function () {
        assert.deepEqual(abi.stringify([ 'uint[]' ], [ [ 1, 2, 3 ] ]), [ '1, 2, 3' ])
    })

    it('should work for uint[3]', function () {
        assert.deepEqual(abi.stringify([ 'uint[3]' ], [ [ 1, 2, 3 ] ]), [ '1, 2, 3' ])
    })

    it('should work for int[]', function () {
        assert.deepEqual(abi.stringify([ 'int[]' ], [ [ -1, -2, -3 ] ]), [ '-1, -2, -3' ])
    })

    it('should work for int[3]', function () {
        assert.deepEqual(abi.stringify([ 'int[3]' ], [ [ -1, -2, -3 ] ]), [ '-1, -2, -3' ])
    })

    it('should work for multiple entries', function () {
        assert.deepEqual(abi.stringify([ 'bool', 'bool' ], [ true, true ]), [ 'true', 'true' ])
    })
})

// Tests for Solidity's tight packing
describe('solidity tight packing bool', function () {
    it('should equal', function () {
        var a = abi.solidityPack(
                                 [ 'bool' ],
                                 [ true ]
                                 )
        var b = '01'
        assert.equal(a.toString('hex'), b.toString('hex'))

        a = abi.solidityPack(
                             [ 'bool' ],
                             [ false ]
                             )
        b = '00'
        assert.equal(a.toString('hex'), b.toString('hex'))
    })
})

describe('solidity tight packing address', function () {
    it('should equal', function () {
        var a = abi.solidityPack(
                                 [ 'address' ],
                                 [ new BN('43989fb883ba8111221e89123897538475893837', 16) ]
                                 )
        var b = '43989fb883ba8111221e89123897538475893837'
        assert.equal(a.toString('hex'), b.toString('hex'))
    })
})

describe('solidity tight packing string', function () {
    it('should equal', function () {
        var a = abi.solidityPack(
                                 [ 'string' ],
                                 [ 'test' ]
                                 )
        var b = '74657374'
        assert.equal(a.toString('hex'), b.toString('hex'))
    })
})

describe('solidity tight packing bytes', function () {
    it('should equal', function () {
        var a = abi.solidityPack(
                                 [ 'bytes' ],
                                 [ new Buffer('123456', 'hex') ]
                                 )
        var b = '123456'
        assert.equal(a.toString('hex'), b.toString('hex'))
    })
})

describe('solidity tight packing bytes8', function () {
    it('should equal', function () {
        var a = abi.solidityPack(
                                 [ 'bytes8' ],
                                 [ new Buffer('123456', 'hex') ]
                                 )
        var b = '1234560000000000'
        assert.equal(a.toString('hex'), b.toString('hex'))
    })
})

describe('solidity tight packing uint', function () {
    it('should equal', function () {
        var a = abi.solidityPack(
                                 [ 'uint' ],
                                 [ 42 ]
                                 )
        var b = '000000000000000000000000000000000000000000000000000000000000002a'
        assert.equal(a.toString('hex'), b.toString('hex'))
    })
})

describe('solidity tight packing uint16', function () {
    it('should equal', function () {
        var a = abi.solidityPack(
                                 [ 'uint16' ],
                                 [ 42 ]
                                 )
        var b = '002a'
        assert.equal(a.toString('hex'), b.toString('hex'))
    })
})

describe('solidity tight packing int', function () {
    it('should equal', function () {
        var a = abi.solidityPack(
                                 [ 'int' ],
                                 [ -42 ]
                                 )
        var b = 'ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffd6'
        assert.equal(a.toString('hex'), b.toString('hex'))
    })
})

describe('solidity tight packing int16', function () {
    it('should equal', function () {
        var a = abi.solidityPack(
                                 [ 'int16' ],
                                 [ -42 ]
                                 )
        var b = 'ffd6'
        assert.equal(a.toString('hex'), b.toString('hex'))
    })
})

describe('solidity tight packing multiple arguments', function () {
    it('should equal', function () {
        var a = abi.solidityPack(
                                 [ 'bytes32', 'uint32', 'uint32', 'uint32', 'uint32' ],
                                 [ new Buffer('123456', 'hex'), 6, 7, 8, 9 ]
                                 )
        var b = '123456000000000000000000000000000000000000000000000000000000000000000006000000070000000800000009'
        assert.equal(a.toString('hex'), b.toString('hex'))
    })
})

describe('solidity tight packing sha3', function () {
    it('should equal', function () {
        var a = abi.soliditySHA3(
                                 [ 'address', 'address', 'uint', 'uint' ],
                                 [ new BN('43989fb883ba8111221e89123897538475893837', 16), 0, 10000, 1448075779 ]
                                 )
        var b = 'c3ab5ca31a013757f26a88561f0ff5057a97dfcc33f43d6b479abc3ac2d1d595'
        assert.equal(a.toString('hex'), b.toString('hex'))
    })
})

describe('solidity tight packing sha3 #2', function () {
    it('should equal', function () {
        var a = abi.soliditySHA3(
                                 [ 'bytes32', 'uint32', 'uint32', 'uint32', 'uint32' ],
                                 [ new Buffer('123456', 'hex'), 6, 7, 8, 9 ]
                                 )
        var b = '1f2eedb6c2ac3e4b4e4c9f7598e626baf1e15a4e848d295479f46ec85d967cba'
        assert.equal(a.toString('hex'), b.toString('hex'))
    })
})

describe('solidity tight packing sha256', function () {
    it('should equal', function () {
        var a = abi.soliditySHA256(
                                   [ 'address', 'address', 'uint', 'uint' ],
                                   [ new BN('43989fb883ba8111221e89123897538475893837', 16), 0, 10000, 1448075779 ]
                                   )
        var b = '344d8cb0711672efbdfe991f35943847c1058e1ecf515ff63ad936b91fd16231'
        assert.equal(a.toString('hex'), b.toString('hex'))
    })
})

describe('solidity tight packing ripemd160', function () {
    it('should equal', function () {
        var a = abi.solidityRIPEMD160(
                                      [ 'address', 'address', 'uint', 'uint' ],
                                      [ new BN('43989fb883ba8111221e89123897538475893837', 16), 0, 10000, 1448075779 ]
                                      )
        var b = '000000000000000000000000a398cc72490f72048efa52c4e92067e8499672e7'
        assert.equal(a.toString('hex'), b.toString('hex'))
    })
})

describe('solidity tight packing with small ints', function () {
    it('should equal', function () {
        var a = abi.soliditySHA3(
                                 [ 'address', 'address', 'int64', 'uint192' ],
                                 [ new BN('43989fb883ba8111221e89123897538475893837', 16), 0, 10000, 1448075779 ]
                                 )
        var b = '1c34bbd3d419c05d028a9f13a81a1212e33cb21f4b96ce1310442911c62c6986'
        assert.equal(a.toString('hex'), b.toString('hex'))
    })
})

describe('utf8 handling', function () {
    it('should encode latin and extensions', function () {
        var a = abi.rawEncode([ 'string' ], [ 'ethereum számítógép' ]).toString('hex')
        var b = '00000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000017657468657265756d20737ac3a16dc3ad74c3b367c3a970000000000000000000'
        assert.equal(a, b)
    })
    it('should encode non-latin characters', function () {
        var a = abi.rawEncode([ 'string' ], [ '为什么那么认真？' ]).toString('hex')
        var b = '00000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000018e4b8bae4bb80e4b988e982a3e4b988e8aea4e79c9fefbc9f0000000000000000'
        assert.equal(a, b)
    })
    it('should decode latin and extensions', function () {
        var a = 'ethereum számítógép'
        var b = abi.rawDecode([ 'string' ], new Buffer('00000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000017657468657265756d20737ac3a16dc3ad74c3b367c3a970000000000000000000', 'hex'))
        assert.equal(a, b[0])
    })
    it('should decode non-latin characters', function () {
        var a = '为什么那么认真？'
        var b = abi.rawDecode([ 'string' ], new Buffer('00000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000018e4b8bae4bb80e4b988e982a3e4b988e8aea4e79c9fefbc9f0000000000000000', 'hex'))
        assert.equal(a, b[0])
    })
})

describe('encoding ufixed128x128', function () {
    it('should equal', function () {
        var a = abi.rawEncode([ 'ufixed128x128' ], [ 1 ]).toString('hex')
        var b = '0000000000000000000000000000000100000000000000000000000000000000'
        assert.equal(a, b)
    })
})

describe('encoding fixed128x128', function () {
    it('should equal', function () {
        var a = abi.rawEncode([ 'fixed128x128' ], [ -1 ]).toString('hex')
        var b = 'ffffffffffffffffffffffffffffffff00000000000000000000000000000000'
        assert.equal(a, b)
    })
})

describe('decoding ufixed128x128', function () {
    it('should equal', function () {
        var a = new Buffer('0000000000000000000000000000000100000000000000000000000000000000', 'hex')
        var b = abi.rawDecode([ 'ufixed128x128' ], a)
        assert.equal(b[0].toNumber(), 1)
    })
    it('decimals should fail', function () {
        var a = new Buffer('0000000000000000000000000000000100000000000000000000000000000001', 'hex')
        assert.throws(function () {
            abi.rawDecode([ 'ufixed128x128' ], a)
        }, /^Error: Decimals not supported yet/)
    })
})

describe('decoding fixed128x128', function () {
    it('should equal', function () {
        var a = new Buffer('ffffffffffffffffffffffffffffffff00000000000000000000000000000000', 'hex')
        var b = abi.rawDecode([ 'fixed128x128' ], a)
        assert.equal(b[0].toNumber(), -1)
    })
    it('decimals should fail', function () {
        var a = new Buffer('ffffffffffffffffffffffffffffffff00000000000000000000000000000001', 'hex')
        assert.throws(function () {
            abi.rawDecode([ 'fixed128x128' ], a)
        }, /^Error: Decimals not supported yet/)
    })
})

describe('encoding -1 as uint', function () {
    it('should throw', function () {
        assert.throws(function () {
            abi.rawEncode([ 'uint' ], [ -1 ])
        }, /^Error: Supplied uint is negative/)
    })
})

describe('encoding 256 bits as bytes', function () {
    it('should not leave trailing zeroes', function () {
        var a = abi.rawEncode([ 'bytes' ], [ new Buffer('ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff', 'hex') ])
        assert.equal(a.toString('hex'), '00000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000020ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff')
    })
})

describe('decoding (uint128[2][3], uint)', function () {
    it('should work', function () {
        var data = `0000000000000000000000000000000000000000000000000000000000000001
        0000000000000000000000000000000000000000000000000000000000000002
        0000000000000000000000000000000000000000000000000000000000000003
        0000000000000000000000000000000000000000000000000000000000000004
        0000000000000000000000000000000000000000000000000000000000000005
        0000000000000000000000000000000000000000000000000000000000000006
        000000000000000000000000000000000000000000000000000000000000000a`
        var a = abi.rawDecode([ 'uint128[2][3]', 'uint' ], new Buffer(data.replace(/\n/g, ''), 'hex'))
        assert.equal(a.length, 2)
        assert.equal(a[0][0][0].toString(10), 1)
        assert.equal(a[0][0][1].toString(10), 2)
        assert.equal(a[0][1][0].toString(10), 3)
        assert.equal(a[0][1][1].toString(10), 4)
        assert.equal(a[0][2][0].toString(10), 5)
        assert.equal(a[0][2][1].toString(10), 6)
        assert.equal(a[1].toString(10), 10)
    })
})

describe('decoding (uint128[2][3][2], uint)', function () {
    it('should work', function () {
        var data = `0000000000000000000000000000000000000000000000000000000000000001
        0000000000000000000000000000000000000000000000000000000000000002
        0000000000000000000000000000000000000000000000000000000000000003
        0000000000000000000000000000000000000000000000000000000000000004
        0000000000000000000000000000000000000000000000000000000000000005
        0000000000000000000000000000000000000000000000000000000000000006
        0000000000000000000000000000000000000000000000000000000000000001
        0000000000000000000000000000000000000000000000000000000000000002
        0000000000000000000000000000000000000000000000000000000000000003
        0000000000000000000000000000000000000000000000000000000000000004
        0000000000000000000000000000000000000000000000000000000000000005
        0000000000000000000000000000000000000000000000000000000000000006
        000000000000000000000000000000000000000000000000000000000000000a`
        var a = abi.rawDecode([ 'uint128[2][3][2]', 'uint' ], new Buffer(data.replace(/\n/g, ''), 'hex'))
        assert.equal(a.length, 2)
        assert.equal(a[0][0][0][0].toString(10), 1)
        assert.equal(a[0][0][0][1].toString(10), 2)
        assert.equal(a[0][0][1][0].toString(10), 3)
        assert.equal(a[0][0][1][1].toString(10), 4)
        assert.equal(a[0][0][2][0].toString(10), 5)
        assert.equal(a[0][0][2][1].toString(10), 6)
        assert.equal(a[0][1][0][0].toString(10), 1)
        assert.equal(a[0][1][0][1].toString(10), 2)
        assert.equal(a[0][1][1][0].toString(10), 3)
        assert.equal(a[0][1][1][1].toString(10), 4)
        assert.equal(a[0][1][2][0].toString(10), 5)
        assert.equal(a[0][1][2][1].toString(10), 6)
        assert.equal(a[1].toString(10), 10)
    })
})

describe('decoding (uint[3][], uint)', function () {
    it('should work', function () {
        var data = `0000000000000000000000000000000000000000000000000000000000000040
        000000000000000000000000000000000000000000000000000000000000000a
        0000000000000000000000000000000000000000000000000000000000000002
        0000000000000000000000000000000000000000000000000000000000000001
        0000000000000000000000000000000000000000000000000000000000000002
        0000000000000000000000000000000000000000000000000000000000000003
        0000000000000000000000000000000000000000000000000000000000000004
        0000000000000000000000000000000000000000000000000000000000000005
        0000000000000000000000000000000000000000000000000000000000000006`
        var a = abi.rawDecode([ 'uint[3][]', 'uint' ], new Buffer(data.replace(/\n/g, ''), 'hex'))
        assert.equal(a.length, 2)
        assert.equal(a[0][0][0].toString(10), 1)
        assert.equal(a[0][0][1].toString(10), 2)
        assert.equal(a[0][0][2].toString(10), 3)
        assert.equal(a[0][1][0].toString(10), 4)
        assert.equal(a[0][1][1].toString(10), 5)
        assert.equal(a[0][1][2].toString(10), 6)
        assert.equal(a[1].toString(10), 10)
    })
})

describe('decoding (uint[][3], uint)', function () {
    it('should work', function () {
        var data = `0000000000000000000000000000000000000000000000000000000000000080
        00000000000000000000000000000000000000000000000000000000000000e0
        0000000000000000000000000000000000000000000000000000000000000140
        000000000000000000000000000000000000000000000000000000000000000a
        0000000000000000000000000000000000000000000000000000000000000002
        0000000000000000000000000000000000000000000000000000000000000001
        0000000000000000000000000000000000000000000000000000000000000002
        0000000000000000000000000000000000000000000000000000000000000002
        0000000000000000000000000000000000000000000000000000000000000003
        0000000000000000000000000000000000000000000000000000000000000004
        0000000000000000000000000000000000000000000000000000000000000002
        0000000000000000000000000000000000000000000000000000000000000005
        0000000000000000000000000000000000000000000000000000000000000006`
        var a = abi.rawDecode([ 'uint[][3]', 'uint' ], new Buffer(data.replace(/\n/g, ''), 'hex'))
        assert.equal(a.length, 2)
        assert.equal(a[1].toString(10), 10)
        assert.equal(a[0][0][0].toString(10), 1)
        assert.equal(a[0][0][1].toString(10), 2)
        assert.equal(a[0][1][0].toString(10), 3)
        assert.equal(a[0][1][1].toString(10), 4)
        assert.equal(a[0][2][0].toString(10), 5)
        assert.equal(a[0][2][1].toString(10), 6)
    })
})
describe('encoding bytes33', function () {
    it('should fail', function () {
        assert.throws(function () {
            abi.rawEncode('fail', [ 'bytes33' ], [ '' ])
        }, Error)
    })
})

describe('encoding uint0', function () {
    it('should fail', function () {
        assert.throws(function () {
            abi.rawEncode('fail', [ 'uint0' ], [ 1 ])
        }, Error)
    })
})

describe('encoding uint257', function () {
    it('should fail', function () {
        assert.throws(function () {
            abi.rawEncode('fail', [ 'uint257' ], [ 1 ])
        }, Error)
    })
})

describe('encoding int0', function () {
    it('should fail', function () {
        assert.throws(function () {
            abi.rawEncode([ 'int0' ], [ 1 ])
        }, Error)
    })
})

describe('encoding int257', function () {
    it('should fail', function () {
        assert.throws(function () {
            abi.rawEncode([ 'int257' ], [ 1 ])
        }, Error)
    })
})

describe('encoding uint[2] with [1,2,3]', function () {
    it('should fail', function () {
        assert.throws(function () {
            abi.rawEncode([ 'uint[2]' ], [ [ 1, 2, 3 ] ])
        }, Error)
    })
})

describe('encoding uint8 with 9bit data', function () {
    it('should fail', function () {
        assert.throws(function () {
            abi.rawEncode([ 'uint8' ], [ new BN(1).iushln(9) ])
        }, Error)
    })
})
#endif
#if 0
const utils = require('ethereumjs-util')
const BN = require('bn.js')

var ABI = function () {
}

ABI.eventID = function (name, types) {
    // FIXME: use node.js util.format?
    var sig = name + '(' + types.map(elementaryName).join(',') + ')'
    return utils.sha3(new Buffer(sig))
}

ABI.methodID = function (name, types) {
    return ABI.eventID(name, types).slice(0, 4)
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

                                         module.exports = ABI


#endif

