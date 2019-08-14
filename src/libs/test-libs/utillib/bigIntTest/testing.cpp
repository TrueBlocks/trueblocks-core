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
#include <string>
#include <iostream>
#include <sstream>
#include "utillib.h"

//----------------------------------------------------------------------
#define TEST(expr, exp) do { \
    string s1, e1; \
    try { \
        s1 = bnu_2_Str((expr)); \
    } catch (const char *err) { \
        s1 = "error"; \
        e1 = err; \
    } \
    cout << "Test " << testID++; \
    cout << " [--" << #expr << "--]"; \
    cout << ": " << s1 << e1 << " expected: " << (exp) << " " << ((s1 != exp) ? redX : greenCheck) << endl; \
} while (0)

//----------------------------------------------------------------------
#define TEST2(expr, exp) do { \
    string s1, e1; \
    try { \
        s1 = bni_2_Str((expr)); \
    } catch (const char *err) { \
        s1 = "error"; \
        e1 = err; \
    } \
    cout << "Test " << testID++; \
    cout << " [--" << #expr << "--]"; \
    cout << ": " << s1 << e1 << " expected: " << (exp) << " " << ((s1 != exp) ? redX : greenCheck) << endl; \
} while (0)

//----------------------------------------------------------------------
extern const biguint_t& check(const biguint_t& x);
extern const bigint_t&  check(const bigint_t& x);

//----------------------------------------------------------------------
int main(int argc, const char *argv[]) {
//    etherlib_init(quickQuitHandler);

    CDefaultOptions options;
    options.minArgs = 0;
    options.prepareArguments(argc, argv);

extern void contentTest(void);
extern void performanceTest(void);
    if (argc == 1)
        contentTest();
    else
        performanceTest();

    return 0;
}

void performanceTest(void) {
#define XXXX 100000
    cout << "\t\t" << TIC();
    biguint_t bn;
    for (size_t i = 0 ; i < XXXX ; i++) {
        bn = str_2_BigUint("0x10000000000000000000000000000000000000000000000000000000000000001",256);
        if (!(i%20)) { cerr << "\r" << i; cerr.flush(); }
    }
    cout << "\t\t\t\t" << TIC() << "\t\tstr_2_BigUint" << endl;

    cout << "\t\t" << TIC();
    for (size_t i = 0 ; i < XXXX ; i++) {
        string_q unused = bnu_2_Str(bn);
        if (!(i%20)) { cerr << "\r" << i << " " << (unused == "3"); cerr.flush(); }
    }
    cout << "\t\t\t\t" << TIC() << "\t\tbnu_2_Str" << endl;

    cout << "\t\t" << TIC();
    bigint_t bi;
    for (size_t i = 0 ; i < XXXX ; i++) {
        bi = str_2_BigInt("0x10000000000000000000000000000000000000000000000000000000000000001",256);
        if (!(i%20)) { cerr << "\r" << i; cerr.flush(); }
    }
    cout << "\t\t\t\t" << TIC() << "\tstr_2_BigInt" << endl;

    cout << "\t\t" << TIC();
    for (size_t i = 0 ; i < XXXX ; i++) {
        string_q unused = bni_2_Str(bi);
        if (!(i%20)) { cerr << "\r" << i << " " << (unused == "3"); cerr.flush(); }
    }
    cout << "\t\t\t\t" << TIC() << "\t\tbni_2_Str" << endl;

    //    cout << (str_2_BigInt ("0x10000000000000000000000000000000000000000000000000000000000000001",256)) << endl;
//    bn = str_2_BigUint("0x12345678901234567890");
}

