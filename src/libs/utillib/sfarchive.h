#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include "exportcontext.h"
#include "database.h"

class CBaseNode;
typedef bool (*VISITARCHIVEFUNC)(CBaseNode& node, void *data);

//-----------------------------------------------------------------------------------------
class SFArchive : public CSharedResource
{
public:
	bool             m_writeDeleted;
	uint32_t         m_archiveSchema;
	bool             m_isReading;
	const CBaseNode  *pParent;
	VISITARCHIVEFUNC writeMsgFunc;
	VISITARCHIVEFUNC readMsgFunc;

	SFArchive(bool isReading, uint32_t schema, bool writeDeleted)
		{
			m_isReading    = isReading;
			m_archiveSchema       = schema;
			m_writeDeleted = writeDeleted;
			pParent        = NULL;
			writeMsgFunc   = NULL;
			readMsgFunc    = NULL;
		}
	bool writeDeleted(void) const
		{
			return (m_writeDeleted);
		}
	bool isWriting(void) const
		{
			return !m_isReading;
		}
	bool isReading(void) const
		{
			return m_isReading;
		}

	SFArchive& operator<<(bool b);
	SFArchive& operator<<(char c);
	SFArchive& operator<<(int d);
	SFArchive& operator<<(unsigned int d);
	SFArchive& operator<<(long dw);
	SFArchive& operator<<(long long dw);
	SFArchive& operator<<(unsigned long dw);
	SFArchive& operator<<(unsigned long long dw);
	SFArchive& operator<<(float f);
	SFArchive& operator<<(double f);
	SFArchive& operator<<(const SFString& str);
	SFArchive& operator<<(const SFTime& tm);
	SFArchive& operator<<(const SFUintBN& bn);
	SFArchive& operator<<(const SFIntBN& bn);
	SFArchive& operator<<(const char *str);

	SFArchive& operator>>(bool& b);
	SFArchive& operator>>(char& c);
	SFArchive& operator>>(int& d);
	SFArchive& operator>>(unsigned int& d);
	SFArchive& operator>>(long& dw);
	SFArchive& operator>>(long long& dw);
	SFArchive& operator>>(unsigned long& dw);
	SFArchive& operator>>(unsigned long long& dw);
	SFArchive& operator>>(float& f);
	SFArchive& operator>>(double& f);
	SFArchive& operator>>(SFString& str);
	SFArchive& operator>>(SFTime& tm);
	SFArchive& operator>>(SFUintBN& bn);
	SFArchive& operator>>(SFIntBN& bn);
};

extern SFArchive& operator<<(SFArchive& archive, const SFStringArray& array);
extern SFArchive& operator<<(SFArchive& archive, const SFBigUintArray& array);

extern SFArchive& operator>>(SFArchive& archive, SFStringArray& array);
extern SFArchive& operator>>(SFArchive& archive, SFBigUintArray& array);
