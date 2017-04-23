#pragma once
/*-------------------------------------------------------------------------
 * Derived from https://mattmccutchen.net/bigint/index.html
 *------------------------------------------------------------------------*/

#include "nla.h"

//----------------------------------------------------------------------
class SFUintBN : public SFBigNumStore<unsigned long>
{
public:
	SFUintBN ( void );
	SFUintBN ( const SFUintBN &x );
	SFUintBN& operator=(const SFUintBN &x);

	SFUintBN ( int, unsigned int c );
	SFUintBN ( const unsigned long *b, unsigned int blen );

	SFUintBN ( long x );
	SFUintBN ( int x );
	SFUintBN ( short x );

	SFUintBN ( unsigned long x );
	SFUintBN ( unsigned int x );
	SFUintBN ( unsigned short x );

	~SFUintBN();

	long           to_long   ( void ) const;
	int            to_int    ( void ) const;
	short          to_short  ( void ) const;

	unsigned long  to_ulong  ( void ) const;
	unsigned int   to_uint   ( void ) const;
	unsigned short to_ushort ( void ) const;

	int            compareTo  ( const SFUintBN &x ) const;
	bool           operator== ( const SFUintBN &x ) const;
	bool           operator!= ( const SFUintBN &x ) const;
	bool           operator<  ( const SFUintBN &x ) const;
	bool           operator<= ( const SFUintBN &x ) const;
	bool           operator>= ( const SFUintBN &x ) const;
	bool           operator>  ( const SFUintBN &x ) const;

	SFUintBN      operator+  ( const SFUintBN &x ) const;
	SFUintBN      operator-  ( const SFUintBN &x ) const;
	SFUintBN      operator*  ( const SFUintBN &x ) const;
	SFUintBN      operator/  ( const SFUintBN &x ) const;
	SFUintBN      operator%  ( const SFUintBN &x ) const;
	SFUintBN      operator&  ( const SFUintBN &x ) const;
	SFUintBN      operator|  ( const SFUintBN &x ) const;
	SFUintBN      operator^  ( const SFUintBN &x ) const;
	SFUintBN      operator<< ( int b ) const;
	SFUintBN      operator>> ( int b ) const;

	void           operator+=  ( const SFUintBN &x );
	void           operator-=  ( const SFUintBN &x );
	void           operator*=  ( const SFUintBN &x );
	void           operator/=  ( const SFUintBN &x );
	void           operator%=  ( const SFUintBN &x );
	void           operator&=  ( const SFUintBN &x );
	void           operator|=  ( const SFUintBN &x );
	void           operator^=  ( const SFUintBN &x );
	void           operator<<= ( int b );
	void           operator>>= ( int b );

	void           operator++  ( void ); // prefix
	void           operator--  ( void );

	void           operator++  ( int ); // postfix
	void           operator--  ( int );

	void           add       ( const SFUintBN &a, const SFUintBN &b );
	void           subtract  ( const SFUintBN &a, const SFUintBN &b );
	void           multiply  ( const SFUintBN &a, const SFUintBN &b );
	void           divide    ( const SFUintBN &b, SFUintBN &q );

	unsigned int   bitLength ( void ) const;
	unsigned long  getBlock  ( unsigned int i ) const;
	void           setBlock  ( unsigned int i, unsigned long newBlock );
	bool           getBit    ( unsigned int bi ) const;
	void           setBit    ( unsigned int bi, bool newBit );
	void           trimLeft  ( void );

protected:

	void           bitwiseAnd ( const SFUintBN &a, const SFUintBN &b );
	void           bitwiseOr  ( const SFUintBN &a, const SFUintBN &b );
	void           bitwiseXor ( const SFUintBN &a, const SFUintBN &b );

	void           shiftLeft  ( const SFUintBN &a, int b );
	void           shiftRight ( const SFUintBN &a, int b );

	friend unsigned long getShiftedBlock(const SFUintBN &num, unsigned int x, unsigned int y);

	template <class X>
	friend X convertBigUnsignedToPrimitiveAccess(const SFUintBN &a);

	template <class X>
	X convertToSignedPrimitive(void) const;

	template <class X>
	X convertToPrimitive(void) const;
};

//----------------------------------------------------------------------
inline bool SFUintBN::operator==(const SFUintBN &x) const
{
	return SFBigNumStore<unsigned long>::operator==(x);
}

//----------------------------------------------------------------------
inline bool SFUintBN::operator!=(const SFUintBN &x) const
{
	return SFBigNumStore<unsigned long>::operator!=(x);
}

//----------------------------------------------------------------------
inline bool SFUintBN::operator<(const SFUintBN &x) const
{
	return compareTo(x) < 0;
}

//----------------------------------------------------------------------
inline bool SFUintBN::operator<=(const SFUintBN &x) const
{
	return compareTo(x) <= 0;
}

//----------------------------------------------------------------------
inline bool SFUintBN::operator>=(const SFUintBN &x) const
{
	return compareTo(x) >= 0;
}

//----------------------------------------------------------------------
inline bool SFUintBN::operator>(const SFUintBN &x) const
{
	return compareTo(x) > 0;
}

