#include <stdio.h>
#include "etherlib.h"
#include "tests.h"

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
        cout << tests[i].check(padBytes(tests[i].input, tests[i].bVal)) << endl;
}

extern bigint_t str_2_BigInt2(const string_q& hexIn);
//-----------------------------------------------------------------------------------------
void hexUtilTest() {
    cout << cTeal << "hexUtilTest:" << cOff << endl;

    T test;

    test.desc     = "bytes32 to signed int";
    test.input    = "0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe";
    test.expected = "-2";
    cout << test.check(bni_2_Str(str_2_BigInt2(test.input))) << endl;

    test.desc     = "bytes32 to unsigned int";
    test.input    = "0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe";
    test.expected = "115792089237316195423570985008687907853269984665640564039457584007913129639934";
    cout << test.check(bnu_2_Str(str_2_BigUint(test.input))) << endl;

    test.desc     = "bytes32 to unsigned int";
    test.input    = "0x0000000000000000000000000000000000000000000000000000000000000020";
    test.expected = "32";
    cout << test.check(bnu_2_Str(str_2_BigUint(test.input))) << endl;

    test.desc     = "bytes32 to bool";
    test.input    = "0x0000000000000000000000000000000000000000000000000000000000000001";
    test.expected = bool_2_Str(true);
    cout << test.check(bool_2_Str(str_2_BigUint(test.input).to_ulong())) << endl;

    test.desc     = "bytes32 to string";
    test.input    = "68656c6c6f20776f726c64000000000000000000000000000000000000000000"; size_t nBytes = 11;
    test.expected = "hello world";
    cout << test.check(hex_2_Str(test.input, nBytes)) << endl;
}
