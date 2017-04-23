#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include <string.h>
#include <iostream>
#include "basetypes.h"
#include "biguint.h"
#include "bigint.h"

using namespace std;

extern       string    to_string         (const SFUintBN& bu);
extern       string    to_hex            (const SFUintBN& bu);
extern       string    to_string         (const SFIntBN&  bi);

extern       SFUintBN  str2BigUint       (const string& s);
inline       SFUintBN  str2BigUint       (const SFString& s) { string ss = (const char*)s; return str2BigUint(ss); }
extern       SFIntBN   str2BigInt        (const string& s);

extern       SFUintBN  hex2BigUint       (const string& s);
extern       SFIntBN   hex2BigInt        (const string& s);

extern       SFUintBN  exp2BigUint       (const string& s);
extern       SFIntBN   exp2BigInt        (const string& s);

extern       ostream&  operator<<        (ostream& os, const SFUintBN& x);
extern       ostream&  operator<<        (ostream& os, const SFIntBN& x);

extern       SFUintBN  gcd               (SFUintBN a, SFUintBN b);
extern       SFUintBN  modinv            (const SFIntBN& x, const SFUintBN& n);
extern       SFUintBN  modexp            (const SFIntBN& base, const SFUintBN& exponent,const SFUintBN& modulus);

//-------------------------------------------------------------------------
inline SFUintBN canonicalWei(SFUint32 _value)
{
	return SFUintBN(_value);
}
inline SFUintBN canonicalWei(const SFString& _value)
{
        if (_value.Contains( "0x" ))
                return hex2BigUint((const char*) _value.substr(2));
        if (_value.Contains( "e"  ))
                return exp2BigUint((const char*) _value);
        return str2BigUint(_value);
}
inline SFUint32 canonicalWeiL(SFUint32 _value)
{
	return _value;
}
inline SFUint32 canonicalWeiL(const SFString& _value)
{
	return toLong(_value);
}
inline SFString asStringBN(const SFUintBN& bu)
{
	return to_string(bu).c_str();
}
inline SFString asStringBN(const SFIntBN& bn)
{
	return to_string(bn).c_str();
}

