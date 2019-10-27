#pragma once
/*-------------------------------------------------------------------------
 * Derived from https://mattmccutchen.net/bigint/index.html
 * From the website under the Legal section: I, Matt McCutchen, the sole author
 * of the original Big Integer Library, waive my copyright to it, placing it
 * in the public domain.  The library comes with absolutely no warranty.
 *------------------------------------------------------------------------*/
#include "basetypes.h"

namespace qblocks {

    //-------------------------------------------------------------------------------
    template <class BaseType>
    class BigNumStore {
    public:
        // TODO(tjayrush): global data
        static const unsigned int N;
        unsigned int capacity;
        unsigned int len;
        BaseType *blk;

        BigNumStore(void);
        BigNumStore(const BigNumStore<BaseType>& x);
        BigNumStore<BaseType>& operator=(const BigNumStore<BaseType>& x);

        explicit BigNumStore(unsigned int size);
        BigNumStore(const BaseType *b, unsigned int len);
        ~BigNumStore();

        void allocate(unsigned int size);
        void allocateAndCopy(unsigned int size);

        bool operator==(const BigNumStore<BaseType>& x) const;
        bool operator!=(const BigNumStore<BaseType>& x) const { return !operator==(x); }
    };

    //-------------------------------------------------------------------------------
    template <class BaseType>
    const unsigned int BigNumStore<BaseType>::N = 8 * sizeof(BaseType);

    //-------------------------------------------------------------------------------
    template <class BaseType>
    inline BigNumStore<BaseType>::BigNumStore(void) : capacity(0), len(0) {
        blk = 0;
    }

    //-------------------------------------------------------------------------------
    template <class BaseType>
    inline BigNumStore<BaseType>::BigNumStore(unsigned int c) : capacity(c), len(0) {
        blk = (capacity > 0) ? (new BaseType[capacity]) : 0;
    }

    //-------------------------------------------------------------------------------
    template <class BaseType>
    BigNumStore<BaseType>& BigNumStore<BaseType>::operator=(const BigNumStore<BaseType> &x) {
        if (this == &x)
            return *this;

        len = x.len;
        allocate(len);
        for (unsigned int i = 0; i < len; i++)
            blk[i] = x.blk[i];
        return *this;
    }

    //-------------------------------------------------------------------------------
    template <class BaseType>
    BigNumStore<BaseType>::BigNumStore(const BigNumStore<BaseType> &x) : len(x.len) {
        capacity = len;
        blk = new BaseType[capacity];
        for (unsigned int i = 0; i < len; i++)
            blk[i] = x.blk[i];
    }

    //-------------------------------------------------------------------------------
    template <class BaseType>
    BigNumStore<BaseType>::BigNumStore(const BaseType *b, unsigned int len) : capacity(len), len(len) {
        blk = new BaseType[capacity];
        for (unsigned int i = 0; i < len; i++)
            blk[i] = b[i];
    }

    //-------------------------------------------------------------------------------
    template <class BaseType>
    inline BigNumStore<BaseType>::~BigNumStore() {
        if (blk)
            delete [] blk;
    }

    //-------------------------------------------------------------------------------
    template <class BaseType>
    void BigNumStore<BaseType>::allocate(unsigned int size) {
        if (size > capacity) {
            if (blk)
                delete [] blk;
            capacity = size;
            blk = new BaseType[capacity];
        }
    }

    //-------------------------------------------------------------------------------
    template <class BaseType>
    void BigNumStore<BaseType>::allocateAndCopy(unsigned int size) {
        if (size > capacity) {
            BaseType *oldBlk = blk;
            capacity = size;
            blk = new BaseType[capacity];
            for (unsigned int i = 0; i < len; i++)
                blk[i] = oldBlk[i];
            delete [] oldBlk;
        }
    }

