#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include "sfstring.h"
#include "sftime.h"

class CExportContext
{
public:
	uint32_t nTabs;
	SFString fmt;
	char     tCh;

	         CExportContext(void);
	virtual ~CExportContext(void) { }

	virtual CExportContext& operator<<(bool b);
	virtual CExportContext& operator<<(char c);
	virtual CExportContext& operator<<(long dw);
	virtual CExportContext& operator<<(unsigned int ui);
	virtual CExportContext& operator<<(unsigned long ui);
	virtual CExportContext& operator<<(unsigned long long ui);
	virtual CExportContext& operator<<(int i);
	virtual CExportContext& operator<<(float f);
	virtual CExportContext& operator<<(double f);
	virtual CExportContext& operator<<(const char *str);
	virtual CExportContext& operator<<(const SFString& str);
	virtual CExportContext& operator<<(const SFTime& tm);

	virtual SFString tabs      (uint32_t add=0) { return SFString(tCh, nTabs+add); }
	virtual SFString inc       (void)          { SFString ret = SFString(tCh, nTabs); nTabs++; return ret; }
	virtual SFString dec       (void)          { nTabs--;  return SFString(tCh, nTabs); }

	virtual void     setOutput (void *output)        = 0;
	virtual void*    getOutput (void) const          = 0;
	virtual void     Output    (const SFString& str) = 0;
	virtual void     Flush     (void)                = 0;
};

// Handy for debugging
class CFileExportContext : public CExportContext
{
public:
	FILE *m_output;

	CFileExportContext(void *output=NULL)
		{
			m_output = ((output == NULL) ? stdout : (FILE*)output);
		}

	CFileExportContext(const SFString& filename, const SFString& mode)
		{
			m_output = fopen((const char *)filename, mode);
			if (!m_output)
				m_output = stdout;
		}

	~CFileExportContext(void)
		{
			Close();
		}

	void  setOutput (void *output);
	void *getOutput (void) const { return m_output; };
	void  Output    (const SFString& str);
	void  Flush     (void)
		{
			ASSERT(m_output)
			fflush(m_output);
		}
	void Close(void)
		{
			Flush();
			if (m_output != stdout && m_output != stderr)
				fclose(m_output);
			m_output = stdout;
		}
};

class CErrorExportContext : public CFileExportContext
{
public:
	CErrorExportContext(void) : CFileExportContext(stderr) {}
};

// Handy for generating HTML into strings
class CStringExportContext : public CExportContext
{
public:
	SFString str;

	CStringExportContext(void) {}

	void  setOutput (void *output);
	void* getOutput (void) const { return NULL; };
	void  Output    (const SFString& s);
	void  Flush     (void) { } // do nothing

	operator SFString(void) const;
};

inline CStringExportContext::operator SFString(void) const
{
   return str;
}

#define Fmt(a) (SFString("`")+SFString(a))
