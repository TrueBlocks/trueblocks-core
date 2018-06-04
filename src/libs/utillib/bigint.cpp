/*-------------------------------------------------------------------------
 * Derived from https://mattmccutchen.net/bigint/index.html
 * From the website under the Legal section: I, Matt McCutchen, the sole author
 * of the original Big Integer Library, waive my copyright to it, placing it
 * in the public domain.  The library comes with absolutely no warranty.
 *------------------------------------------------------------------------*/
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
#include "bigint.h"

namespace qblocks {

    //------------------------------------------------------------
    SFIntBN::SFIntBN(void) : sign(0), mag()
    {
    }

    //------------------------------------------------------------
    SFIntBN::SFIntBN(const SFIntBN &x) : sign(x.sign), mag(x.mag)
    {
    }

    //------------------------------------------------------------
    SFIntBN& SFIntBN::operator=(const SFIntBN& x)
    {
        if (thisIsMe(x))
            return *this;
        sign = x.sign;
        mag  = x.mag;
        return *this;
    }

    //------------------------------------------------------------
    SFIntBN::SFIntBN(const SFUintBN &x) : mag(x)
    {
        sign = (mag.len==0) ? 0 : 1;
    }

    //------------------------------------------------------------
    SFIntBN::SFIntBN(const uint64_t *b, unsigned int len) : mag(b, len)
    {
        sign = (mag.len==0) ? 0 : 1;
    }

    //------------------------------------------------------------
    SFIntBN::SFIntBN(const uint64_t *b, unsigned int blen, int sgn) : mag(b, blen)
    {
        switch (sgn)
        {
            case 0:
                if (mag.len!=0)
                    throw "SFIntBN::SFIntBN(const uint64_t *, unsigned int, int): Cannot use a sign of zero with a nonzero magnitude";
                sign = 0;
                break;

            case  1:
            case -1:
                sign = ((mag.len==0) ? 0 : sgn);
                break;

            default:
                // do nothing with bad data
                ;
        }
    }

    //------------------------------------------------------------
    SFIntBN::SFIntBN(const SFUintBN &x, int sgn) : mag(x)
    {
        switch (sgn)
        {
            case 0:
                if (mag.len!=0)
                    throw "SFIntBN::SFIntBN(const SFUintBN &, int): Cannot use a sign of zero with a nonzero magnitude";
                sign = 0;
                break;

            case  1:
            case -1:
                // If the magnitude is zero, force the sign to zero.
                sign = ((mag.len==0) ? 0 : sgn);
                break;

            default:
                // do nothing with bad data
                ;
        }
    }

    //------------------------------------------------------------
    SFIntBN::SFIntBN(uint64_t  x) : mag(x)
    {
        sign = ((mag.len==0) ? 0 : 1);
    }

    //------------------------------------------------------------
    SFIntBN::SFIntBN(unsigned int   x) : mag(x)
    {
        sign = ((mag.len==0) ? 0 : 1);
    }

    //------------------------------------------------------------
    SFIntBN::SFIntBN(unsigned short x) : mag(x)
    {
        sign = ((mag.len==0) ? 0 : 1);
    }

    // For signed input, determine the desired magnitude and sign separately.
    template <class X, class UX>
    uint64_t magOf(X x)
    {
        /* UX(...) cast needed to stop short(-2^15), which negates to
         * itself, from sign-extending in the conversion to uint64_t. */
        return ((uint64_t)(x < 0 ? UX(-x) : (uint32_t)x));
    }
    template <class X>
    int signOf(X x) {
        return (x == 0) ? 0
        : (x > 0) ? 1
        : -1;
    }

    //------------------------------------------------------------
    SFIntBN::SFIntBN(int64_t x) : sign(signOf(x)), mag(magOf<int64_t , uint64_t >(x)) {}
    //------------------------------------------------------------
    SFIntBN::SFIntBN(int x) : sign(signOf(x)), mag(magOf<int  , unsigned int  >(x)) {}
    //------------------------------------------------------------
    SFIntBN::SFIntBN(short x) : sign(signOf(x)), mag(magOf<short, unsigned short>(x)) {}

    //------------------------------------------------------------
    template <class X>
    X convertBigUnsignedToPrimitiveAccess(const SFUintBN &a)
    {
        return a.convertToPrimitive<X>();
    }

    //------------------------------------------------------------
    template <class X>
    X SFIntBN::convertToUnsignedPrimitive() const
    {
        if (sign == -1)
            throw "SFIntBN::to<Primitive>: Cannot convert a negative integer to an unsigned type";
        else
            return convertBigUnsignedToPrimitiveAccess<X>(mag);
    }

