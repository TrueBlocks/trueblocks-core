#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include "biguint.h"

//---------------------------------------------------------------
class SFIntBN
{
public:
	int      sign;
	SFUintBN mag;

	SFIntBN ( void );
	SFIntBN ( const SFIntBN& x );
	SFIntBN& operator=( const SFIntBN& x );

	SFIntBN ( const SFUintBN& x );
	SFIntBN ( const SFUintBN& x, int s );

	SFIntBN ( const unsigned long *b, unsigned int len );
	SFIntBN ( const unsigned long *b, unsigned int len, int sgn );

	SFIntBN ( long x );
	SFIntBN ( int x );
	SFIntBN ( short x );

	SFIntBN ( unsigned long x );
	SFIntBN ( unsigned int x );
	SFIntBN ( unsigned short x );

	long  to_long  ( void ) const;
	int   to_int   ( void ) const;
	short to_short ( void ) const;

	unsigned long  to_ulong ( void ) const;
	unsigned int   to_uint  ( void ) const;
	unsigned short to_ushort( void ) const;

	int      compareTo  ( const SFIntBN& x ) const;
	bool     operator== ( const SFIntBN& x ) const;
	bool     operator!= ( const SFIntBN& x ) const;
	bool     operator<  ( const SFIntBN& x ) const;
	bool     operator>= ( const SFIntBN& x ) const;
	bool     operator>  ( const SFIntBN& x ) const;
	bool     operator<= ( const SFIntBN& x ) const;

	SFIntBN operator+  ( const SFIntBN& x ) const;
	SFIntBN operator-  ( const SFIntBN& x ) const;
	SFIntBN operator*  ( const SFIntBN& x ) const;
	SFIntBN operator/  ( const SFIntBN& x ) const;
	SFIntBN operator%  ( const SFIntBN& x ) const;
	SFIntBN operator-  ( void ) const;

	void     operator+= ( const SFIntBN& x );
	void     operator-= ( const SFIntBN& x );
	void     operator*= ( const SFIntBN& x );
	void     operator/= ( const SFIntBN& x );
	void     operator%= ( const SFIntBN& x );

	void     operator++ (     );
	void     operator++ ( int );
	void     operator-- (     );
	void     operator-- ( int );

	void     negate     (const SFIntBN& a);
	void     add        (const SFIntBN& a, const SFIntBN& b);
	void     subtract   (const SFIntBN& a, const SFIntBN& b);
	void     multiply   (const SFIntBN& a, const SFIntBN& b);
	void     divide     (const SFIntBN& b, SFIntBN& q);

	      bool       isNegative  ( void ) const;
	const SFUintBN& getMagnitude( void ) const;
	      unsigned long getBlock    ( unsigned int i ) const;

	template <class X>           X convertToUnsignedPrimitive() const;
	template <class X, class UX> X convertToSignedPrimitive() const;
};

//------------------------------------------------------------
inline void SFIntBN::operator++(int) { operator++(); }
inline void SFIntBN::operator++(void)
{
	if (sign == -1)
	{
		mag--;
		if (mag == 0)
			sign = 0;
	} else
	{
		mag++;
		sign = 1;
	}
}

//------------------------------------------------------------
inline void SFIntBN::operator--(int) { operator--(); }
inline void SFIntBN::operator--(void)
{
	if (sign == 1)
	{
		mag--;
		if (mag == 0)
			sign = 0;
	} else
	{
		mag++;
		sign = -1;
	}
}

//------------------------------------------------------------
inline bool SFIntBN::operator!=(const SFIntBN &x) const
{
	return !operator==(x);
}

//------------------------------------------------------------
inline bool SFIntBN::operator<(const SFIntBN &x) const
{
	return compareTo(x) < 0;
}

//------------------------------------------------------------
inline bool SFIntBN::operator<=(const SFIntBN &x) const
{
	return compareTo(x) <= 0;
}

//------------------------------------------------------------
inline bool SFIntBN::operator>(const SFIntBN &x) const
{
	return compareTo(x) > 0;
}

//------------------------------------------------------------
inline bool SFIntBN::operator>=(const SFIntBN &x) const
{
	return compareTo(x) >= 0;
}

//------------------------------------------------------------
inline bool SFIntBN::operator==(const SFIntBN &x) const
{
	return (sign == x.sign) && (mag == x.mag);
}

//------------------------------------------------------------
inline SFIntBN SFIntBN::operator+(const SFIntBN &x) const
{
	SFIntBN ans;
	ans.add(*this, x);
	return ans;
}

//------------------------------------------------------------
inline SFIntBN SFIntBN::operator-(const SFIntBN &x) const
{
	SFIntBN ans;
	ans.subtract(*this, x);
	return ans;
}

//------------------------------------------------------------
inline SFIntBN SFIntBN::operator*(const SFIntBN &x) const
{
	SFIntBN ans;
	ans.multiply(*this, x);
	return ans;
}

//------------------------------------------------------------
inline SFIntBN SFIntBN::operator/(const SFIntBN &x) const
{
	//TODO: Why not use mag.len==0 for test for zero to be consistent?
	if (sign==0)
		throw "SFIntBN::operator /: division by zero";
	SFIntBN q, r;
	r = *this;
	r.divide(x, q);
	return q;
}

//------------------------------------------------------------
inline SFIntBN SFIntBN::operator%(const SFIntBN &x) const
{
	//TODO: Why not use mag.len==0 for test for zero to be consistent?
	if (x.sign==0)
		throw "SFIntBN::operator %: division by zero";
	SFIntBN q, r;
	r = *this;
	r.divide(x, q);
	return r;
}

//------------------------------------------------------------
inline SFIntBN SFIntBN::operator-() const
{
	SFIntBN ans;
	ans.negate(*this);
	return ans;
}

//------------------------------------------------------------
inline void SFIntBN::operator+=(const SFIntBN &x)
{
	add(*this, x);
}

//------------------------------------------------------------
inline void SFIntBN::operator-=(const SFIntBN &x)
{
	subtract(*this, x);
}

//------------------------------------------------------------
inline void SFIntBN::operator*=(const SFIntBN &x)
{
	multiply(*this, x);
}

//------------------------------------------------------------
inline void SFIntBN::operator/=(const SFIntBN &x)
{
	//TODO: Why not use mag.len==0 for test for zero to be consistent?
	if (x.sign==0)
		throw "SFIntBN::operator /=: division by zero";

	SFIntBN q;
	divide(x, q);
	*this = q;
}

//------------------------------------------------------------
inline void SFIntBN::operator %=(const SFIntBN &x)
{
	//TODO: Why not use mag.len==0 for test for zero to be consistent?
	if (x.sign==0)
		throw "SFIntBN::operator %=: division by zero";
	SFIntBN q;
	divide(x, q);
}
