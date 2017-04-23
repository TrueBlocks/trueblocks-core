#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include "list.h"

//-------------------------------------------------------------------------
class CTomlGroup
{
public:
	SFString  groupName;
	bool      comment;
	CTomlKeyList keys;

	CTomlGroup(void);
	CTomlGroup(const CTomlGroup& group);

	~CTomlGroup(void);

	CTomlGroup& operator=(const CTomlGroup& group);

	CTomlKey *addKey(const SFString& keyName, const SFString& val, bool commented);
	CTomlKey *findKey(const SFString& keyName) const;

private:
	void Clear(void);
	void Copy(const CTomlGroup& group);

};
typedef SFList<CTomlGroup*> CTomlGroupList;

//-------------------------------------------------------------------------
inline CTomlGroup::CTomlGroup(void)
{
	Clear();
}

//-------------------------------------------------------------------------
inline CTomlGroup::CTomlGroup(const CTomlGroup& group)
{
	Copy(group);
}

//-------------------------------------------------------------------------
inline CTomlGroup::~CTomlGroup(void)
{
	Clear();
}

//-------------------------------------------------------------------------
inline CTomlGroup& CTomlGroup::operator=(const CTomlGroup& group)
{
	Copy(group);
	return *this;
}

//-------------------------------------------------------------------------
inline void CTomlGroup::Clear(void)
{
	groupName = EMPTY;
	comment = false;
	LISTPOS kPos = keys.GetHeadPosition();
	while (kPos)
	{
		CTomlKey *key = keys.GetNext(kPos);
		delete key;
	}
	keys.RemoveAll();
}

//-------------------------------------------------------------------------
inline void CTomlGroup::Copy(const CTomlGroup& group)
{
	Clear();

	groupName = group.groupName;
	comment = group.comment;

	LISTPOS kPos = group.keys.GetHeadPosition();
	while (kPos)
	{
		CTomlKey *orig = group.keys.GetNext(kPos);
		CTomlKey *copy = new CTomlKey(*orig);
		keys.AddTail(copy);
	}
}
