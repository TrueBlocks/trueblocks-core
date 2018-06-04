#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks(tm) - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
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

using namespace std;

//----------------------------------------------------------------------
#define TEST(expr,exp) do {\
	string s1,e1;\
	try {\
		s1 = to_string((expr));\
	} catch (const char *err) {\
		s1 = "error";\
		e1 = err;\
	}\
	cout << "Test " << testID++ << ": " << s1 << e1 << " expected: " << (exp) << " " << ((s1!=exp)?redX:greenCheck) << endl;\
} while (0)

//----------------------------------------------------------------------
extern const SFUintBN& check(const SFUintBN& x);
extern const SFIntBN&  check(const SFIntBN& x);

//----------------------------------------------------------------------
int main(int argc, const char *argv[])
{
    CDefaultOptions options;
    options.minArgs = 0;
    options.prepareArguments(argc, argv);

	try
	{
		short pathologicalShort = (short)~((unsigned short)(~0) >> 1);
		int   pathologicalInt   = (int)~((unsigned int)(~0) >> 1);
		long  pathologicalLong  = (long)~((unsigned long)(~0) >> 1);
		unsigned long testID = 0;

		SFIntBN a;
		int b = 535;
		SFIntBN c(a);
		SFIntBN d(-314159265);
//		SFIntBN e(3141592653589793238462643383279); // doesn't compile because there is no literal that big -- okay!
		SFIntBN f;
		SFIntBN g(314159), h(265);
		SFUintBN i(0xFF0000FF), j(0x0000FFFF);
		string s("3141592653589793238462643383279"),s2;
		string results[] = {
			"1","314","98596","30959144","9721171216","3052447761824","958468597212736",
			"300959139524799104","94501169810786918656","29673367320587092457984","9317437338664347031806976"
		};
		SFUintBN x(1), big314(314);int power=0;
		SFUintBN z(0), one(1), ten(10);
		SFUintBN coreDump = hex2BigUint("de0b6b3a7640000");

		uint64_t myBlocks[3];
		myBlocks[0] = 3;
		myBlocks[1] = 4;
		myBlocks[2] = 0;
		SFUintBN bu(myBlocks, 3);

		uint64_t myZeroBlocks[1];
		myZeroBlocks[0] = 0;

		SFUintBN aa = check(SFUintBN(314159265) * 358979323);
		SFUintBN bb;
		SFUintBN bbb(314);
		SFUintBN bbbb(25);

		SFUintBN ss1 = str2BigUint(SFString("314159265358979323"));
		SFUintBN ss2 = str2BigUint(SFString("314159265358979324"));
		SFUintBN ss3 = str2BigUint(SFString("4294967296"));
		SFUintBN ss4 = str2BigUint(SFString("4294967298"));

		SFUintBN p1 = SFUintBN(3) * 5;
		SFIntBN p2 = SFIntBN(SFUintBN(3)) * -5;
		SFIntBN num(3), denom(5), quotient;
		SFIntBN zero(0), three(3), ans;
		SFIntBN fff=4; fff *= 3;
		SFUintBN aaaaa(0), bbbbb(3);
		SFUintBN four(4),eight(8),sixteen(16);

		TEST( a,                                                          "0"                 );
		a = b;
		TEST( a,                                                          "535"               );
		b=a.to_int();
		f=str2BigInt(s);
		s2=to_string(f);
		TEST( f,                                                          "3141592653589793238462643383279");
		s="1142b0090b6460000";f=hex2BigUint(s);
		TEST( f,                                                          "19900000000000000000");
		TEST( g+h,                                                        "314424"            );
		TEST( g-h,                                                        "313894"            );
		TEST( g*h,                                                        "83252135"          );
		TEST( g/h,                                                        "1185"              );
		TEST( g%h,                                                        "134"               );
//       i=0xFF0000FF
//       j=0x0000FFFF;
// i & j = 0x0000FFFF - true if both are true
		TEST( i&j,                                                        "255"               );
// i | j = 0xFF00FF00 - true if one is true and the other is false
		TEST( i|j,                                                        "4278255615"        );
// i ^ j = 0xFF00FFFF - true if either is true
		TEST( i^j,                                                        "4278255360"        );
		TEST( j<<21,                                                      "137436856320"      );
		TEST( j>>10,                                                      "63"                );
		TEST( x,                                                          results[power]      ); x *= big314 ; power++;
		TEST( x,                                                          results[power]      ); x *= big314 ; power++;
		TEST( x,                                                          results[power]      ); x *= big314 ; power++;
		TEST( x,                                                          results[power]      ); x *= big314 ; power++;
		TEST( x,                                                          results[power]      ); x *= big314 ; power++;
		TEST( x,                                                          results[power]      ); x *= big314 ; power++;
		TEST( x,                                                          results[power]      ); x *= big314 ; power++;
		TEST( x,                                                          results[power]      ); x *= big314 ; power++;
		TEST( x,                                                          results[power]      ); x *= big314 ; power++;
		TEST( x,                                                          results[power]      ); x *= big314 ; power++;
		TEST( x,                                                          results[power]      ); x *= big314 ; power++;
		TEST( gcd(SFUintBN(60), 72),                                      "12"                );
		TEST( modinv(SFUintBN(7), 11),                                    "8"                 );
		TEST( modexp(SFUintBN(314), 159, 2653),                           "1931"              );
		TEST( z,                                                          "0"                 );
		TEST( one,                                                        "1"                 );
		TEST( ten,                                                        "10"                );
		TEST( check( SFUintBN() ),                                        "0"                 );
		TEST( check( SFIntBN () ),                                        "0"                 );
		TEST( check( bu ),                                                "17179869187"       );
		TEST( check( SFIntBN(myBlocks, 3) ),                              "-7179869187"       );
		TEST( check( SFIntBN(bu) ),                                       "error"             );
		TEST( check( SFIntBN(myBlocks, 3, 1) ),                           "17179869187"       );
		TEST( check( SFIntBN(myBlocks, 3, -1) ),                          "-17179869187"      );
		TEST( check( SFIntBN(myBlocks, 3, 0    ) ),                       "error"             );
		TEST( check( SFIntBN(bu, 1) ),                                    "17179869187"       );
		TEST( check( SFIntBN(bu, -1) ),                                   "-17179869187"      );
		TEST( check( SFIntBN(bu, 0    ) ),                                "error"             );
		TEST( check( SFIntBN(myZeroBlocks, 1, 1) ),                       "0"                 );
		TEST( check( SFIntBN(myZeroBlocks, 1, -1) ),                      "0"                 );
		TEST( check( SFIntBN(myZeroBlocks, 1, 0) ),                       "0"                 );
		TEST( SFUintBN   (0).to_ulong(),                                  "0"                 );
		TEST( SFUintBN   (4294967295U).to_ulong(),                        "4294967295"        );
		TEST( str2BigUint (SFString("4294967296")).to_ulong(),            "error"             );
		TEST( SFUintBN   (0).to_long(),                                   "0"                 );
		TEST( SFUintBN   (2147483647).to_long(),                          "2147483647"        );
		TEST( SFUintBN   (2147483648U).to_long(),                         "error"             );
		TEST( SFUintBN   (0).to_uint(),                                   "0"                 );
		TEST( SFUintBN   (4294967295U).to_uint(),                         "4294967295"        );
		TEST( str2BigUint (SFString("4294967296")).to_uint(),             "error"             );
		TEST( SFUintBN   (0).to_int(),                                    "0"                 );
		TEST( SFUintBN   (2147483647).to_int(),                           "2147483647"        );
		TEST( SFUintBN   (2147483648U).to_int(),                          "error"             );
		TEST( SFUintBN   (0).to_ushort(),                                 "0"                 );
		TEST( SFUintBN   (65535).to_ushort(),                             "65535"             );
		TEST( SFUintBN   (65536).to_ushort(),                             "error"             );
		TEST( SFUintBN   (0).to_short(),                                  "0"                 );
		TEST( SFUintBN   (32767).to_short(),                              "32767"             );
		TEST( SFUintBN   (32768).to_short(),                              "error"             );
		TEST( SFIntBN    (-1).to_ulong(),                                 "error"             );
		TEST( SFIntBN    (0).to_ulong(),                                  "0"                 );
		TEST( SFIntBN    (4294967295U).to_ulong(),                        "4294967295"        );
		TEST( str2BigInt  ("4294967296").to_ulong(),                      "error"             );
		TEST( str2BigInt  ("-2147483649").to_long(),                      "error"             );
		TEST( str2BigInt  ("-2147483648").to_long(),                      "-2147483648"       );
		TEST( SFIntBN    (-2147483647).to_long(),                         "-2147483647"       );
		TEST( SFIntBN    (0).to_long(),                                   "0"                 );
		TEST( SFIntBN    (2147483647).to_long(),                          "2147483647"        );
		TEST( SFIntBN    (2147483648U).to_long(),                         "error"             );
		TEST( SFIntBN    (-1).to_uint(),                                  "error"             );
		TEST( SFIntBN    (0).to_uint(),                                   "0"                 );
		TEST( SFIntBN    (4294967295U).to_uint(),                         "4294967295"        );
		TEST( str2BigInt  ("4294967296").to_uint(),                       "error"             );
		TEST( str2BigInt  ("-2147483649").to_int(),                       "error"             );
		TEST( str2BigInt  ("-2147483645").to_int(),                       "-2147483645"       );
		TEST( SFIntBN    (-2147483647).to_int(),                          "-2147483647"       );
		TEST( SFIntBN    (0).to_int(),                                    "0"                 );
		TEST( SFIntBN    (2147483647).to_int(),                           "2147483647"        );
		TEST( SFIntBN    (2147483648U).to_int(),                          "error"             );
		TEST( SFIntBN    (-1).to_ushort(),                                "error"             );
		TEST( SFIntBN    (0).to_ushort(),                                 "0"                 );
		TEST( SFIntBN    (65535).to_ushort(),                             "65535"             );
		TEST( SFIntBN    (65536).to_ushort(),                             "error"             );
		TEST( SFIntBN    (-32769).to_short(),                             "error"             );
		TEST( SFIntBN    (-32768).to_short(),                             "-32768"            );
		TEST( SFIntBN    (-32767).to_short(),                             "-32767"            );
		TEST( SFIntBN    (0).to_short(),                                  "0"                 );
		TEST( SFIntBN    (32767).to_short(),                              "32767"             );
		TEST( SFIntBN    (32768).to_short(),                              "error"             );
		TEST( SFUintBN   (short(-1)),                                     "error"             );
		TEST( SFUintBN   (pathologicalShort),                             "error"             );
		TEST( SFUintBN   (-1),                                            "error"             );
		TEST( SFUintBN   (pathologicalInt),                               "error"             );
		TEST( SFUintBN   (long(-1)),                                      "error"             );
		TEST( SFUintBN   (pathologicalLong),                              "error"             );
		TEST( SFUintBN   (5) - SFUintBN(6),                               "error"             );
		TEST( ss1 - ss2,                                                  "error"             );
		TEST( check( SFUintBN(5)-SFUintBN(5) ),                           "0"                 );
		TEST( check( ss1 - ss2 ),                                         "0"                 );
		TEST( check( ss3 - SFUintBN(1) ),                                 "4294967295"        );
		TEST( check( SFUintBN(0) + 0 ),                                   "0"                 );
		TEST( check( SFUintBN(0) + 1 ),                                   "1"                 );
		TEST( check( str2BigUint(SFString("8589934591")) + ss4 ),         "12884901889"       );
		TEST( check( SFUintBN(0xFFFFFFFFU) + 1 ),                         "4294967296"        );
		TEST( check( SFUintBN(1) - 0 ),                                   "1"                 );
		TEST( check( SFUintBN(1) - 1 ),                                   "0"                 );
		TEST( check( SFUintBN(2) - 1 ),                                   "1"                 );
		TEST( check( str2BigUint(SFString("12884901889")) - ss4),         "8589934591"        );
		TEST( check( ss3 - 1 ),                                           "4294967295"        );
		TEST( aa,                                                         "112776680263877595");
		TEST( aa / 123,                                                   "916883579381118"   );
		TEST( aa % 123,                                                   "81"                );
		TEST( SFUintBN(5) / 0,                                            "error"             );
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
		TEST( SFUintBN(0).bitLength(),                                    "0"                 );
		TEST( SFUintBN(1).bitLength(),                                    "1"                 );
		TEST( SFUintBN(4095).bitLength(),                                 "12"                );
		TEST( SFUintBN(4096).bitLength(),                                 "13"                );
		TEST( str2BigUint(SFString("5000000000")).bitLength(),            "33"                );
		TEST( bbbb.getBit(4),                                             "1"                 );
		TEST( bbbb.getBit(3),                                             "1"                 );
		TEST( bbbb.getBit(2),                                             "0"                 );
		TEST( bbbb.getBit(1),                                             "0"                 );
		TEST( bbbb.getBit(0),                                             "1"                 );
		TEST( bbbb.bitLength(),                                           "5"                 ); bbbb.setBit(32, true);
		TEST( bbbb,                                                       "4294967321"        ); bbb.setBit(31, true);bbb.setBit(32, false);
		TEST( check(bbbb) ,                                               "2147483673"        );
		TEST( p1,                                                         "15"                );
		TEST( p2,                                                         "-15"               );
		TEST( (num.divide(denom,quotient),check(quotient) ),              "2"                 ); num = 5;
		TEST( (num.divide(denom,quotient),check(quotient)),               "0"                 ); ans = zero - three;
		TEST( check(ans).sign,                                            "-1"                );
		TEST( check(fff),                                                 "12"                ); ans = aaaaa ^ bbbbb;
		TEST( ans,                                                        "3"                 ); num = 5; num *= num;
		TEST( check(num),                                                 "25"                );
		TEST( SFUintBN(0xf),                                              "15"                );
		TEST( one<<2,                                                     "4"                 );
		TEST( (one<<2)>>2,                                                "1"                 );
		TEST( (one<<255),                                                 "57896044618658097711785492504343953926634992332820282019728792003956564819968"     );
		TEST( four,                                                       "4"                 );
		TEST( eight,                                                      "8"                 );
		TEST( sixteen,                                                    "16"                );
		TEST( four|eight,                                                 "12"                );
		TEST( four|sixteen|3,                                             "23"                );
		TEST( four&eight,                                                 "0"                 );
		TEST( SFIntBN(four)-sixteen,                                      "-12"               );
		TEST( (four&sixteen)|four,                                        "4"                 );
		TEST( sixteen%9,                                                  "7"                 );
		TEST( coreDump,                                                   "1000000000000000000");
		TEST( modexp(10,9,10000000000)*modexp(10,9,10000000000),          "1000000000000000000");

	} catch(char const* err)
	{
		cout << "The library threw an exception: " << err << endl;
	}

	return 0;
}

const SFUintBN &check(const SFUintBN &x)
{
	try
	{
		unsigned int l = x.len;
		if (l != 0 && x.getBlock(l-1) == 0)
			cout << "check: Unzapped number of size " << l << " " << x << endl;
		if (l > x.capacity)
			cout << "check: Capacity inconsistent with length!" << endl;

	} catch (const char *err)
	{
		cout << "error";
	}
	return x;
}

const SFIntBN &check(const SFIntBN &x)
{
	try
	{
		if (x.sign == 0 && x.getMagnitude().len!=0)
			cout << "check: Sign should not be zero!" << endl;
		if (x.sign != 0 && x.getMagnitude().len==0)
			cout << "check: Sign should be zero!" << endl;
		check(x.getMagnitude());
	} catch (const char *err)
	{
		cout << "error";
	}
	return x;
}
