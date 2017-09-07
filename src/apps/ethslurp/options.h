#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include <algorithm>
#include "etherlib.h"

class COptions : public COptionsBase {
public:
    bool prettyPrint;
    bool rerun;
    bool incomeOnly;
    bool expenseOnly;
    uint64_t firstBlock2Read;
    uint64_t lastBlock2Read;
    SFTime firstDate;
    SFString funcFilter;
    int errFilt;
    bool reverseSort;
    SFTime lastDate;
    bool openFile;
    SFString addr;
    uint32_t maxTransactions;
    uint32_t pageSize;
    SFString exportFormat;
    SFString name;
    SFString archiveFile;
    bool wantsArchive;
    bool cache;
    uint32_t acct_id;
    FILE *output;  // for use when -a is on

    COptions(void);
    ~COptions(void);

    bool parseArguments(SFString& command);
    void Init(void);
};
