#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass. Edit only those parts of the code
 * outside of the BEG_CODE/END_CODE sections
 */
#include "acctlib.h"

// BEG_ERROR_DEFINES
// END_ERROR_DEFINES

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
  public:
    // BEG_CODE_DECLARE
    // END_CODE_DECLARE

    CAccountWatchArray monitors;
    blkrange_t fileRange;
    size_t visitTypes;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);

    bool visitBinaryFile(const string_q& path, void* data);
    void moveToProduction(void);
};

#define VIS_FINAL (1 << 1)
#define VIS_STAGING (1 << 2)
#define VIS_UNRIPE (1 << 3)

extern bool visitFinalIndexFiles(const string_q& path, void* data);
extern bool visitStagingIndexFiles(const string_q& path, void* data);
extern bool visitUnripeIndexFiles(const string_q& path, void* data);
