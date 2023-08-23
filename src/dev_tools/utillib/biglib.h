#pragma once
/*-------------------------------------------------------------------------
 * Derived from https://mattmccutchen.net/bigint/index.html
 * From the website under the Legal section: I, Matt McCutchen, the sole author
 * of the original Big Integer Library, waive my copyright to it, placing it
 * in the public domain.  The library comes with absolutely no warranty.
 *------------------------------------------------------------------------*/
#include "basetypes.h"
#include "biguint.h"
#include "bigint.h"

namespace qblocks {

extern ostream& operator<<(ostream& os, const biguint_t& x);
extern ostream& operator<<(ostream& os, const bigint_t& x);

extern biguint_t gcd(biguint_t a, biguint_t b);
extern biguint_t modinv(const bigint_t& x, const biguint_t& n);
extern biguint_t modexp(const bigint_t& base, const biguint_t& exponent, const biguint_t& modulus);

}  // namespace qblocks
