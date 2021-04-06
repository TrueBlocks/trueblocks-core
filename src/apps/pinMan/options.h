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
#include "pinlib.h"

// BEG_ERROR_DEFINES
// END_ERROR_DEFINES

//-------------------------------------------------------------------------
class COptions : public CAbiOptions {
  public:
    // BEG_CODE_DECLARE
    string_q mode;
    bool hash;
    string_q pin;
    string_q unpin;
    bool init;
    bool license;
    // END_CODE_DECLARE

    CPinApiLicense lic;
    CPinnedChunkArray pList;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command) override;
    void Init(void) override;
    bool Mocked(void);

    hash_t getCurrentManifest(void);
    bool freshenBlooms(bool download, const string_q& currManifest);
    void handle_unpin(void);
    void handle_pin(void);
};

//-------------------------------------------------------------------------
#define unchainedIndexAddr "0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd"
#define manifestHashEncoding "0x337f3f32"
