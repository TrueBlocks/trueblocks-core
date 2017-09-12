#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

//-------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    bool isAll;
    bool isRun;
    bool isEdit;
    bool isRemove;
    bool isList;
    bool silent;
    SFString namesp;
    bool writeHeader;
    bool writeSource;

    SFString classNames;
    SFString filter;
    SFString dataFile;
    CToml classFile;

    COptions(void);
    ~COptions(void);

    bool parseArguments(SFString& command);
    void Init(void);
};

//-------------------------------------------------------------------
extern bool listClasses(const SFString& path, void *data);
