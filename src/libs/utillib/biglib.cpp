/*-------------------------------------------------------------------------
 * Derived from https://mattmccutchen.net/bigint/index.html
 * From the website under the Legal section: I, Matt McCutchen, the sole author
 * of the original Big Integer Library, waive my copyright to it, placing it
 * in the public domain.  The library comes with absolutely no warranty.
 *------------------------------------------------------------------------*/
#include <string>
#include "basetypes.h"
#include "biglib.h"
#include "conversions.h"
#include "performance.h"

/*
 * A BigUnsignedInABase object represents a nonnegative integer of size limited
 * only by available memory, represented in a user-specified base that can fit
 * in an 'unsigned short' (most can, and this saves memory).
 *
 * BigUnsignedInABase is intended as an intermediary class with little
 * functionality of its own.  BigUnsignedInABase objects can be constructed
 * from, and converted to, biguint_ts (requiring multiplication, mods, etc.)
 * and 'string's (by switching igit values for appropriate characters).
 *
 * BigUnsignedInABase is similar to biguint_t.  Note the following:
 *
 * (1) They represent the number in exactly the same way, except that
 * BigUnsignedInABase uses ''igits'' (or igit) where biguint_t uses
 * ''blocks'' (or Blk).
 *
 * (2) Both use the management features of BigNumStore.  (In fact, my desire
 * to add a BigUnsignedInABase class without duplicating a lot of code led me to
 * introduce BigNumStore.)
 *
 * (3) The only arithmetic operation supported by BigUnsignedInABase is an
 * equality test.  Use biguint_t for arithmetic.
 */

