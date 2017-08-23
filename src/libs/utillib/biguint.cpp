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
#include "biguint.h"

namespace qblocks {

    //----------------------------------------------------------------------
    SFUintBN::SFUintBN(void) : SFBigNumStore<uint64_t>()
    {
    }

    //----------------------------------------------------------------------
    SFUintBN::SFUintBN(const SFUintBN &x) : SFBigNumStore<uint64_t>(x)
    {
    }

    //----------------------------------------------------------------------
    SFUintBN& SFUintBN::operator=(const SFUintBN &x)
    {
        SFBigNumStore<uint64_t>::operator=(x);
        return *this;
    }

    //----------------------------------------------------------------------
    SFUintBN::SFUintBN(int unused, unsigned int c) : SFBigNumStore<uint64_t>(0, c)
    {
    }

    //----------------------------------------------------------------------
    SFUintBN::SFUintBN(const uint64_t *b, unsigned int blen) : SFBigNumStore<uint64_t>(b, blen)
    {
        trimLeft();
    }

    //----------------------------------------------------------------------
    SFUintBN::~SFUintBN()
    {
    }

    //----------------------------------------------------------------------
    static const char *msg="Cannot convert from negative number.";
    SFUintBN::SFUintBN(uint64_t x) { if (x!=0) {capacity=0;allocate(1);if(blk){len=1;blk[0]=(uint64_t)(x);}}}
    SFUintBN::SFUintBN(unsigned int x) { if (x!=0) {capacity=0;allocate(1);if(blk){len=1;blk[0]=(uint64_t)(x);}}}
    SFUintBN::SFUintBN(unsigned short x) { if (x!=0) {capacity=0;allocate(1);if(blk){len=1;blk[0]=(uint64_t)(x);}}}
    SFUintBN::SFUintBN(long x) { if (x<0) throw msg; else if (x!=0) {capacity=0;allocate(1);if(blk){len=1;blk[0]=(uint64_t)(x);}}}
    SFUintBN::SFUintBN(int x) { if (x<0) throw msg; else if (x!=0) {capacity=0;allocate(1);if(blk){len=1;blk[0]=(uint64_t)(x);}}}
    SFUintBN::SFUintBN(short x) { if (x<0) throw msg; else if (x!=0) {capacity=0;allocate(1);if(blk){len=1;blk[0]=(uint64_t)(x);}}}