    //------------------------------------------------------------
    template <class X, class UX>
    X SFIntBN::convertToSignedPrimitive() const
    {
        if (sign == 0)
        {
            return 0;

        } else if (mag.len == 1)
        {
            // The single block might fit in an X.  Try the conversion.
            uint64_t b = mag.getBlock(0);
            if (sign == 1)
            {
                X x = X(b);
                if (x >= 0 && ((uint64_t)(x)) == b)
                    return x;
            } else
            {
                X x = -X(b);
                /* UX(...) needed to avoid rejecting conversion of
                 * -2^15 to a short. */
                if (x < 0 && ((uint64_t)(UX(-x))) == b)
                    return x;
            }
            // fall through
        }
        throw "SFIntBN::to<Primitive>: Value is too big to fit in the requested type";
    }

    //------------------------------------------------------------
    template <class X>
    X SFUintBN::convertToPrimitive() const
    {
        if (len == 0)
            return 0;
        else if (len == 1) {
            X x = X(blk[0]);
            if (((uint64_t)(x)) == blk[0])
                return x;
            // fall through
        }
        throw "SFUintBN::to<Primitive>: Value is too big to fit in the requested type";
    }

    //------------------------------------------------------------
    template <class X>
    X SFUintBN::convertToSignedPrimitive() const {
        X x = convertToPrimitive<X>();
        if (x >= 0)
            return x;
        else
            throw "SFUintBN::to(Primitive): Value is too big to fit in the requested type";
    }

    //------------------------------------------------------------
    int64_t SFIntBN::to_long(void) const
    {
        return convertToSignedPrimitive  <int64_t, uint64_t> ();
    }

    //------------------------------------------------------------
    int SFIntBN::to_int(void) const { return convertToSignedPrimitive<int, unsigned int>(); }
    //------------------------------------------------------------
    short SFIntBN::to_short(void) const { return convertToSignedPrimitive<short, unsigned short>(); }
    //------------------------------------------------------------
    uint64_t SFIntBN::to_ulong(void) const { return convertToUnsignedPrimitive<uint64_t>(); }
    //------------------------------------------------------------
    unsigned int SFIntBN::to_uint(void) const { return convertToUnsignedPrimitive<unsigned int>(); }
    //------------------------------------------------------------
    unsigned short SFIntBN::to_ushort(void) const { return convertToUnsignedPrimitive<unsigned short>(); }

    //------------------------------------------------------------
    int SFIntBN::compareTo(const SFIntBN &x) const
    {
        if (sign < x.sign)
            return -1;

        else if (sign > x.sign)
            return 1;

        else switch (sign)
        {
            case  0: return  0;
            case  1: return  mag.compareTo(x.mag);
            case -1: return -mag.compareTo(x.mag);
            default:
                // ignore bad data
                ;
        }
        return 0;
    }

    //------------------------------------------------------------
    #define DTRT_ALIASED(cond, op) \
    if (cond) { \
    SFIntBN tmpThis; \
    tmpThis.op; \
    *this = tmpThis; \
    return; \
    }

    //------------------------------------------------------------
    void SFIntBN::add(const SFIntBN &a, const SFIntBN &b)
    {
        DTRT_ALIASED(this == &a || this == &b, add(a, b));

        // If one argument is zero, copy the other.
        if (a.sign == 0)
            operator =(b);
        else if (b.sign == 0)
            operator =(a);
        // If the arguments have the same sign, take the
        // common sign and add their magnitudes.
        else if (a.sign == b.sign) {
            sign = a.sign;
            mag.add(a.mag, b.mag);
        } else {
            // Otherwise, their magnitudes must be compared.
            switch (a.mag.compareTo(b.mag)) {
                case 0:
                    // If their magnitudes are the same, copy zero.
                    mag = 0;
                    sign = 0;
                    break;
                    // Otherwise, take the sign of the greater, and subtract
                    // the lesser magnitude from the greater magnitude.
                case 1:
                    sign = a.sign;
                    mag.subtract(a.mag, b.mag);
                    break;
                case -1:
                    sign = b.sign;
                    mag.subtract(b.mag, a.mag);
                    break;
            }
        }
    }

