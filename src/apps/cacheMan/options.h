#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "acctlib.h"

//-------------------------------------------------------------------------
class CStats {
public:
    uint64_t nDups;
    uint64_t nReversals;
    uint64_t nFixed;
    uint64_t nRecords;
    uint64_t nTruncs;
    CStats(void) : nDups(0), nReversals(0), nFixed(0), nRecords(0), nTruncs(0)
        {}
};

//-------------------------------------------------------------------------
class COptions;
typedef bool (*APPEARANCEFILTERFUNC)(CAppearanceArray_base& dataArray, const CAppearance_base& item);

//-------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
// BEG_CODE_DECLARE
    bool data;
    blknum_t truncate;
    uint32_t skip;
    blknum_t start;
    blknum_t end;
// END_CODE_DECLARE

    string_q mode;
    CStats stats;
    CAccountWatchArray monitors;
    CAppearanceArray_base removals;

    COptions(void);
    ~COptions(void);

    bool loadMonitorData(CAppearanceArray_base& items, const address_t& addr);
    bool handleMerge    (void) const;
    bool handleSort     (void) const;
    bool handleImport   (void) const;
    bool handleRemove   (void) const;
    bool handleRead     (const string_q& mode, size_t filesToUse, CAppearanceArray_base& dataArray) const;
    bool handleWrite    (const string_q& outputFilename, const CAppearanceArray_base& dataArray, APPEARANCEFILTERFUNC filterFunc) const;

    bool parseArguments(string_q& command);
    void Init(void);
};

//-------------------------------------------------------------------------
extern bool handleList      (COptions& options);
extern bool handleFix       (COptions& options);
extern int  sortByBlock     (const void *v1, const void *v2);

//-------------------------------------------------------------------------
extern const char *STR_DEFAULT_DISPLAY;
extern const char *STR_DATA_DISPLAY;
