#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include "database.h"
#include "toml_key.h"
#include "toml_group.h"

//-------------------------------------------------------------------------
class CToml : public CSharedResource
{
public:
	CTomlGroupList groups;

public:
	 CToml(void);
	~CToml(void);

	SFString getConfigStr (const SFString& group, const SFString& key, const SFString& def) const;
	uint32_t getConfigInt (const SFString& group, const SFString& key, uint32_t def) const;
    bool     getConfigBool(const SFString& group, const SFString& key, bool del) const;

	void     setConfigStr (const SFString& group, const SFString& key, const SFString& value);
	void     setConfigInt (const SFString& group, const SFString& key, uint32_t value);
	void     setConfigBool(const SFString& group, const SFString& key, bool value);

	bool     writeFile    (void);
	bool     readFile     (const SFString& filename);
	void     mergeFile    (CToml *tomlIn);

protected:
	CTomlGroup *addGroup  (const SFString& group, bool commented);
	CTomlGroup *findGroup (const SFString& group) const;

	CTomlKey *addKey      (const SFString& group, const SFString& key, const SFString& val, bool commented);
	CTomlKey *findKey     (const SFString& group, const SFString& key) const;

private:
	void Clear(void);
	// no copies
	CToml(const CToml& toml);
	CToml& operator= (const CToml& toml);
};

//-------------------------------------------------------------------------
extern ostream& operator<<(ostream& os, const CToml& t);

//-------------------------------------------------------------------------
inline CToml::CToml(void)
{
}

//-------------------------------------------------------------------------
inline CToml::~CToml(void)
{
	Clear();
}

//-------------------------------------------------------------------------
inline void CToml::Clear(void)
{
	LISTPOS gPos = groups.GetHeadPosition();
	while (gPos)
	{
		CTomlGroup *group = groups.GetNext(gPos);
		delete group;
	}
	groups.RemoveAll();
}

//-------------------------------------------------------------------------
inline CTomlGroup *CToml::addGroup(const SFString& group, bool commented)
{
	ASSERT(!findGroup(group));
	CTomlGroup *newGroup = new CTomlGroup;
	newGroup->comment = commented;
	newGroup->groupName = group;
	groups.AddTail(newGroup);
	return newGroup;
}

//-------------------------------------------------------------------------
inline CTomlGroup *CToml::findGroup(const SFString& group) const
{
	LISTPOS gPos = groups.GetHeadPosition();
	while (gPos)
	{
		CTomlGroup *grp = groups.GetNext(gPos);
		if (grp->groupName == group)
			return grp;
	}
	return NULL;
}

//-------------------------------------------------------------------------
inline CTomlKey *CToml::addKey(const SFString& group, const SFString& key, const SFString& val, bool commented)
{
	CTomlGroup *grp = findGroup(group);
	ASSERT(grp);
	return grp->addKey(key, val, commented);
}

//-------------------------------------------------------------------------
inline CTomlKey *CToml::findKey(const SFString& group, const SFString& key) const
{
	CTomlGroup *grp = findGroup(group);
	if (grp)
		return grp->findKey(key);
	return NULL;
}

//-------------------------------------------------------------------------
inline uint32_t CToml::getConfigInt(const SFString& group, const SFString& key, uint32_t def) const
{
	SFString ret = getConfigStr(group, key, asString(def));
	return toLong32u(ret);
}

//-------------------------------------------------------------------------
inline bool CToml::getConfigBool(const SFString& group, const SFString& key, bool def) const
{
	SFString ret = getConfigStr(group, key, asString(def));
	return ((ret == "true" || ret == "1") ? true : false);
}

//-------------------------------------------------------------------------
inline void CToml::setConfigInt(const SFString& group, const SFString& key, uint32_t value)
{
	setConfigStr(group, key, asString(value));
}

//-------------------------------------------------------------------------
inline void CToml::setConfigBool(const SFString& group, const SFString& key, bool value)
{
	setConfigStr(group, key, asString(value));
}