    //------------------------------------------------------------
    void SFIntBN::subtract(const SFIntBN &a, const SFIntBN &b)
    {
        // Notice that this routine is identical to SFIntBN::add,
        // if one replaces b.sign by its opposite.
        DTRT_ALIASED(this == &a || this == &b, subtract(a, b));
        // If a is zero, copy b and flip its sign.  If b is zero, copy a.
        if (a.sign == 0) {
            mag = b.mag;
            // Take the negative of _b_'s, sign, not ours.
            // Bug pointed out by Sam Larkin on 2005.03.30.
            sign = -b.sign;
        } else if (b.sign == 0)
            operator =(a);
        // If their signs differ, take a.sign and add the magnitudes.
        else if (a.sign != b.sign) {
            sign = a.sign;
            mag.add(a.mag, b.mag);
        } else {
            // Otherwise, their magnitudes must be compared.
            switch (a.mag.compareTo(b.mag)) {
                    // If their magnitudes are the same, copy zero.
                case 0:
                    mag = 0;
                    sign = 0;
                    break;
                    // If a's magnitude is greater, take a.sign and
                    // subtract a from b.
                case 1:
                    sign = a.sign;
                    mag.subtract(a.mag, b.mag);
                    break;
                    // If b's magnitude is greater, take the opposite
                    // of b.sign and subtract b from a.
                case -1:
                    sign = -b.sign;
                    mag.subtract(b.mag, a.mag);
                    break;
            }
        }
    }

    //------------------------------------------------------------
    void SFIntBN::multiply(const SFIntBN &a, const SFIntBN &b)
    {
        DTRT_ALIASED(this == &a || this == &b, multiply(a, b));

        // If one object is zero, copy zero and return.
        if (a.sign == 0 || b.sign == 0) {
            sign = 0;
            mag = 0;
            return;
        }

        // If the signs of the arguments are the same, the result
        // is positive, otherwise it is negative.
        sign = (a.sign == b.sign) ? 1 : -1;
        // Multiply the magnitudes.
        mag.multiply(a.mag, b.mag);
    }

    //------------------------------------------------------------
    void SFIntBN::divide(const SFIntBN &b, SFIntBN &q)
    {
        if (this == &q)
            throw "SFIntBN::divide: Cannot write quotient and remainder into the same variable";

        if (this == &b || &q == &b) {
            SFIntBN tmpB(b);
            divide(tmpB, q);
            return;
        }

        // Division by zero gives quotient 0 and remainder *this
        if (b.sign == 0) {
            q.mag = 0;
            q.sign = 0;
            return;
        }
        // 0 / b gives quotient 0 and remainder 0
        if (sign == 0) {
            q.mag = 0;
            q.sign = 0;
            return;
        }

        // Here *this != 0, b != 0.

        // Do the operands have the same sign?
        if (sign == b.sign) {
            // Yes: easy case.  Quotient is zero or positive.
            q.sign = 1;
        } else {
            // No: harder case.  Quotient is negative.
            q.sign = -1;
            // Decrease the magnitude of the dividend by one.
            mag--;
            /*
             * We tinker with the dividend before and with the
             * quotient and remainder after so that the result
             * comes out right.  To see why it works, consider the following
             * list of examples, where A is the magnitude-decreased
             * a, Q and R are the results of SFUintBN division
             * with remainder on A and |b|, and q and r are the
             * final results we want:
             *
             *	a	A	b	Q	R	q	r
             *	-3	-2	3	0	2	-1	0
             *	-4	-3	3	1	0	-2	2
             *	-5	-4	3	1	1	-2	1
             *	-6	-5	3	1	2	-2	0
             *
             * It appears that we need a total of 3 corrections:
             * Decrease the magnitude of a to get A.  Increase the
             * magnitude of Q to get q (and make it negative).
             * Find r = (b - 1) - R and give it the desired sign.
             */
        }
        
        // Divide the magnitudes.
        mag.divide(b.mag, q.mag);
        
        if (sign != b.sign) {
            // More for the harder case (as described):
            // Increase the magnitude of the quotient by one.
            q.mag++;
            // Modify the remainder.
            mag.subtract(b.mag, mag);
            mag--;
        }
        
        // Sign of the remainder is always the sign of the divisor b.
        sign = b.sign;
        
        // Set signs to zero as necessary.  (Thanks David Allen!)
        if (mag.len==0)
            sign = 0;
        if (q.mag.len==0)
            q.sign = 0;
        
        // WHEW!!!
    }
    
    //------------------------------------------------------------
    void SFIntBN::negate(const SFIntBN &a)
    {
        DTRT_ALIASED(this == &a, negate(a));
        mag  =  a.mag;
        sign = -a.sign;
    }
    
    //------------------------------------------------------------
    bool SFIntBN::isNegative(void) const
    {
        return operator<(0);
    }
    
    //------------------------------------------------------------
    const SFUintBN& SFIntBN::getMagnitude(void) const
    {
        return mag;
    }
    
    //------------------------------------------------------------
    uint64_t SFIntBN::getBlock(unsigned int i) const
    {
        return mag.getBlock(i);
    }
}
