#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include "runtimeclass.h"

//----------------------------------------------------------------------------
class SFArchive;

//----------------------------------------------------------------------------
class CExportOptions
{
public:
	uint32_t lev, spcs;
	bool noFrst;
	char tab, nl;
	bool quoteNums;
	bool hexNums;
	bool hashesOnly;
	bool colored;
	CExportOptions(void)
		{
			noFrst=false;
			lev=0; spcs=2;
			tab=' '; nl='\n';
			quoteNums=false;
			hexNums=false;
			hashesOnly=false;
			colored=false;
		}
};
extern CExportOptions& expContext(void);
extern void incIndent(void);
extern void decIndent(void);
extern SFString indent(void);

//----------------------------------------------------------------------------
class CBaseNode
{
public:
	SFUint32 m_deleted;
	SFUint32 m_schema;
	SFUint32 m_showing;

public:
	const CBaseNode *pParent;

	                 CBaseNode       (void);
	virtual         ~CBaseNode       (void);

		bool     isDeleted       (void) const;
		void     setDeleted      (bool val);

	virtual bool     isKindOf        (const CRuntimeClass* pClass) const;
	virtual char    *parseJson       (char *s, uint32_t& nFields);
	virtual char    *parseJson       (char *s);
	virtual char    *parseCSV        (char *s, uint32_t& nFields, const SFString *fields);
	virtual char    *parseText       (char *s, uint32_t& nFields, const SFString *fields);
	virtual SFString toJson          (void) const;
	virtual SFString toJson          (const SFString& fields) const;
	virtual SFString toJson          (const CFieldList *fields) const;
	virtual SFString toJsonFldList   (const CFieldList *fieldList) const;

	DECLARE_NODE_BASE(CBaseNode)

	virtual SFString getValueByName (const SFString& fieldName) const = 0;
	virtual bool     setValueByName (const SFString& fieldName, const SFString& fieldValue) = 0;
	virtual bool     Serialize      (SFArchive& archive) = 0;
	virtual bool     SerializeC     (SFArchive& archive) const = 0;
	virtual void     finishParse    (void) = 0;
	virtual SFString Format         (const SFString& fmtIn="") const = 0;

protected:
	        bool     preSerialize (SFArchive& archive);
		bool     postSerialize(SFArchive& archive);
	        bool     preSerializeC(SFArchive& archive) const;
		bool     postSerializeC(SFArchive& archive) const;

		void	 Init   (void);
		void	 Copy   (const CBaseNode& bn);
};

//----------------------------------------------------------------------------
#undef ghDELETE
#define ghDELETE(_nodE) \
	if ((_nodE)) \
		if ((_nodE)->Dereference()) \
			delete (_nodE); \
	(_nodE) = NULL;

extern char *cleanUpJson(char *s);

//--------------------------------------------------------------------------------------------------------------
typedef SFString (*NEXTCHUNKFUNC) (const SFString& fieldIn, bool& forceShow, const void *data);

//--------------------------------------------------------------------------------------------------------------
SFString getNextChunk(SFString& fmtOut, NEXTCHUNKFUNC func, const void *data);
