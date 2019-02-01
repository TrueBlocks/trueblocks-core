#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved.
 *------------------------------------------------------------------------*/
#include "etherlib.h"

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    string_q sourceFolder;
    string_q monitorFolder;
    string_q monitorName;
    string_q addrList;

    COptions(void);
    ~COptions(void);

    string_q postProcess(const string_q& which, const string_q& str) const;
    bool parseArguments(string_q& command);
    void Init(void);
};

//-----------------------------------------------------------------------------
class CQuestion;
typedef bool (*QUESTIONFUNC)(const COptions& options, CQuestion *q);

//-----------------------------------------------------------------------------
class CQuestion {
public:
    string_q question;
    string_q answer;
    QUESTIONFUNC func;
    string_q color;
    bool wantsInput;
    bool nl;
    CQuestion(const string_q& q,bool h, const string_q& c, QUESTIONFUNC f = NULL)
        : question(q), answer(""), func(f), color(c), wantsInput(h), nl(true) {}
    bool getResponse(void);
};

//--------------------------------------------------------------
extern bool createFolders(const COptions& options, CQuestion *q);
extern bool createRebuild(const COptions& options, CQuestion *q);
extern bool createConfig (const COptions& options, CQuestion *q);
extern bool createCache  (const COptions& options, CQuestion *q);
extern bool editMakeLists(const COptions& options, CQuestion *q);