//----------------------------------------------------------------------
inline void SFUintBN::operator+=(const SFUintBN &x)
{
	add(*this, x);
}

//----------------------------------------------------------------------
inline void SFUintBN::operator-=(const SFUintBN &x)
{
	subtract(*this, x);
}

//----------------------------------------------------------------------
inline void SFUintBN::operator*=(const SFUintBN &x)
{
	multiply(*this, x);
}

//----------------------------------------------------------------------
inline void SFUintBN::operator/=(const SFUintBN &x)
{
	if (x.len==0) throw "SFUintBN::operator /=: division by zero";
	SFUintBN q;
	divide(x, q);
	*this = q;
}

//----------------------------------------------------------------------
inline void SFUintBN::operator%=(const SFUintBN &x)
{
	if (x.len==0) throw "SFUintBN::operator %=: division by zero";
	SFUintBN q;
	divide(x, q);
}

//----------------------------------------------------------------------
inline void SFUintBN::operator&=(const SFUintBN &x)
{
	bitwiseAnd(*this, x);
}

//----------------------------------------------------------------------
inline void SFUintBN::operator|=(const SFUintBN &x)
{
	bitwiseOr(*this, x);
}

//----------------------------------------------------------------------
inline void SFUintBN::operator^=(const SFUintBN &x)
{
	bitwiseXor(*this, x);
}

//----------------------------------------------------------------------
inline void SFUintBN::operator<<=(int b)
{
	shiftLeft(*this, b);
}

//----------------------------------------------------------------------
inline void SFUintBN::operator>>=(int b)
{
	shiftRight(*this, b);
}

//----------------------------------------------------------------------
inline SFUintBN SFUintBN::operator+(const SFUintBN &x) const
{
	SFUintBN ans;
	ans.add(*this, x);
	return ans;
}

//----------------------------------------------------------------------
inline SFUintBN SFUintBN::operator-(const SFUintBN &x) const
{
	SFUintBN ans;
	ans.subtract(*this, x);
	return ans;
}

//----------------------------------------------------------------------
inline SFUintBN SFUintBN::operator*(const SFUintBN &x) const
{
	SFUintBN ans;
	ans.multiply(*this, x);
	return ans;
}

//----------------------------------------------------------------------
inline SFUintBN SFUintBN::operator/(const SFUintBN &x) const
{
	if (x.len==0) throw "SFUintBN::operator /: division by zero";
	SFUintBN q;
	SFUintBN r = *this;
	r.divide(x, q);
	return q;
}

//----------------------------------------------------------------------
inline SFUintBN SFUintBN::operator%(const SFUintBN &x) const
{
	if (x.len==0) throw "SFUintBN::operator %: division by zero";
	SFUintBN q, r;
	r = *this;
	r.divide(x, q);
	return r;
}

//----------------------------------------------------------------------
inline SFUintBN SFUintBN::operator&(const SFUintBN &x) const
{
	SFUintBN ans;
	ans.bitwiseAnd(*this, x);
	return ans;
}

//----------------------------------------------------------------------
inline SFUintBN SFUintBN::operator|(const SFUintBN &x) const
{
	SFUintBN ans;
	ans.bitwiseOr(*this, x);
	return ans;
}

//----------------------------------------------------------------------
inline SFUintBN SFUintBN::operator^(const SFUintBN &x) const
{
	SFUintBN ans;
	ans.bitwiseXor(*this, x);
	return ans;
}

//----------------------------------------------------------------------
inline SFUintBN SFUintBN::operator<<(int b) const
{
	SFUintBN ans;
	ans.shiftLeft(*this, b);
	return ans;
}

//----------------------------------------------------------------------
inline SFUintBN SFUintBN::operator>>(int b) const
{
	SFUintBN ans;
	ans.shiftRight(*this, b);
	return ans;
}

//----------------------------------------------------------------------
inline void SFUintBN::operator++(int) { operator++(); }
inline void SFUintBN::operator++(void)
{
	unsigned int i;
	bool carry = true;
	for (i=0;i<len&&carry;i++)
	{
		blk[i]++;
		carry = (blk[i] == 0);
	}
	if (carry)
	{
		// Allocate and then increase length, as in divide
		allocateAndCopy(len + 1);
		len++;
		blk[i] = 1;
	}
}

//----------------------------------------------------------------------
inline void SFUintBN::operator--(int) { operator--(); }
inline void SFUintBN::operator--(void)
{
	if (len == 0)
		throw "SFUintBN::operator--(): Cannot decrement an unsigned zero";
	unsigned int i;
	bool borrow = true;
	for (i = 0; borrow; i++)
	{
		borrow = (blk[i] == 0);
		blk[i]--;
	}
	trimLeft();
}

//----------------------------------------------------------------------
inline unsigned long SFUintBN::getBlock(unsigned int i) const
{
	return i >= len ? 0 : blk[i];
}

//----------------------------------------------------------------------
inline bool SFUintBN::getBit(unsigned int bi) const
{
	return (getBlock(bi / N) & (((unsigned long)(1)) << (bi % N))) != 0;
}