    //----------------------------------------------------------------------
    template <class X>
    X SFUintBN::convertToPrimitive() const {
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

    //----------------------------------------------------------------------
    template <class X>
    X SFUintBN::convertToSignedPrimitive() const {
        X x = convertToPrimitive<X>();
        if (x >= 0)
            return x;
        else
            throw "SFUintBN::to(Primitive): Value is too big to fit in the requested type";
    }

    //----------------------------------------------------------------------
    uint64_t SFUintBN::to_ulong(void) const { return convertToPrimitive<uint64_t> (); }
    unsigned int SFUintBN::to_uint(void) const { return convertToPrimitive<unsigned int> (); }
    unsigned short SFUintBN::to_ushort(void) const { return convertToPrimitive<unsigned short> (); }
    long SFUintBN::to_long(void) const { return convertToSignedPrimitive <long> (); }
    int SFUintBN::to_int(void) const { return convertToSignedPrimitive <int> (); }
    short SFUintBN::to_short(void) const { return convertToSignedPrimitive <short> (); }

    //----------------------------------------------------------------------
    void SFUintBN::trimLeft(void)
    {
        while (len > 0 && blk[len-1] == 0)
            len--;
    }

    //----------------------------------------------------------------------
    void SFUintBN::setBlock(unsigned int i, uint64_t newBlock)
    {
        if (newBlock==0)
        {
            if (i<len)
            {
                blk[i] = 0;
                trimLeft();
            }

        } else
        {
            if (i>=len)
            {
                allocateAndCopy(i+1);
                for (unsigned int j = len; j < i; j++)
                    blk[j] = 0;
                len = i+1;
            }
            blk[i] = newBlock;
        }
    }

    //----------------------------------------------------------------------
    unsigned int SFUintBN::bitLength() const
    {
        if (len==0)
        {
            return 0;

        } else
        {
            uint64_t leftmostBlock = getBlock(len - 1);
            unsigned int leftmostBlockLen = 0;
            while (leftmostBlock != 0) {
                leftmostBlock >>= 1;
                leftmostBlockLen++;
            }
            return leftmostBlockLen + (len - 1) * N;
        }
    }

    //----------------------------------------------------------------------
    void SFUintBN::setBit(unsigned int bi, bool newBit)
    {
        unsigned int blockI = bi / N;
        uint64_t block = getBlock(blockI), mask = ((uint64_t)(1)) << (bi % N);
        block = newBit ? (block | mask) : (block & ~mask);
        setBlock(blockI, block);
    }

    //----------------------------------------------------------------------
    int SFUintBN::compareTo(const SFUintBN &x) const
    {
        unsigned int i = len;

        if (i < x.len) return -1;
        if (i > x.len) return  1;
        while (i > 0)
        {
            i--;
            if (blk[i] == x.blk[i]) continue;
            else if (blk[i] >  x.blk[i]) return  1;
            else                         return -1;
        }
        return 0;
    }

    //----------------------------------------------------------------------
    void SFUintBN::add(const SFUintBN &a, const SFUintBN &b)
    {
        if (thisIsMe(a) || thisIsMe(b))
        {
            // don't operate on top of ourselves
            SFUintBN tmp;
            tmp.add(a,b);
            *this = tmp;
            return;
        }

        if (a.len == 0)
        {
            operator =(b);
            return;

        } else if (b.len == 0)
        {
            operator =(a);
            return;
        }

        bool carryIn, carryOut;
        uint64_t temp;
        unsigned int i;

        const SFUintBN *a2, *b2;
        if (a.len >= b.len)
        {
            a2 = &a;
            b2 = &b;
        } else
        {
            a2 = &b;
            b2 = &a;
        }

        len = a2->len + 1;
        allocate(len);

        for (i = 0, carryIn = false; i < b2->len; i++)
        {
            temp = a2->blk[i] + b2->blk[i];
            carryOut = (temp < a2->blk[i]); // If a rollover occurred, the result is less than either input.
            if (carryIn)
            {
                temp++;
                carryOut |= (temp == 0);
            }
            blk[i]  = temp;
            carryIn = carryOut;
        }

        // If there is a carry left over, increase blocks until one does not roll over.
        for (; i < a2->len && carryIn; i++)
        {
            temp = a2->blk[i] + 1;
            carryIn = (temp == 0);
            blk[i] = temp;
        }


        // If the carry was resolved but the larger number still has blocks, copy them over.
        for (; i < a2->len; i++)
            blk[i] = a2->blk[i];

        // Set the extra block if there's still a carry, decrease length otherwise
        if (carryIn)
            blk[i] = 1;
        else
            len--;
    }

    //----------------------------------------------------------------------
    void SFUintBN::subtract(const SFUintBN &a, const SFUintBN &b)
    {
        if (thisIsMe(a) || thisIsMe(b))
        {
            // don't operate on top of ourselves
            SFUintBN tmp;
            tmp.subtract(a,b);
            *this = tmp;
            return;
        }

        if (b.len == 0)
        {
            operator=(a);
            return;

        } else if (a.len < b.len)
        {
            throw "SFUintBN::subtract: Negative result in unsigned calculation";
        }

        bool borrowIn, borrowOut;
        uint64_t temp;
        unsigned int i;

        len = a.len;
        allocate(len);

        for (i = 0, borrowIn = false; i < b.len; i++)
        {
            temp = a.blk[i] - b.blk[i];
            borrowOut = (temp > a.blk[i]); // If a reverse rollover occurred, the result is greater than the block from a.
            if (borrowIn)
            {
                borrowOut |= (temp == 0);
                temp--;
            }
            blk[i] = temp;
            borrowIn = borrowOut;
        }

        // If there is a borrow left over, decrease blocks until one does not reverse rollover.
        for (; i < a.len && borrowIn; i++)
        {
            borrowIn = (a.blk[i] == 0);
            blk[i] = a.blk[i] - 1;
        }

        // If there's still a borrow, the result is negative. Throw an exception, but zero out this
        // object so as to leave it in a predictable state.
        if (borrowIn)
        {
            len = 0;
            throw "SFUintBN::subtract: Negative result in unsigned calculation";
        } else
        {
            for (; i < a.len; i++)
                blk[i] = a.blk[i];
        }

        trimLeft();
    }

    /*
     * I eventually decided to use bit-shifting algorithms.  To multiply `a'
     * and `b', we zero out the result.  Then, for each `1' bit in `a', we
     * shift `b' left the appropriate amount and add it to the result.
     * Similarly, to divide `a' by `b', we shift `b' left varying amounts,
     * repeatedly trying to subtract it from `a'.  When we succeed, we note
     * the fact by setting a bit in the quotient.  While these algorithms
     * have the same O(n^2) time complexity as Knuth's, the ``constant factor''
     * is likely to be larger.
     *
     * Because I used these algorithms, which require single-block addition
     * and subtraction rather than single-block multiplication and division,
     * the innermost loops of all four routines are very similar.  Study one
     * of them and all will become clear.
     *
     * This is a little function used by both the multiplication
     * routine and the division routine.
     *
     * `getShiftedBlock' returns the `x'th block of `num << y'.
     * `y' may be anything from 0 to N - 1, and `x' may be anything from
     * 0 to `num.len'.
     *
     * Two things contribute to this block:
     *
     * (1) The `N - y' low bits of `num.blk[x]', shifted `y' bits left.
     *
     * (2) The `y' high bits of `num.blk[x-1]', shifted `N - y' bits right.
     *
     * But we must be careful if `x == 0' or `x == num.len', in
     * which case we should use 0 instead of (2) or (1), respectively.
     *
     * If `y == 0', then (2) contributes 0, as it should.  However,
     * in some computer environments, for a reason I cannot understand,
     * `a >> b' means `a >> (b % N)'.  This means `num.blk[x-1] >> (N - y)'
     * will return `num.blk[x-1]' instead of the desired 0 when `y == 0';
     * the test `y == 0' handles this case specially.
     */
    //----------------------------------------------------------------------
    uint64_t getShiftedBlock(const SFUintBN &num, unsigned int x, unsigned int y)
    {
        uint64_t part1 = (x == 0 || y == 0) ? 0 : (num.blk[x - 1] >> (SFUintBN::N - y));
        uint64_t part2 = (x == num.len) ? 0 : (num.blk[x] << y);
        return part1 | part2;
    }

    //----------------------------------------------------------------------
    void SFUintBN::multiply(const SFUintBN &a, const SFUintBN &b)
    {
        if (thisIsMe(a) || thisIsMe(b))
        {
            // don't operate on top of ourselves
            SFUintBN tmp;
            tmp.multiply(a,b);
            *this = tmp;
            return;
        }

        // If either a or b is zero, set to zero.
        if (a.len == 0 || b.len == 0)
        {
            len = 0;
            return;
        }

        /*
         * Overall method:
         *
         * Set this = 0.
         * For each 1-bit of `a' (say the `i2'th bit of block `i'):
         *    Add `b << (i blocks and i2 bits)' to *this.
         */
        // Variables for the calculation
        unsigned int i, j, k;
        unsigned int i2;
        uint64_t temp;
        bool carryIn, carryOut;
        // Set preliminary length and make room
        len = a.len + b.len;
        allocate(len);
        // Zero out this object
        for (i = 0; i < len; i++)
            blk[i] = 0;
        // For each block of the first number...
        for (i = 0; i < a.len; i++)
        {
            // For each 1-bit of that block...
            for (i2 = 0; i2 < N; i2++)
            {
                if ((a.blk[i] & (((uint64_t)(1)) << i2)) == 0)
                    continue;
                /*
                 * Add b to this, shifted left i blocks and i2 bits.
                 * j is the index in b, and k = i + j is the index in this.
                 *
                 * `getShiftedBlock', a short nline function defined above,
                 * is now used for the bit handling.  It replaces the more
                 * complex `bHigh' code, in which each run of the loop dealt
                 * immediately with the low bits and saved the high bits to
                 * be picked up next time.  The last run of the loop used to
                 * leave leftover high bits, which were handled separately.
                 * Instead, this loop runs an additional time with j == b.len.
                 * These changes were made on 2005.01.11.
                 */
                for (j = 0, k = i, carryIn = false; j <= b.len; j++, k++)
                {
                    /*
                     * The body of this loop is very similar to the body of the first loop
                     * in `add', except that this loop does a `+=' instead of a `+'.
                     */
                    temp = blk[k] + getShiftedBlock(b, j, i2);
                    carryOut = (temp < blk[k]);
                    if (carryIn) {
                        temp++;
                        carryOut |= (temp == 0);
                    }
                    blk[k] = temp;
                    carryIn = carryOut;
                }
                // No more extra iteration to deal with `bHigh'.
                // Roll-over a carry as necessary.
                for (; carryIn; k++)
                {
                    blk[k]++;
                    carryIn = (blk[k] == 0);
                }
            }
        }
        // Zap possible leading zero
        if (blk[len - 1] == 0)
            len--;
    }

    /*
     * DIVISION WITH REMAINDER
     * This monstrous function mods *this by the given divisor b while storing the
     * quotient in the given object q; at the end, *this contains the remainder.
     * The seemingly bizarre pattern of inputs and outputs was chosen so that the
     * function copies as little as possible (since it is implemented by repeated
     * subtraction of multiples of b from *this).
     *
     * "modWithQuotient" might be a better name for this function, but I would
     * rather not change the name now.
     *
     * `a.divide(b, q)' is like `q = a / b, a %= b'.
     * / and % use semantics similar to Knuth's, which differ from the
     * primitive integer semantics under division by zero.  See the
     * implementation in SFUintBN.cc for details.
     * `a.divide(b, a)' throws an exception: it doesn't make
     * sense to write quotient and remainder into the same variable.
     */

    //----------------------------------------------------------------------
    void SFUintBN::divide(const SFUintBN &b, SFUintBN &q) {
        /* Defending against aliased calls is more complex than usual because we
         * are writing to both *this and q.
         *
         * It would be silly to try to write quotient and remainder to the
         * same variable.  Rule that out right away. */
        if (this == &q)
            throw "SFUintBN::divide: Cannot write quotient and remainder into the same variable";

        /* At this point *this and q are separate, so the only concern is that b might be
         * aliased to one of them.  If so, use a temporary copy of b. */
        if (thisIsMe(b) || &q == &b)
        {
            // don't operate on top of ourselves
            SFUintBN tmpB(b);
            divide(tmpB,q);
            return;
        }

        if (this == &b || &q == &b) {
            SFUintBN tmpB(b);
            divide(tmpB, q);
            return;
        }

        /*
         * Knuth's definition of mod (which this function uses) is somewhat
         * different from the C++ definition of % in case of division by 0.
         *
         * We let a / 0 == 0 (it doesn't matter much) and a % 0 == a, no
         * exceptions thrown.  This allows us to preserve both Knuth's demand
         * that a mod 0 == a and the useful property that
         * (a / b) * b + (a % b) == a.
         */
        if (b.len == 0) {
            q.len = 0;
            return;
        }

        /*
         * If *this.len < b.len, then *this < b, and we can be sure that b doesn't go into
         * *this at all.  The quotient is 0 and *this is already the remainder (so leave it alone).
         */
        if (len < b.len) {
            q.len = 0;
            return;
        }

        // At this point we know (*this).len >= b.len > 0.  (Whew!)

        /*
         * Overall method:
         *
         * For each appropriate i and i2, decreasing:
         *    Subtract (b << (i blocks and i2 bits)) from *this, storing the
         *      result in subtractBuf.
         *    If the subtraction succeeds with a nonnegative result:
         *        Turn on bit i2 of block i of the quotient q.
         *        Copy subtractBuf back into *this.
         *    Otherwise bit i2 of block i remains off, and *this is unchanged.
         *
         * Eventually q will contain the entire quotient, and *this will
         * be left with the remainder.
         *
         * subtractBuf[x] corresponds to blk[x], not blk[x+i], since 2005.01.11.
         * But on a single iteration, we don't touch the i lowest blocks of blk
         * (and don't use those of subtractBuf) because these blocks are
         * unaffected by the subtraction: we are subtracting
         * (b << (i blocks and i2 bits)), which ends in at least `i' zero
         * blocks. */
        // Variables for the calculation
        unsigned int i, j, k;
        unsigned int i2;
        uint64_t temp;
        bool borrowIn, borrowOut;

        /*
         * Make sure we have an extra zero block just past the value.
         *
         * When we attempt a subtraction, we might shift `b' so
         * its first block begins a few bits left of the dividend,
         * and then we'll try to compare these extra bits with
         * a nonexistent block to the left of the dividend.  The
         * extra zero block ensures sensible behavior; we need
         * an extra block in `subtractBuf' for exactly the same reason.
         */
        unsigned int origLen = len; // Save real length.
        /* To avoid an out-of-bounds access in case of reallocation, allocate
         * first and then increment the logical length. */
        allocateAndCopy(len + 1);
        len++;
        blk[origLen] = 0; // Zero the added block.

        // subtractBuf holds part of the result of a subtraction; see above.
        uint64_t *subtractBuf = new uint64_t[len];

        // Set preliminary length for quotient and make room
        q.len = origLen - b.len + 1;
        q.allocate(q.len);
        // Zero out the quotient
        for (i = 0; i < q.len; i++)
            q.blk[i] = 0;

        // For each possible left-shift of b in blocks...
        i = q.len;
        while (i > 0) {
            i--;
            // For each possible left-shift of b in bits...
            // (Remember, N is the number of bits in a Blk.)
            q.blk[i] = 0;
            i2 = N;
            while (i2 > 0) {
                i2--;
                /*
                 * Subtract b, shifted left i blocks and i2 bits, from *this,
                 * and store the answer in subtractBuf.  In the for loop, `k == i + j'.
                 *
                 * Compare this to the middle section of `multiply'.  They
                 * are in many ways analogous.  See especially the discussion
                 * of `getShiftedBlock'.
                 */
                for (j = 0, k = i, borrowIn = false; j <= b.len; j++, k++) {
                    temp = blk[k] - getShiftedBlock(b, j, i2);
                    borrowOut = (temp > blk[k]);
                    if (borrowIn) {
                        borrowOut |= (temp == 0);
                        temp--;
                    }
                    // Since 2005.01.11, indices of `subtractBuf' directly match those of `blk', so use `k'.
                    subtractBuf[k] = temp;
                    borrowIn = borrowOut;
                }
                // No more extra iteration to deal with `bHigh'.
                // Roll-over a borrow as necessary.
                for (; k < origLen && borrowIn; k++) {
                    borrowIn = (blk[k] == 0);
                    subtractBuf[k] = blk[k] - 1;
                }
                /*
                 * If the subtraction was performed successfully (!borrowIn),
                 * set bit i2 in block i of the quotient.
                 *
                 * Then, copy the portion of subtractBuf filled by the subtraction
                 * back to *this.  This portion starts with block i and ends--
                 * where?  Not necessarily at block `i + b.len'!  Well, we
                 * increased k every time we saved a block into subtractBuf, so
                 * the region of subtractBuf we copy is just [i, k).
                 */
                if (!borrowIn) {
                    q.blk[i] |= (((uint64_t)(1)) << i2);
                    while (k > i) {
                        k--;
                        blk[k] = subtractBuf[k];
                    }
                }
            }
        }
        // Zap possible leading zero in quotient
        if (q.blk[q.len - 1] == 0)
            q.len--;
        // Zap any/all leading zeros in remainder
        trimLeft();
        // Deallocate subtractBuf.
        // (Thanks to Brad Spencer for noticing my accidental omission of this!)
        delete [] subtractBuf;
    }

    //----------------------------------------------------------------------
    void SFUintBN::bitwiseAnd(const SFUintBN &a, const SFUintBN &b)
    {
        if (thisIsMe(a) || thisIsMe(b))
        {
            // don't operate on top of ourselves
            SFUintBN tmp;
            tmp.bitwiseAnd(a,b);
            *this = tmp;
            return;
        }

        len = (a.len >= b.len) ? b.len : a.len;
        allocate(len);
        unsigned int i;
        for (i = 0; i < len; i++)
            blk[i] = a.blk[i] & b.blk[i];
        trimLeft();
    }

    //----------------------------------------------------------------------
    void SFUintBN::bitwiseOr(const SFUintBN &a, const SFUintBN &b)
    {
        if (thisIsMe(a) || thisIsMe(b))
        {
            // don't operate on top of ourselves
            SFUintBN tmp;
            tmp.bitwiseOr(a,b);
            *this = tmp;
            return;
        }

        unsigned int i;
        const SFUintBN *a2, *b2;

        if (a.len >= b.len)
        {
            a2 = &a;
            b2 = &b;
        } else
        {
            a2 = &b;
            b2 = &a;
        }
        allocate(a2->len);
        for (i = 0; i < b2->len; i++)
            blk[i] = a2->blk[i] | b2->blk[i];
        for (; i < a2->len; i++)
            blk[i] = a2->blk[i];
        len = a2->len;

        // Doesn't need trimming
    }

    //----------------------------------------------------------------------
    void SFUintBN::bitwiseXor(const SFUintBN &a, const SFUintBN &b)
    {
        if (thisIsMe(a) || thisIsMe(b))
        {
            // don't operate on top of ourselves
            SFUintBN tmp;
            tmp.bitwiseXor(a,b);
            *this = tmp;
            return;
        }

        unsigned int i;
        const SFUintBN *a2, *b2;
        if (a.len >= b.len)
        {
            a2 = &a;
            b2 = &b;
        } else
        {
            a2 = &b;
            b2 = &a;
        }

        allocate(a2->len);
        for (i = 0; i < b2->len; i++)
            blk[i] = a2->blk[i] ^ b2->blk[i];
        for (; i < a2->len; i++)
            blk[i] = a2->blk[i];
        len = a2->len;

        trimLeft();
    }

    // Negative shift amounts translate to opposite-direction shifts, except for -2^(8*sizeof(int)-1) which is unimplemented.
    //----------------------------------------------------------------------
    void SFUintBN::shiftLeft(const SFUintBN &a, int b)
    {
        if (thisIsMe(a))
        {
            // don't operate on top of ourselves
            SFUintBN tmp;
            tmp.shiftLeft(a,b);
            *this = tmp;
            return;
        }

        if (b < 0)
        {
            if (b << 1 == 0)
            {
                throw "SFUintBN::shiftLeft: Pathological shift amount not implemented";
            } else
            {
                shiftRight(a, -b);
                return;
            }
        }

        unsigned int shiftBlocks = (unsigned int)b / N;
        unsigned int shiftBits   = (unsigned int)b % N;

        // + 1: room for high bits nudged left into another block
        len = a.len + shiftBlocks + 1;
        allocate(len);

        unsigned int i, j;
        for (i = 0; i < shiftBlocks; i++)
            blk[i] = 0;

        for (j = 0, i = shiftBlocks; j <= a.len; j++, i++)
            blk[i] = getShiftedBlock(a, j, shiftBits);

        trimLeft();
    }

    //----------------------------------------------------------------------
    void SFUintBN::shiftRight(const SFUintBN &a, int b)
    {
        if (thisIsMe(a))
        {
            // don't operate on top of ourselves
            SFUintBN tmp;
            tmp.shiftRight(a,b);
            *this = tmp;
            return;
        }

        if (b < 0)
        {
            if (b << 1 == 0)
            {
                throw "SFUintBN::shiftRight: Pathological shift amount not implemented";
            } else
            {
                shiftLeft(a,-b);
                return;
            }
        }

        // This calculation is wacky, but expressing the shift as a left bit shift within each block lets us use getShiftedBlock.
        unsigned int rightShiftBlocks = ((unsigned int)b + N - 1) / N;
        unsigned int leftShiftBits    = N * rightShiftBlocks - (unsigned int)b;

        // At this point (N * rightShiftBlocks - leftShiftBits) == b and 0 <= leftShiftBits < N.
        if (rightShiftBlocks >= a.len + 1)
        {
            // All of a is guaranteed to be shifted off, even considering the left bit shift.
            len = 0;
            return;
        }

        // At this point we're allocating a positive amount. + 1: room for high bits nudged left into another block
        len = a.len + 1 - rightShiftBlocks;
        allocate(len);
        unsigned int i, j;
        for (j = rightShiftBlocks, i = 0; j <= a.len; j++, i++)
            blk[i] = getShiftedBlock(a, j, leftShiftBits);

        trimLeft();
    }
}
