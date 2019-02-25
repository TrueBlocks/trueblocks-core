#include <stdio.h>
#include "etherlib.h"
#include "tests.h"

//-----------------------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    COptions(void) {}
    bool parseArguments(string_q& command) { return true; };
    void Init(void) { minArgs = 0; };
};

//-----------------------------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init(quickQuitHandler);

    // We want to get the latestBlock prior to turning on --prove for example
    COptions options;
    options.minArgs = 0;
    if (!options.prepareArguments(argc, argv))
        return 0;

    padTest();
    hexUtilTest();
    decodeTest();

    return 0;
}

//-----------------------------------------------------------------------------------------
void decodeTest(void) {
    cout << cTeal << "decodeTest2:" << cOff << endl;
    string_q contents;
    asciiFileToString("./decode.txt", contents);
    contents = substitute(contents, "\\\n", "");
    contents = substitute(contents, "\n\n", "\n");
    CStringArray tests;
    explode(tests, contents, '\n');
    for (auto testcase: tests) {
        if (!startsWith(testcase, '#') && !startsWith(testcase, ';')) {
            if (verbose > 1)
                cout << endl << testcase.substr(0,200) << endl;
            CStringArray parts;
            explode(parts, testcase, '|');
            T test;
            test.desc = parts[2];
            test.input = parts[3];
            test.expected = parts[4];
            cout << test.check(decode(test.desc, test.input));
        } else if (startsWith(testcase, "#end"))
            return;
    }
}

//-----------------------------------------------------------------------------------------
string_q padBytes(const string_q& hexStr, bool toLeft){
    if (toLeft) return toLower(padLeft (hexStr, 64, '0'));
    else        return toLower(padRight(hexStr, 64, '0'));
}

//-----------------------------------------------------------------------------------------
void padTest() {
    if (getEnvStr("NO_COLOR") == "true")
        colorsOff();

    T tests[] = {
        T("padLeft",  true,  "0F49DEA",  "0000000000000000000000000000000000000000000000000000000000f49dea"),
        T("padRight", false, "DEADBEEF", "deadbeef00000000000000000000000000000000000000000000000000000000")
    };
    size_t nTs = sizeof(tests) / sizeof(T);

    cout << cTeal << "padTest:" << cOff << endl;
    for (size_t i = 0 ; i < nTs ; i++)
        cout << tests[i].check(padBytes(tests[i].input, tests[i].bVal));
}

//-----------------------------------------------------------------------------------------
void hexUtilTest() {
    cout << cTeal << "hexUtilTest:" << cOff << endl;

    T tests[] = {
        T("signed int 1", "0xff", "-1"),
        T("signed int 2", "0x00", "0"),
        T("signed int 3", "0x7f", "127"),
        T("signed int 4", "0x80", "-128"),
        T("signed int 5", "0xffffffffffffffffffffffffffffffff", "-1"),
        T("signed int 6", "0xfffffffffffffffffffffffffffffffe", "-2"),
        T("signed int 7", "0x7fffffffffffffffffffffffffffffff", "170141183460469231731687303715884105727"),
        T("signed int 8", "0x80000000000000000000000000000000", "-170141183460469231731687303715884105728"),
    };
    for (size_t i = 0 ; i < 8 ; i++)
        cout << tests[i].check(bni_2_Str(str_2_BigInt(tests[i].input)));

    T test;

    test.desc     = "bytes32 to signed int";
    test.input    = "0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff";
    test.expected = "-1";
    cout << test.check(bni_2_Str(str_2_BigInt(test.input)));

    test.desc     = "bytes32 to signed int";
    test.input    = "0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe";
    test.expected = "-2";
    cout << test.check(bni_2_Str(str_2_BigInt(test.input)));

    test.desc     = "bytes32 to signed int";
    test.input    = "0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff";
    test.expected = "57896044618658097711785492504343953926634992332820282019728792003956564819967";
    cout << test.check(bni_2_Str(str_2_BigInt(test.input)));

    test.desc     = "bytes32 to signed int";
    test.input    = "0x8000000000000000000000000000000000000000000000000000000000000000";
    test.expected = "-57896044618658097711785492504343953926634992332820282019728792003956564819968";
    cout << test.check(bni_2_Str(str_2_BigInt(test.input)));

    test.desc     = "bytes32 to unsigned int";
    test.input    = "0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe";
    test.expected = "115792089237316195423570985008687907853269984665640564039457584007913129639934";
    cout << test.check(bnu_2_Str(str_2_BigUint(test.input)));

    test.desc     = "bytes32 to unsigned int";
    test.input    = "0x0000000000000000000000000000000000000000000000000000000000000020";
    test.expected = "32";
    cout << test.check(bnu_2_Str(str_2_BigUint(test.input)));

    test.desc     = "bytes32 to bool";
    test.input    = "0x0000000000000000000000000000000000000000000000000000000000000001";
    test.expected = bool_2_Str(true);
    cout << test.check(bool_2_Str(str_2_BigUint(test.input).to_ulong()));

    test.desc     = "bytes32 to string";
    test.input    = "68656c6c6f20776f726c64000000000000000000000000000000000000000000"; size_t nBytes = 11;
    test.expected = "hello world";
    cout << test.check(hex_2_Str(test.input, nBytes));

    string_q tooBig = "115792089237316195423570985008687907853269984665640564039457584007913129639936";
    string_q max256 = "115792089237316195423570985008687907853269984665640564039457584007913129639935";
    test.desc     = "max 256 bit unsigned bigint";
    test.input    = tooBig;
    test.expected = "1";
    cout << test.check(bnu_2_Str(str_2_BigUint(test.input, 256)));

    test.desc     = "max 256 bit signed bigint";
    test.input    = max256;
    test.expected = max256;
    cout << test.check(bni_2_Str(str_2_BigInt(test.input, 256)));
}