    //-------------------------------------------------------------------------------
    template <class BaseType>
    bool BigNumStore<BaseType>::operator==(const BigNumStore<BaseType> &x) const {
        if (len != x.len)
            return false;

        for (unsigned int i = 0; i < len; i++)
            if (blk[i] != x.blk[i])
                return false;
        return true;
    }

    //-------------------------------------------------------------------------------
#define thisIsMe(a) (this == &a)

    //----------------------------------------------------------------------
    class biguint_t : public BigNumStore<uint64_t> {
    public:
        biguint_t(void);
        biguint_t(const biguint_t &x);
        biguint_t& operator=(const biguint_t &x);

        biguint_t(int, unsigned int c);
        biguint_t(const uint64_t *b, unsigned int blen);

        biguint_t(int64_t x);  // NOLINT
        biguint_t(int32_t x);  // NOLINT
        biguint_t(int16_t x);  // NOLINT

        biguint_t(uint64_t x);  // NOLINT
        biguint_t(uint32_t x);  // NOLINT
        biguint_t(uint16_t x);  // NOLINT

        ~biguint_t();

        int64_t to_long(void) const;
        int32_t to_int(void) const;

        uint64_t to_ulong(void) const;
        uint32_t to_uint(void) const;

        int compareTo(const biguint_t &x) const;
        bool operator==(const biguint_t &x) const;
        bool operator!=(const biguint_t &x) const;
        bool operator<(const biguint_t &x) const;
        bool operator<=(const biguint_t &x) const;
        bool operator>=(const biguint_t &x) const;
        bool operator>(const biguint_t &x) const;

        biguint_t operator+(const biguint_t &x) const;
        biguint_t operator-(const biguint_t &x) const;
        biguint_t operator*(const biguint_t &x) const;
        biguint_t operator/(const biguint_t &x) const;
        biguint_t operator%(const biguint_t &x) const;
        biguint_t operator&(const biguint_t &x) const;
        biguint_t operator|(const biguint_t &x) const;
        biguint_t operator^(const biguint_t &x) const;
        biguint_t operator<<(int b) const;
        biguint_t operator>>(int b) const;

        void operator+=(const biguint_t &x);
        void operator-=(const biguint_t &x);
        void operator*=(const biguint_t &x);
        void operator/=(const biguint_t &x);
        void operator%=(const biguint_t &x);
        void operator&=(const biguint_t &x);
        void operator|=(const biguint_t &x);
        void operator^=(const biguint_t &x);
        void operator<<=(int b);
        void operator>>=(int b);

        void operator++(void);  // prefix
        void operator--(void);

        void operator++(int);  // postfix
        void operator--(int);

        void add(const biguint_t &a, const biguint_t &b);
        void subtract(const biguint_t &a, const biguint_t &b);
        void multiply(const biguint_t &a, const biguint_t &b);
        void divide(const biguint_t &b, biguint_t &q);

        unsigned int bitLength(void) const;
        uint64_t getBlock(unsigned int i) const;
        void setBlock(unsigned int i, uint64_t newBlock);
        bool getBit(unsigned int bi) const;
        void setBit(unsigned int bi, bool newBit);
        void trimLeadingZeros(void);

// protected:
        void bitwiseAnd(const biguint_t &a, const biguint_t &b);
        void bitwiseOr(const biguint_t &a, const biguint_t &b);
        void bitwiseXor(const biguint_t &a, const biguint_t &b);

        void shiftLeft(const biguint_t &a, int b);
        void shiftRight(const biguint_t &a, int b);

        friend uint64_t getShiftedBlock(const biguint_t &num, unsigned int x, unsigned int y);

        // template <class X>
        // friend X convertBigUnsignedToPrimitiveAccess(const biguint_t &a);

        template <class X>
        X convertToSignedPrimitive(void) const;

        template <class X>
        X convertToPrimitive(void) const;
    };

    //----------------------------------------------------------------------
    inline bool biguint_t::operator==(const biguint_t &x) const {
        return BigNumStore<uint64_t>::operator==(x);
    }