namespace qblocks {

inline BigUnsignedInABase::BigUnsignedInABase(const unsigned short* d,              // NOLINT
                                              unsigned int l, unsigned short base)  // NOLINT
    : BigNumStore<unsigned short>(d, l), base(base) {                               // NOLINT
    if (base < 2)
        throw "BigUnsignedInABase::BigUnsignedInABase(const unsigned short *, unsigned int, unsigned short): "
                    "The base must be at least 2";

    for (unsigned int i = 0; i < l; i++)
        if (blk[i] >= base)
            throw "BigUnsignedInABase::BigUnsignedInABase(const unsigned short *, unsigned int, unsigned short): "
                    "A digit is too large for the specified base";

    trimLeadingZeros();
}

//------------------------------------------------------------------
unsigned int bitLen(unsigned int x) {
    unsigned int len = 0;
    while (x > 0) {
        x >>= 1;
        len++;
    }
    return len;
}

//------------------------------------------------------------------
BigUnsignedInABase::BigUnsignedInABase(const biguint_t& x, unsigned short base) {  // NOLINT
    //        cerr << "\t" << TIC() << " ";
    if (base < 2)
        throw "BigUnsignedInABase(biguint_t, Base): The base must be at least 2";
    this->base = base;

    // Get an upper bound on how much space we need
    int maxBitLenOfX = static_cast<int>(x.len * biguint_t::N);
    int minBitsPerDigit = static_cast<int>(bitLen(base) - 1);
    int maxDigitLenOfX = (maxBitLenOfX + minBitsPerDigit - 1) / minBitsPerDigit;

    len = (unsigned int)maxDigitLenOfX;  // Another change to comply with 'staying in bounds'.
    allocate(len);                       // Get the space

    biguint_t x2(x), buBase(base);
    unsigned int digitNum = 0;

    while (x2.len != 0) {
        // Get last digit.  This is like 'lastDigit = x2 % buBase, x2 /= buBase'.
        biguint_t lastDigit(x2);
        lastDigit.divide(buBase, x2);
        // Save the digit.
        blk[digitNum] = (uint16_t)lastDigit.to_uint();
        // Move on.  We can't run out of room: we figured it out above.
        digitNum++;
    }

    // Save the actual length.
    len = digitNum;
    //        cerr << TIC() << " BigUnsigned" << endl;
}

//------------------------------------------------------------------
BigUnsignedInABase::BigUnsignedInABase(const string& s, unsigned short base) {  // NOLINT
    if (base > 36)
        throw "BigUnsignedInABase(string, unsigned short): "
                    "The default string conversion routines use the symbol set 0-9, A-Z and "
                    "therefore support only up to base 36.  You tried a conversion with a base "
                    "over 36; write your own string conversion routine.";

    this->base = base;

    // 's.length()' is a 'size_t', while 'len' is a unsigned int,
    // also known as an 'unsigned int'.  Some compilers warn without this cast.
    len = (unsigned int)(s.length());
    allocate(len);

    unsigned int digitNum, symbolNumInString;
    for (digitNum = 0; digitNum < len; digitNum++) {
        symbolNumInString = len - 1 - digitNum;
        char theSymbol = s[symbolNumInString];

        if (theSymbol >= '0' && theSymbol <= '9')
            blk[digitNum] = (unsigned short)(theSymbol - '0');  // NOLINT

        else if (theSymbol >= 'A' && theSymbol <= 'Z')
            blk[digitNum] = (unsigned short)(theSymbol - 'A' + 10);  // NOLINT

        else if (theSymbol >= 'a' && theSymbol <= 'z')
            blk[digitNum] = (unsigned short)(theSymbol - 'a' + 10);  // NOLINT

        else
            throw "BigUnsignedInABase(string, unsigned short): "
                        "Bad symbol in input.  Only 0-9, A-Z, a-z are accepted.";

        if (blk[digitNum] >= base)
            throw "BigUnsignedInABase::BigUnsignedInABase(const unsigned short *, unsigned int, unsigned short): "
                    "A digit is too large for the specified base";
    }
    trimLeadingZeros();
}

//------------------------------------------------------------------
BigUnsignedInABase::operator biguint_t(void) const {
    biguint_t ans(0), buBase(base), temp;
    unsigned int digitNum = len;
    while (digitNum > 0) {
        digitNum--;
        temp.multiply(ans, buBase);
        ans.add(temp, biguint_t(blk[digitNum]));
    }
    return ans;
}

//------------------------------------------------------------------
BigUnsignedInABase::operator string(void) const {
    if (len == 0)
        return string("0");
    string_q ret;
    ret.reserve(len + 1);
    for (unsigned int p = 0; p < len; p++) {
        unsigned short c = blk[len - 1 - p];  // NOLINT
        ret += static_cast<char>((c < 10) ? ('0' + c) : ('A' + c - 10));
    }
    return ret;
}

//--------------------------------------------------------------------------------
biguint_t gcd(biguint_t a, biguint_t b) {
    biguint_t trash;
    for (;;) {
        if (b.len == 0)
            return a;
        a.divide(b, trash);
        if (a.len == 0)
            return b;
        b.divide(a, trash);
    }
}

//--------------------------------------------------------------------------------
biguint_t modexp(const bigint_t& base, const biguint_t& exponent, const biguint_t& modulus) {
    biguint_t ans = 1, base2 = (base % modulus).getMagnitude();
    unsigned int i = exponent.bitLength();
    // For each bit of the exponent, most to least significant...
    while (i > 0) {
        i--;
        // Square.
        ans *= ans;
        ans %= modulus;
        // And multiply if the bit is a 1.
        if (exponent.getBit(i)) {
            ans *= base2;
            ans %= modulus;
        }
    }
    return ans;
}

//--------------------------------------------------------------------------------
static void exteuclidean(bigint_t m, bigint_t n, bigint_t& g, bigint_t& r, bigint_t& s) {
    if (&g == &r || &g == &s || &r == &s)
        throw "bigint_t exteuclidean: Outputs are aliased";
    bigint_t r1(1), s1(0), r2(0), s2(1), q;
    /*
     * Invariants:
     * r1*m(orig) + s1*n(orig) == m(current)
     * r2*m(orig) + s2*n(orig) == n(current)
     */
    for (;;) {
        if (n.sign == 0) {
            r = r1;
            s = s1;
            g = m;
            return;
        }
        // Subtract q times the second invariant from the first invariant.
        m.divide(n, q);
        r1 -= q * r2;
        s1 -= q * s2;

        if (m.sign == 0) {
            r = r2;
            s = s2;
            g = n;
            return;
        }
        // Subtract q times the first invariant from the second invariant.
        n.divide(m, q);
        r2 -= q * r1;
        s2 -= q * s1;
    }
}

//--------------------------------------------------------------------------------
biguint_t modinv(const bigint_t& x, const biguint_t& n) {
    bigint_t g, r, s;
    exteuclidean(x, n, g, r, s);
    if (g == 1)
        // r*x + s*n == 1, so r*x === 1 (mod n), so r is the answer.
        return (r % n).getMagnitude();  // (r % n) will be nonnegative
    else
        throw "bigint_t modinv: x and n have a common factor";
}

//--------------------------------------------------------------------------------
ostream& operator<<(ostream& os, const biguint_t& x) {
    unsigned short base;  // NOLINT

    long osFlags = os.flags();  // NOLINT
    if (osFlags & os.dec) {
        base = 10;

    } else if (osFlags & os.hex) {
        base = 16;
        if (osFlags & os.showbase)
            os << "0x";

    } else if (osFlags & os.oct) {
        base = 8;
        if (osFlags & os.showbase)
            os << '0';
    } else {
        throw "ostream << biguint_t: Could not determine the desired base from output-stream flags";
    }
    os << string(BigUnsignedInABase(x, base));
    return os;
}

//--------------------------------------------------------------------------------
ostream& operator<<(ostream& os, const bigint_t& x) {
    if (x.isNegative())
        os << '-';
    os << x.getMagnitude();
    return os;
}

}  // namespace qblocks
