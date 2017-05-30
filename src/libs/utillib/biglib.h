#pragma once
/*-------------------------------------------------------------------------
 * Derived from https://mattmccutchen.net/bigint/index.html
 * From the website under the Legal section: I, Matt McCutchen, the sole author
 * of the original Big Integer Library, waive my copyright to it, placing it
 * in the public domain.  The library comes with absolutely no warranty.
 *------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "basetypes.h"
#include "biguint.h"
#include "bigint.h"

namespace qblocks {

    extern string to_string(const SFUintBN& bu);
    extern string to_hex(const SFUintBN& bu);
    extern string to_string(const SFIntBN&  bi);

    extern SFUintBN str2BigUint(const string& s);
    inline SFUintBN str2BigUint(const SFString& s) { string ss = (const char*)s; return str2BigUint(ss); }
    extern SFIntBN str2BigInt(const string& s);

    extern SFUintBN hex2BigUint(const string& s);
    extern SFIntBN hex2BigInt(const string& s);

    extern SFUintBN exp2BigUint(const string& s);
    extern SFIntBN exp2BigInt(const string& s);

    extern ostream& operator<<(ostream& os, const SFUintBN& x);
    extern ostream& operator<<(ostream& os, const SFIntBN& x);

    extern SFUintBN gcd(SFUintBN a, SFUintBN b);
    extern SFUintBN modinv(const SFIntBN& x, const SFUintBN& n);
    extern SFUintBN modexp(const SFIntBN& base, const SFUintBN& exponent,const SFUintBN& modulus);

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
        extern SFUint32 toLong(const SFString& in);
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

    typedef SFArrayBase<SFUintBN> SFBigUintArray;
    typedef SFArrayBase<SFIntBN> SFBigIntArray;

}  // namespace qblocks

using namespace qblocks;