    //----------------------------------------------------------------------
    inline bool biguint_t::operator!=(const biguint_t &x) const {
        return BigNumStore<uint64_t>::operator!=(x);
    }

    //----------------------------------------------------------------------
    inline bool biguint_t::operator<(const biguint_t &x) const {
        return compareTo(x) < 0;
    }

    //----------------------------------------------------------------------
    inline bool biguint_t::operator<=(const biguint_t &x) const {
        return compareTo(x) <= 0;
    }

    //----------------------------------------------------------------------
    inline bool biguint_t::operator>=(const biguint_t &x) const {
        return compareTo(x) >= 0;
    }

    //----------------------------------------------------------------------
    inline bool biguint_t::operator>(const biguint_t &x) const {
        return compareTo(x) > 0;
    }

    //----------------------------------------------------------------------
    inline void biguint_t::operator+=(const biguint_t &x) {
        add(*this, x);
    }

    //----------------------------------------------------------------------
    inline void biguint_t::operator-=(const biguint_t &x) {
        subtract(*this, x);
    }

    //----------------------------------------------------------------------
    inline void biguint_t::operator*=(const biguint_t &x) {
        multiply(*this, x);
    }

    //----------------------------------------------------------------------
    inline void biguint_t::operator/=(const biguint_t &x) {
        if (x.len == 0) throw "biguint_t::operator /=: division by zero";
        biguint_t q;
        divide(x, q);
        *this = q;
    }

    //----------------------------------------------------------------------
    inline void biguint_t::operator%=(const biguint_t &x) {
        if (x.len == 0)
            throw "biguint_t::operator %=: division by zero";
        biguint_t q;
        divide(x, q);
    }

    //----------------------------------------------------------------------
    inline void biguint_t::operator&=(const biguint_t &x) {
        bitwiseAnd(*this, x);
    }

    //----------------------------------------------------------------------
    inline void biguint_t::operator|=(const biguint_t &x) {
        bitwiseOr(*this, x);
    }

    //----------------------------------------------------------------------
    inline void biguint_t::operator^=(const biguint_t &x) {
        bitwiseXor(*this, x);
    }

    //----------------------------------------------------------------------
    inline void biguint_t::operator<<=(int b) {
        shiftLeft(*this, b);
    }

    //----------------------------------------------------------------------
    inline void biguint_t::operator>>=(int b) {
        shiftRight(*this, b);
    }

    //----------------------------------------------------------------------
    inline biguint_t biguint_t::operator+(const biguint_t &x) const {
        biguint_t ans;
        ans.add(*this, x);
        return ans;
    }

    //----------------------------------------------------------------------
    inline biguint_t biguint_t::operator-(const biguint_t &x) const {
        biguint_t ans;
        ans.subtract(*this, x);
        return ans;
    }

    //----------------------------------------------------------------------
    inline biguint_t biguint_t::operator*(const biguint_t &x) const {
        biguint_t ans;
        ans.multiply(*this, x);
        return ans;
    }

    //----------------------------------------------------------------------
    inline biguint_t biguint_t::operator/(const biguint_t &x) const {
        if (x.len == 0)
            throw "biguint_t::operator /: division by zero";
        biguint_t q;
        biguint_t r = *this;
        r.divide(x, q);
        return q;
    }

    //----------------------------------------------------------------------
    inline biguint_t biguint_t::operator%(const biguint_t &x) const {
        if (x.len == 0)
            throw "biguint_t::operator %: division by zero";
        biguint_t q, r;
        r = *this;
        r.divide(x, q);
        return r;
    }

    //----------------------------------------------------------------------
    inline biguint_t biguint_t::operator&(const biguint_t &x) const {
        biguint_t ans;
        ans.bitwiseAnd(*this, x);
        return ans;
    }

    //----------------------------------------------------------------------
    inline biguint_t biguint_t::operator|(const biguint_t &x) const {
        biguint_t ans;
        ans.bitwiseOr(*this, x);
        return ans;
    }