void contentTest(void) {
    try {
        short    pathologicalShort = (short)~((unsigned short)(~0) >> 1);  // NOLINT
        int      pathologicalInt   = (int)~((unsigned int)(~0) >> 1);  // NOLINT
        uint64_t pathologicalLong  = (uint64_t)~((uint64_t)(~0) >> 1);
        uint64_t testID = 0;

        bigint_t a;
        int b = 535;
        bigint_t c(a);
        bigint_t d(-314159265);
//        bigint_t e(3141592653589793238462643383279); doesn't compile because there is no literal that big -- okay!
        bigint_t f;
        bigint_t g(314159), h(265);
        biguint_t i(0xFF0000FF), j(0x0000FFFF);
        string s("3141592653589793238462643383279"), s2;
        string results[] = {
            "1", "314", "98596", "30959144", "9721171216", "3052447761824", "958468597212736",
            "300959139524799104", "94501169810786918656", "29673367320587092457984", "9317437338664347031806976"
        };
        biguint_t x(1), big314(314);
        int power = 0;
        biguint_t z(0), one(1), ten(10);
        biguint_t coreDump = biguint_t(BigUnsignedInABase("de0b6b3a7640000", 16));

        uint64_t myBlocks[3];
        myBlocks[0] = 3;
        myBlocks[1] = 4;
        myBlocks[2] = 0;
        biguint_t bu(myBlocks, 3);

        uint64_t myZeroBlocks[1];
        myZeroBlocks[0] = 0;

        biguint_t aa = check(biguint_t(314159265) * 358979323);
        biguint_t bb;
        biguint_t bbb(314);
        biguint_t bbbb(25);

        biguint_t ss1 = str_2_BigUint(string_q("314159265358979323"));
        biguint_t ss2 = str_2_BigUint(string_q("314159265358979324"));
        biguint_t ss3 = str_2_BigUint(string_q("4294967296"));
        biguint_t ss4 = str_2_BigUint(string_q("4294967298"));

        biguint_t p1 = biguint_t(3) * 5;
        bigint_t p2 = bigint_t(biguint_t(3)) * -5;
        bigint_t num(3), denom(5), quotient;
        bigint_t zero(0), three(3), ans;
        bigint_t fff=4; fff *= 3;
        biguint_t aaaaa(0), bbbbb(3);
        biguint_t four(4), eight(8), sixteen(16);

        TEST2( a,                                                          "0"                 );
        a = b;
        TEST2( a,                                                          "535"               );
        b = a.to_int();
        f = str_2_BigInt(s);
        s2 = bni_2_Str(f);
        TEST2( f,                                                          "3141592653589793238462643383279");
        f = biguint_t(BigUnsignedInABase("1142b0090b6460000", 16));
        TEST2( f,                                                          "19900000000000000000");
        TEST2( g+h,                                                        "314424"            );
        TEST2( g-h,                                                        "313894"            );
        TEST2( g*h,                                                        "83252135"          );
        TEST2( g/h,                                                        "1185"              );
        TEST2( g%h,                                                        "134"               );
//       i=0xFF0000FF
//       j=0x0000FFFF;
// i & j = 0x0000FFFF - true if both are true
        TEST( i&j,                                                        "255"               );
// i | j = 0xFF00FF00 - true if one is true and the other is false
        TEST( i|j,                                                        "4278255615"        );
// i ^ j = 0xFF00FFFF - true if either is true
        TEST( i^j,                                                        "4278255360"        );
        TEST( j << 21,                                                    "137436856320"      );
        TEST( j >> 10,                                                    "63"                );
        TEST( x,                                   results[power]      ); x *= big314 ; power++;
        TEST( x,                                   results[power]      ); x *= big314 ; power++;
        TEST( x,                                   results[power]      ); x *= big314 ; power++;
        TEST( x,                                   results[power]      ); x *= big314 ; power++;
        TEST( x,                                   results[power]      ); x *= big314 ; power++;
        TEST( x,                                   results[power]      ); x *= big314 ; power++;
        TEST( x,                                   results[power]      ); x *= big314 ; power++;
        TEST( x,                                   results[power]      ); x *= big314 ; power++;
        TEST( x,                                   results[power]      ); x *= big314 ; power++;
        TEST( x,                                   results[power]      ); x *= big314 ; power++;
        TEST( x,                                   results[power]      ); x *= big314 ; power++;
        TEST( gcd(biguint_t(60), 72),                                      "12"                );
        TEST( modinv(biguint_t(7), 11),                                    "8"                 );
        TEST( modexp(biguint_t(314), 159, 2653),                           "1931"              );
        TEST( z,                                                          "0"                 );
        TEST( one,                                                        "1"                 );
        TEST( ten,                                                        "10"                );
        TEST( check( biguint_t() ),                                        "0"                 );
        TEST2( check( bigint_t () ),                                        "0"                 );
        TEST( check( bu ),                                                "17179869187"       );
        TEST2( check( bigint_t(myBlocks, 3) ),                              "-7179869187"       );
        TEST2( check( bigint_t(bu) ),                                       "error"             );
        TEST2( check( bigint_t(myBlocks, 3, 1) ),                           "17179869187"       );
        TEST2( check( bigint_t(myBlocks, 3, -1) ),                          "-17179869187"      );
        TEST2( check( bigint_t(myBlocks, 3, 0    ) ),                       "error"             );
        TEST2( check( bigint_t(bu, 1) ),                                    "17179869187"       );
        TEST2( check( bigint_t(bu, -1) ),                                   "-17179869187"      );
        TEST2( check( bigint_t(bu, 0    ) ),                                "error"             );
        TEST2( check( bigint_t(myZeroBlocks, 1, 1) ),                       "0"                 );
        TEST2( check( bigint_t(myZeroBlocks, 1, -1) ),                      "0"                 );
        TEST2( check( bigint_t(myZeroBlocks, 1, 0) ),                       "0"                 );
        TEST( biguint_t   (0).to_ulong(),                                  "0"                 );
        TEST( biguint_t   (4294967295U).to_ulong(),                        "4294967295"        );
        TEST( str_2_BigUint (string_q("4294967296")).to_ulong(),            "error"             );
        TEST( biguint_t   (0).to_long(),                                   "0"                 );
        TEST( biguint_t   (2147483647).to_long(),                          "2147483647"        );
        TEST( biguint_t   (2147483648U).to_long(),                         "error"             );
        TEST( biguint_t   (0).to_uint(),                                   "0"                 );
        TEST( biguint_t   (4294967295U).to_uint(),                         "4294967295"        );
        TEST( str_2_BigUint (string_q("4294967296")).to_uint(),             "error"             );
        TEST( biguint_t   (0).to_int(),                                    "0"                 );
        TEST( biguint_t   (2147483647).to_int(),                           "2147483647"        );
        TEST( biguint_t   (2147483648U).to_int(),                          "error"             );
        TEST( biguint_t   (0).to_uint(),                                 "0"                 );
        TEST( biguint_t   (65535).to_uint(),                             "65535"             );
        TEST( biguint_t   (65536).to_uint(),                             "error"             );
        TEST( biguint_t   (0).to_int(),                                  "0"                 );
        TEST( biguint_t   (32767).to_int(),                              "32767"             );
        TEST( biguint_t   (32768).to_int(),                              "error"             );
        TEST2( bigint_t    (-1).to_ulong(),                                 "error"             );
        TEST2( bigint_t    (0).to_ulong(),                                  "0"                 );
        TEST2( bigint_t    (4294967295U).to_ulong(),                        "4294967295"        );
        TEST2( str_2_BigInt  ("4294967296").to_ulong(),                      "error"             );
        TEST2( str_2_BigInt  ("-2147483649").to_long(),                      "error"             );
        TEST2( str_2_BigInt  ("-2147483648").to_long(),                      "-2147483648"       );
        TEST2( bigint_t    (-2147483647).to_long(),                         "-2147483647"       );
        TEST2( bigint_t    (0).to_long(),                                   "0"                 );
        TEST2( bigint_t    (2147483647).to_long(),                          "2147483647"        );
        TEST2( bigint_t    (2147483648U).to_long(),                         "error"             );
        TEST2( bigint_t    (-1).to_uint(),                                  "error"             );
        TEST2( bigint_t    (0).to_uint(),                                   "0"                 );
        TEST2( bigint_t    (4294967295U).to_uint(),                         "4294967295"        );
        TEST2( str_2_BigInt  ("4294967296").to_uint(),                       "error"             );
        TEST2( str_2_BigInt  ("-2147483649").to_int(),                       "error"             );
        TEST2( str_2_BigInt  ("-2147483645").to_int(),                       "-2147483645"       );
        TEST2( bigint_t    (-2147483647).to_int(),                          "-2147483647"       );
        TEST2( bigint_t    (0).to_int(),                                    "0"                 );
        TEST2( bigint_t    (2147483647).to_int(),                           "2147483647"        );
        TEST2( bigint_t    (2147483648U).to_int(),                          "error"             );
        TEST2( bigint_t    (-1).to_uint(),                                "error"             );
        TEST2( bigint_t    (0).to_uint(),                                 "0"                 );
        TEST2( bigint_t    (65535).to_uint(),                             "65535"             );
        TEST2( bigint_t    (65536).to_uint(),                             "error"             );
        TEST2( bigint_t    (-32769).to_int(),                             "error"             );
        TEST2( bigint_t    (-32768).to_int(),                             "-32768"            );
        TEST2( bigint_t    (-32767).to_int(),                             "-32767"            );
        TEST2( bigint_t    (0).to_int(),                                  "0"                 );
        TEST2( bigint_t    (32767).to_int(),                              "32767"             );
        TEST2( bigint_t    (32768).to_int(),                              "error"             );
        TEST( biguint_t   (int16_t(-1)),                                     "error"           );
        TEST( biguint_t   (pathologicalShort),                             "error"             );
        TEST( biguint_t   (-1),                                            "error"             );
        TEST( biguint_t   (pathologicalInt),                               "error"             );
        TEST( biguint_t   (uint64_t(-1)),                                  "18446744073709551615");
        TEST( biguint_t   (pathologicalLong),                              "9223372036854775808");
        TEST( biguint_t   (5) - biguint_t(6),                               "error"             );
        TEST( ss1 - ss2,                                                  "error"             );
        TEST( check( biguint_t(5)-biguint_t(5) ),                           "0"                 );
        TEST( check( ss1 - ss2 ),                                         "0"                 );
        TEST( check( ss3 - biguint_t(1) ),                                 "4294967295"        );
        TEST( check( biguint_t(0) + 0 ),                                   "0"                 );
        TEST( check( biguint_t(0) + 1 ),                                   "1"                 );
        TEST( check( str_2_BigUint(string_q("8589934591")) + ss4 ),         "12884901889"       );
        TEST( check( biguint_t(0xFFFFFFFFU) + 1 ),                         "4294967296"        );
        TEST( check( biguint_t(1) - 0 ),                                   "1"                 );
        TEST( check( biguint_t(1) - 1 ),                                   "0"                 );
        TEST( check( biguint_t(2) - 1 ),                                   "1"                 );
        TEST( check( str_2_BigUint(string_q("12884901889")) - ss4),         "8589934591"        );
        TEST( check( ss3 - 1 ),                                           "4294967295"        );
        TEST( aa,                                                         "112776680263877595");
        TEST( aa / 123,                                                   "916883579381118"   );
        TEST( aa % 123,                                                   "81"                );
        TEST( biguint_t(5) / 0,                                            "error"             );
        TEST( bb,                                                         "0"                 );
        TEST( bb.getBlock(0),                                             "0"                 ); bb.setBlock(1, 314);
        TEST( check(bb),                                                  "1348619730944"     );
        TEST( bb.len,                                                     "2"                 );
        TEST( bb.getBlock(0),                                             "0"                 );
        TEST( bb.getBlock(1),                                             "314"               ); bb.setBlock(1, 0);
        TEST( check(bb),                                                  "0"                 ); bbb.setBlock(1, 159);
        TEST( bbb.getBlock(0),                                            "314"               );
        TEST( bbb.getBlock(1),                                            "159"               ); bbb.add(1, 2);
        TEST( bbb.getBlock(0),                                            "3"                 );
        TEST( bbb.getBlock(1),                                            "0"                 );
        TEST( bbb.getBlock(2),                                            "0"                 );
        TEST( bbb.getBlock(314159),                                       "0"                 );
        TEST( biguint_t(0).bitLength(),                                    "0"                 );
        TEST( biguint_t(1).bitLength(),                                    "1"                 );
        TEST( biguint_t(4095).bitLength(),                                 "12"                );
        TEST( biguint_t(4096).bitLength(),                                 "13"                );
        TEST( str_2_BigUint(string_q("5000000000")).bitLength(),            "33"                );
        TEST( bbbb.getBit(4),                                             "1"                 );
        TEST( bbbb.getBit(3),                                             "1"                 );
        TEST( bbbb.getBit(2),                                             "0"                 );
        TEST( bbbb.getBit(1),                                             "0"                 );
        TEST( bbbb.getBit(0),                                             "1"                 );
        TEST( bbbb.bitLength(),                                           "5"                 ); bbbb.setBit(32, true);  // NOLINT
        TEST( bbbb,                                                       "4294967321"        ); bbb.setBit(31, true);bbb.setBit(32, false);  // NOLINT
        TEST( check(bbbb) ,                                               "2147483673"        );
        TEST( p1,                                                         "15"                );
        TEST2( p2,                                                         "-15"               );
        TEST2( (num.divide(denom, quotient), check(quotient)),             "2"                 ); num = 5;
        TEST2( (num.divide(denom, quotient), check(quotient)),             "0"                 ); ans = zero - three;
        TEST2( check(ans).sign,                                            "-1"                );
        TEST2( check(fff),                                                 "12"                ); ans = aaaaa ^ bbbbb;
        TEST2( ans,                                                        "3"                 ); num = 5; num *= num;
        TEST2( check(num),                                                 "25"                );
        TEST( biguint_t(0xf),                                              "15"                );
        TEST( one << 2,                                                   "4"                 );
        TEST( (one << 2) >> 2,                                            "1"                 );
        TEST( (one<<255),                                                 "57896044618658097711785492504343953926634992332820282019728792003956564819968"     );  // NOLINT
        TEST( four,                                                       "4"                 );
        TEST( eight,                                                      "8"                 );
        TEST( sixteen,                                                    "16"                );
        TEST( four|eight,                                                 "12"                );
        TEST( four|sixteen|3,                                             "23"                );
        TEST( four&eight,                                                 "0"                 );
        TEST2( bigint_t(four)-sixteen,                                      "-12"               );
        TEST( (four&sixteen)|four,                                        "4"                 );
        TEST( sixteen%9,                                                  "7"                 );
        TEST( coreDump,                                                   "1000000000000000000");
        TEST( modexp(10,
                     9,
                     uint64_t(10000000000)
                     ) *
              modexp(10,
                     9,
                     uint64_t(10000000000)), "1000000000000000000");

#if 1
        cout << "18446744073709551615" << ": ";
        cout << (str_2_Uint("0xffffffffffffffffffffffffffffffff")) << endl;
        cout << "1" << ": ";
        cout << (str_2_Uint("0xffffffffffffffffffffffffffffffff")+2) << endl;
        cout << "115792089237316195423570985008687907853269984665640564039457584007913129639935" << ": ";
        cout << (str_2_BigUint("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff")) << endl;
        cout << "1" << ": ";
        cout << (str_2_BigUint("0x10000000000000000000000000000000000000000000000000000000000000001",256)) << endl;

        cout << "-1" << ": ";
        cout << (str_2_Int ("0xffffffffffffffffffffffffffffffff")) << endl;
        cout << "1" << ": ";
        cout << (str_2_Int ("0xffffffffffffffffffffffffffffffff")+2) << endl;
        cout << "-1" << ": ";
        cout << (str_2_BigInt ("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff")) << endl;
        cout << "1" << ": ";
        cout << (str_2_BigInt ("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff")+2) << endl;
        cout << (str_2_BigInt ("0x10000000000000000000000000000000000000000000000000000000000000001",256)) << endl;
#endif

        cout << endl;
        bool done = false;
        // leading zeros
        string_q v = "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff2";
        while (!done) {
            size_t bits = 256 - (countOf(v, '0') * 4);
            string_q type = "int" + uint_2_Str(bits);

            string_q v1 = substitute(v, "00", "ff");
            string_q v2 = substitute(v, "00", "");

            cout << setw(7) << ("u"+type) << ": " << setw(64) << v << ": " << str_2_BigUint("0x" + v) << endl;
            cout << setw(7) << ("u"+type) << ": " << setw(64) << v1 << ": " << str_2_BigUint("0x" + v1, bits) << endl;
            cout << setw(7) << ("u"+type) << ": " << setw(64) << v2 << ": " << str_2_BigUint("0x" + v2) << endl;

            cout << setw(7) << type << ": " << setw(64) << v << ": " << str_2_BigInt ("0x" + v)        << endl;
            cout << setw(7) << type << ": " << setw(64) << v1 << ": " << str_2_BigInt ("0x" + v1, bits) << endl;
            cout << setw(7) << type << ": " << setw(64) << v2 << ": " << str_2_BigInt ("0x" + v2)       << endl;

            done = (v == "00000000000000000000000000000000000000000000000000000000000000f2");
            replace(v, "ff", "00");
            cout << endl;
        }

    } catch (char const* err) {
        cout << "The library threw an exception: " << err << endl;
    }
}

const biguint_t &check(const biguint_t &x) {
    try {
        unsigned int l = x.len;
        if (l != 0 && x.getBlock(l-1) == 0)
            cout << "check: Unzapped number of size " << l << " " << x << endl;
        if (l > x.capacity)
            cout << "check: Capacity inconsistent with length!" << endl;

    } catch (const char *err) {
        cout << "error";
    }
    return x;
}

const bigint_t &check(const bigint_t &x) {
    try {
        if (x.sign == 0 && x.getMagnitude().len != 0)
            cout << "check: Sign should not be zero!" << endl;
        if (x.sign != 0 && x.getMagnitude().len == 0)
            cout << "check: Sign should be zero!" << endl;
        check(x.getMagnitude());
    } catch (const char *err) {
        cout << "error";
    }
    return x;
}
