#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "utillib.h"
#include "options.h"

class CAccountName {
public:
    SFString addr;
    SFString source;
    SFString name;
    CAccountName(void) { }
    explicit CAccountName(SFString& nameIn) {
        source = nameIn;
        addr = toLower(nextTokenClear(source, '\t'));
        name = nextTokenClear(source, '\t');
    }
    SFString Format12(void) {
#define F(a) (SFString("`") + (SFString(#a) + "`: `" + a + "` ")).Substitute("`", "\"")
        return ("{" + F(addr) + ", " + F(name) + ", " + F(source) + "},").Substitute(" ,", ",");
    }
    bool Match(const SFString& s1, const SFString& s2, const SFString& s3, bool matchCase, bool all);
};

extern CAccountName *accounts;
extern uint32_t nNames;
extern CFileExportContext out;
extern void loadData(void);
extern uint32_t countOf(const SFString& addr);
extern SFString showName(const SFString& s1, const SFString& s2, const SFString& s3, bool match, bool all);
extern SFString setName(const SFString& addr, const SFString& name);