    //----------------------------------------------------------------------
    inline biguint_t biguint_t::operator^(const biguint_t &x) const {
        biguint_t ans;
        ans.bitwiseXor(*this, x);
        return ans;
    }

    //----------------------------------------------------------------------
    inline biguint_t biguint_t::operator<<(int b) const {
        biguint_t ans;
        ans.shiftLeft(*this, b);
        return ans;
    }

    //----------------------------------------------------------------------
    inline biguint_t biguint_t::operator>>(int b) const {
        biguint_t ans;
        ans.shiftRight(*this, b);
        return ans;
    }

    //----------------------------------------------------------------------
    inline void biguint_t::operator++(int) {
        operator++();
    }

    //----------------------------------------------------------------------
    inline void biguint_t::operator++(void) {
        unsigned int i;
        bool carry = true;
        for (i = 0 ; i < len&&carry ; i++) {
            blk[i]++;
            carry =(blk[i] == 0);
        }

        if (carry) {
            // Allocate and then increase length, as in divide
            allocateAndCopy(len + 1);
            len++;
            blk[i] = 1;
        }
    }

    //----------------------------------------------------------------------
    inline void biguint_t::operator--(int) {
        operator--();
    }

    //----------------------------------------------------------------------
    inline void biguint_t::operator--(void) {
        if (len == 0)
            throw "biguint_t::operator--(): Cannot decrement an unsigned zero";
        unsigned int i;
        bool borrow = true;
        for (i = 0; borrow; i++) {
            borrow =(blk[i] == 0);
            blk[i]--;
        }
        trimLeadingZeros();
    }

    //----------------------------------------------------------------------
    inline uint64_t biguint_t::getBlock(unsigned int i) const {
        return i >= len ? 0 : blk[i];
    }

    //----------------------------------------------------------------------
    inline bool biguint_t::getBit(unsigned int bi) const {
        return (getBlock(bi / N) & (((uint64_t)(1)) << (bi % N))) != 0;
    }

    //----------------------------------------------------------------------
    class BigUnsignedInABase : public BigNumStore<unsigned short> {  // NOLINT
    public:
        unsigned short base;  // NOLINT

        // Creates a BigUnsignedInABase with a capacity; for internal use.
        BigUnsignedInABase(int, unsigned int c) : BigNumStore<unsigned short>(0, c) {}  // NOLINT

        // Decreases len to eliminate any leading zero igits.
        void trimLeadingZeros(void) {
            while (len > 0 && blk[len - 1] == 0)
                len--;
        }

        // Constructs zero in base 2.
        BigUnsignedInABase() : BigNumStore<unsigned short>(), base(2) {}  // NOLINT

        // Copy constructor
        BigUnsignedInABase(const BigUnsignedInABase &x) : BigNumStore<unsigned short>(x), base(x.base) {}  // NOLINT

        // Assignment operator
        void operator =(const BigUnsignedInABase &x) {
            BigNumStore<unsigned short>::operator =(x);  // NOLINT
            base = x.base;
        }

        // Constructor that copies from a given array of igits.
        BigUnsignedInABase(const unsigned short *d, unsigned int l, unsigned short base);  // NOLINT

        // Destructor.  BigNumStore does the delete for us.
        ~BigUnsignedInABase() {}

        // LINKS TO BIGUNSIGNED
        BigUnsignedInABase(const biguint_t &x, unsigned short base);  // NOLINT
        operator biguint_t() const;

        operator string() const;
        BigUnsignedInABase(const string &s, unsigned short base);  // NOLINT

        /*
         * Equality test.  For the purposes of this test, two BigUnsignedInABase
         * values must have the same base to be equal.
         */
        bool operator ==(const BigUnsignedInABase &x) const {
            return base == x.base && BigNumStore<unsigned short>::operator==(x);  // NOLINT
        }
        bool operator !=(const BigUnsignedInABase &x) const { return !operator==(x); }

    };

}  // namespace qblocks
